// src/url_repository.h
#ifndef URL_REPOSITORY_H
#define URL_REPOSITORY_H

#include <string>
#include <optional>
#include "models.h"

class URLRepository {
public:
    virtual ~URLRepository() = default;

    virtual std::optional<URLRecord> createShortURL(
        const std::string& original_url,
        const std::string& custom_alias = "",
        int expires_in_days = 0,
        std::optional<int64_t> user_id = std::nullopt
    ) = 0;

    virtual std::optional<URLRecord> getURLByCode(const std::string& code) = 0;
    virtual bool incrementHits(int64_t url_id) = 0;
};

#endif // URL_REPOSITORY_H
