// src/sqlite_url_repository.h
#ifndef SQLITE_URL_REPOSITORY_H
#define SQLITE_URL_REPOSITORY_H

#include "url_repository.h"
#include <sqlite3.h>

class SQLiteURLRepository : public URLRepository {
public:
    explicit SQLiteURLRepository(const std::string& db_path);
    ~SQLiteURLRepository() override;

    // Prevent copy operations
    SQLiteURLRepository(const SQLiteURLRepository&) = delete;
    SQLiteURLRepository& operator=(const SQLiteURLRepository&) = delete;

    void initSchema();

    std::optional<URLRecord> createShortURL(
        const std::string& original_url,
        const std::string& custom_alias = "",
        int expires_in_days = 0,
        std::optional<int64_t> user_id = std::nullopt
    ) override;

    std::optional<URLRecord> getURLByCode(const std::string& code) override;
    bool incrementHits(int64_t url_id) override;

    struct GlobalStats {
        int64_t total_urls = 0;
        int64_t total_hits = 0;
    };
    GlobalStats getGlobalStats();

private:
    sqlite3* db_;
    URLRecord rowToRecord(sqlite3_stmt* stmt);
};

#endif // SQLITE_URL_REPOSITORY_H
