// tests/test_cache.cpp
#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>
#include "../src/cache.h"

void test_set_and_get() {
    Cache cache;
    cache.set("abc123", "https://www.google.com");
    auto result = cache.get("abc123");
    assert(result.has_value());
    assert(*result == "https://www.google.com");
    std::cout << "[PASS] set and get\n";
}

void test_cache_miss() {
    Cache cache;
    auto result = cache.get("notexist");
    assert(!result.has_value());
    std::cout << "[PASS] cache miss returns nullopt\n";
}

void test_invalidate() {
    Cache cache;
    cache.set("key1", "https://example.com");
    cache.invalidate("key1");
    assert(!cache.get("key1").has_value());
    std::cout << "[PASS] invalidate removes key\n";
}

void test_ttl_expiry() {
    Cache cache(1);  // 1-second TTL
    cache.set("ttlkey", "https://expiring.com");
    assert(cache.get("ttlkey").has_value());
    std::this_thread::sleep_for(std::chrono::seconds(2));
    assert(!cache.get("ttlkey").has_value());
    std::cout << "[PASS] TTL expiry\n";
}

void test_hit_miss_stats() {
    Cache cache;
    cache.set("s1", "https://a.com");
    cache.get("s1");  // hit
    cache.get("s1");  // hit
    cache.get("nope"); // miss
    auto stats = cache.getStats();
    assert(stats.hits == 2);
    assert(stats.misses == 1);
    std::cout << "[PASS] hit/miss stats\n";
}

int main() {
    test_set_and_get();
    test_cache_miss();
    test_invalidate();
    test_ttl_expiry();
    test_hit_miss_stats();
    std::cout << "\nAll cache tests passed!\n";
    return 0;
}
