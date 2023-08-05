#pragma once
#include <string>
#include "SystemDef.h"
#include "json.h"
class Utils
{
public:
	/**
	 * @brief �Դ�����ַ�������Base64����
	 * @param str ��Ҫ����Base64������ַ���ָ��
	 * @param bytes �ַ����ĳ��ȣ���λ���ֽڣ�
	 * @return ����Base64�������ַ���
	 */
	static std::string Base64Encode(const unsigned char* str, int bytes);
	/**
	 * @brief �Դ����Base64�ַ������н���
	 * @param str ��Ҫ���н����Base64�ַ���ָ��
	 * @param length Base64�ַ����ĳ��ȣ���λ���ַ���
	 * @return ���ؽ�����ԭʼ�ַ���
	 */
	static std::string Base64Decode(const char* str, int length);
	/**
	 * @brief ��ȡ��ǰ���ں�ʱ����ַ�����ʾ����ȷ����
	 * @param bMillisecond �Ƿ�������룬Ϊtrue��ȷ�����룬����ȷ����
	 * @return ���ص�ǰ���ں�ʱ����ַ�����ʾ����ʽΪyyyy/MM/dd hh:mm:ss[.ms]
	 */
	static std::string GetDateTime(bool bMillisecond = false);
	/**
	 * @brief ����ָ��ʱ����֮��֮ǰ��������ʱ���ַ���
	 * @param dateTime ԭʼ����ʱ���ַ�������ʽΪyyyy/MM/dd hh:mm:ss[.ms]
	 * @param intervalSeconds ʱ��������λ���룬����Ϊ֮�󣬸���Ϊ֮ǰ
	 * @return ����ָ��ʱ����֮��֮ǰ��������ʱ���ַ���
	 */
	static std::string GetDateTimeFrom(const std::string& dateTime, long intervalSeconds);
	/**
	 * @brief ������ʱ���ַ���ת��Ϊ���ڽṹ��
	 * @param dateTime ����ʱ���ַ�������ʽΪyyyy/MM/dd hh:mm:ss[.ms]
	 * @return ����ת��������ڽṹ��
	 */
	static SYSTEMTIME DateTimeToStruct(const std::string& dateTime);
	/**
	 * @brief ��JSON��ʽ���ַ���ת��ΪJson::Value����
	 * @param jsString JSON��ʽ���ַ���
	 * @return ����ת�����Json::Value����
	 */
	static Json::Value StrToJson(const std::string& jsString);
	/**
	 * @brief ��Json::Value����ת��ΪJSON��ʽ���ַ���
	 * @param jsVal Json::Value����
	 * @return ����ת�����JSON��ʽ���ַ���
	 */
	static std::string JsonToStr(const Json::Value& jsVal);
	/**
	 * @brief ��ָ��·����JSON�ļ��ж�ȡ���ݲ�ת��ΪJson::Value����
	 * @param filePath JSON�ļ�������·��
	 * @return ���ض�ȡ��ת�����Json::Value����
	 */
	static Json::Value ReadJsonFile(const std::string& filePath);
	/**
	 * @brief ��Json::Value����д��ָ��·����JSON�ļ���
	 * @param filePath JSON�ļ�������·��
	 * @param jsVal ��Ҫд���ļ���Json::Value����
	 * @return ����д���Ƿ�ɹ��Ľ�����ɹ�����true�����򷵻�false
	 */
	static bool WriteJsonFile(const std::string& filePath,const Json::Value& jsVal);

	/**
	 * @brief ���ҽ��̿���
	 * @param strProcessName ������
	 * @return bool �Ƿ��ҵ��ý��̵Ŀ���
	 */
	static bool FindProcess(const SStringT& strProcessName);
	/**
	 * @brief ��������
	 * @param strProcessName ������
	 * @return bool �Ƿ�ɹ���������
	 */
	static bool StartProcess(SStringT strProcessName);
	/**
	 * @brief Ŀ¼���������������ļ�
	 * @param path Ŀ¼·��
	 * @return std::vector<std::string> ����Ŀ¼�������ļ�������
	 */
	static std::vector<std::string> FindAllFiles(const std::string& path);
	/**
	 * @brief ɾ��������־
	 * @param currentTime ��ǰʱ��
	 * @param path ��־·��
	 * @return bool �Ƿ�ɹ�ɾ����־
	 */
	static bool RemoveOutdatedLog(const std::string& currentTime, const std::string& path);

};
