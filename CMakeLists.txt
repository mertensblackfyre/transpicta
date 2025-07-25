cmake_minimum_required(VERSION 3.10)
project(transpicta)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(spdlog REQUIRED)

add_executable(transpicta
    main.cc
)

target_include_directories(transpicta PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
    /usr/include   # for libharu headers on Arch
)

# Link to libraries
target_link_libraries(transpicta PRIVATE
    webp
    png
    jpeg
    spdlog::spdlog
)
