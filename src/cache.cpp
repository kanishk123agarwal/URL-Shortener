// src/cache.cpp
#include "cache.h"

Cache::Cache(int ttl_seconds) : ttl_seconds_(ttl_seconds) {}

std::optional<std::string> Cache::get(const std::string& short_code) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = store_.find(short_code);
    if (it == store_.end()) {
        ++stats_.misses;
        return std::nullopt;
    }

    // Check TTL expiry
    if (std::chrono::steady_clock::now() > it->second.expires_at) {
        store_.erase(it);
        ++stats_.misses;
        return std::nullopt;
    }

    ++stats_.hits;
    return it->second.original_url;
}

void Cache::set(const std::string& short_code, const std::string& original_url) {
    std::lock_guard<std::mutex> lock(mutex_);
    store_[short_code] = {
        original_url,
        std::chrono::steady_clock::now() + std::chrono::seconds(ttl_seconds_)
    };
}

void Cache::invalidate(const std::string& short_code) {
    std::lock_guard<std::mutex> lock(mutex_);
    store_.erase(short_code);
}

Cache::Stats Cache::getStats() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return stats_;
}
