# CMakeList.txt: TRV2 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 将源代码添加到此项目的可执行文件。
add_executable ("TRV2" "TRV2.cpp" "TRV2.h")


set_target_properties ("TRV2" PROPERTIES
                      CXX_STANDARD 17
                      CXX_EXTENSIONS OFF
                      )

# 导入包含文件夹
include_directories ("${PROJECT_SOURCE_DIR}/ext")
include_directories ("${PROJECT_SOURCE_DIR}/ext/GLFW/include")


# TODO: 如有需要，请添加测试并安装目标。
