# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.13)

project ("TRV2_Engine" VERSION 0.2.0.0)

# 指定版本号的配置文件
configure_file(
    "${PROJECT_SOURCE_DIR}/Configs/CompileTimeConfig.h.in"
    "${PROJECT_SOURCE_DIR}/../../generated/CompileTimeConfig_Engine.h"
)

# 包含子项目。
add_library("TRV2_Engine" STATIC
"TREngine.cpp"
"Configs/ClientConfig.cpp"
"Graphics/GraphicsDevices/OpenGLGraphicsDevice.cpp"
"Utils/Logging/Logger.cpp"
"Core/TRWindow/OpenGLWindow.cpp" 
"Graphics/OpenGLTRGameGraphicsAPI.cpp"
"Graphics/Renderers/OpenGLSpriteRenderer.cpp"
"Assets/AssetsManager.cpp"
"Assets/Loaders/OpenGLShaderLoader.cpp" 
"Graphics/Shaders/OpenGLShader.cpp")


message(STATUS "${PROJECT_SOURCE_DIR}")
message(STATUS "${PROJECT_BINARY_DIR}")


# 编译选项
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if ( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options("TRV2_Engine" PRIVATE "-Wall -Wextra")
endif()
if ( MSVC )
    target_compile_options("TRV2_Engine" PRIVATE "/W4")
endif()

# 添加库引用（引擎和游戏只用添加一次）
add_subdirectory("../../ext/libs/glad" "${PROJECT_BINARY_DIR}/libs/glad")
add_subdirectory("../../ext/libs/GLFW" "${PROJECT_BINARY_DIR}/libs/GLFW") 
add_subdirectory("../../ext/libs/SJson" "${PROJECT_BINARY_DIR}/libs/SJson")

find_package(OpenGL REQUIRED)
target_link_libraries("TRV2_Engine" PRIVATE OpenGL::GL)
target_link_libraries("TRV2_Engine" PRIVATE glad glfw SJson)

message(STATUS "${PROJECT_SOURCE_DIR}")
message(STATUS "${PROJECT_BINARY_DIR}")

# Include 目录
include_directories("${PROJECT_SOURCE_DIR}/../../generated")
include_directories("${PROJECT_SOURCE_DIR}/../../ext/includes")
include_directories("${PROJECT_SOURCE_DIR}")


add_custom_command(TARGET "TRV2_Engine" PRE_BUILD
                   COMMAND python "${PROJECT_SOURCE_DIR}/../../encoding.py" "${PROJECT_SOURCE_DIR}" "utf-8-sig")


