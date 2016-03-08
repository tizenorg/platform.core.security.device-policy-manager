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
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

#include <string>

#include "filesystem.hxx"
#include "error.hxx"
#include "exception.hxx"

namespace Runtime {

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
        throw Runtime::Exception(Runtime::GetSystemErrorMessage());
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
        throw Runtime::Exception(Runtime::GetSystemErrorMessage());
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
        throw Runtime::Exception(Runtime::GetSystemErrorMessage());
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
        throw Runtime::Exception(Runtime::GetSystemErrorMessage());
    }

    return S_ISLNK(st.st_mode);
}

bool File::isFile() const
{
    struct stat st;
    if (::stat(path.getPathname().c_str(), &st) != 0) {
        throw Runtime::Exception(Runtime::GetSystemErrorMessage());
    }

    return S_ISREG(st.st_mode);
}

bool File::isDirectory() const
{
    struct stat st;
    if (::stat(path.getPathname().c_str(), &st) != 0) {
        throw Runtime::Exception(Runtime::GetSystemErrorMessage());
    }

    return S_ISDIR(st.st_mode);
}

bool File::isDevice() const
{
    struct stat st;
    if (::stat(path.getPathname().c_str(), &st) != 0) {
        throw Runtime::Exception(Runtime::GetSystemErrorMessage());
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
        throw Runtime::Exception(Runtime::GetSystemErrorMessage());
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
            throw Runtime::Exception(Runtime::GetSystemErrorMessage());
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
            throw Runtime::Exception(Runtime::GetSystemErrorMessage());
        }
    } else {
        if (::unlink(path.getPathname().c_str()) != 0) {
            throw Runtime::Exception(Runtime::GetSystemErrorMessage());
        }
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
        throw Runtime::Exception(Runtime::GetSystemErrorMessage());
    }

    next();
}

void DirectoryIterator::next()
{
    std::string name;
    struct dirent* ent;

    while ((ent = readdir(directoryHandle)) != NULL) {
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

} // namespace Runtime
