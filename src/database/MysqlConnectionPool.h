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
	int curSize;								//��ǰ�ѽ��������ݿ���������  
	int maxSize = 50;							//���ӳ��ж����������ݿ�������  
	std::string username = "root";
	std::string password = "REALology@2021@Vertechs";
	std::string url = "tcp://127.0.0.1:3306/realology";
	std::list<Connection*> connList;				//���ӳص���������  STL list ˫������
	std::mutex m_mutex;								//�߳���  
	Driver *driver;

	/**
	* @brief ����һ������
	* @param void
	* @return �������Ӷ���
	*/
	Connection* CreateConnection();
	/**
	* @brief ��ʼ�����ݿ����ӳ�
	* @param iInitialSize ��ʼ������
	* @return void
	*/
	void InitConnection(int iInitialSize); 
	/**
	* @brief �������ݿ����Ӷ���
	* @param conn ���Ӷ���
	* @return void
	*/
	void DestoryConnection(Connection *conn);  
	/**
	* @brief �������ݿ����ӳ�
	* @param void
	* @return void
	*/
	void DestoryConnPool();

public:
	MysqlConnectionPool();
	~MysqlConnectionPool();
	/**
	* @brief ������ݿ�����
	* @param void
	* @return Connection���Ͷ���
	*/
	Connection* GetConnection();
	/**
	* @brief �����ݿ����ӷŻص����ӳص�������
	* @param Connection���Ͷ���
	* @return void
	*/
	void ReleaseConnection(Connection *conn);
};