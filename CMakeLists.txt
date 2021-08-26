# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.13)

project ("TRV2")


# 包含子项目。
add_subdirectory ("TRV2")
add_subdirectory ("ext/glad")
add_subdirectory ("ext/GLFW")

find_package(OpenGL REQUIRED)
target_link_libraries("TRV2" PRIVATE OpenGL::GL)
target_link_libraries ("TRV2" PRIVATE glad glfw)




message(STATUS "\n**************************************** Executing Encoding-check Script ****************************************\n")
execute_process(COMMAND python ${PROJECT_SOURCE_DIR}/encoding.py "${PROJECT_SOURCE_DIR}/TRV2" "utf-8-sig")
message(STATUS "\n**************************************** Finished Encoding-check Script *****************************************\n")

message(STATUS "\n**************************************** Executing Resources-copy Script ****************************************\n")
execute_process(COMMAND python ${PROJECT_SOURCE_DIR}/build.py "${PROJECT_SOURCE_DIR}/Resources" "${PROJECT_BINARY_DIR}/TRV2")
message(STATUS "\n**************************************** Finished Resources-copy Script *****************************************\n")

message(STATUS "${PROJECT_SOURCE_DIR}")
message(STATUS "${PROJECT_BINARY_DIR}")
