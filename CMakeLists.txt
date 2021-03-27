cmake_minimum_required(VERSION 3.17)
project(HttpServer)

set(CMAKE_CXX_STANDARD 98)

add_executable(HttpServer main.cpp src/core/mem_pool.cpp src/core/mem_pool.h src/config/config.h)