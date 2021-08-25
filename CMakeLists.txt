# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("TRV2")


# 包含子项目。
add_subdirectory ("TRV2")
add_subdirectory ("ext/glad")
add_subdirectory ("ext/GLFW")

find_package(OpenGL REQUIRED)
target_link_libraries("TRV2" PRIVATE OpenGL::GL)
target_link_libraries ("TRV2" PRIVATE glad glfw)



message(STATUS "\n**************************************** Executing Post-Build Script ****************************************\n")
execute_process(COMMAND python ${PROJECT_SOURCE_DIR}/build.py "${PROJECT_SOURCE_DIR}/Resources" "${PROJECT_BINARY_DIR}")
message(STATUS "\n**************************************** Finished Post-Build Script ****************************************\n")
message(STATUS "")

message(STATUS "${PROJECT_SOURCE_DIR}")
message(STATUS "${PROJECT_BINARY_DIR}")
