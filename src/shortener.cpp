// src/shortener.cpp
#include "shortener.h"
#include <algorithm>
#include <stdexcept>

const std::string Shortener::BASE62_CHARS =
    "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const int Shortener::BASE = 62;
const int64_t Shortener::OFFSET = 100000;

std::string Shortener::encode(int64_t num) {
    std::string result;
    do {
        result += BASE62_CHARS[num % BASE];
        num /= BASE;
    } while (num > 0);
    std::reverse(result.begin(), result.end());
    return result;
}

int64_t Shortener::decode(const std::string& code) {
    int64_t result = 0;
    for (char c : code) {
        size_t pos = BASE62_CHARS.find(c);
        result = (pos == std::string::npos) ?
            throw std::invalid_argument("Invalid Base62 character: " + std::string(1, c)) :
            (result * BASE + static_cast<int64_t>(pos));
    }
    return result;
}

std::string Shortener::generateShortCode(int64_t db_id) {
    return encode(db_id + OFFSET);
}
