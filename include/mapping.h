#pragma once
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <filesystem>

class Mapping {
    public:
        char* mmap_;
        Mapping(const std::string& path) {
            fd_ = open(path.c_str(), O_RDONLY);
            if (fd_ == -1) {
                // ?
                std::terminate();
            }

            len_ = std::filesystem::file_size(path);
            mmap_ = static_cast<char*>(mmap(NULL, len_, PROT_READ, MAP_SHARED, fd_, 0));
        }

        ~Mapping() {
            close(fd_);
            munmap(mmap_, len_);
            // tear down mmap
        }
    private:
        int fd_{-1};
        std::uintmax_t len_{0};
};
