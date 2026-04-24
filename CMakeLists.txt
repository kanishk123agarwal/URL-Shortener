cmake_minimum_required(VERSION 3.15)
project(url_shortener CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SQLite3
find_package(SQLite3 REQUIRED)

# Include directories
include_directories(include src)

# Main Executable
add_executable(url_shortener
    src/main.cpp
    src/cache.cpp
    src/rate_limiter.cpp
    src/shortener.cpp
    src/sqlite_url_repository.cpp
)
target_link_libraries(url_shortener SQLite::SQLite3 pthread)

# Build tests
add_executable(test_shortener
    tests/test_shortener.cpp
    src/shortener.cpp
)
target_link_libraries(test_shortener pthread)

add_executable(test_database
    tests/test_repository.cpp
    src/shortener.cpp
    src/sqlite_url_repository.cpp
)
target_link_libraries(test_database SQLite::SQLite3 pthread)

add_executable(test_cache
    tests/test_cache.cpp
    src/cache.cpp
)
target_link_libraries(test_cache pthread)

add_executable(test_rate_limiter
    tests/test_rate_limiter.cpp
    src/rate_limiter.cpp
)
target_link_libraries(test_rate_limiter pthread)
