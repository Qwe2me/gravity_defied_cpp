#pragma once

#include "FileStream.h"
#include <fstream>
#include <string>

class EmbedFileStream : public FileStream {
public:
    EmbedFileStream(const std::string& embedFilePath);
    virtual void setPos(std::streampos pos) override;
    virtual bool isOpen() override;

private:
    virtual void read_impl(char* s, std::streamsize n) override;
    virtual void write_impl(char* s, std::streamsize n) override;

    std::ifstream fileStream;
    bool open = false;
};
