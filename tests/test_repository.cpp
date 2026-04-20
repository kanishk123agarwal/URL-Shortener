// tests/test_repository.cpp
#include <cassert>
#include <iostream>
#include <memory>
#include "../src/sqlite_url_repository.h"

void test_create_and_get() {
    std::unique_ptr<URLRepository> repo = std::make_unique<SQLiteURLRepository>(":memory:");
    auto record = repo->createShortURL("https://www.google.com");
    assert(record.has_value());
    assert(record->original_url == "https://www.google.com");
    assert(!record->short_code.empty());
    assert(record->is_custom_alias == false);
    assert(!record->user_id.has_value());

    auto fetched = repo->getURLByCode(record->short_code);
    assert(fetched.has_value());
    assert(fetched->id == record->id);
    assert(fetched->original_url == "https://www.google.com");
    std::cout << "[PASS] create and get URL via repository\n";
}

void test_dedup_same_original_url() {
    std::unique_ptr<URLRepository> repo = std::make_unique<SQLiteURLRepository>(":memory:");
    auto r1 = repo->createShortURL("https://example.com");
    auto r2 = repo->createShortURL("https://example.com");
    assert(r1.has_value() && r2.has_value());
    assert(r1->short_code == r2->short_code);
    std::cout << "[PASS] duplicate original URL returns same code\n";
}

void test_increment_hits() {
    std::unique_ptr<URLRepository> repo = std::make_unique<SQLiteURLRepository>(":memory:");
    auto record = repo->createShortURL("https://example.org");
    assert(record.has_value());
    repo->incrementHits(record->id);
    repo->incrementHits(record->id);
    auto updated = repo->getURLByCode(record->short_code);
    assert(updated->hit_count == 2);
    std::cout << "[PASS] hit increment via repository\n";
}

void test_nonexistent_code() {
    std::unique_ptr<URLRepository> repo = std::make_unique<SQLiteURLRepository>(":memory:");
    auto result = repo->getURLByCode("xxxxxx");
    assert(!result.has_value());
    std::cout << "[PASS] nonexistent code returns nullopt\n";
}

void test_custom_alias() {
    std::unique_ptr<URLRepository> repo = std::make_unique<SQLiteURLRepository>(":memory:");
    auto record = repo->createShortURL("https://github.com", "gh");
    assert(record.has_value());
    assert(record->short_code == "gh");
    assert(record->is_custom_alias == true);

    auto fetched = repo->getURLByCode("gh");
    assert(fetched.has_value());
    assert(fetched->original_url == "https://github.com");
    std::cout << "[PASS] custom alias creation and fetch\n";
}

int main() {
    test_create_and_get();
    test_dedup_same_original_url();
    test_increment_hits();
    test_nonexistent_code();
    test_custom_alias();
    std::cout << "\nAll repository tests passed!\n";
    return 0;
}
