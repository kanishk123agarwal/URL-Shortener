// tests/test_rate_limiter.cpp
#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>
#include "../src/rate_limiter.h"

void test_rate_limiter_allowance() {
    RateLimiter limiter(3, 1); // 3 requests per 1 second
    assert(limiter.allow("127.0.0.1"));
    assert(limiter.allow("127.0.0.1"));
    assert(limiter.allow("127.0.0.1"));
    assert(!limiter.allow("127.0.0.1")); // 4th request blocked
    std::cout << "[PASS] allowance and blocking\n";
}

void test_rate_limiter_reset() {
    RateLimiter limiter(2, 1); // 2 requests per 1 second
    assert(limiter.allow("127.0.0.2"));
    assert(limiter.allow("127.0.0.2"));
    assert(!limiter.allow("127.0.0.2")); // blocked

    // Sleep for 1.2 seconds to cross the window boundary
    std::this_thread::sleep_for(std::chrono::milliseconds(1200));

    assert(limiter.allow("127.0.0.2")); // allowed again
    std::cout << "[PASS] window reset after expiry\n";
}

void test_rate_limiter_different_ips() {
    RateLimiter limiter(1, 60); // 1 request per minute
    assert(limiter.allow("127.0.0.3"));
    assert(!limiter.allow("127.0.0.3"));
    assert(limiter.allow("127.0.0.4")); // different IP allowed
    std::cout << "[PASS] isolated IP limits\n";
}

int main() {
    test_rate_limiter_allowance();
    test_rate_limiter_reset();
    test_rate_limiter_different_ips();
    std::cout << "\nAll rate limiter tests passed!\n";
    return 0;
}
