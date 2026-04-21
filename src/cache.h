// src/cache.h
#ifndef CACHE_H
#define CACHE_H

#include <string>
#include <optional>
#include <unordered_map>
#include <mutex>
#include <chrono>
#include <cstdint>

struct CacheEntry {
    std::string original_url;
    std::chrono::steady_clock::time_point expires_at;
};

class Cache {
public:
    explicit Cache(int ttl_seconds = 3600);

    std::optional<std::string> get(const std::string& short_code);
    void set(const std::string& short_code, const std::string& original_url);
    void invalidate(const std::string& short_code);

    struct Stats {
        int64_t hits = 0;
        int64_t misses = 0;
    };
    Stats getStats() const;

private:
    std::unordered_map<std::string, CacheEntry> store_;
    mutable std::mutex mutex_;
    int ttl_seconds_;
    Stats stats_;
};

#endif // CACHE_H
