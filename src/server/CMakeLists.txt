# 定义SRC_LIST 包含目录下所有源文件
aux_source_directory(. SRC_LIST)
aux_source_directory(./db DB_LIST)
aux_source_directory(./model MODEL_LIST)
aux_source_directory(./redis REDIS_LIST)

# 指定生成可执行文件
add_executable(ChatServer ${SRC_LIST} ${DB_LIST} ${MODEL_LIST} ${REDIS_LIST})

# 配置头文件搜索路径
# include_directories(${PROJECT_SOURCE_DIR})

# 执行可执行文件 链接时需要依赖的 库文件
target_link_libraries(ChatServer muduo_net muduo_base pthread mysqlclient hiredis)