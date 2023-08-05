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
	std::vector<std::thread> tasks;							//任务线程池
	std::map<DeviceIns, InsInfo> m_mapInsInfo;				//指令行为map
	uint16_t m_val40001 = 0;								//40001地址值存储
	modbus_t* m_pmbs = nullptr;								//mbs通讯指针
	std::atomic_bool threadHold = true;						//线程池保持
	std::atomic_bool connectReady = false;					//连接状态

	std::mutex mtxConnect;
	std::condition_variable cvConnect;						

	uint16_t m_vFrom40009to40026[18] = {};					//地址值数组
	std::atomic<uint16_t> m_dataIndex = 0;					//数据采集序号
	std::mutex m_mtxIns;
	std::list<DeviceIns> m_lvCurIns;						//指令集

	std::mutex mtxCollect;
	std::map<uint16_t, std::vector<SampleData>> mapCollectData; //源数据采集组
	std::atomic_int16_t nCollectFreq = 0;						//采集频次
	std::atomic_bool bCollecting = false;						//采集状态
	std::atomic_bool bCalculating = false;						//计算状态
	std::atomic_bool bStartProcessing = false;					//启动状态

	std::mutex mtxCollectionData_;
	CollectionData realCollectionData_;							//实时采集数据

	RecordData curRecordData_;									//当前计算数据记录
	std::vector<RealF> curPointList_;							//当前实际曲线点数据

	std::atomic_uint32_t alarmInterval_{0};						//报警间隔时间

	MachineState devState;										//实时设备状态

	std::atomic_bool bAdvanceLog_{ false };						//是否记录调试日志
	std::mutex mtxAdvanceReal_;
	AdvanceData advanceRealData_;								//实时调试日志记录
public:
	ModbusDevice();
	~ModbusDevice()
	{
		threadHold = false;
		for (auto& it : tasks)
			it.join();
	}
	/**
	 * @brief 执行工作入口
	 */
	void Work();
	/**
	 * @brief 推送设备指令
	 *
	 * @param ins 要推送的设备指令对象
	 */
	void PushInstruction(DeviceIns ins);
	/**
	 * @brief 获取采集数据对象
	 *
	 * @return 返回当前采集到的数据对象引用
	 */
	const CollectionData& GetCollection();
	/**
	 * @brief 获取记录数据对象
	 *
	 * @return 返回当前记录的数据对象引用
	 */
	const RecordData& GetRecord();
	/**
	 * @brief 获取数据点集合
	 *
	 * @return 返回当前数据点集合对象引用
	 *
	 * @details 该函数用于获取当前数据点集合对象的引用，以供其他操作使用。
	 */
	const std::vector<RealF>& GetPoints();
	/**
	 * @brief 清洗工序
	 *
	 * @param cleanTimes 清洗次数
	 * @param bTest 是否为测试模式
	 *
	 * @details 该函数用于执行设备的清洗工序，具体的清洗流程将根据参数决定。
	 */
	void CleanProcedure(int cleanTimes, bool bTest);
	/**
	 * @brief 获取设备状态对象
	 *
	 * @return 返回当前设备状态对象引用
	 *
	 * @details 该函数用于获取当前设备状态对象的引用，以供其他操作使用。
	 */
	const MachineState& DeviceStates();
	/**
	 * @brief 获取数据对象
	 *
	 * @return 返回当前数据对象引用
	 */
	const AdvanceData& GetAdvanceReal();
	/**
	 * @brief 设置数据调试模式
	 *
	 * @param bDebug 是否开启数据调试模式
	 */
	void SetAdvanceDebug(bool bDebug);
	/**
	 * @brief 获取设备连接状态
	 * @return 返回当前设备连接状态
	 */
	bool IsConnected();
	/**
	 * @brief 数据重新加载
	 *
	 * @param record 指定要加载的记录数据对象，默认为当前记录对象
	 *
	 * @details 该函数用于重新加载数据，如果指定了记录数据对象，则会使用指定的记录数据对象进行数据加载，否则将使用当前记录数据对象进行数据加载。
	 */
	void DataReload(const RecordData& record = RecordData());
protected:
	/**
	 * @brief 更新设备状态
	 * @param realData 设备实时采集数据
	 * @details 根据实时采集的设备数据，更新设备的状态信息。该函数用于实现设备状态的实时更新。
	 */
	void UpdateStates(const CollectionData& realData);
	/**
	 * @brief 更新设备运行状态
	 * @param realData 设备实时采集数据
	 * @details 根据实时采集的设备数据，更新设备的运行状态信息。该函数用于实现设备运行状态的实时更新。
	 */
	void RunsStates(const CollectionData& realData);
	/**
	 * @brief 更新设备硬件故障状态
	 * @param realData 设备实时采集数据
	 * @details 根据实时采集的设备数据，更新设备的硬件故障状态信息。该函数用于实现设备硬件故障状态的实时更新。
	 */
	void HardStates(const CollectionData& realData);
	/**
	 * @brief 更新设备通信故障状态
	 * @param realData 设备实时采集数据
	 * @details 根据实时采集的设备数据，更新设备的通信故障状态信息。该函数用于实现设备通信故障状态的实时更新。
	 */
	void CommunicatedStates(const CollectionData& realData);
	/**
	 * @brief 更新设备工艺保护状态
	 * @param realData 设备实时采集数据
	 * @details 根据实时采集的设备数据，更新设备的工艺保护状态信息。该函数用于实现设备工艺保护状态的实时更新。
	 */
	void ProtectStates(const CollectionData& realData);
	/**
	 * @brief 更新设备工艺警告状态
	 * @param realData 设备实时采集数据
	 * @details 根据实时采集的设备数据，更新设备的工艺警告状态信息。该函数用于实现设备工艺警告状态的实时更新。
	 */
	void WarnStates(const CollectionData& realData);
	/**
	 * @brief 生成诊断数据
	 * @param info 诊断数据项
	 * @param code 错误代码
	 * @param des 错误描述
	 * @param bInsert 是否插入数据
	 * @details 根据输入的错误代码和描述，生成诊断数据项，并插入到诊断数据中。该函数用于实现设备故障诊断功能。
	 */
	void MakeDiagData(const SStringT&& title, const SStringT&& code, const SStringT&& des, bool bInsert);

	/**
	 * @brief 开始连接函数，启动数据采集处理。
	 *
	 */
	void StartConnect();
	/**
	 * @brief 循环内部处理函数，用于从设备读取数据并进行采集处理。
	 *
	 */
	void DataFromLoop();
	/**
	 * @brief 采集处理函数，用于处理从设备读取的原始数据并转换为有效数据。
	 *
	 * @param realData 实时采集数据。
	 */
	void CollectProcess(const CollectionData& realData);
	/**
	 * @brief 记录并计算处理函数，用于记录处理数据并进行计算。
	 *
	 */
	void RecordAndCalc();
	/**
	 * @brief 暂停测试后处理函数，用于暂停测试并进行清理。
	 *
	 */
	void AfterPauseTest();
	/**
	 * @brief 准备清理函数，用于在停止测试前准备清理工作。
	 *
	 */
	void PrepareToClean();
	/**
	 * @brief 开始处理函数，用于启动数据处理。
	 *
	 * @param bStart 是否启动数据处理。
	 */
	void StartProcessing(bool bStart);
	/**
	 * @brief 设置采集数据函数，用于设置实时采集数据。
	 *
	 * @param realData 实时采集数据。
	 */
	void SetCollectionData(const CollectionData& realData);
	/**
	 * @brief 设置实时数据函数，用于设置实时采集数据。
	 *
	 * @param realData 实时采集数据。
	 * @param time 采集数据时间。
	 */
	void SetAdvanceRealData(const CollectionData& realData,const std::string time);
	/**
	 * @brief 采样计算函数，用于计算采样数据。
	 *
	 * @param mapInfo 采样信息。
	 */
	void CalcOfSamples(std::map<uint16_t, std::vector<SampleData>> mapInfo);
	/**
	 * @brief 设备时间计时器函数，用于计算设备时间。
	 *
	 * @param realData 实时采集数据。
	 */
	void DeviceChrono(const CollectionData& realData);
	/**
	 * @brief 清理时间计时器函数，用于计算清理时间。
	 *
	 * @param time 清理时间。
	 */
	void CleanChrono(const std::string& time);
	/**
	 * @brief 报警时间计时器函数，用于计算报警时间。
	 *
	 */
	void AlarmChrono();
private:
	/**
	 * @brief 运行当前指令函数
	 * @details 该函数用于运行当前指令，不接收任何参数，也不返回任何值。
	 */
	void RunCurrentInstruction();
	/**
	 * @brief 寄存器写入回调函数
	 * @param addr 寄存器地址信息
	 * @param buf 写入的数据缓
	 * @return bool 写入是否成功的布尔值
	 */
	bool OnRigsterWrite(AddrType addr, uint16_t buf);
	/**
	 * @brief 寄存器写入回调函数
	 * @return bool 写入是否成功的布尔值
	 * @details 该函数作为多个寄存器写入的回调函数，不接收任何参数，但返回一个布尔值，表示写入是否成功。
	 */
	bool OnRigstersWrite();

	std::map<std::string, std::string> mapFlowmeterType_{ {"CDKSD","1"},{"DFJD","2"},{"SURE","3"} };
};