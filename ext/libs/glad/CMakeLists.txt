cmake_minimum_required(VERSION 3.13)

project(glad LANGUAGES CXX)

set(SOURCES_GLAD "glad.c")

add_library(glad STATIC ${SOURCES_GLAD})