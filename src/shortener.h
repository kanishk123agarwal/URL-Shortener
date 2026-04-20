// src/shortener.h
#ifndef SHORTENER_H
#define SHORTENER_H

#include <string>
#include <cstdint>

class Shortener {
private:
    static const std::string BASE62_CHARS;
    static const int BASE;
    static const int64_t OFFSET;

public:
    static std::string encode(int64_t num);
    static int64_t decode(const std::string& code);
    static std::string generateShortCode(int64_t db_id);
};

#endif // SHORTENER_H
