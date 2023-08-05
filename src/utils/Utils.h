#pragma once
#include <string>
#include "SystemDef.h"
#include "json.h"
class Utils
{
public:
	/**
	 * @brief 对传入的字符串进行Base64编码
	 * @param str 需要进行Base64编码的字符串指针
	 * @param bytes 字符串的长度（单位：字节）
	 * @return 返回Base64编码后的字符串
	 */
	static std::string Base64Encode(const unsigned char* str, int bytes);
	/**
	 * @brief 对传入的Base64字符串进行解码
	 * @param str 需要进行解码的Base64字符串指针
	 * @param length Base64字符串的长度（单位：字符）
	 * @return 返回解码后的原始字符串
	 */
	static std::string Base64Decode(const char* str, int length);
	/**
	 * @brief 获取当前日期和时间的字符串表示，精确到秒
	 * @param bMillisecond 是否包含毫秒，为true则精确到毫秒，否则精确到秒
	 * @return 返回当前日期和时间的字符串表示，格式为yyyy/MM/dd hh:mm:ss[.ms]
	 */
	static std::string GetDateTime(bool bMillisecond = false);
	/**
	 * @brief 返回指定时间间隔之后（之前）的日期时间字符串
	 * @param dateTime 原始日期时间字符串，格式为yyyy/MM/dd hh:mm:ss[.ms]
	 * @param intervalSeconds 时间间隔，单位：秒，正数为之后，负数为之前
	 * @return 返回指定时间间隔之后（之前）的日期时间字符串
	 */
	static std::string GetDateTimeFrom(const std::string& dateTime, long intervalSeconds);
	/**
	 * @brief 将日期时间字符串转换为日期结构体
	 * @param dateTime 日期时间字符串，格式为yyyy/MM/dd hh:mm:ss[.ms]
	 * @return 返回转换后的日期结构体
	 */
	static SYSTEMTIME DateTimeToStruct(const std::string& dateTime);
	/**
	 * @brief 将JSON格式的字符串转换为Json::Value对象
	 * @param jsString JSON格式的字符串
	 * @return 返回转换后的Json::Value对象
	 */
	static Json::Value StrToJson(const std::string& jsString);
	/**
	 * @brief 将Json::Value对象转换为JSON格式的字符串
	 * @param jsVal Json::Value对象
	 * @return 返回转换后的JSON格式的字符串
	 */
	static std::string JsonToStr(const Json::Value& jsVal);
	/**
	 * @brief 从指定路径的JSON文件中读取数据并转换为Json::Value对象
	 * @param filePath JSON文件的完整路径
	 * @return 返回读取并转换后的Json::Value对象
	 */
	static Json::Value ReadJsonFile(const std::string& filePath);
	/**
	 * @brief 将Json::Value对象写入指定路径的JSON文件中
	 * @param filePath JSON文件的完整路径
	 * @param jsVal 需要写入文件的Json::Value对象
	 * @return 返回写入是否成功的结果，成功返回true，否则返回false
	 */
	static bool WriteJsonFile(const std::string& filePath,const Json::Value& jsVal);

	/**
	 * @brief 查找进程快照
	 * @param strProcessName 进程名
	 * @return bool 是否找到该进程的快照
	 */
	static bool FindProcess(const SStringT& strProcessName);
	/**
	 * @brief 启动进程
	 * @param strProcessName 进程名
	 * @return bool 是否成功启动进程
	 */
	static bool StartProcess(SStringT strProcessName);
	/**
	 * @brief 目录遍历，查找所有文件
	 * @param path 目录路径
	 * @return std::vector<std::string> 包含目录下所有文件的向量
	 */
	static std::vector<std::string> FindAllFiles(const std::string& path);
	/**
	 * @brief 删除过期日志
	 * @param currentTime 当前时间
	 * @param path 日志路径
	 * @return bool 是否成功删除日志
	 */
	static bool RemoveOutdatedLog(const std::string& currentTime, const std::string& path);

};
