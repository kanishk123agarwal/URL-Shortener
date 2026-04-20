// tests/test_shortener.cpp
#include <cassert>
#include <iostream>
#include <set>
#include <cctype>
#include "../src/shortener.h"

void test_encode_decode_roundtrip() {
    for (int64_t i : {1LL, 100LL, 9999LL, 123456LL, 999999999LL}) {
        assert(Shortener::decode(Shortener::encode(i)) == i);
    }
    std::cout << "[PASS] encode/decode roundtrip\n";
}

void test_short_code_is_alphanumeric() {
    std::string code = Shortener::generateShortCode(1);
    for (char c : code)
        assert(std::isalnum(static_cast<unsigned char>(c)));
    std::cout << "[PASS] short code is alphanumeric\n";
}

void test_different_ids_give_different_codes() {
    std::set<std::string> codes;
    for (int64_t i = 1; i <= 100; ++i)
        codes.insert(Shortener::generateShortCode(i));
    assert(codes.size() == 100);
    std::cout << "[PASS] different IDs produce unique codes\n";
}

void test_encode_zero() {
    assert(Shortener::encode(0) == "0");
    std::cout << "[PASS] encode(0) == '0'\n";
}

int main() {
    test_encode_decode_roundtrip();
    test_short_code_is_alphanumeric();
    test_different_ids_give_different_codes();
    test_encode_zero();
    std::cout << "\nAll shortener tests passed!\n";
    return 0;
}
