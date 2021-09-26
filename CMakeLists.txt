# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.13)

project ("TRV2_Game" VERSION 0.3.0.0)

# 初始化变量
set(TRV2_GAME_SRC_DIR "${PROJECT_SOURCE_DIR}/src/TRGame")
set(TRV2_Engine_SRC_DIR "${PROJECT_SOURCE_DIR}/src/TREngine")
message(STATUS "${PROJECT_SOURCE_DIR}")
message(STATUS "${PROJECT_BINARY_DIR}")


# 指定版本号的配置文件
configure_file(
    "${TRV2_GAME_SRC_DIR}/Configs/CompileTimeConfig.h.in"
    "${PROJECT_SOURCE_DIR}/generated/CompileTimeConfig_Game.hpp"
)

# 包含子项目。
add_executable("TRV2_Game" 
"${TRV2_GAME_SRC_DIR}/main.cpp"
"${TRV2_GAME_SRC_DIR}/TRGame.cpp"
"${TRV2_GAME_SRC_DIR}/Worlds/GameWorld.cpp"
 "src/TRGame/Worlds/TileSection.h" 
 "src/TRGame/Worlds/TileSection.cpp" 
 "src/TRGame/Player/Player.cpp" 
 "src/TRGame/Lighting/Lighting.h" 
 "src/TRGame/Lighting/Lighting.cpp" 
 "src/TRGame/Scenes/MainGameScene.h" "src/TRGame/Scenes/MainGameScene.cpp" "src/TRGame/Scenes/MapScene.h" "src/TRGame/Scenes/MapScene.cpp" "src/TRGame/Worlds/WorldMap.h" "src/TRGame/Worlds/WorldMap.cpp")

# 编译选项
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include 目录
target_include_directories("TRV2_Game" SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/generated")
target_include_directories("TRV2_Game" SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/ext/includes")
target_include_directories("TRV2_Game" SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/src/TREngine")
target_include_directories("TRV2_Game" PRIVATE "${PROJECT_SOURCE_DIR}/src")


# 添加库引用
# 引用引擎
add_subdirectory("src/TREngine")

# 引用库
target_link_libraries("TRV2_Game" PRIVATE TRV2_Engine)

message(STATUS "${PROJECT_SOURCE_DIR}")
message(STATUS "${PROJECT_BINARY_DIR}")

add_custom_command(TARGET "TRV2_Game" PRE_BUILD
                   COMMAND python ${PROJECT_SOURCE_DIR}/encoding.py "${TRV2_GAME_SRC_DIR}" "utf-8-sig")
add_custom_command(TARGET "TRV2_Game" PRE_BUILD
                   COMMAND python ${PROJECT_SOURCE_DIR}/encoding.py "${PROJECT_SOURCE_DIR}/Resources" "utf-8-sig")
execute_process(COMMAND python ${PROJECT_SOURCE_DIR}/build.py "${PROJECT_SOURCE_DIR}/Resources" "${PROJECT_BINARY_DIR}")


