// src/main.cpp
#include "crow/app.h"
#include "sqlite_url_repository.h"
#include "cache.h"
#include "rate_limiter.h"
#include "index_html.h"
#include <cstdlib>
#include <string>
#include <optional>
#include <iostream>

int main() {
    const char* db_path = std::getenv("DB_PATH") ? std::getenv("DB_PATH") : "urls.db";
    const char* base_url = std::getenv("BASE_URL") ? std::getenv("BASE_URL") : "http://localhost:8080";

    SQLiteURLRepository repo(db_path);
    Cache cache;
    RateLimiter limiter;

    crow::SimpleApp app;

    // ── GET / (Frontend) ──
    CROW_ROUTE(app, "/")
    ([]() {
        crow::response res;
        res.set_header("Content-Type", "text/html");
        res.write(INDEX_HTML);
        return res;
    });

    // ── GET /health ──
    CROW_ROUTE(app, "/health")
    ([]() {
        return crow::response(200, "{\"status\":\"ok\"}");
    });

    // ── POST /shorten ──
    CROW_ROUTE(app, "/shorten").methods("POST"_method)
    ([&repo, &limiter, base_url](const crow::request& req) {
        // Enforce rate limiting by IP
        auto ip = req.remote_ip_address;
        if (!limiter.allow(ip)) {
            return crow::response(429, "{\"error\":\"Rate limit exceeded\"}");
        }

        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "{\"error\":\"Invalid JSON payload\"}");
        }

        std::string original_url;
        if (body.has("original_url")) {
            original_url = body["original_url"].s();
        } else if (body.has("long_url")) {
            original_url = body["long_url"].s();
        } else {
            return crow::response(422, "{\"error\":\"original_url is required\"}");
        }

        std::string custom_alias = body.has("custom_alias")
            ? std::string(body["custom_alias"].s()) : "";
        int expires_in_days = body.has("expires_in_days")
            ? body["expires_in_days"].i() : 0;

        // Basic URL validation
        if (original_url.length() < 4 || original_url.substr(0, 4) != "http") {
            return crow::response(422, "{\"error\":\"Invalid URL — must start with http\"}");
        }

        if (!custom_alias.empty()) {
            for (char c : custom_alias) {
                if (!std::isalnum(static_cast<unsigned char>(c))) {
                    return crow::response(400, "{\"error\":\"Custom alias must be alphanumeric\"}");
                }
            }
        }

        auto record = repo.createShortURL(original_url, custom_alias, expires_in_days);
        if (!record.has_value()) {
            return crow::response(500, "{\"error\":\"Failed to create short URL\"}");
        }

        crow::json::wvalue res;
        res["short_url"] = std::string(base_url) + "/" + record->short_code;
        res["short_code"] = record->short_code;
        res["original_url"] = record->original_url;
        res["created_at"] = static_cast<int64_t>(record->created_at);
        return crow::response(200, res);
    });

    // ── GET /stats (Combined DB + Cache stats) — MUST be before /<string> ──
    CROW_ROUTE(app, "/stats")
    ([&repo, &cache]() {
        auto dbStats = repo.getGlobalStats();
        auto cacheStats = cache.getStats();
        int64_t total = cacheStats.hits + cacheStats.misses;
        double hit_rate = total > 0 ? (100.0 * cacheStats.hits / total) : 0.0;

        crow::json::wvalue res;
        res["total_urls"]   = dbStats.total_urls;
        res["total_hits"]   = dbStats.total_hits;
        res["cache_hits"]   = cacheStats.hits;
        res["cache_misses"] = cacheStats.misses;
        res["hit_rate"]     = hit_rate;
        return crow::response(200, res);
    });

    // ── GET /cache/stats — MUST be before /<string> ──
    CROW_ROUTE(app, "/cache/stats")
    ([&cache]() {
        auto stats = cache.getStats();
        int64_t total = stats.hits + stats.misses;
        double hit_rate = total > 0 ? (100.0 * stats.hits / total) : 0.0;

        crow::json::wvalue res;
        res["hits"]     = stats.hits;
        res["misses"]   = stats.misses;
        res["hit_rate"] = hit_rate;
        return crow::response(200, res);
    });

    // ── GET /analytics/{code} — MUST be before /<string> ──
    CROW_ROUTE(app, "/analytics/<string>")
    ([&repo](std::string code) {
        auto record = repo.getURLByCode(code);
        if (!record.has_value()) {
            return crow::response(404, "{\"error\":\"Short code not found\"}");
        }

        crow::json::wvalue res;
        res["short_code"]     = record->short_code;
        res["original_url"]   = record->original_url;
        res["hit_count"]      = record->hit_count;
        res["created_at"]     = static_cast<int64_t>(record->created_at);
        res["is_custom_alias"]= record->is_custom_alias;
        return crow::response(200, res);
    });

    // ── GET /{code} (Redirect) — wildcard, MUST be last ──
    CROW_ROUTE(app, "/<string>")
    ([&repo, &cache](const crow::request& /*req*/, std::string code) {
        // Ignore browser auto-requests and guard known routes
        if (code == "favicon.ico" || code == "robots.txt" ||
            code == "stats"       || code == "health"     ||
            code == "shorten"     || code == "analytics"  ||
            code == "cache") {
            return crow::response(404);
        }

        // 1. Check in-memory Cache first
        auto cached = cache.get(code);
        if (cached.has_value()) {
            // Increment hit counter even on a cache hit
            auto record = repo.getURLByCode(code);
            if (record.has_value()) {
                repo.incrementHits(record->id);
            }
            crow::response res(302);
            res.add_header("Location", *cached);
            return res;
        }

        // 2. Cache Miss - Query Database
        auto record = repo.getURLByCode(code);
        if (!record.has_value()) {
            return crow::response(404, "{\"error\":\"Short code not found\"}");
        }

        // Check expiry
        if (record->expires_at > 0 && std::time(nullptr) > record->expires_at) {
            return crow::response(410, "{\"error\":\"This link has expired\"}");
        }

        // 3. Populate Cache for next request
        cache.set(code, record->original_url);

        // 4. Increment hit counter
        repo.incrementHits(record->id);

        // 302 = Temporary Redirect
        crow::response res(302);
        res.add_header("Location", record->original_url);
        return res;
    });
    std::cout << "\n============================================\n";
    std::cout << "🚀 URL-shortener Service is Running!\n";
    std::cout << "👉 Please open: http://localhost:8080\n";
    std::cout << "⚠️  DO NOT use: http://0.0.0.0:8080 (browsers block API fetch requests to 0.0.0.0 for security).\n";
    std::cout << "============================================\n\n";

    // Read PORT from env (Railway injects $PORT; fallback to 8080 locally)
    const char* port_env = std::getenv("PORT");
    int port = port_env ? std::stoi(std::string(port_env)) : 8080;

    app.port(port).multithreaded().run();
    return 0;
}
