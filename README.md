# project

realology client

## 链接器参数
```shell
$(THIRD_PARTY)\xlnt\lib;$(THIRD_PARTY)\opencv\lib;$(THIRD_PARTY)\python\lib;$(THIRD_PARTY)\modbus\lib;$(THIRD_PARTY)\jsoncpp\lib;$(THIRD_PARTY)\mysql\lib\vs14;$(SOUIPATH)\bin64;%(AdditionalLibraryDirectories)
```
## 附加目录
```shell
$(THIRD_PARTY)\xlnt\include;$(THIRD_PARTY)\opencv\include;$(THIRD_PARTY)\python\include;$(THIRD_PARTY)\modbus\include;$(THIRD_PARTY)\boost\include;$(THIRD_PARTY)\jsoncpp\include;$(THIRD_PARTY)\mysql\include;./analysis;./modbus;./adapter;./database;./extensions;./utils;./layout;./;$(SOUIPATH)\config;$(SOUIPATH)\components;$(SOUIPATH)\SOUI\include;$(SOUIPATH)\utilities\include;%(AdditionalIncludeDirectories)
```

## 需要添加的环境变量 
- THIRD_PARTY
- SOUIPATH

### THIRD_PARTY
```shell
git clone http://192.168.30.241:9898/realology/third_party.git
```
THIRD_PARTY的值应该设置为该git库的目录

# initial project
```shell
git clone -b ${branch} http://192.168.30.241:9898/realology/realology.git ${path}
cd ${path}
git submodule update --init
```
## 当子模块更新时
`git submodule update --remote`

# Mysql数据库Docker运行语句
```shell
docker run --name some-mysql -e MYSQL_ROOT_PASSWORD="REALology@2021@Vertechs" -p 3306:3306 -d docker-regisrty.vertechs.com:5000/vertechs/karl/base/mysql:8.0.32-debian --lower-case-table-names=1
```
在这个语句中，指明了容器名称为`some-mysql`,同时指明了环境变量`MYSQL_ROOT_PASSWORD="REALology@2021@Vertechs"`,暴露了通讯端口`3306`，使用的镜像其实是`mysql:8.0.32-debian`tag而来的镜像，放在内网镜像仓库，方便拉取。
其中`--lower-case-table-names=1`命令显示的指定了数据库以及即将初始化的数据字典路径对于大小写不敏感。
使用该命令的原因是，linux下mysql默认大小写敏感，而windows上，mysql默认大小写不敏感。
## 数据库数据导入和导出
### 导出
`mysqldump -u root --password=REALology@2021@Vertechs realology > realology_dump.sql`
### 导入
`mysql -u root --password=REALology@2021@Vertechs realology < realology_dump.sql`
