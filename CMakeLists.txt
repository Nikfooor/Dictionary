cmake_minimum_required(VERSION 3.5.0)
project(Dictionary VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Dictionary
    Dict.cpp
    Dict.h
    main.cpp)

