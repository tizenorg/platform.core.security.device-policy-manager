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

#ifndef __RUNTIME_FILESYSTEM_H__
#define __RUNTIME_FILESYSTEM_H__

#include <dirent.h>

#include <string>

namespace runtime {

class Path {
public:
    Path();
    Path(const std::string& path);
    Path(const char* path);
    Path(const Path& path);

    ~Path();

    Path& operator=(const Path& path);
    Path& operator=(const std::string& path);
    Path& operator=(const char* path);

    bool operator==(const Path& path) const
    {
        return (pathname == path.pathname);
    }

    bool operator!=(const Path& path) const
    {
        return !(pathname == path.pathname);
    }

    bool isAbsolute() const
    {
        return (pathname[0] == '/');
    }

    bool isRelative() const
    {
        return !(pathname[0] == '/');
    }

    const std::string& getPathname() const
    {
        return pathname;
    }

private:
    Path& assign(const Path& path);
    Path& assign(const std::string& path);
    Path& assign(const char* path);

    std::string pathname;
};

class File {
public:
    File();
    File(const std::string& pathname);
    File(const File& file);
    File(const Path& filePath);

    ~File();

    File& operator=(const std::string& pathname)
    {
        path = pathname;
        return *this;
    }

    File& operator=(const File& file)
    {
        path = file.path;
        return *this;
    }

    File& operator=(const Path& filePath)
    {
        path = filePath;
        return *this;
    }

    bool operator<(const File& file) const;
    bool operator>(const File& file) const;
    bool operator<=(const File& file) const;
    bool operator>=(const File& file) const;

    bool operator==(const File& file) const
    {
        return (path == file.path);
    }

    bool operator!=(const File& file) const
    {
        return !(path == file.path);
    }

    bool exists() const;
    bool canRead() const;
    bool canWrite() const;
    bool canExecute() const;

    bool isLink() const;
    bool isFile() const;
    bool isDirectory() const;
    bool isDevice() const;
    bool isHidden() const;

    void open(int flags, mode_t mode);
    void close();
    void copyTo(const std::string& pathname) const;
    void moveTo(const std::string& pathname);
    void renameTo(const std::string& pathname);
    void remove(bool recursive = false);
    void makeDirectory(bool recursive = false);

    void chown(uid_t uid, gid_t gid);
    void chmod(mode_t mode);

    std::string toString() const;

    const std::string& getPath() const
    {
        return path.getPathname();
    }

private:
    int descriptor;
    Path path;
};

class DirectoryIterator {
public:
    DirectoryIterator();
    DirectoryIterator(const std::string& dir);
    DirectoryIterator(const Path& dir);

    ~DirectoryIterator();

    DirectoryIterator& operator=(const std::string& dir);
    DirectoryIterator& operator=(const Path& dir);
    DirectoryIterator& operator++();

    bool operator==(const DirectoryIterator& iterator) const
    {
        return (current == iterator.current);
    }

    bool operator!=(const DirectoryIterator& iterator) const
    {
        return !(current == iterator.current);
    }

    const File& operator*() const
    {
        return current;
    }

    File& operator*()
    {
        return current;
    }

    const File* operator->() const
    {
        return &current;
    }

    File* operator->()
    {
        return &current;
    }

private:
    void next();
    void reset(const std::string& dir);

    File current;
    DIR* directoryHandle;
    std::string basename;
};

class Mount final {
public:
    Mount() = delete;

    static void mountEntry(const std::string& src, const std::string& dest,
                           const std::string& type, const std::string& opts);
};

int Open(const std::string& path, int flags, mode_t mode);
void Close(int fd);

} // namespace runtime
#endif //__RUNTIME_FILESYSTEM_H__
