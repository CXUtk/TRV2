# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.13)

project ("TRV2_Game" VERSION 1.0.0.0)

# 指定版本号的配置文件
configure_file(
    "${PROJECT_SOURCE_DIR}/TRGame/Configs/CompileTimeConfig.h.in"
    "${PROJECT_SOURCE_DIR}/generated/CompileTimeConfig.h"
)

# 包含子项目。
add_executable("TRV2_Game" 
"TRGame/main.cpp"
"TRGame/TRGame.cpp"
"TRGame/Worlds/GameWorld.cpp" "ext/includes/glad/KHR/khrplatform.h")

message(STATUS "${PROJECT_SOURCE_DIR}")
message(STATUS "${PROJECT_BINARY_DIR}")

# 编译选项
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if ( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options("TRV2_Game" PRIVATE "-Wall -Wextra")
endif()
if ( MSVC )
    target_compile_options("TRV2_Game" PRIVATE "/W4")
endif()

# 添加库引用

# 引用引擎
add_subdirectory("TREngine")

# 引用库
find_package(OpenGL REQUIRED)
target_link_libraries("TRV2_Game" PRIVATE OpenGL::GL)
target_link_libraries("TRV2_Game" PRIVATE glad glfw SJson TRV2_Engine)

# Include 目录
include_directories("${PROJECT_SOURCE_DIR}/generated")
include_directories("${PROJECT_SOURCE_DIR}/ext/includes")
include_directories("${PROJECT_SOURCE_DIR}/TREngine")
include_directories("${PROJECT_SOURCE_DIR}/TRGame")

add_custom_command(TARGET "TRV2_Game" PRE_BUILD
                   COMMAND python ${PROJECT_SOURCE_DIR}/encoding.py "${PROJECT_SOURCE_DIR}/TRGame" "utf-8-sig")
add_custom_command(TARGET "TRV2_Game" POST_BUILD
                   COMMAND python ${PROJECT_SOURCE_DIR}/build.py "${PROJECT_SOURCE_DIR}/Resources" "${PROJECT_BINARY_DIR}")


