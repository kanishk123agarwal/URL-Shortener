// src/models.h
#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <ctime>
#include <cstdint>
#include <optional>

struct URLRecord {
    int64_t                id;
    std::optional<int64_t> user_id;
    std::string            short_code;
    std::string            original_url;
    bool                   is_custom_alias;
    time_t                 created_at;
    time_t                 expires_at;
    int64_t                hit_count;
};

#endif // MODELS_H
