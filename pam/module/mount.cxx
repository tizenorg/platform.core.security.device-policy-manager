/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Contact: Sungbae Yoo <sungbae.yoo@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/**
 * @file
 * @author  Sungbae Yoo (sungbae.yoo@samsung.com)
 * @brief   Implementation to process fstab file for mounts in mnt namespace
 */

#include <string>
#include <sstream>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mount.h>

#include "exception.hxx"
#include "mount.hxx"

namespace Runtime {

static struct mountOption {
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

static unsigned long parseMountOptions(std::string& opts, std::string& mntdata)
{
    std::stringstream opts_sstream(opts);
    unsigned long mntflags = 0L;
    struct mountOption* mo;
    std::string opt;

    while (std::getline(opts_sstream, opt, ',')) {
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

void Mount::mountEntry(std::string src, std::string dest, std::string type, std::string opts, bool optional)
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

        if (ret && !optional) {
            throw Runtime::Exception("failed to mount " + src + " on " + dest);
        }
    }
}

} // namespace Runtime
