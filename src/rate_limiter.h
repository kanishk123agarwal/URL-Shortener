// src/rate_limiter.h
#ifndef RATE_LIMITER_H
#define RATE_LIMITER_H

#include <string>
#include <unordered_map>
#include <mutex>
#include <chrono>
#include <cstdint>

struct RateLimitWindow {
    int64_t count = 0;
    std::chrono::steady_clock::time_point reset_at;
};

class RateLimiter {
public:
    explicit RateLimiter(int max_per_window = 10, int window_seconds = 60);

    // Returns true if request is allowed, false if rate-limited
    bool allow(const std::string& ip);

private:
    int max_per_window_;
    int window_seconds_;
    std::unordered_map<std::string, RateLimitWindow> windows_;
    std::mutex mutex_;
};

#endif // RATE_LIMITER_H
