cmake_minimum_required(VERSION 3.10)

project(pix28 VERSION 1.0)

include_directories(include)

find_package(OpenSSL REQUIRED)

add_executable(out
    src/main.cpp
)

target_link_libraries(out PRIVATE OpenSSL::SSL OpenSSL::Crypto)