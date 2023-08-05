/*
 *File: MysqlConnectionPool.h
 *Author: hzk
 *Update: 2020-1-25
 */
#pragma once
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>  
#include <jdbc/cppconn/exception.h>  
#include <jdbc/cppconn/driver.h>  
#include <jdbc/cppconn/connection.h>  
#include <jdbc/cppconn/resultset.h>  
#include <jdbc/cppconn/prepared_statement.h>  
#include <jdbc/cppconn/statement.h>  
#include <mutex>  
#include <list>
using namespace std;
using namespace sql;
class MysqlConnectionPool {
private:
	int curSize;								//当前已建立的数据库连接数量  
	int maxSize = 50;							//连接池中定义的最大数据库连接数  
	std::string username = "root";
	std::string password = "REALology@2021@Vertechs";
	std::string url = "tcp://127.0.0.1:3306/realology";
	std::list<Connection*> connList;				//连接池的容器队列  STL list 双向链表
	std::mutex m_mutex;								//线程锁  
	Driver *driver;

	/**
	* @brief 创建一个连接
	* @param void
	* @return 返回连接对象
	*/
	Connection* CreateConnection();
	/**
	* @brief 初始化数据库连接池
	* @param iInitialSize 初始化个数
	* @return void
	*/
	void InitConnection(int iInitialSize); 
	/**
	* @brief 销毁数据库连接对象
	* @param conn 连接对象
	* @return void
	*/
	void DestoryConnection(Connection *conn);  
	/**
	* @brief 销毁数据库连接池
	* @param void
	* @return void
	*/
	void DestoryConnPool();

public:
	MysqlConnectionPool();
	~MysqlConnectionPool();
	/**
	* @brief 获得数据库连接
	* @param void
	* @return Connection类型对象
	*/
	Connection* GetConnection();
	/**
	* @brief 将数据库连接放回到连接池的容器中
	* @param Connection类型对象
	* @return void
	*/
	void ReleaseConnection(Connection *conn);
};