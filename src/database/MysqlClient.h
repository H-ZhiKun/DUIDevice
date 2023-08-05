/*
*数据库服务接口类
*/
#pragma once
#include "MysqlConnectionPool.h"
#include <string>
#include <vector>
#include "SystemDef.h"
#include "json.h"
class MysqlClient
{
protected:
	/**
	 * @brief 从数据库中选择符合条件的记录集
	 * @param sql 包含SQL语句的字符串
	 * @return 返回一个ResultSet指针，指向数据库返回的结果集
	 */
	static ResultSet* SelectSql(const std::string&& sql);
	/**
	 * @brief 在数据库中更新符合条件的记录
	 * @param sql 包含SQL语句的字符串
	 * @return 返回受影响的记录数
	 */
	static int UpdateSql(const std::string&& sql);
	/**
	 * @brief 在数据库中执行给定的SQL语句
	 * @param sql 包含SQL语句的字符串
	 * @return 如果执行成功，返回true，否则返回false
	 */
	static bool ExecSql(const std::string&& sql);
	/**
	 * @brief 创建一个新的FluidInfo表
	 */
	static bool CreateFluidInfo(const std::string& wellName);
	/**
	 * @brief 创建一个新的OriginData表
	 */
	static bool CreateOriginData(const std::string& wellName);
	/**
	 * @brief 创建一个新的Samples表
	 */
	static bool CreateSampleTable(const std::string& wellName);
	/**
	 * @brief 创建一个新的AuxillaryData表
	 */
	static bool CreateAuxillaryData(const std::string& wellName);
	/**
	 * @brief 创建一个新的RecordData表
	 */
	static bool CreateRecordData(const std::string& wellName);
	/**
	 * @brief 创建一个新的SensorsRecord表
	 */
	static bool CreateSensorsRecord(const std::string& wellName);
	/**
	 * @brief 创建一个新的CommentsData表
	 */
	static bool CreateCommentsData(const std::string& wellName);
	/**
	 * @brief 创建一个新的AdvanceData表
	 */
	static bool CreateAdvanceData(const std::string& wellName);
	/**
	 * @brief 创建一个新的WellDiag表
	 */
	static bool CreateWellDiag(const std::string& wellName);
public:
	/**
	 * @brief 创建井表组。
	 * @param wellName 井名称，默认为空。
	 * @return 创建是否成功的布尔值。
	 */
	static bool CreateWellTableGroup(const std::string& wellName);

	/**
	* @brief 登录验证函数
	* @param userName 用户名
	* @param password 密码
	* @return bool 是否验证通过
	*/
	static bool LoginVerification(const std::string& userName, const std::string& password);
	/**
	 * @brief 获取井名
	 * @param vName 存储井名的字符串向量
	 * @return bool 是否获取成功
	 */
	static bool GetWellNames(std::vector<std::string>& vName);
	/**
	 * @brief 获取系统选项信息
	 * @param info 存储系统选项信息的结构体
	 * @return bool 是否获取成功
	 */
	static bool GetWellOptions(SystemOptionsInfo& info);
	/**
	 * @brief 更新或插入系统选项信息
	 * @param info 存储系统选项信息的结构体
	 * @return bool 是否更新或插入成功
	 */
	static bool UpsertWellOptions(const SystemOptionsInfo& info);
	/**
	 * @brief 获取流体信息函数
	 * @param[out] info 流体信息结构体的引用，存储获取到的流体信息
	 * @return 返回一个bool类型值，表示是否成功获取流体信息
	 */
	static bool GetFluidInfo(FluidInfo& info);
	/**
	 * @brief 插入流体信息。
	 * @param info 存储流体信息的结构体对象。
	 * @return 插入是否成功的布尔值。
	 */
	static bool InsertFluidInfo(const FluidInfo& info);
	/**
	 * @brief 插入操作记录。
	 * @param info 存储操作记录的字符串对象。
	 * @return 插入是否成功的布尔值。
	 */
	static bool InsertOperation(const std::string&& info);
	/**
	 * @brief 获取操作记录的总数。
	 * @param[out] totals 存储操作记录总数的整数对象。
	 * @return 获取是否成功的布尔值。
	 */
	static bool GetOplogsCount(size_t& totals);
	/**
	 * @brief 获取一页操作记录。
	 * @param[out] lvInfo 存储操作记录的链表对象。
	 * @param page 页码数。
	 * @param count 获取操作记录的个数，默认为10。
	 * @return 获取是否成功的布尔值。
	 */
	static bool GetPageOplogs(std::list<OpLogsData>& lvInfo, size_t page, size_t count = 10);

	/**
	 * @brief 插入传感器数据
	 * @param sensorType 传感器类型
	 * @param value 传感器值
	 * @return 插入是否成功
	 */
	static bool InsertSensors(const std::string& sensorType, float value);
	/**
	 * @brief 获取最新传感器数据
	 * @param sensorType 传感器类型
	 * @param value 传感器值
	 * @return 是否成功获取数据
	 */
	static bool GetLastSensors(const std::string& sensorType, float& value);
	/**
	 * @brief 获取指定时间范围内的传感器数据记录
	 * @param sensorType 传感器类型
	 * @param info 传感器数据记录
	 * @param timeStart 开始时间，格式为 "YYYY/MM/DD hh:mm:ss"
	 * @param timeEnd 结束时间，格式为 "YYYY/MM/DD hh:mm:ss"
	 * @return 是否成功获取数据
	 */
	static bool GetSensorsRecordInTime(const std::string& sensorType, std::vector<SensorsRecord>& info,
		const std::string& timeStart, const std::string& timeEnd);
	/**
	 * @brief 获取指定时间范围内的传感器数据记录（分页）
	 * @param sensorType 传感器类型
	 * @param info 传感器数据记录
	 * @param timeStart 开始时间，格式为 "YYYY/MM/DD hh:mm:ss"
	 * @param timeEnd 结束时间，格式为 "YYYY/MM/DD hh:mm:ss"
	 * @param page 页码
	 * @param count 每页数据条数，默认为 10
	 * @return 是否成功获取数据
	 */
	static bool GetSensorsRecordPage(const std::string& sensorType, std::vector<SensorsRecord>& info,
		const std::string& timeStart, const std::string& timeEnd, size_t page, size_t count = 10);
	/**
	 * @brief 插入记录数据
	 * @param record 记录数据
	 * @return 是否成功插入数据
	 * @details 将记录数据插入数据库中。
	 */
	static bool InsertRecord(const RecordData& record);
	/**
	 * @brief 获取最新记录数据
	 * @param data 记录数据
	 * @return 是否成功获取数据
	 * @details 从数据库中获取最新的记录数据。
	 */
	static bool GetLastRecord(RecordData& data);
	/**
	 * @brief 获取指定时间范围内的记录数据
	 * @param data 记录数据
	 * @param timeStart 开始时间，格式为 "YYYY/MM/DD hh:mm:ss"
	 * @param timeEnd 结束时间，格式为 "YYYY/MM/DD hh:mm:ss"
	 * @return 是否成功获取数据
	 */
	static bool GetRecordsInTime(std::vector<RecordData>& data,
		const std::string& timeStart,const std::string& timeEnd);
	/**
	 * @brief 获取记录页面数据
	 * @param[out] data 记录数据
	 * @param[in] timeStart 开始时间
	 * @param[in] timeEnd 结束时间
	 * @param[in] page 当前页码
	 * @param[in] count 每页数据条数，默认为10
	 * @return 成功返回true，否则返回false
	 */
	static bool GetRecordPage(std::vector<RecordData>& data,
		const std::string& timeStart, const std::string& timeEnd, size_t page, size_t count = 10);
	/**
	 * @brief 插入样本数据到原始数据表
	 * @param[in] mapData 样本数据
	 * @return 成功返回true，否则返回false
	 */
	static bool InsertSamples(const std::map<uint16_t, std::vector<SampleData>>& mapData);
	/**
	 * @brief 获取系统配置项
	 * @param[out] config 配置项
	 * @return 成功返回true，否则返回false
	 */
	static bool GetItemConfig(std::map<std::string, Json::Value>& config);
	/**
	 * @brief 更新系统配置项
	 * @param[in] itemName 配置项名称
	 * @param[in] itemValue 配置项值
	 * @return 成功返回true，否则返回false
	 */
	static bool UpdateItemConfig(std::string itemName, Json::Value& itemValue);
	/**
	 * @brief 插入advance debug数据
	 * @param[in] data 
	 * @param[in] time 时间，默认为空字符串
	 * @return 成功返回true，否则返回false
	 */
	static bool InsertAdvance(const CollectionData& data,const std::string& time = "");
	/**
	 * @brief 获取advance debug数据
	 * @param[out] data
	 * @param[in] timeStart 开始时间
	 * @return 成功返回true，否则返回false
	 */
	static bool GetAdvance(AdvanceData& data, const std::string& timeStart);
	/**
	 * @brief 插入诊断数据到诊断数据表
	 * @param[in] Info 诊断数据项
	 * @return 成功返回true，否则返回false
	 */
	static bool InsertWellDiags(const DiagDataItem&& Info);
	/**
	 * @brief 获取指定类型和关键字的诊断数据
	 * @param[out] lvInfo 诊断数据列表
	 * @param[in] type 诊断类型
	 * @param[in] key 关键字
	 * @param[in] order 排序方式
	 * @param[in] page 当前页码
	 * @param[in] count 每页数据条数，默认为10
	 * @return 成功返回true，否则返回false
	 */
	static bool SelectDiags(std::list<DiagDataItem>& lvInfo,
		const std::string& type, const std::string& key, const std::string& order,
		size_t page, size_t count = 10);
	/**
	 * @brief 获取指定类型的诊断数据条数
	 * @param[out] total 数据条数
	 * @param[in] type 诊断类型
	 * @return 成功返回true，否则返回false
	 */
	static bool SelectDiagsCount(size_t& total,const std::string& type);
	/**
	 * @brief 获取指定时间段内的评论信息
	 *
	 * @param [out] info 评论信息列表
	 * @param [in] timeStart 开始时间
	 * @param [in] timeEnd 结束时间
	 * @return bool 获取成功返回true，否则返回false
	 */
	static bool GetCommentsInTime(std::vector<CommentsData>& info,
		const std::string& timeStart, const std::string& timeEnd);
	/**
	 * @brief 插入一条评论信息
	 *
	 * @param [in] data 待插入的评论信息
	 * @return bool 插入成功返回true，否则返回false
	 */
	static bool InsertComments(CommentsData& data);
	//测试接口
	static bool GetLastOriginData(std::map<uint16_t, std::vector<SampleData>>& pInSample);
	static bool GetOriginDataById(std::map<uint16_t, std::vector<SampleData>>& pInSample,
		int id, int year, int month);
};

