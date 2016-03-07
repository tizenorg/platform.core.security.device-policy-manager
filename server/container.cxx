/*
 *  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */

#include <aul.h>
#include <bundle.h>
#include <tzplatform_config.h>
#include <gio/gio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "shadow.hxx"
#include "process.hxx"
#include "container.hxx"
#include "parameter.hxx"
#include "filesystem.hxx"
#include "access-control.hxx"
#include "xml/parser.hxx"
#include "xml/document.hxx"
#include "audit/logger.hxx"

#define CONTAINER_UID_MIN       60001
#define CONTAINER_UID_MAX       65000

#define CONTAINER_GROUP         "containers"

#define CONTAINER_MANIFEST_DIR   CONF_PATH "/container/"

#define FREEDESKTOP_LOGIN_INTERFACE \
    "org.freedesktop.login1",   \
    "/org/freedesktop/login1",  \
    "org.freedesktop.login1.Manager"

#define HOME_SMACKLABEL     "User::Home"
#define SHARED_SMACKLABEL   "User::App::Shared"

#define TEMPORARY_UMASK(mode)   \
        std::unique_ptr<mode_t, void(*)(mode_t *)> umask_##mode(new mode_t, \
        [](mode_t *prev) {umask(*prev);}); \
        *umask_##mode = mode;

namespace DevicePolicyManager {

static int callLogind(const char* function, GVariant* param)
{
    GDBusConnection* connection;
    GError* error = NULL;
    GVariant* var;

    connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);

    if (connection == NULL) {
        ERROR(std::string("Failed to get system DBUS : ") +  error->message);
        g_error_free(error);
        return -1;
    }

    var = g_dbus_connection_call_sync(connection, FREEDESKTOP_LOGIN_INTERFACE,
                                      function, param,
                                      NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL,
                                      &error);

    if (var == NULL) {
        ERROR(std::string("Failed to call ") + function + " : " + error->message);
        g_error_free(error);
        return -1;
    } else {
        g_variant_unref(var);
    }
    /*
        ret = g_dbus_connection_close_sync(connection, NULL, &error);
        if (ret == FALSE) {
            g_printerr("Failed to close system DBUS : %s\n", error->message);
            g_error_free(error);
            return -1;
        }
    */
    return 0;
}


Container::Container(PolicyControlContext& ctx)
    : context(ctx)
{
    Ipc::Service& manager = context.getServiceManager();

    manager.registerParametricMethod(this, (int)(Container::create)(std::string, std::string));
    manager.registerParametricMethod(this, (int)(Container::remove)(std::string));
    manager.registerParametricMethod(this, (int)(Container::lock)(std::string));
    manager.registerParametricMethod(this, (int)(Container::unlock)(std::string));
    manager.registerNonparametricMethod(this, (Vector<String>)(Container::getList)());
}

Container::~Container()
{
}

int Container::create(const std::string& name, const std::string& setupWizAppid)
{
    Ipc::Service& manager = context.getServiceManager();
    std::unique_ptr<Xml::Document> bundleXml;
    Xml::Node::NodeList nodes;
    std::string manifest_path;
    bundle* b;
    int ret;

    try {
        b = ::bundle_create();
        if (b == nullptr) {
            throw Runtime::Exception("Failed to create bundle: out of memory");
        }

        ::bundle_add_str(b, "id", name.c_str());

        ret = ::aul_launch_app_for_uid(setupWizAppid.c_str(), b, manager.getPeerUid());
        ::bundle_free(b);
        if (ret < 0) {
            throw Runtime::Exception("Failed to launch application: " + std::to_string(ret));
        }

//TODO : API should get manifest from setup wizard
        manifest_path = "/etc/dpm/container/owner.xml";

        Shadow::User user = Shadow::User::create
                            (name, CONTAINER_GROUP, CONTAINER_UID_MIN, CONTAINER_UID_MAX);

        TEMPORARY_UMASK(0022);

        struct {
            enum tzplatform_variable dir;
            const char* smack;
        } dirs[] = {
            {TZ_USER_HOME, HOME_SMACKLABEL},
            {TZ_USER_CACHE, SHARED_SMACKLABEL},
            {TZ_USER_APPROOT, HOME_SMACKLABEL},
            {TZ_USER_DB, HOME_SMACKLABEL},
            {TZ_USER_PACKAGES, HOME_SMACKLABEL},
            {TZ_USER_ICONS, HOME_SMACKLABEL},
            {TZ_USER_CONFIG, SHARED_SMACKLABEL},
            {TZ_USER_DATA, HOME_SMACKLABEL},
            {TZ_USER_SHARE, SHARED_SMACKLABEL},
            {TZ_USER_ETC, HOME_SMACKLABEL},
            {TZ_USER_LIVE, HOME_SMACKLABEL},
            {TZ_USER_UG, HOME_SMACKLABEL},
            {TZ_USER_APP, HOME_SMACKLABEL},
            {TZ_USER_CONTENT, SHARED_SMACKLABEL},
            {TZ_USER_CAMERA, SHARED_SMACKLABEL},
            {TZ_USER_VIDEOS, SHARED_SMACKLABEL},
            {TZ_USER_IMAGES, SHARED_SMACKLABEL},
            {TZ_USER_SOUNDS, SHARED_SMACKLABEL},
            {TZ_USER_MUSIC, SHARED_SMACKLABEL},
            {TZ_USER_GAMES, SHARED_SMACKLABEL},
            {TZ_USER_DOCUMENTS, SHARED_SMACKLABEL},
            {TZ_USER_OTHERS, SHARED_SMACKLABEL},
            {TZ_USER_DOWNLOADS, SHARED_SMACKLABEL},
            {TZ_SYS_HOME, NULL},
        };

        ::tzplatform_set_user(user.getUid());
        for (int i = 0; dirs[i].dir != TZ_SYS_HOME; i++) {
            Runtime::File dir(std::string(::tzplatform_getenv(dirs[i].dir)));
            dir.makeDirectory();
            dir.chown(user.getUid(), user.getGroup().getGid());
            Runtime::SmackFileLabel::setAccess(dir, dirs[i].smack);
            Runtime::SmackFileLabel::setTransmute(dir, true);
        }
        ::tzplatform_reset_user();

        bundleXml = std::unique_ptr<Xml::Document>(Xml::Parser::parseFile(manifest_path));

        nodes = bundleXml->evaluate("//bundle-manifest/hooks/create/exec");
        for (Xml::Node::NodeList::iterator it = nodes.begin();
                it != nodes.end(); it++) {
            std::vector<std::string> args;
            std::string path, arg;
            Xml::Node::NodeList children = it->getChildren();
            for (Xml::Node::NodeList::iterator it = children.begin();
                    it != children.end(); it++) {
                if (it->getName() == "path") {
                    path = it->getChildren().begin()->getContent();
                    Runtime::Parameter::parse(path, name);
                } else if (it->getName() == "args") {
                    Xml::Node::NodeList children = it->getChildren();
                    for (Xml::Node::NodeList::iterator it = children.begin();
                            it != children.end(); it++) {
                        if (it->getName() == "arg") {
                            arg = it->getChildren().begin()->getContent();
                            Runtime::Parameter::parse(arg, name);
                            args.push_back(arg);
                        }
                    }
                }
            }
            Runtime::Process exec(path, args);
            exec.execute();
        }

        bundleXml->write(CONTAINER_MANIFEST_DIR + name + ".xml", "UTF-8", true);
    } catch (Runtime::Exception& e) {
        ERROR(e.what());
        return -1;
    }

    return 0;
}

int Container::remove(const std::string& name)
{
    std::unique_ptr<Xml::Document> bundleXml;
    Xml::Node::NodeList nodes;
    std::string manifest_path;
    int ret;

    //lock the user
    ret = lock(name);
    if (ret != 0) {
        return -1;
    }

    try {
        manifest_path = CONTAINER_MANIFEST_DIR + name + ".xml";

        bundleXml = std::unique_ptr<Xml::Document>(Xml::Parser::parseFile(manifest_path));

        nodes = bundleXml->evaluate("//bundle-manifest/hooks/destroy/exec");
        for (Xml::Node::NodeList::iterator it = nodes.begin();
                it != nodes.end(); it++) {
            std::vector<std::string> args;
            std::string path, arg;
            Xml::Node::NodeList children = it->getChildren();
            for (Xml::Node::NodeList::iterator it = children.begin();
                    it != children.end(); it++) {
                if (it->getName() == "path") {
                    path = it->getChildren().begin()->getContent();
                    Runtime::Parameter::parse(path, name);
                } else if (it->getName() == "args") {
                    Xml::Node::NodeList children = it->getChildren();
                    for (Xml::Node::NodeList::iterator it = children.begin();
                            it != children.end(); it++) {
                        if (it->getName() == "arg") {
                            arg = it->getChildren().begin()->getContent();
                            Runtime::Parameter::parse(arg, name);
                            args.push_back(arg);
                        }
                    }
                }
            }
            Runtime::Process exec(path, args);
            exec.execute();
        }

        Shadow::User user(name);
        user.remove();

        Runtime::File bundle(manifest_path);
        bundle.remove();
    } catch (Runtime::Exception& e) {
        ERROR(e.what());
        return -1;
    }

    return 0;
}

int Container::lock(const std::string& name)
{
    int result;

    try {
        Shadow::User user(name);

        //disable user linger by using systemd
        result = callLogind("SetUserLinger",
                            g_variant_new("(ubb)", user.getUid(), 0, 1));
        if (result != 0) {
            return -1;
        }

    } catch (Runtime::Exception& e) {
        ERROR(e.what());
        return -1;
    }

    return 0;
}

int Container::unlock(const std::string& name)
{
    int result;

    try {
        Shadow::User user(name);

        //enable user linger by using systemd
        result = callLogind("SetUserLinger",
                            g_variant_new("(ubb)", user.getUid(), 1, 1));
        if (result != 0) {
            return -1;
        }

    } catch (Runtime::Exception& e) {
        ERROR(e.what());
        return -1;
    }

    return 0;
}

Vector<String> Container::getList()
{
    Vector<String> list;
    try {
        Runtime::DirectoryIterator iter(Runtime::Path(CONTAINER_MANIFEST_DIR)), end;

        while (iter != end) {
            const std::string& path = (*iter).getPath();
            size_t name = path.rfind('/') + 1;
            size_t ext = path.rfind(".xml");
            list.value.push_back(path.substr(name, ext - name));
            ++iter;
        }
    } catch (Runtime::Exception& e) {}

    return list;
}

Container containerPolicy(DevicePolicyServer::Server::instance());

} // namespace DevicePolicyManager
