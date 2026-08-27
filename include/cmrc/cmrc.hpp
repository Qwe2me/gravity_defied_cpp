// cmrc/cmrc.hpp - заглушка для сборки на Miyoo CFW
#pragma once
#include <string>
#include <vector>

namespace cmrc {
    class embedded_file {
    public:
        embedded_file(const char* data, size_t size) : data_(data), size_(size) {}
        const char* data() const { return data_; }
        size_t size() const { return size_; }
    private:
        const char* data_;
        size_t size_;
    };

    class embedded_filesystem {
    public:
        embedded_file open(const std::string& path) const {
            static const char empty[] = "";
            return embedded_file(empty, 0);
        }
    };
}

#define CMRC_HEADER_ONLY
