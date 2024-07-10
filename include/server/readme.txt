 
 
AllGroup 表： 保存组id 组name 和组结构desc
GroupUser表： 保存当前组id 用户id 和用户在组内的角色role

群组信息group 负责组的四个属性， 组id 组名称groupname 组组成groupdesc 和组内成员 users
groupuser  设置组内的 角色表 是创建者 管理员 和普通用户
groupmodel 操作组相关的数据库 如 创建组，添加组，显示用户所在组，对组内成员进行群发消息