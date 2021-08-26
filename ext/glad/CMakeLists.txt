cmake_minimum_required(VERSION 3.13)

project(glad LANGUAGES CXX)

message(STATUS "${PROJECT_SOURCE_DIR}")
message(STATUS "${PROJECT_BINARY_DIR}")

set(SOURCES_GLAD "glad.c"
                 "KHR/khrplatform.h")

include_directories ("${PROJECT_SOURCE_DIR}/../")

add_library(glad STATIC ${SOURCES_GLAD})