#include "stdafx.h"
#include "MysqlConnectionPool.h"
#include <stdexcept>  
#include <exception>  
#include <stdio.h>  

MysqlConnectionPool::MysqlConnectionPool()
{
	try {
		this->driver = sql::mysql::get_driver_instance();
	}
	catch (sql::SQLException& e) {

		MessageBoxA(nullptr, SStringA().Format("数据库驱动连接出错: %s", e.what()), nullptr, MB_OK);
	}
	this->InitConnection(maxSize / 2);
}

//初始化连接池，创建最大连接数的一半连接数量  
void MysqlConnectionPool::InitConnection(int iInitialSize)
{
	Connection* conn;
	std::lock_guard<std::mutex> auto_lock(m_mutex);
	for (int i = 0; i < iInitialSize; i++) {
		conn = this->CreateConnection();
		if (conn) {
			connList.push_back(conn);
			++(this->curSize);
		}
		else
		{
			MessageBoxA(nullptr, "数据库连接出错", nullptr, MB_OK);
		}
	}
}

//创建连接,返回一个Connection  
Connection* MysqlConnectionPool::CreateConnection()
{
	Connection* conn;
	try
	{
		conn = driver->connect(this->url, this->username, this->password); //建立连接  
	}
	catch (sql::SQLException& e)
	{
		MessageBoxA(nullptr, e.what(), nullptr, MB_OK);
	}
	return conn;
}

//在连接池中获得一个连接  
Connection* MysqlConnectionPool::GetConnection()
{
	std::lock_guard<std::mutex> auto_lock(m_mutex);
	Connection* con = nullptr;
	if (connList.size() > 0)
	{   //连接池容器中还有连接  
		con = connList.front(); //得到第一个连接  
		connList.pop_front();   //移除第一个连接  
		try
		{
			con->setSchema("realology");
		}
		catch (sql::SQLException e)
		{
			OutputDebugStringA(e.what());
			con->close();
			delete con;
			con = CreateConnection();
			if (con) con->setSchema("realology");
			else --curSize;
		}
	}
	else
	{
		con = this->CreateConnection();
		if (con) {
			con->setSchema("realology");
			++curSize;
		}
	}
	return con;
}

//回收数据库连接  
void MysqlConnectionPool::ReleaseConnection(sql::Connection* conn) {
	if (conn == nullptr) return;
	std::lock_guard<std::mutex> auto_lock(m_mutex);
	connList.push_back(conn);
}

//连接池的析构函数  
MysqlConnectionPool::~MysqlConnectionPool() {
	this->DestoryConnPool();
}

//销毁连接池,首先要先销毁连接池的中连接  
void MysqlConnectionPool::DestoryConnPool() {
	list<Connection*>::iterator icon;
	std::lock_guard<std::mutex> auto_lock(m_mutex);
	for (icon = connList.begin(); icon != connList.end(); ++icon) {
		this->DestoryConnection(*icon); //销毁连接池中的连接  
	}
	curSize = 0;
	connList.clear(); //清空连接池中的连接  
}

//销毁一个连接  
void MysqlConnectionPool::DestoryConnection(Connection* conn) {
	if (conn) {
		try {
			conn->close();
		}
		catch (sql::SQLException& e) {
			std::string str = std::string("数据库连接: ") + e.what();
			MessageBoxA(nullptr, str.c_str(), nullptr, MB_OK);
		}
		delete conn;
	}
}
