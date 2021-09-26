# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.13)

project ("TRV2_Engine" VERSION 0.3.0.0)

# 指定版本号的配置文件
configure_file(
    "${PROJECT_SOURCE_DIR}/Configs/CompileTimeConfig.h.in"
    "${PROJECT_SOURCE_DIR}/../../generated/CompileTimeConfig_Engine.hpp"
)

# 包含子项目。
add_library("TRV2_Engine" STATIC
"Engine.cpp"
"Core/Structures/EngineSettings.cpp"
"Graphics/GraphicsDevices/OpenGLGraphicsDevice.cpp"
"Core/Utils/Logging/Logger.cpp"
"Core/Render/SpriteRenderer.cpp"
"Platform/GameWindow/GLFWGameWindow.cpp"
"Core/Assets/AssetsManager.cpp"

"Core/Structures/VertexLayout.cpp"

"Graphics/OpenGLProvider.cpp"   
"Core/Gamplay/InputController.h" 
"Core/Gamplay/InputController.cpp"
"Core/Render/Texture2D.cpp"
"Core/Render/Texture2D.h"
"Core/Render/RenderTarget2D.cpp"
"Core/Render/RenderTarget2D.h"
"Core/Render/RawShader.cpp"
"Core/Render/RawShader.h"
"Core/Render/ShaderProgram.cpp"
"Core/Render/ShaderProgram.h" 
"Graphics/ResourceManager/OpenGLGraphicsResourceManager.h"
"Graphics/ResourceManager/OpenGLGraphicsResourceManager.cpp"
"Core/Utils/GameTimer.h"
"Core/Utils/GameTimer.cpp"
"Platform/GLFWProvider.h"
"Platform/GLFWProvider.cpp" "Core/Utils/Collision.h" "Core/Gamplay/Scene.h" "Core/Render/render.h" "Core/Assets/assets.h" "Core/Gamplay/gameplay.h" "Platform/Platform_glfw.h" "Graphics/Graphics_opengl.h")


message(STATUS "${PROJECT_SOURCE_DIR}")
message(STATUS "${PROJECT_BINARY_DIR}")


# 编译选项
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# 添加库引用（引擎和游戏只用添加一次）
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("../../ext/libs/glad" "${PROJECT_BINARY_DIR}/libs/glad" EXCLUDE_FROM_ALL)
add_subdirectory("../../ext/libs/GLFW" "${PROJECT_BINARY_DIR}/libs/GLFW" EXCLUDE_FROM_ALL) 
add_subdirectory("../../ext/libs/SJson" "${PROJECT_BINARY_DIR}/libs/SJson")

find_package(OpenGL REQUIRED)
target_link_libraries("TRV2_Engine" PRIVATE OpenGL::GL)
target_link_libraries("TRV2_Engine" PRIVATE glad glfw SJson)

message(STATUS "${PROJECT_SOURCE_DIR}")
message(STATUS "${PROJECT_BINARY_DIR}")

# Include 目录
target_include_directories("TRV2_Engine" SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/../../generated")
target_include_directories("TRV2_Engine" SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/../../ext/includes")
target_include_directories("TRV2_Engine" PRIVATE "${PROJECT_SOURCE_DIR}")


add_custom_command(TARGET "TRV2_Engine" PRE_BUILD
                   COMMAND python "${PROJECT_SOURCE_DIR}/../../encoding.py" "${PROJECT_SOURCE_DIR}" "utf-8-sig")


