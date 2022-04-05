cmake_minimum_required(VERSION 3.16)

project(getopt VERSION 1.0 LANGUAGES C)

add_library(getopt STATIC)
target_sources(getopt
    PRIVATE
        ya_getopt.c
        ya_getopt.h
        )
target_compile_features(getopt
    INTERFACE
        c_std_11
        )
if(MSVC)
    target_compile_options(getopt
        INTERFACE
            /W4
            )
else()
    target_compile_options(getopt
        INTERFACE
            -Wall -Wextra -Wpedantic
            )
endif()
