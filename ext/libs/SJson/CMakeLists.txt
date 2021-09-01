cmake_minimum_required (VERSION 3.8)

project ("SJson")


add_library("SJson" STATIC "SJson.cpp" "SJson.h")


set_target_properties (SJson PROPERTIES
                      CXX_STANDARD 17
                      CXX_EXTENSIONS OFF
                      )
