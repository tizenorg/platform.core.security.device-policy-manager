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

#include <sys/types.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <syslog.h>

#include <string>
#include <sstream>
#include <iostream>

#include "filesystem.h"
#include "error.h"
#include "exception.h"
#include "ecryptfs.h"
#include "key/ring.h"
#include "key/manager.h"
#include "key/generator.h"

namespace runtime {

Path::Path()
{
}

Path::Path(const std::string& path)
{
    assign(path);
}

Path::Path(const char* path)
{
    assign(path);
}

Path::Path(const Path& path)
    : pathname(path.pathname)
{
}

Path::~Path()
{
}

Path& Path::assign(const Path& path)
{
    if (&path != this) {
        pathname = path.pathname;
    }

    return *this;
}

Path& Path::assign(const std::string& path)
{
    pathname = path;

    return *this;
}

Path& Path::assign(const char* path)
{
    return assign(std::string(path));
}

Path& Path::operator=(const Path& path)
{
    return assign(path);
}

Path& Path::operator=(const std::string& path)
{
    return assign(path);
}

Path& Path::operator=(const char* path)
{
    return assign(path);
}

File::File()
    : descriptor(-1)
{
}

File::File(const char* pathname)
    : descriptor(-1), path(pathname)
{
}

File::File(const std::string& pathname)
    : descriptor(-1), path(pathname)
{
}

File::File(const Path& filePath)
    : descriptor(-1), path(filePath)
{
}

File::File(const File& file)
    : File(file.getPath())
{
}

File::File(const std::string& pathname, int flags)
    : File(pathname)
{
    open(flags);
}

File::~File()
{
    if (descriptor != -1) {
        close();
    }
}

bool File::exists() const
{
    struct stat st;
    return (::stat(path.getPathname().c_str(), &st) == 0);
}

bool File::canRead() const
{
    struct stat st;
    if (::stat(path.getPathname().c_str(), &st) != 0) {
        throw runtime::Exception(runtime::GetSystemErrorMessage());
    }

    if (st.st_uid == geteuid()) {
        return ((st.st_mode & S_IRUSR) != 0);
    } else if (st.st_gid == getegid()) {
        return ((st.st_mode & S_IRGRP) != 0);
    }

    return (((st.st_mode & S_IROTH) != 0) || (geteuid() == 0));
}

bool File::canWrite() const
{
    struct stat st;
    if (::stat(path.getPathname().c_str(), &st) != 0) {
        throw runtime::Exception(runtime::GetSystemErrorMessage());
    }

    if (st.st_uid == geteuid()) {
        return ((st.st_mode & S_IWUSR) != 0);
    } else if (st.st_gid == getegid()) {
        return ((st.st_mode & S_IWGRP) != 0);
    }

    return (((st.st_mode & S_IWOTH) != 0) || (geteuid() == 0));
}

bool File::canExecute() const
{
    struct stat st;
    if (::stat(path.getPathname().c_str(), &st) != 0) {
        throw runtime::Exception(runtime::GetSystemErrorMessage());
    }

    if (st.st_uid == geteuid()) {
        return ((st.st_mode & S_IXUSR) != 0);
    } else if (st.st_gid == getegid()) {
        return ((st.st_mode & S_IXGRP) != 0);
    }

    return ((st.st_mode & S_IXOTH) != 0);
}

bool File::isLink() const
{
    struct stat st;
    if (::stat(path.getPathname().c_str(), &st) != 0) {
        throw runtime::Exception(runtime::GetSystemErrorMessage());
    }

    return S_ISLNK(st.st_mode);
}

bool File::isFile() const
{
    struct stat st;
    if (::stat(path.getPathname().c_str(), &st) != 0) {
        throw runtime::Exception(runtime::GetSystemErrorMessage());
    }

    return S_ISREG(st.st_mode);
}

bool File::isDirectory() const
{
    struct stat st;
    if (::stat(path.getPathname().c_str(), &st) != 0) {
        throw runtime::Exception(runtime::GetSystemErrorMessage());
    }

    return S_ISDIR(st.st_mode);
}

bool File::isDevice() const
{
    struct stat st;
    if (::stat(path.getPathname().c_str(), &st) != 0) {
        throw runtime::Exception(runtime::GetSystemErrorMessage());
    }

    return (S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode));
}

bool File::isHidden() const
{
    return false;
}

std::string File::toString() const
{
    struct stat st;
    if (::stat(path.getPathname().c_str(), &st) != 0) {
        throw runtime::Exception(runtime::GetSystemErrorMessage());
    }

    return std::string("Test String");
}

void File::create(mode_t mode)
{
    while (1) {
        descriptor = ::creat(path.getPathname().c_str(), mode);
        if (descriptor == -1) {
            if (errno != EINTR) {
                continue;
            }
            throw runtime::Exception(runtime::GetSystemErrorMessage());
        }
        return;
    }
}

void File::open(int flags)
{
    while (1) {
        descriptor = ::open(path.getPathname().c_str(), flags);
        if (descriptor == -1) {
            if (errno != EINTR) {
                continue;
            }
            throw runtime::Exception(runtime::GetSystemErrorMessage());
        }
        return;
    }
}

void File::close()
{
    while (1) {
        if (::close(descriptor) == -1) {
            if (errno == EINTR) {
                continue;
            }
        }
        return;
    }
}

void File::read(void *buffer, const size_t size) const
{
    size_t total = 0;

    while (total < size) {
        int bytes = ::read(descriptor, reinterpret_cast<char*>(buffer) + total, size - total);
        if (bytes >= 0) {
            total += bytes;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
            continue;
        } else {
            throw runtime::Exception(runtime::GetSystemErrorMessage());
        }
    }
}

void File::write(const void *buffer, const size_t size) const
{
    size_t written = 0;

    while (written < size) {
        int bytes = ::write(descriptor, reinterpret_cast<const char*>(buffer) + written, size - written);
        if (bytes >= 0) {
            written += bytes;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
            continue;
        } else {
            throw runtime::Exception(runtime::GetSystemErrorMessage());
        }
    }
}

void File::copyTo(const std::string& dest) const
{
}

void File::moveTo(const std::string& dest)
{
}

void File::renameTo(const std::string& dest)
{
}

void File::remove(bool recursive)
{
    if (isDirectory()) {
        if (recursive) {
            DirectoryIterator iter(path), end;
            while (iter != end) {
                iter->remove(true);
                ++iter;
            }
        }
        if (::rmdir(path.getPathname().c_str()) != 0) {
            throw runtime::Exception(runtime::GetSystemErrorMessage());
        }
    } else {
        if (::unlink(path.getPathname().c_str()) != 0) {
            throw runtime::Exception(runtime::GetSystemErrorMessage());
        }
    }
}

void File::makeBaseDirectory(uid_t uid, gid_t gid)
{
    std::string::size_type i = path.isRelative() ? -1 : 0;
    const std::string& pathStr = path.getPathname();
    while ((i = pathStr.find('/', i + 1)) != std::string::npos) {
        std::string substr = pathStr.substr(0, i);
        int ret = ::mkdir(substr.c_str(), 0777);
        if ((ret == -1) && (errno == EEXIST)) {
            continue;
        } else if (ret == 0) {
            if ((uid | gid) != 0) {
                if (::chown(substr.c_str(), uid, gid) == -1) {
                    throw runtime::Exception(runtime::GetSystemErrorMessage());
                }
            }
        } else {
            throw runtime::Exception(runtime::GetSystemErrorMessage());
        }
    }
}

void File::makeDirectory(bool recursive, uid_t uid, gid_t gid)
{
    if (recursive) {
        makeBaseDirectory(uid, gid);
    }

    if (::mkdir(path.getPathname().c_str(), 0777) == -1) {
        throw runtime::Exception("mkdir failed in makefirectory: " + path.getPathname());
    }

    if ((uid | gid) != 0) {
        chown(uid, gid);
    }
}

void File::chown(uid_t uid, gid_t gid)
{
    if (::chown(path.getPathname().c_str(), uid, gid) != 0) {
        throw runtime::Exception(runtime::GetSystemErrorMessage());
    }
}

void File::chmod(mode_t mode)
{
    if (::chmod(path.getPathname().c_str(), mode) != 0) {
        throw runtime::Exception(runtime::GetSystemErrorMessage());
    }
}

void File::lock() const
{
    if (::flock(descriptor, LOCK_EX) == -1) {
        throw runtime::Exception(runtime::GetSystemErrorMessage());
    }
}

void File::unlock() const
{
    if (::flock(descriptor, LOCK_UN) == -1) {
        throw runtime::Exception(runtime::GetSystemErrorMessage());
    }
}

DirectoryIterator::DirectoryIterator()
    : directoryHandle(nullptr)
{
}

DirectoryIterator::DirectoryIterator(const std::string& dir)
    : directoryHandle(nullptr)
{
    reset(dir);
}

DirectoryIterator::DirectoryIterator(const Path& dir)
    : DirectoryIterator(dir.getPathname())
{
}

DirectoryIterator::~DirectoryIterator()
{
    if (directoryHandle != nullptr) {
        ::closedir(directoryHandle);
    }
}

void DirectoryIterator::reset(const std::string& dir)
{
    if (directoryHandle != nullptr) {
        ::closedir(directoryHandle);
        directoryHandle = nullptr;
    }

    basename = dir;
    directoryHandle = ::opendir(basename.c_str());
    if (directoryHandle == nullptr) {
        throw runtime::Exception(runtime::GetSystemErrorMessage());
    }

    next();
}

void DirectoryIterator::next()
{
    std::string name;
    struct dirent entry, *ent;

    while (1) {
        if (readdir_r(directoryHandle, &entry, &ent) != 0) {
            throw runtime::Exception(runtime::GetSystemErrorMessage());
        }

        if (ent == NULL)
            break;

        if (ent->d_name[0] == '.' && ent->d_name[1] == '\0') {
            continue;
        }

        if (ent->d_name[0] == '.' &&
                ent->d_name[1] == '.' && ent->d_name[2] == '\0') {
            continue;
        }

        name = basename + "/" + std::string(ent->d_name);
        break;
    }

    current = name;
}

DirectoryIterator& DirectoryIterator::operator=(const std::string& dir)
{
    reset(dir);
    return *this;
}

DirectoryIterator& DirectoryIterator::operator=(const Path& dir)
{
    reset(dir.getPathname());
    return *this;
}

DirectoryIterator& DirectoryIterator::operator++()
{
    next();
    return *this;
}

static const struct mountOption {
    const char* name;
    int clear;
    int flag;
} mountOptions[] = {
    { "defaults",      0, 0                },
    { "ro",            0, MS_RDONLY        },
    { "rw",            1, MS_RDONLY        },
    { "suid",          1, MS_NOSUID        },
    { "nosuid",        0, MS_NOSUID        },
    { "dev",           1, MS_NODEV         },
    { "nodev",         0, MS_NODEV         },
    { "exec",          1, MS_NOEXEC        },
    { "noexec",        0, MS_NOEXEC        },
    { "sync",          0, MS_SYNCHRONOUS   },
    { "async",         1, MS_SYNCHRONOUS   },
    { "dirsync",       0, MS_DIRSYNC       },
    { "remount",       0, MS_REMOUNT       },
    { "mand",          0, MS_MANDLOCK      },
    { "nomand",        1, MS_MANDLOCK      },
    { "atime",         1, MS_NOATIME       },
    { "noatime",       0, MS_NOATIME       },
    { "diratime",      1, MS_NODIRATIME    },
    { "nodiratime",    0, MS_NODIRATIME    },
    { "bind",          0, MS_BIND          },
    { "rbind",         0, MS_BIND | MS_REC },
    { "relatime",      0, MS_RELATIME      },
    { "norelatime",    1, MS_RELATIME      },
    { "strictatime",   0, MS_STRICTATIME   },
    { "nostrictatime", 1, MS_STRICTATIME   },
    { NULL,            0, 0                },
};

static unsigned long parseMountOptions(const std::string& opts, std::string& mntdata)
{
    std::stringstream optsSstream(opts);
    const struct mountOption* mo;
    unsigned long mntflags = 0L;
    std::string opt;

    while (std::getline(optsSstream, opt, ',')) {
        for (mo = mountOptions; mo->name != NULL; mo++) {
            if (opt == mo->name) {
                if (mo->clear) {
                    mntflags &= ~mo->flag;
                } else {
                    mntflags |= mo->flag;
                }
                break;
            }
            if (mo->name != NULL) {
                continue;
            }
            if (!mntdata.empty()) {
                mntdata += ",";
            }
            mntdata += opt;
        }
    }

    return mntflags;
}

void Mount::mountEntry(const std::string& src, const std::string& dest, const std::string& type, const std::string& opts)
{
    int ret;
    unsigned long mntflags;
    std::string mntdata;

    mntflags = parseMountOptions(opts, mntdata);

    ret = ::mount(src.c_str(),
                  dest.c_str(),
                  type.c_str(),
                  mntflags & ~MS_REMOUNT,
                  mntdata.c_str());

    if (ret) {
        if ((mntflags & MS_REMOUNT) || (mntflags & MS_BIND)) {
            ret = ::mount(src.c_str(),
                          dest.c_str(),
                          type.c_str(),
                          mntflags | MS_REMOUNT,
                          mntdata.c_str());
        }

        if (ret) {
            throw runtime::Exception("failed to mount " + src + " on " + dest);
        }
    }
}

void Mount::mountEcryptfsEntry(const std::string& src, const std::string& keyName)
{
    std::string key, opts;
    int rc;

    if (!key::Manager::isKeyExist(keyName)) {
        std::string pass, salt, wrappedKey;

        key::Generator::initialize();
        pass = key::Generator::generateRandomKey(ECRYPTFS_MAX_KEY_SIZE);
        salt = key::Generator::generateRandomKey(ECRYPTFS_MAX_SALT_SIZE);
        wrappedKey = key::Generator::wrapKey(pass, salt, ECRYPTFS_MAX_KEY_HEX);
        key::Manager::addKey(keyName, wrappedKey);
    }

    key = key::Manager::getKey(keyName);

    rc = key::Ring::link(KEY_SPEC_USER_KEYRING, KEY_SPEC_SESSION_KEYRING);
    if (rc != 0) {
        throw runtime::Exception("failed to validate keyring");
    }

    rc = key::Ring::search(KEY_SPEC_USER_KEYRING, "user", key.c_str(), 0);
    if (rc != 0) {
        ecryptfs_payload authTok;
        struct ecryptfs_password* tokenKey;
        tokenKey = &authTok.token.password;

        memset(&authTok, 0, sizeof(ecryptfs_payload));

        authTok.version = ECRYPTFS_VERSION;
        authTok.token_type = ECRYPTFS_PWD_PAYLOAD_TYPE;
        tokenKey->session_key_encryption_key_bytes = ECRYPTFS_MAX_KEY_SIZE;
        tokenKey->flags = ECRYPTFS_SESSION_KEY_ENCRYPTION_KEY_SET;
        memcpy(tokenKey->session_key_encryption_key, key.c_str(), ECRYPTFS_MAX_KEY_SIZE);
        memcpy(tokenKey->signature, key.c_str(), ECRYPTFS_MAX_SIG_HEX);

        rc = key::Ring::add("user", key.c_str(),
                            (void*)&authTok, sizeof(ecryptfs_payload),
                            KEY_SPEC_USER_KEYRING);
        if (rc != 0) {
            throw runtime::Exception("failed to add key to keyring");
        }
    }

    opts = "ecryptfs_passthrough,"
           "ecryptfs_cipher=aes,"
           "ecryptfs_key_bytes=32,"
           "ecryptfs_sig=" + key +  ","
           "smackfsroot=*,smackfsdef=*";

    rc = ::mount(src.c_str(), src.c_str(), "ecryptfs", MS_NODEV, opts.c_str());
    if (rc != 0)
        throw runtime::Exception("Failed to ecryptfs mount");
}

void Mount::umountEntry(const std::string& dest)
{
    int ret;

    ret = umount(dest.c_str());
    if (ret != 0) {
        ret = umount2(dest.c_str(), MNT_EXPIRE);
        if (ret != 0 || errno == EAGAIN) {
            ret = umount2(dest.c_str(), MNT_EXPIRE);
            ret = (ret != 0) ? -errno : 0;
        } else {
            ret = 0;
        }
    }

    if (ret != 0) {
        ::sync();
        ret = umount2(dest.c_str(), MNT_DETACH);
    }

    if (ret != 0)
        throw runtime::Exception("Failed to unmount");
}

} // namespace runtime
