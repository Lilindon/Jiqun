# cmake_minimum_required(VERSION 3.10)
# project(main)

# # 配置编译选项
# set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

# # 配置头文件搜索路径 -I/usr/include
# # include_directories()
# # 配置库文件搜索路径 -L/usr/libs
# # link_directories()

# # 设置需要编译的源文件列表
# set(SRC_LIST muduo_server.cpp)
# # 把.指定路径下的所有源文件名字放入变量名SRC_LIST中
# # aux_source_directory(. SRC_LIST)

# # 设置可执行文件最终生成的路径  PROJECT_SOURCE_DIR当前的目录
# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 

# # 指定搜索的子目录 查找CMakeLists 默认从当前根目录开始
# # add_subdirectory(testmuduo)

# # 生成可执行文件
# # add_executable(server muduo_server.cpp)
# add_executable(server ${SRC_LIST})

# # 链接库 -lmuduo_net 表示server目标程序 需要链接muduo_net muduo_base pthread这三个库文件
# target_link_libraries(server muduo_net muduo_base pthread)


# # 1:cmake .
# # 2:make

# # bin 可执行文件
# # lib 链接库
# # include 库文件
# # src 源
# # build cmake
# # thirdparty 第三方库
# # CMakeLists.txt