cmake_minimum_required(VERSION 3.10)
project(chat)
set(CMAKE_SUPPRESS_REGENERATION true)
# 配置编译选项
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

# 配置可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 配置头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/server)
include_directories(${PROJECT_SOURCE_DIR}/include/server/db)
include_directories(${PROJECT_SOURCE_DIR}/include/server/model)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)

# set(CMAKE_INSTALL_PREFIX "./bulid" CACHE PATH "Install path prefix prepended to install directories.")

# 加载子目录
add_subdirectory(src)
