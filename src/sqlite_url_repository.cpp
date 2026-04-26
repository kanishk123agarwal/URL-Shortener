// src/sqlite_url_repository.cpp
#include "sqlite_url_repository.h"
#include "shortener.h"
#include <stdexcept>
#include <cstring>
#include <ctime>

SQLiteURLRepository::SQLiteURLRepository(const std::string& db_path) {
    if (sqlite3_open(db_path.c_str(), &db_) != SQLITE_OK) {
        throw std::runtime_error("Cannot open database: " + std::string(sqlite3_errmsg(db_)));
    }
    // Enable foreign keys constraint enforcement
    char* err = nullptr;
    if (sqlite3_exec(db_, "PRAGMA foreign_keys = ON;", nullptr, nullptr, &err) != SQLITE_OK) {
        std::string msg = err ? err : "Unknown PRAGMA errmsg";
        if (err) sqlite3_free(err);
        throw std::runtime_error("Failed to enable foreign keys: " + msg);
    }
    initSchema();
}

SQLiteURLRepository::~SQLiteURLRepository() {
    if (db_) {
        sqlite3_close(db_);
    }
}

void SQLiteURLRepository::initSchema() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS users (
            id            INTEGER PRIMARY KEY AUTOINCREMENT,
            name          TEXT,
            email         TEXT UNIQUE,
            password_hash TEXT,
            created_at    INTEGER DEFAULT (strftime('%s', 'now'))
        );

        CREATE TABLE IF NOT EXISTS urls (
            id              INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id         INTEGER REFERENCES users(id) ON DELETE SET NULL,
            short_code      TEXT UNIQUE NOT NULL,
            original_url    TEXT NOT NULL,
            is_custom_alias INTEGER DEFAULT 0,
            created_at      INTEGER DEFAULT (strftime('%s', 'now')),
            expires_at      INTEGER DEFAULT 0,
            hit_count       INTEGER DEFAULT 0
        );
        CREATE INDEX IF NOT EXISTS idx_short_code ON urls(short_code);

        CREATE TABLE IF NOT EXISTS clicks (
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            url_id      INTEGER NOT NULL REFERENCES urls(id) ON DELETE CASCADE,
            clicked_at  INTEGER DEFAULT (strftime('%s', 'now')),
            ip_address  TEXT,
            country     TEXT,
            device_type TEXT,
            browser     TEXT
        );

        CREATE TABLE IF NOT EXISTS api_keys (
            id         INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id    INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
            api_key    TEXT UNIQUE NOT NULL,
            created_at INTEGER DEFAULT (strftime('%s', 'now')),
            expires_at INTEGER DEFAULT 0
        );

        CREATE TABLE IF NOT EXISTS rate_limits (
            id             INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id        INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
            requests_count INTEGER DEFAULT 0,
            window_start   INTEGER DEFAULT (strftime('%s', 'now'))
        );

        CREATE TABLE IF NOT EXISTS domains (
            id           INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id      INTEGER REFERENCES users(id) ON DELETE SET NULL,
            domain_name  TEXT UNIQUE NOT NULL,
            verified     INTEGER DEFAULT 0
        );
    )";

    char* err = nullptr;
    if (sqlite3_exec(db_, sql, nullptr, nullptr, &err) != SQLITE_OK) {
        std::string msg = err ? err : "Unknown database schema error";
        if (err) {
            sqlite3_free(err);
        }
        throw std::runtime_error("Schema error: " + msg);
    }
}

std::optional<URLRecord> SQLiteURLRepository::createShortURL(
    const std::string& original_url,
    const std::string& custom_alias,
    int expires_in_days,
    std::optional<int64_t> user_id)
{
    // Dedup check — return existing if original_url already shortened and NOT a custom alias
    {
        const char* sql = "SELECT id, user_id, short_code, original_url, is_custom_alias, created_at, expires_at, hit_count "
                          "FROM urls WHERE original_url = ? AND is_custom_alias = 0 LIMIT 1;";
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, original_url.c_str(), -1, SQLITE_STATIC);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                auto record = rowToRecord(stmt);
                sqlite3_finalize(stmt);
                return record;
            }
            sqlite3_finalize(stmt);
        }
    }

    time_t expires_at = 0;
    if (expires_in_days > 0) {
        expires_at = time(nullptr) + static_cast<time_t>(expires_in_days) * 86400;
    }

    // Insert with placeholder short_code
    const char* ins = R"(
        INSERT INTO urls (user_id, short_code, original_url, is_custom_alias, expires_at)
        VALUES (?, 'placeholder', ?, ?, ?);
    )";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, ins, -1, &stmt, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }

    if (user_id.has_value()) {
        sqlite3_bind_int64(stmt, 1, *user_id);
    } else {
        sqlite3_bind_null(stmt, 1);
    }
    sqlite3_bind_text(stmt, 2, original_url.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, custom_alias.empty() ? 0 : 1);
    sqlite3_bind_int64(stmt, 4, expires_at);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return std::nullopt;
    }
    sqlite3_finalize(stmt);

    int64_t new_id = sqlite3_last_insert_rowid(db_);

    std::string code = custom_alias.empty()
        ? Shortener::generateShortCode(new_id)
        : custom_alias;

    const char* upd = "UPDATE urls SET short_code = ? WHERE id = ?;";
    if (sqlite3_prepare_v2(db_, upd, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, code.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int64(stmt, 2, new_id);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    } else {
        return std::nullopt;
    }

    return getURLByCode(code);
}

std::optional<URLRecord> SQLiteURLRepository::getURLByCode(const std::string& code) {
    const char* sql = R"(
        SELECT id, user_id, short_code, original_url, is_custom_alias,
               created_at, expires_at, hit_count
        FROM urls WHERE short_code = ? LIMIT 1;
    )";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }

    sqlite3_bind_text(stmt, 1, code.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        auto record = rowToRecord(stmt);
        sqlite3_finalize(stmt);
        return record;
    }
    sqlite3_finalize(stmt);
    return std::nullopt;
}

bool SQLiteURLRepository::incrementHits(int64_t url_id) {
    const char* sql = "UPDATE urls SET hit_count = hit_count + 1 WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    sqlite3_bind_int64(stmt, 1, url_id);
    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return ok;
}

URLRecord SQLiteURLRepository::rowToRecord(sqlite3_stmt* stmt) {
    URLRecord r;
    r.id = sqlite3_column_int64(stmt, 0);

    if (sqlite3_column_type(stmt, 1) == SQLITE_NULL) {
        r.user_id = std::nullopt;
    } else {
        r.user_id = sqlite3_column_int64(stmt, 1);
    }

    const char* code_ptr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    r.short_code = code_ptr ? code_ptr : "";

    const char* url_ptr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    r.original_url = url_ptr ? url_ptr : "";

    r.is_custom_alias = (sqlite3_column_int(stmt, 4) != 0);
    r.created_at = sqlite3_column_int64(stmt, 5);
    r.expires_at = sqlite3_column_int64(stmt, 6);
    r.hit_count = sqlite3_column_int64(stmt, 7);
    return r;
}

SQLiteURLRepository::GlobalStats SQLiteURLRepository::getGlobalStats() {
    GlobalStats stats;
    const char* sql = "SELECT COUNT(*), COALESCE(SUM(hit_count), 0) FROM urls;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            stats.total_urls = sqlite3_column_int64(stmt, 0);
            stats.total_hits = sqlite3_column_int64(stmt, 1);
        }
        sqlite3_finalize(stmt);
    }
    return stats;
}
