# Muduo :: ChatServer
技术栈：nginx | redis | muduo | mysql  
工作在linux环境下的nginx tcp负载均衡的集群聊天服务器及客户端源码

# Depends  
mysql> sudo apt install mysql-server  
mysql> sudo apt install libmysqlclient-dev  

# Tips：  
1> cd ./bulid  
2> rm -rf *  
3> cmake ..  
4> make  
5> cd ../bin  
6> ./ChatServer  
7> ./ChatClient  

# Git
查看当前绑定的远程仓库  
git remote -v  
删除远程仓库  
git remote remove <remote_name>  
添加远程仓库   
git remote add <remote_name> <repository_url>  
提交当前到远程仓库  
git push -u <remote_name> <local_branch>  
