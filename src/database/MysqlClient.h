/*
*���ݿ����ӿ���
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
	 * @brief �����ݿ���ѡ����������ļ�¼��
	 * @param sql ����SQL�����ַ���
	 * @return ����һ��ResultSetָ�룬ָ�����ݿⷵ�صĽ����
	 */
	static ResultSet* SelectSql(const std::string&& sql);
	/**
	 * @brief �����ݿ��и��·��������ļ�¼
	 * @param sql ����SQL�����ַ���
	 * @return ������Ӱ��ļ�¼��
	 */
	static int UpdateSql(const std::string&& sql);
	/**
	 * @brief �����ݿ���ִ�и�����SQL���
	 * @param sql ����SQL�����ַ���
	 * @return ���ִ�гɹ�������true�����򷵻�false
	 */
	static bool ExecSql(const std::string&& sql);
	/**
	 * @brief ����һ���µ�FluidInfo��
	 */
	static bool CreateFluidInfo(const std::string& wellName);
	/**
	 * @brief ����һ���µ�OriginData��
	 */
	static bool CreateOriginData(const std::string& wellName);
	/**
	 * @brief ����һ���µ�Samples��
	 */
	static bool CreateSampleTable(const std::string& wellName);
	/**
	 * @brief ����һ���µ�AuxillaryData��
	 */
	static bool CreateAuxillaryData(const std::string& wellName);
	/**
	 * @brief ����һ���µ�RecordData��
	 */
	static bool CreateRecordData(const std::string& wellName);
	/**
	 * @brief ����һ���µ�SensorsRecord��
	 */
	static bool CreateSensorsRecord(const std::string& wellName);
	/**
	 * @brief ����һ���µ�CommentsData��
	 */
	static bool CreateCommentsData(const std::string& wellName);
	/**
	 * @brief ����һ���µ�AdvanceData��
	 */
	static bool CreateAdvanceData(const std::string& wellName);
	/**
	 * @brief ����һ���µ�WellDiag��
	 */
	static bool CreateWellDiag(const std::string& wellName);
public:
	/**
	 * @brief ���������顣
	 * @param wellName �����ƣ�Ĭ��Ϊ�ա�
	 * @return �����Ƿ�ɹ��Ĳ���ֵ��
	 */
	static bool CreateWellTableGroup(const std::string& wellName);

	/**
	* @brief ��¼��֤����
	* @param userName �û���
	* @param password ����
	* @return bool �Ƿ���֤ͨ��
	*/
	static bool LoginVerification(const std::string& userName, const std::string& password);
	/**
	 * @brief ��ȡ����
	 * @param vName �洢�������ַ�������
	 * @return bool �Ƿ��ȡ�ɹ�
	 */
	static bool GetWellNames(std::vector<std::string>& vName);
	/**
	 * @brief ��ȡϵͳѡ����Ϣ
	 * @param info �洢ϵͳѡ����Ϣ�Ľṹ��
	 * @return bool �Ƿ��ȡ�ɹ�
	 */
	static bool GetWellOptions(SystemOptionsInfo& info);
	/**
	 * @brief ���»����ϵͳѡ����Ϣ
	 * @param info �洢ϵͳѡ����Ϣ�Ľṹ��
	 * @return bool �Ƿ���»����ɹ�
	 */
	static bool UpsertWellOptions(const SystemOptionsInfo& info);
	/**
	 * @brief ��ȡ������Ϣ����
	 * @param[out] info ������Ϣ�ṹ������ã��洢��ȡ����������Ϣ
	 * @return ����һ��bool����ֵ����ʾ�Ƿ�ɹ���ȡ������Ϣ
	 */
	static bool GetFluidInfo(FluidInfo& info);
	/**
	 * @brief ����������Ϣ��
	 * @param info �洢������Ϣ�Ľṹ�����
	 * @return �����Ƿ�ɹ��Ĳ���ֵ��
	 */
	static bool InsertFluidInfo(const FluidInfo& info);
	/**
	 * @brief ���������¼��
	 * @param info �洢������¼���ַ�������
	 * @return �����Ƿ�ɹ��Ĳ���ֵ��
	 */
	static bool InsertOperation(const std::string&& info);
	/**
	 * @brief ��ȡ������¼��������
	 * @param[out] totals �洢������¼��������������
	 * @return ��ȡ�Ƿ�ɹ��Ĳ���ֵ��
	 */
	static bool GetOplogsCount(size_t& totals);
	/**
	 * @brief ��ȡһҳ������¼��
	 * @param[out] lvInfo �洢������¼���������
	 * @param page ҳ������
	 * @param count ��ȡ������¼�ĸ�����Ĭ��Ϊ10��
	 * @return ��ȡ�Ƿ�ɹ��Ĳ���ֵ��
	 */
	static bool GetPageOplogs(std::list<OpLogsData>& lvInfo, size_t page, size_t count = 10);

	/**
	 * @brief ���봫��������
	 * @param sensorType ����������
	 * @param value ������ֵ
	 * @return �����Ƿ�ɹ�
	 */
	static bool InsertSensors(const std::string& sensorType, float value);
	/**
	 * @brief ��ȡ���´���������
	 * @param sensorType ����������
	 * @param value ������ֵ
	 * @return �Ƿ�ɹ���ȡ����
	 */
	static bool GetLastSensors(const std::string& sensorType, float& value);
	/**
	 * @brief ��ȡָ��ʱ�䷶Χ�ڵĴ��������ݼ�¼
	 * @param sensorType ����������
	 * @param info ���������ݼ�¼
	 * @param timeStart ��ʼʱ�䣬��ʽΪ "YYYY/MM/DD hh:mm:ss"
	 * @param timeEnd ����ʱ�䣬��ʽΪ "YYYY/MM/DD hh:mm:ss"
	 * @return �Ƿ�ɹ���ȡ����
	 */
	static bool GetSensorsRecordInTime(const std::string& sensorType, std::vector<SensorsRecord>& info,
		const std::string& timeStart, const std::string& timeEnd);
	/**
	 * @brief ��ȡָ��ʱ�䷶Χ�ڵĴ��������ݼ�¼����ҳ��
	 * @param sensorType ����������
	 * @param info ���������ݼ�¼
	 * @param timeStart ��ʼʱ�䣬��ʽΪ "YYYY/MM/DD hh:mm:ss"
	 * @param timeEnd ����ʱ�䣬��ʽΪ "YYYY/MM/DD hh:mm:ss"
	 * @param page ҳ��
	 * @param count ÿҳ����������Ĭ��Ϊ 10
	 * @return �Ƿ�ɹ���ȡ����
	 */
	static bool GetSensorsRecordPage(const std::string& sensorType, std::vector<SensorsRecord>& info,
		const std::string& timeStart, const std::string& timeEnd, size_t page, size_t count = 10);
	/**
	 * @brief �����¼����
	 * @param record ��¼����
	 * @return �Ƿ�ɹ���������
	 * @details ����¼���ݲ������ݿ��С�
	 */
	static bool InsertRecord(const RecordData& record);
	/**
	 * @brief ��ȡ���¼�¼����
	 * @param data ��¼����
	 * @return �Ƿ�ɹ���ȡ����
	 * @details �����ݿ��л�ȡ���µļ�¼���ݡ�
	 */
	static bool GetLastRecord(RecordData& data);
	/**
	 * @brief ��ȡָ��ʱ�䷶Χ�ڵļ�¼����
	 * @param data ��¼����
	 * @param timeStart ��ʼʱ�䣬��ʽΪ "YYYY/MM/DD hh:mm:ss"
	 * @param timeEnd ����ʱ�䣬��ʽΪ "YYYY/MM/DD hh:mm:ss"
	 * @return �Ƿ�ɹ���ȡ����
	 */
	static bool GetRecordsInTime(std::vector<RecordData>& data,
		const std::string& timeStart,const std::string& timeEnd);
	/**
	 * @brief ��ȡ��¼ҳ������
	 * @param[out] data ��¼����
	 * @param[in] timeStart ��ʼʱ��
	 * @param[in] timeEnd ����ʱ��
	 * @param[in] page ��ǰҳ��
	 * @param[in] count ÿҳ����������Ĭ��Ϊ10
	 * @return �ɹ�����true�����򷵻�false
	 */
	static bool GetRecordPage(std::vector<RecordData>& data,
		const std::string& timeStart, const std::string& timeEnd, size_t page, size_t count = 10);
	/**
	 * @brief �����������ݵ�ԭʼ���ݱ�
	 * @param[in] mapData ��������
	 * @return �ɹ�����true�����򷵻�false
	 */
	static bool InsertSamples(const std::map<uint16_t, std::vector<SampleData>>& mapData);
	/**
	 * @brief ��ȡϵͳ������
	 * @param[out] config ������
	 * @return �ɹ�����true�����򷵻�false
	 */
	static bool GetItemConfig(std::map<std::string, Json::Value>& config);
	/**
	 * @brief ����ϵͳ������
	 * @param[in] itemName ����������
	 * @param[in] itemValue ������ֵ
	 * @return �ɹ�����true�����򷵻�false
	 */
	static bool UpdateItemConfig(std::string itemName, Json::Value& itemValue);
	/**
	 * @brief ����advance debug����
	 * @param[in] data 
	 * @param[in] time ʱ�䣬Ĭ��Ϊ���ַ���
	 * @return �ɹ�����true�����򷵻�false
	 */
	static bool InsertAdvance(const CollectionData& data,const std::string& time = "");
	/**
	 * @brief ��ȡadvance debug����
	 * @param[out] data
	 * @param[in] timeStart ��ʼʱ��
	 * @return �ɹ�����true�����򷵻�false
	 */
	static bool GetAdvance(AdvanceData& data, const std::string& timeStart);
	/**
	 * @brief ����������ݵ�������ݱ�
	 * @param[in] Info ���������
	 * @return �ɹ�����true�����򷵻�false
	 */
	static bool InsertWellDiags(const DiagDataItem&& Info);
	/**
	 * @brief ��ȡָ�����ͺ͹ؼ��ֵ��������
	 * @param[out] lvInfo ��������б�
	 * @param[in] type �������
	 * @param[in] key �ؼ���
	 * @param[in] order ����ʽ
	 * @param[in] page ��ǰҳ��
	 * @param[in] count ÿҳ����������Ĭ��Ϊ10
	 * @return �ɹ�����true�����򷵻�false
	 */
	static bool SelectDiags(std::list<DiagDataItem>& lvInfo,
		const std::string& type, const std::string& key, const std::string& order,
		size_t page, size_t count = 10);
	/**
	 * @brief ��ȡָ�����͵������������
	 * @param[out] total ��������
	 * @param[in] type �������
	 * @return �ɹ�����true�����򷵻�false
	 */
	static bool SelectDiagsCount(size_t& total,const std::string& type);
	/**
	 * @brief ��ȡָ��ʱ����ڵ�������Ϣ
	 *
	 * @param [out] info ������Ϣ�б�
	 * @param [in] timeStart ��ʼʱ��
	 * @param [in] timeEnd ����ʱ��
	 * @return bool ��ȡ�ɹ�����true�����򷵻�false
	 */
	static bool GetCommentsInTime(std::vector<CommentsData>& info,
		const std::string& timeStart, const std::string& timeEnd);
	/**
	 * @brief ����һ��������Ϣ
	 *
	 * @param [in] data �������������Ϣ
	 * @return bool ����ɹ�����true�����򷵻�false
	 */
	static bool InsertComments(CommentsData& data);
	//���Խӿ�
	static bool GetLastOriginData(std::map<uint16_t, std::vector<SampleData>>& pInSample);
	static bool GetOriginDataById(std::map<uint16_t, std::vector<SampleData>>& pInSample,
		int id, int year, int month);
};

