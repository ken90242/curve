/*
 *  Copyright (c) 2020 NetEase Inc.
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
 *  limitations under the License.
 */

/*
 * Project: curve
 * Created Date: 21-08-13
 * Author: hzwuhongsong
 */

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>

#include "curvefs/src/client/s3/disk_cache_base.h"

namespace curvefs {

namespace client {

#define CACHE_WRITE_DIR "cachewrite"
#define CACHE_READ_DIR "cacheread"

void DiskCacheBase::Init(std::shared_ptr<PosixWrapper> wrapper,
                      const std::string cacheDir) {
    cacheDir_ = cacheDir;
    posixWrapper_ = wrapper;
}

int DiskCacheBase::CreateIoDir(bool writreDir) {
    struct stat statFile;
    bool ret;
    std::string FullDirPath;

    if (writreDir) {
        cacheIoDir_ = CACHE_WRITE_DIR;
    } else {
        cacheIoDir_ = CACHE_READ_DIR;
    }
    FullDirPath = cacheDir_ + "/" + cacheIoDir_;
    ret = IsFileExist(FullDirPath);
    if (!ret) {
        if (posixWrapper_->mkdir(FullDirPath.c_str(), 0755) < 0) {
            LOG(ERROR) << "create cache dir error. errno = " << errno
                       << ", dir = " << FullDirPath;
            return -1;
        }
        LOG(INFO) << "read cache dir is not exist, create it success."
                     << ", dir = " << FullDirPath;
    } else {
        LOG(INFO) << "read cache dir is exist."
                     << ", dir = " << FullDirPath;
    }
    return 0;
}

bool DiskCacheBase::IsFileExist(const std::string file) {
    struct stat statFile;
    int ret;
    ret = posixWrapper_->stat(file.c_str(), &statFile);
    if (ret < 0) {
        LOG(INFO) << "file is not exist, dir = " << file
                     << ", errno = " << errno;
        return false;
    }
    LOG(INFO) << "file is exist, dir = " << file;
    return true;
}

std::string DiskCacheBase::GetCacheIoFullDir() {
    std::string fullPath;
    fullPath = cacheDir_ + "/" + cacheIoDir_;
    return fullPath;
}

}  // namespace client
}  // namespace curvefs
