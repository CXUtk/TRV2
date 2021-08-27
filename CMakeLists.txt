# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.13)

project ("TRV2")


# 包含子项目。
add_executable ("TRV2" "src/main.cpp" "src/TRGame.cpp" "src/Config/ClientConfig.cpp" "src/Graphics/GraphicsInterface/OpenGLGraphicsDevice.cpp")


# 编译选项
set_target_properties ("TRV2" PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)

if ( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options("TRV2" PRIVATE "-Wall -Wextra")
endif()
if ( MSVC )
    target_compile_options("TRV2" PRIVATE "/W4")
endif()

add_subdirectory ("ext/glad")
add_subdirectory ("ext/GLFW")
add_subdirectory ("ext/SJson")

message(STATUS "${PROJECT_SOURCE_DIR}")
message(STATUS "${PROJECT_BINARY_DIR}")

find_package(OpenGL REQUIRED)
target_link_libraries("TRV2" PRIVATE OpenGL::GL)
target_link_libraries ("TRV2" PRIVATE glad glfw SJson)

# Include 目录
include_directories("${PROJECT_SOURCE_DIR}/ext")
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/ext/GLFW/include")


message(STATUS "\n************************************* Executing Encoding-check Script *************************************\n")
execute_process(COMMAND python ${PROJECT_SOURCE_DIR}/encoding.py "${PROJECT_SOURCE_DIR}/src" "utf-8-sig")
message(STATUS "\n************************************* Finished Encoding-check Script **************************************\n")

message(STATUS "\n************************************* Executing Resources-copy Script *************************************\n")
execute_process(COMMAND python ${PROJECT_SOURCE_DIR}/build.py "${PROJECT_SOURCE_DIR}/Resources" "${PROJECT_BINARY_DIR}")
message(STATUS "\n************************************* Finished Resources-copy Script **************************************\n")


