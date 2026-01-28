cmake_minimum_required(VERSION 3.20)
project(OlympiaCodingChallenge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(describe_object 
    main.cpp
)

target_include_directories(describe_object PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/shapes
)

target_compile_options(describe_object PRIVATE -Wall -Wextra -Wpedantic)
