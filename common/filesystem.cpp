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
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

#include <string>
#include <sstream>

#include "filesystem.h"
#include "error.h"
#include "exception.h"
#include "cryptofs.h"

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

void File::open(int flags, mode_t mode)
{
    while (1) {
        descriptor = ::open(path.getPathname().c_str(), flags, mode);
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

void File::makeDirectory(bool recursive)
{
    if (recursive) {
        const std::string& pathStr = path.getPathname();
        for (unsigned int i = 0; i != std::string::npos;) {
            i = pathStr.find('/', i + 1);
            ::mkdir(pathStr.substr(0, i).c_str(), 0777);
        }
    } else if (::mkdir(path.getPathname().c_str(), 0777) != 0) {
        throw runtime::Exception(runtime::GetSystemErrorMessage());
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

void Mount::mountCryptoEntry(const std::string& src, const std::string& keyName)
{
    int rc;
    char* sig;
    Cryptofs cryptofs;

    if (cryptofs.keymngt.keySearch(keyName.c_str()) != 1) {
        try {
            cryptofs.keymngt.keyNew(keyName.c_str());
        } catch (runtime::Exception &e) {
            throw e;
        }
    }

    try {
        cryptofs.keymngt.keyPush(keyName.c_str());
        sig = cryptofs.keymngt.keyGetSig(keyName.c_str());
    } catch (runtime::Exception &e) {
        throw e;
    }

    rc = cryptofs.mountEcryptfs(src, sig);
    if (rc != 0)
        throw runtime::Exception("Failed to ecryptfs mount");
}

void Mount::umountCryptoEntry(const std::string& src)
{
    int ret;
    Cryptofs cryptofs;

    ret = cryptofs.umountEcryptfs(src.c_str());
    if (ret != 0)
        throw runtime::Exception("Failed to unmount error is " + std::to_string(ret));
}

} // namespace runtime
