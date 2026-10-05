cmake_minimum_required(VERSION 3.16)
project(lab2 LANGUAGES CXX)

add_executable(lab2 main.cpp tasks.cpp tasks.h)
configure_file(glasses.txt glasses.txt COPYONLY)
set_target_properties(lab2 PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if(MSVC)
    target_compile_options(lab2 PRIVATE /W4 /utf8)
else()
    target_compile_options(lab2 PRIVATE -Wall -Wextra -Wpedantic)
endif()
