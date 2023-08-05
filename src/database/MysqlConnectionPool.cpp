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

		MessageBoxA(nullptr, SStringA().Format("���ݿ��������ӳ���: %s", e.what()), nullptr, MB_OK);
	}
	this->InitConnection(maxSize / 2);
}

//��ʼ�����ӳأ����������������һ����������  
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
			MessageBoxA(nullptr, "���ݿ����ӳ���", nullptr, MB_OK);
		}
	}
}

//��������,����һ��Connection  
Connection* MysqlConnectionPool::CreateConnection()
{
	Connection* conn;
	try
	{
		conn = driver->connect(this->url, this->username, this->password); //��������  
	}
	catch (sql::SQLException& e)
	{
		MessageBoxA(nullptr, e.what(), nullptr, MB_OK);
	}
	return conn;
}

//�����ӳ��л��һ������  
Connection* MysqlConnectionPool::GetConnection()
{
	std::lock_guard<std::mutex> auto_lock(m_mutex);
	Connection* con = nullptr;
	if (connList.size() > 0)
	{   //���ӳ������л�������  
		con = connList.front(); //�õ���һ������  
		connList.pop_front();   //�Ƴ���һ������  
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

//�������ݿ�����  
void MysqlConnectionPool::ReleaseConnection(sql::Connection* conn) {
	if (conn == nullptr) return;
	std::lock_guard<std::mutex> auto_lock(m_mutex);
	connList.push_back(conn);
}

//���ӳص���������  
MysqlConnectionPool::~MysqlConnectionPool() {
	this->DestoryConnPool();
}

//�������ӳ�,����Ҫ���������ӳص�������  
void MysqlConnectionPool::DestoryConnPool() {
	list<Connection*>::iterator icon;
	std::lock_guard<std::mutex> auto_lock(m_mutex);
	for (icon = connList.begin(); icon != connList.end(); ++icon) {
		this->DestoryConnection(*icon); //�������ӳ��е�����  
	}
	curSize = 0;
	connList.clear(); //������ӳ��е�����  
}

//����һ������  
void MysqlConnectionPool::DestoryConnection(Connection* conn) {
	if (conn) {
		try {
			conn->close();
		}
		catch (sql::SQLException& e) {
			std::string str = std::string("���ݿ�����: ") + e.what();
			MessageBoxA(nullptr, str.c_str(), nullptr, MB_OK);
		}
		delete conn;
	}
}
