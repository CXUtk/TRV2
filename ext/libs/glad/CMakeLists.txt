cmake_minimum_required(VERSION 3.13)

project(glad LANGUAGES CXX)

set(SOURCES_GLAD "glad.c")

include_directories("${PROJECT_SOURCE_DIR}/../../../includes")

add_library(glad STATIC ${SOURCES_GLAD})