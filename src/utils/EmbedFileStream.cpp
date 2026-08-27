#include "EmbedFileStream.h"

#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>  // Для отладки

// Удаляем CMRC_DECLARE(assets);

EmbedFileStream::EmbedFileStream(const std::string& embedFileName)
    : FileStream()
{
    // Формируем путь к файлу на SD-карте (папка assets/)
    std::string fullPath = "assets/" + embedFileName;
    
    // Открываем файл в бинарном режиме
    fileStream.open(fullPath, std::ios::binary);
    open = fileStream.is_open();
    
    if (!open) {
        // Если файл не найден - пробуем без папки assets/
        fileStream.open(embedFileName, std::ios::binary);
        open = fileStream.is_open();
    }
    
    if (!open) {
        // Для отладки: можно вывести сообщение
        // std::cerr << "Failed to open: " << fullPath << std::endl;
    }
}

void EmbedFileStream::setPos(std::streampos pos)
{
    if (open) {
        fileStream.seekg(pos, std::ios::beg);
    }
}

void EmbedFileStream::read_impl(char* s, std::streamsize n)
{
    if (open) {
        fileStream.read(s, n);
    }
}

void EmbedFileStream::write_impl([[maybe_unused]] char* s, [[maybe_unused]] std::streamsize n)
{
    throw std::runtime_error("Write to buffer not supported!");
}

bool EmbedFileStream::isOpen()
{
    return open;
}
