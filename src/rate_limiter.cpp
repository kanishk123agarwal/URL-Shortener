// src/rate_limiter.cpp
#include "rate_limiter.h"
#include <cstdlib>

RateLimiter::RateLimiter(int max_per_window, int window_seconds)
    : max_per_window_(max_per_window), window_seconds_(window_seconds)
{
    // Allow environment variable override for compatibility
    const char* env = std::getenv("RATE_LIMIT_PER_MINUTE");
    if (env) {
        max_per_window_ = std::atoi(env);
    }
}

bool RateLimiter::allow(const std::string& ip) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto now = std::chrono::steady_clock::now();

    auto& win = windows_[ip];

    // New window or expired window — reset counter and reset time
    if (win.count == 0 || now >= win.reset_at) {
        win.count = 1;
        win.reset_at = now + std::chrono::seconds(window_seconds_);
        return true;
    }

    ++win.count;
    return win.count <= max_per_window_;
}
