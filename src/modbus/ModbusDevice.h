#pragma once
#include <vector>
#include <string>
#include <mutex>
#include <thread>
#include <map>
#include <atomic>
#include <list>
#include "modbus.h"
#include "DeviceBase.h"
#include "SystemDef.h"


enum AddrType
{
	addr40001 = 0,
	addr40002,
	addr40003,
	addr40009 = 8
};

struct InsInfo
{
	InsInfo() :addr(AddrType::addr40001), sigVal(0) {}
	InsInfo(AddrType a, uint16_t sig) :addr(a), sigVal(sig) {}
	AddrType addr;
	uint16_t sigVal;
};

class ModbusDevice :public DeviceBase
{
private:
	std::vector<std::thread> tasks;							//�����̳߳�
	std::map<DeviceIns, InsInfo> m_mapInsInfo;				//ָ����Ϊmap
	uint16_t m_val40001 = 0;								//40001��ֵַ�洢
	modbus_t* m_pmbs = nullptr;								//mbsͨѶָ��
	std::atomic_bool threadHold = true;						//�̳߳ر���
	std::atomic_bool connectReady = false;					//����״̬

	std::mutex mtxConnect;
	std::condition_variable cvConnect;						

	uint16_t m_vFrom40009to40026[18] = {};					//��ֵַ����
	std::atomic<uint16_t> m_dataIndex = 0;					//���ݲɼ����
	std::mutex m_mtxIns;
	std::list<DeviceIns> m_lvCurIns;						//ָ�

	std::mutex mtxCollect;
	std::map<uint16_t, std::vector<SampleData>> mapCollectData; //Դ���ݲɼ���
	std::atomic_int16_t nCollectFreq = 0;						//�ɼ�Ƶ��
	std::atomic_bool bCollecting = false;						//�ɼ�״̬
	std::atomic_bool bCalculating = false;						//����״̬
	std::atomic_bool bStartProcessing = false;					//����״̬

	std::mutex mtxCollectionData_;
	CollectionData realCollectionData_;							//ʵʱ�ɼ�����

	RecordData curRecordData_;									//��ǰ�������ݼ�¼
	std::vector<RealF> curPointList_;							//��ǰʵ�����ߵ�����

	std::atomic_uint32_t alarmInterval_{0};						//�������ʱ��

	MachineState devState;										//ʵʱ�豸״̬

	std::atomic_bool bAdvanceLog_{ false };						//�Ƿ��¼������־
	std::mutex mtxAdvanceReal_;
	AdvanceData advanceRealData_;								//ʵʱ������־��¼
public:
	ModbusDevice();
	~ModbusDevice()
	{
		threadHold = false;
		for (auto& it : tasks)
			it.join();
	}
	/**
	 * @brief ִ�й������
	 */
	void Work();
	/**
	 * @brief �����豸ָ��
	 *
	 * @param ins Ҫ���͵��豸ָ�����
	 */
	void PushInstruction(DeviceIns ins);
	/**
	 * @brief ��ȡ�ɼ����ݶ���
	 *
	 * @return ���ص�ǰ�ɼ��������ݶ�������
	 */
	const CollectionData& GetCollection();
	/**
	 * @brief ��ȡ��¼���ݶ���
	 *
	 * @return ���ص�ǰ��¼�����ݶ�������
	 */
	const RecordData& GetRecord();
	/**
	 * @brief ��ȡ���ݵ㼯��
	 *
	 * @return ���ص�ǰ���ݵ㼯�϶�������
	 *
	 * @details �ú������ڻ�ȡ��ǰ���ݵ㼯�϶�������ã��Թ���������ʹ�á�
	 */
	const std::vector<RealF>& GetPoints();
	/**
	 * @brief ��ϴ����
	 *
	 * @param cleanTimes ��ϴ����
	 * @param bTest �Ƿ�Ϊ����ģʽ
	 *
	 * @details �ú�������ִ���豸����ϴ���򣬾������ϴ���̽����ݲ���������
	 */
	void CleanProcedure(int cleanTimes, bool bTest);
	/**
	 * @brief ��ȡ�豸״̬����
	 *
	 * @return ���ص�ǰ�豸״̬��������
	 *
	 * @details �ú������ڻ�ȡ��ǰ�豸״̬��������ã��Թ���������ʹ�á�
	 */
	const MachineState& DeviceStates();
	/**
	 * @brief ��ȡ���ݶ���
	 *
	 * @return ���ص�ǰ���ݶ�������
	 */
	const AdvanceData& GetAdvanceReal();
	/**
	 * @brief �������ݵ���ģʽ
	 *
	 * @param bDebug �Ƿ������ݵ���ģʽ
	 */
	void SetAdvanceDebug(bool bDebug);
	/**
	 * @brief ��ȡ�豸����״̬
	 * @return ���ص�ǰ�豸����״̬
	 */
	bool IsConnected();
	/**
	 * @brief �������¼���
	 *
	 * @param record ָ��Ҫ���صļ�¼���ݶ���Ĭ��Ϊ��ǰ��¼����
	 *
	 * @details �ú����������¼������ݣ����ָ���˼�¼���ݶ������ʹ��ָ���ļ�¼���ݶ���������ݼ��أ�����ʹ�õ�ǰ��¼���ݶ���������ݼ��ء�
	 */
	void DataReload(const RecordData& record = RecordData());
protected:
	/**
	 * @brief �����豸״̬
	 * @param realData �豸ʵʱ�ɼ�����
	 * @details ����ʵʱ�ɼ����豸���ݣ������豸��״̬��Ϣ���ú�������ʵ���豸״̬��ʵʱ���¡�
	 */
	void UpdateStates(const CollectionData& realData);
	/**
	 * @brief �����豸����״̬
	 * @param realData �豸ʵʱ�ɼ�����
	 * @details ����ʵʱ�ɼ����豸���ݣ������豸������״̬��Ϣ���ú�������ʵ���豸����״̬��ʵʱ���¡�
	 */
	void RunsStates(const CollectionData& realData);
	/**
	 * @brief �����豸Ӳ������״̬
	 * @param realData �豸ʵʱ�ɼ�����
	 * @details ����ʵʱ�ɼ����豸���ݣ������豸��Ӳ������״̬��Ϣ���ú�������ʵ���豸Ӳ������״̬��ʵʱ���¡�
	 */
	void HardStates(const CollectionData& realData);
	/**
	 * @brief �����豸ͨ�Ź���״̬
	 * @param realData �豸ʵʱ�ɼ�����
	 * @details ����ʵʱ�ɼ����豸���ݣ������豸��ͨ�Ź���״̬��Ϣ���ú�������ʵ���豸ͨ�Ź���״̬��ʵʱ���¡�
	 */
	void CommunicatedStates(const CollectionData& realData);
	/**
	 * @brief �����豸���ձ���״̬
	 * @param realData �豸ʵʱ�ɼ�����
	 * @details ����ʵʱ�ɼ����豸���ݣ������豸�Ĺ��ձ���״̬��Ϣ���ú�������ʵ���豸���ձ���״̬��ʵʱ���¡�
	 */
	void ProtectStates(const CollectionData& realData);
	/**
	 * @brief �����豸���վ���״̬
	 * @param realData �豸ʵʱ�ɼ�����
	 * @details ����ʵʱ�ɼ����豸���ݣ������豸�Ĺ��վ���״̬��Ϣ���ú�������ʵ���豸���վ���״̬��ʵʱ���¡�
	 */
	void WarnStates(const CollectionData& realData);
	/**
	 * @brief �����������
	 * @param info ���������
	 * @param code �������
	 * @param des ��������
	 * @param bInsert �Ƿ��������
	 * @details ��������Ĵ������������������������������뵽��������С��ú�������ʵ���豸������Ϲ��ܡ�
	 */
	void MakeDiagData(const SStringT&& title, const SStringT&& code, const SStringT&& des, bool bInsert);

	/**
	 * @brief ��ʼ���Ӻ������������ݲɼ�����
	 *
	 */
	void StartConnect();
	/**
	 * @brief ѭ���ڲ������������ڴ��豸��ȡ���ݲ����вɼ�����
	 *
	 */
	void DataFromLoop();
	/**
	 * @brief �ɼ������������ڴ�����豸��ȡ��ԭʼ���ݲ�ת��Ϊ��Ч���ݡ�
	 *
	 * @param realData ʵʱ�ɼ����ݡ�
	 */
	void CollectProcess(const CollectionData& realData);
	/**
	 * @brief ��¼�����㴦���������ڼ�¼�������ݲ����м��㡣
	 *
	 */
	void RecordAndCalc();
	/**
	 * @brief ��ͣ���Ժ�������������ͣ���Բ���������
	 *
	 */
	void AfterPauseTest();
	/**
	 * @brief ׼����������������ֹͣ����ǰ׼����������
	 *
	 */
	void PrepareToClean();
	/**
	 * @brief ��ʼ�������������������ݴ���
	 *
	 * @param bStart �Ƿ��������ݴ���
	 */
	void StartProcessing(bool bStart);
	/**
	 * @brief ���òɼ����ݺ�������������ʵʱ�ɼ����ݡ�
	 *
	 * @param realData ʵʱ�ɼ����ݡ�
	 */
	void SetCollectionData(const CollectionData& realData);
	/**
	 * @brief ����ʵʱ���ݺ�������������ʵʱ�ɼ����ݡ�
	 *
	 * @param realData ʵʱ�ɼ����ݡ�
	 * @param time �ɼ�����ʱ�䡣
	 */
	void SetAdvanceRealData(const CollectionData& realData,const std::string time);
	/**
	 * @brief �������㺯�������ڼ���������ݡ�
	 *
	 * @param mapInfo ������Ϣ��
	 */
	void CalcOfSamples(std::map<uint16_t, std::vector<SampleData>> mapInfo);
	/**
	 * @brief �豸ʱ���ʱ�����������ڼ����豸ʱ�䡣
	 *
	 * @param realData ʵʱ�ɼ����ݡ�
	 */
	void DeviceChrono(const CollectionData& realData);
	/**
	 * @brief ����ʱ���ʱ�����������ڼ�������ʱ�䡣
	 *
	 * @param time ����ʱ�䡣
	 */
	void CleanChrono(const std::string& time);
	/**
	 * @brief ����ʱ���ʱ�����������ڼ��㱨��ʱ�䡣
	 *
	 */
	void AlarmChrono();
private:
	/**
	 * @brief ���е�ǰָ���
	 * @details �ú����������е�ǰָ��������κβ�����Ҳ�������κ�ֵ��
	 */
	void RunCurrentInstruction();
	/**
	 * @brief �Ĵ���д��ص�����
	 * @param addr �Ĵ�����ַ��Ϣ
	 * @param buf д������ݻ�
	 * @return bool д���Ƿ�ɹ��Ĳ���ֵ
	 */
	bool OnRigsterWrite(AddrType addr, uint16_t buf);
	/**
	 * @brief �Ĵ���д��ص�����
	 * @return bool д���Ƿ�ɹ��Ĳ���ֵ
	 * @details �ú�����Ϊ����Ĵ���д��Ļص��������������κβ�����������һ������ֵ����ʾд���Ƿ�ɹ���
	 */
	bool OnRigstersWrite();

	std::map<std::string, std::string> mapFlowmeterType_{ {"CDKSD","1"},{"DFJD","2"},{"SURE","3"} };
};