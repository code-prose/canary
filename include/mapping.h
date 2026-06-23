#pragma once
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <iostream>

class Mapping {
    public:
        Mapping(const std::string& path) {
            fd_ = open(path.c_str(), O_RDONLY);
            if (fd_ == -1) {
                throw std::system_error(errno, std::generic_category(), "open " + path);
            }

            struct stat st;
            if (fstat(fd_, &st) == -1) {
                throw std::system_error(errno, std::generic_category(), "fstat");
            }
            len_ = static_cast<std::size_t>(st.st_size);

            mmap_ = static_cast<char*>(mmap(NULL, len_, PROT_READ, MAP_PRIVATE, fd_, 0));

            if (mmap_ == MAP_FAILED) {
                std::cerr << "Failed to memory map file\n";
            }

        }

        Mapping(const Mapping& rhs) = delete;
        Mapping& operator=(const Mapping& rhs) = delete;

        ~Mapping() {
            close(fd_);
            munmap(mmap_, len_);
            // tear down mmap
        }

        char* data() const { return mmap_; }
        std::size_t length() const { return len_; }
    private:
        char* mmap_;
        std::size_t len_{0};
        int fd_{-1};
};
