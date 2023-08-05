#include "stdafx.h"
#include "ModbusDevice.h"
#include "Utils.h"
#include <bitset>
#include "AppFramework.h"
#include "AnalysisSession.h"
#include "MysqlClient.h"
#include "Logger.h"

using namespace realology;

ModbusDevice::ModbusDevice()
{
	//预设指令信息addr40001
	m_mapInsInfo[DeviceIns::save_complete] = InsInfo(AddrType::addr40001, static_cast<uint16_t>(std::bitset<16>().set(0, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::test_start] = InsInfo(AddrType::addr40001, static_cast<uint16_t>(std::bitset<16>().set(1, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::test_pause] = InsInfo(AddrType::addr40001, static_cast<uint16_t>(std::bitset<16>().set(2, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::auto_clean] = InsInfo(AddrType::addr40001, static_cast<uint16_t>(std::bitset<16>().set(3, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::soft_stop] = InsInfo(AddrType::addr40001, static_cast<uint16_t>(std::bitset<16>().set(4, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::soft_restore] = InsInfo(AddrType::addr40001, 0);
	m_mapInsInfo[DeviceIns::pump_reset] = InsInfo(AddrType::addr40001, static_cast<uint16_t>(std::bitset<16>().set(5, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::calc_error] = InsInfo(AddrType::addr40001, static_cast<uint16_t>(std::bitset<16>().set(6, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::calc_complete] = InsInfo(AddrType::addr40001, static_cast<uint16_t>(std::bitset<16>().set(7, 1).to_ulong()));


	//预设指令信息addr40002
	m_mapInsInfo[DeviceIns::circulating_pump_1] = InsInfo(AddrType::addr40002, static_cast<uint16_t>(std::bitset<16>().set(0, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::circulating_pump_0] = InsInfo(AddrType::addr40002, static_cast<uint16_t>(std::bitset<16>().set(1, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::security_valve_1] = InsInfo(AddrType::addr40002, static_cast<uint16_t>(std::bitset<16>().set(2, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::security_valve_0] = InsInfo(AddrType::addr40002, static_cast<uint16_t>(std::bitset<16>().set(3, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::purge_valve_1] = InsInfo(AddrType::addr40002, static_cast<uint16_t>(std::bitset<16>().set(4, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::purge_valve_0] = InsInfo(AddrType::addr40002, static_cast<uint16_t>(std::bitset<16>().set(5, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::washing_pump_1] = InsInfo(AddrType::addr40002, static_cast<uint16_t>(std::bitset<16>().set(6, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::washing_pump_0] = InsInfo(AddrType::addr40002, static_cast<uint16_t>(std::bitset<16>().set(7, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::return_valve_mud_1] = InsInfo(AddrType::addr40002, static_cast<uint16_t>(std::bitset<16>().set(8, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::return_valve_mud_0] = InsInfo(AddrType::addr40002, static_cast<uint16_t>(std::bitset<16>().set(9, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::return_valve_detergent_1] = InsInfo(AddrType::addr40002, static_cast<uint16_t>(std::bitset<16>().set(10, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::return_valve_detergent_0] = InsInfo(AddrType::addr40002, static_cast<uint16_t>(std::bitset<16>().set(11, 1).to_ulong()));

	m_mapInsInfo[DeviceIns::debug_mode_1] = InsInfo(AddrType::addr40003, static_cast<uint16_t>(std::bitset<16>().set(15, 1).to_ulong()));
	m_mapInsInfo[DeviceIns::debug_mode_0] = InsInfo(AddrType::addr40003, 0);
}

void ModbusDevice::Work()
{
	DataReload();
	StartConnect();
	DataFromLoop();
}

void ModbusDevice::StartConnect()
{
	tasks.emplace_back(std::thread([this]()
		{
			while (threadHold)
			{
				std::unique_lock<std::mutex> lock(mtxConnect);
				cvConnect.wait(lock, [this] {
					return connectReady == false;
					});
				if (m_pmbs != nullptr)
				{
					modbus_close(m_pmbs);
					modbus_free(m_pmbs);
					m_pmbs = nullptr;
				}
				std::string ip = App().GetItemConfig("Communication", "device_ip");
				m_pmbs = modbus_new_tcp(ip.c_str(), 502);

				modbus_set_slave(m_pmbs, 1);

				int nRet = modbus_connect(m_pmbs);

				if (-1 == nRet)
				{
					modbus_close(m_pmbs);
					modbus_free(m_pmbs);
					m_pmbs = nullptr;
					App().AsyncEvent(WM_NETWORK_UPDATE);
					std::this_thread::sleep_for(std::chrono::seconds(1));
					continue;
				}

				struct timeval tv;
				tv.tv_sec = 0;
				tv.tv_usec = 2000000;
				modbus_set_response_timeout(m_pmbs, tv.tv_sec, tv.tv_usec);
				App().AsyncEvent(WM_NETWORK_UPDATE);
				connectReady = true;
				cvConnect.notify_all();
			}
		}));
}

void ModbusDevice::DataFromLoop()
{
	tasks.emplace_back(std::thread([this]()
		{
			while (threadHold)
			{
				std::unique_lock<std::mutex> lock(mtxConnect);
				cvConnect.wait(lock, [this] {
					return connectReady == true;
					});
				CollectionData realData;
				uint16_t val40001[1] = {};			//read from 40001			读写寄存器地址
				uint16_t val40201to40202[2] = {};	//read from 40201 to 40202	读操作位反馈地址
				uint16_t val40211to40216[6] = {};	//read from 40211 to 40216	读报警位地址
				uint16_t val40221[1] = {};			//read from 40221			读状态标志位
				uint16_t val40231to40264[34] = {};			//read from 40231 to 40260	读参数采集

				if (modbus_read_registers(m_pmbs, 0, 1, val40001) == -1 ||
					modbus_read_registers(m_pmbs, 200, 2, val40201to40202) == -1 ||
					modbus_read_registers(m_pmbs, 210, 6, val40211to40216) == -1 ||
					modbus_read_registers(m_pmbs, 220, 1, val40221) == -1 ||
					modbus_read_registers(m_pmbs, 230, 34, val40231to40264) == -1)
				{
					connectReady = false;
					cvConnect.notify_all();
					continue;
				}

				m_val40001 = val40001[0];
				realData.addr001 = m_val40001;
				realData.addr201 = val40201to40202[0];
				realData.addr202 = val40201to40202[1];
				realData.addr211 = val40211to40216[0];
				realData.addr212 = val40211to40216[1];
				realData.addr213 = val40211to40216[2];
				realData.addr214 = val40211to40216[3];
				realData.addr215 = val40211to40216[4];
				realData.addr216 = val40211to40216[5];
				realData.addr221 = val40221[0];


				realData.fPumpPressure = modbus_get_float(&val40231to40264[0]);
				realData.fUpstreamPressure = modbus_get_float(&val40231to40264[2]);
				realData.fDownstreamPressure = modbus_get_float(&val40231to40264[4]);
				realData.fOutPressure = modbus_get_float(&val40231to40264[6]);
				realData.fOpening = modbus_get_float(&val40231to40264[8]);
				realData.fMassFlow = modbus_get_float(&val40231to40264[10]);
				realData.fVolFlow = modbus_get_float(&val40231to40264[12]);
				realData.fTemperature = modbus_get_float(&val40231to40264[14]);
				realData.fDensity = modbus_get_float(&val40231to40264[16]);
				realData.fDiffPressure = modbus_get_float(&val40231to40264[18]);
				realData.fDiffPressureStr = modbus_get_float(&val40231to40264[20]);
				realData.fEV0 = modbus_get_float(&val40231to40264[22]);
				realData.fEVn = modbus_get_float(&val40231to40264[24]);
				realData.fMudLevel = modbus_get_float(&val40231to40264[26]);
				realData.nSampleIndex = val40231to40264[28];
				m_dataIndex = val40231to40264[28];

				realData.nPLCType = val40231to40264[29];
				realData.fPH = modbus_get_float(&val40231to40264[30]);
				realData.fCl = modbus_get_float(&val40231to40264[32]);
				SetCollectionData(realData);
				DeviceChrono(realData);
				UpdateStates(realData);
				App().AsyncEvent(WM_WNDREAL_UPDATE);
				RunCurrentInstruction();
			}
		}));
}

void ModbusDevice::CollectProcess(const CollectionData& realData)
{
	if (bCollecting || bCalculating)
		return;
	bCollecting = true;
	App().AsyncTask([this, realData] {
		uint16_t index = m_dataIndex.load();
		SampleData data;
		size_t dataSize = 0;
		std::unique_lock<std::mutex> lock(mtxCollect);
		if (mapCollectData.find(index) != mapCollectData.end())
		{
			data = mapCollectData[index].back();
			dataSize = mapCollectData[index].size();
		}
		if (realData.fMassFlow > 0 && realData.fUpstreamPressure > 0 &&
			data.massFlow != realData.fMassFlow && data.fUpstreamPressure != realData.fUpstreamPressure)
		{
			nCollectFreq = 0;
			data.fVolFlow = realData.fVolFlow;
			data.fOpening = realData.fOpening;
			data.fMudLevel = realData.fMudLevel;
			data.pumpPressure = realData.fPumpPressure * 1000.0f;
			data.massFlow = realData.fMassFlow;
			data.temperature = realData.fTemperature;
			data.density = realData.fDensity;
			data.diffPressure = realData.fDiffPressure * 1000.0f;
			data.fUpstreamPressure = realData.fUpstreamPressure * 1000.0f;
			data.fDownstreamPressure = realData.fDownstreamPressure * 1000.0f;

			data.sampleTime = Utils::GetDateTime();
			mapCollectData[index].push_back(data);
		}
		else
		{
			nCollectFreq++;
		}
		lock.unlock();
		if (dataSize >= 20 || nCollectFreq >= 1500)
		{
			PushInstruction(DeviceIns::save_complete);
			LOGINFO("complete index={},freq={}", index, nCollectFreq);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			nCollectFreq = 0;
		}
		bCollecting = false;
		}
	);
}

void ModbusDevice::RecordAndCalc()
{
	if (bCalculating)
		return;
	bCalculating = true;
	PrepareToClean();
	std::lock_guard<std::mutex> lock(mtxCollect);
	if (mapCollectData.size() > 0)
	{
		if (mapCollectData.size() > 20)
		{
			LOGINFO("calc start with data size = {}", mapCollectData.size());
			CalcOfSamples(mapCollectData);
			MysqlClient::InsertSamples(mapCollectData);
		}
		else
		{
			PushInstruction(DeviceIns::calc_error);
		}
		LOGINFO("mapInfo clear data size = {}", mapCollectData.size());
		mapCollectData.clear();
		PushInstruction(DeviceIns::calc_complete);
	}
	bCalculating = false;
}

void ModbusDevice::AfterPauseTest()
{
	PrepareToClean();
	std::lock_guard<std::mutex> lock(mtxCollect);
	if (mapCollectData.size() > 0)
	{
		LOGINFO("pause clean data size = {}", mapCollectData.size());
		mapCollectData.clear();
	}
}

void ModbusDevice::PrepareToClean()
{
	if (devState.bAppNeedClean)
	{
		devState.bAppNeedClean = false;
		int clean = std::stoi(App().GetItemConfig("Other", "clean_times"));
		if (clean > 0)
		{
			CleanProcedure(clean, true);
		}
	}
}

void ModbusDevice::StartProcessing(bool bStart)
{
	if (bStartProcessing) return;
	bStartProcessing = true;
	App().AsyncTask([this, bStart] {
		PushInstruction(bStart ? test_start : test_pause);
		std::this_thread::sleep_for(std::chrono::seconds(5));
		bStartProcessing = false;
		});
}

void ModbusDevice::SetCollectionData(const CollectionData& realData)
{
	std::lock_guard<std::mutex> lock(mtxCollectionData_);
	realCollectionData_ = realData;
}

void ModbusDevice::SetAdvanceRealData(const CollectionData& realData, const std::string time)
{
	App().AsyncTask([this, realData, time] {
		if (bAdvanceLog_)
		{
			MysqlClient::InsertAdvance(realCollectionData_, time);
		}
		std::lock_guard<std::mutex> lglock(mtxAdvanceReal_);
		std::string timeStart = Utils::GetDateTimeFrom(time, -1800);
		advanceRealData_.vTime.push_back(time);
		advanceRealData_.mapValue[L"checked_press"].push_back(realData.fPumpPressure);
		advanceRealData_.mapValue[L"checked_vflow"].push_back(realData.fVolFlow);
		advanceRealData_.mapValue[L"checked_diffpress"].push_back(realData.fDiffPressure);
		advanceRealData_.mapValue[L"checked_opening"].push_back(realData.fOpening / 100.0f);
		advanceRealData_.mapValue[L"checked_mlevel"].push_back(realData.fMudLevel / 1000.0f);
		while (advanceRealData_.vTime.front() < timeStart)
		{
			advanceRealData_.RemoveOne();
		}
		});
}

void ModbusDevice::CalcOfSamples(std::map<uint16_t, std::vector<SampleData>> mapInfo)
{
	App().AsyncTask([this, mapInfo = std::move(mapInfo)] {
		AnalysisConfig config = App().GetAnalysisConfig();
		config.model_ = App().GetItemConfig("Calculation", "calcmodel");
		AnalysisSession analysis(config);
		analysis.Analysis(mapInfo);
		if ((analysis.caclRecord_.size() == 0) || (analysis.realPoint_.size() == 0))
		{
			PushInstruction(DeviceIns::calc_error);
			LOGINFO("check up device and data");
			return;
		}
		DataReload(analysis.caclRecord_.rbegin()->second);
		curPointList_ = analysis.realPoint_.rbegin()->second;
		alarmInterval_ = std::stoi(App().GetItemConfig("Communication", "alarm_interval")) * 60;

		App().AsyncEvent(WM_WNDCALC_UPDATE);
		App().SendToServer();
		});
}

void ModbusDevice::DeviceChrono(const CollectionData& realData)
{
	App().AsyncTask([this, realData] {
		static std::string lastTime = Utils::GetDateTime();
		std::string curTime = Utils::GetDateTime();
		if (lastTime == curTime)
			return;
		lastTime = curTime;
		SetAdvanceRealData(realData, curTime);
		CleanChrono(curTime);
		AlarmChrono();
		});
}

void ModbusDevice::CleanChrono(const std::string& time)
{
	//定时清洗
	App().AsyncTask([this, time] {
		if (App().GetItemConfig("Other", "enable_cyclic_clean") == "1")
		{
			std::string end_time = App().GetItemConfig("Other", "clean_end_time");
			if (end_time == time)
			{
				if (devState.bSignTesting)
				{
					LOGINFO("prepare to clean when test pause");
					devState.bAppNeedClean = true;
				}
				else
				{
					int clean = std::stoi(App().GetItemConfig("Other", "clean_times"));
					if (clean > 0)
					{
						LOGINFO("time to clean");
						CleanProcedure(clean, false);
					}
				}
				App().AsyncEvent(WM_TIMER_CLEAN);
			}
		}
		});
}

void ModbusDevice::AlarmChrono()
{
	App().AsyncTask([this] {
		--alarmInterval_;
		if (alarmInterval_ <= 0)
		{
			App().PostAlarm();
			alarmInterval_ = std::stoi(App().GetItemConfig("Communication", "alarm_interval")) * 60;
			LOGINFO("timeout calculation alarm");
		}
		});
}



void ModbusDevice::PushInstruction(DeviceIns ins)
{
	if (!connectReady) return;
	if (ins == test_start)
	{
		App().SetItemConfig(STRING_RUNTIME, STRING_ISTEST, "1");
	}
	if (ins == test_pause)
	{
		App().SetItemConfig(STRING_RUNTIME, STRING_ISTEST, "0");
	}
	std::lock_guard<std::mutex> lock_local(m_mtxIns);
	if (std::find(m_lvCurIns.begin(), m_lvCurIns.end(), ins) == m_lvCurIns.end())
	{
		m_lvCurIns.push_back(ins);
	}
}

const CollectionData& ModbusDevice::GetCollection()
{
	std::lock_guard<std::mutex> lock(mtxCollectionData_);
	return realCollectionData_;
}

const RecordData& ModbusDevice::GetRecord()
{
	return curRecordData_;
}

const std::vector<RealF>& ModbusDevice::GetPoints()
{
	return curPointList_;
}

void ModbusDevice::CleanProcedure(int cleanTimes, bool bTest)
{
	static bool cleanFlag = false;
	if (cleanFlag == true) return;
	App().AsyncTask([this, cleanTimes, bTest] {
		cleanFlag = true;
		if (bTest)
		{
			int pauseTimeout = 0;//暂停测试超时
			PushInstruction(DeviceIns::test_pause);
			while (true)
			{
				pauseTimeout++;
				if (pauseTimeout > 60)
				{
					LOGINFO("test pause failure,break clean");
					cleanFlag = false;
					return;
				}
				if (!devState.bSignTesting)
				{
					LOGINFO("test pause and clean");
					break;
				}
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}

		int times = 1;
		LOGINFO("clean start");
		while (true)
		{
			if (times > cleanTimes)
			{
				LOGINFO("clean complete");
				break;
			}
			if (!devState.bSignCleaning)
			{
				PushInstruction(DeviceIns::auto_clean);
				LOGINFO("auto clean {}", times);
				MysqlClient::InsertOperation(SStringA().Format("auto Clean %d", times).GetBuffer(0));
				int cleanTimeout = 0;//超时设置十分钟
				while (true)
				{
					cleanTimeout++;
					if (cleanTimeout > 6000)
					{
						LOGINFO("auto clean time over {} seconds", cleanTimeout);
						times++;
						break;
					}
					if (devState.bSignCleanEnd)
					{
						times++;
						break;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		if (bTest)
		{
			int timeout = 0;
			while (true)
			{
				PushInstruction(DeviceIns::test_start);
				timeout++;
				if (timeout > 60)
				{
					LOGINFO("clean complete,start test failure");
					break;
				}
				if (devState.bSignTesting)
				{
					LOGINFO("clean complete,start test");
					break;
				}
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
		cleanFlag = false;
		});
}

const MachineState& ModbusDevice::DeviceStates()
{
	return devState;
}

const AdvanceData& ModbusDevice::GetAdvanceReal()
{
	std::lock_guard<std::mutex> lock(mtxAdvanceReal_);
	return advanceRealData_;
}

void ModbusDevice::SetAdvanceDebug(bool bDebug)
{
	bAdvanceLog_ = bDebug;
}

bool ModbusDevice::IsConnected()
{
	return connectReady;
}

void ModbusDevice::DataReload(const RecordData& record)
{
	alarmInterval_ = std::stoi(App().GetItemConfig("Communication", "alarm_interval")) * 60;
	static std::mutex mtx;
	std::lock_guard<std::mutex> lock(mtx);
	if (record.model.empty())
		MysqlClient::GetLastRecord(curRecordData_);
	else
	{
		curRecordData_ = record;
		curRecordData_.time = Utils::GetDateTime();
		MysqlClient::InsertRecord(curRecordData_);
	}
}

void ModbusDevice::UpdateStates(const CollectionData& realData)
{
	App().AsyncTask([this, realData] {
		RunsStates(realData);
		HardStates(realData);
		CommunicatedStates(realData);
		ProtectStates(realData);
		WarnStates(realData);
		}
	);
}

void ModbusDevice::RunsStates(const CollectionData& realData)
{
	//设备状态 初始化
	devState.bEStop
		= std::bitset<16>(realData.addr001)[4];


	devState.bCirculatingPumpOperation
		= std::bitset<16>(realData.addr201)[1];
	devState.bSecurityValveOpen
		= std::bitset<16>(realData.addr201)[2];
	devState.bSecurityValveClose
		= std::bitset<16>(realData.addr201)[3];
	devState.bPurgeValveOpen
		= std::bitset<16>(realData.addr201)[4];
	devState.bPurgeValveClose
		= std::bitset<16>(realData.addr201)[5];
	devState.bReturnValveMudOpen
		= std::bitset<16>(realData.addr201)[6];
	devState.bReturnValveMudClose
		= std::bitset<16>(realData.addr201)[7];

	devState.bReturnValveDetergentOpen
		= std::bitset<16>(realData.addr202)[0];
	devState.bReturnValveDetergentClose
		= std::bitset<16>(realData.addr202)[1];
	devState.bCirculatingPumpTrip
		= std::bitset<16>(realData.addr202)[2];

	devState.bWashingPumpOperation
		= std::bitset<16>(realData.addr202)[4];

	//测试过程状态 初始化
	devState.bSignManual
		= std::bitset<16>(realData.addr221)[0];
	devState.bSignAuto
		= std::bitset<16>(realData.addr221)[1];
	devState.bSignTesting
		= std::bitset<16>(realData.addr221)[2];
	devState.bSignTestEnd
		= std::bitset<16>(realData.addr221)[3];
	devState.bSignCleaning
		= std::bitset<16>(realData.addr221)[4];
	devState.bSignCleanEnd
		= std::bitset<16>(realData.addr221)[5];
	devState.bSignDataCollection
		= std::bitset<16>(realData.addr221)[6];
	devState.bSignCollectionCycleing
		= std::bitset<16>(realData.addr221)[7];
	devState.bSignCollectionCycleEnd
		= std::bitset<16>(realData.addr221)[8];

	if (devState.bSignDataCollection)
		CollectProcess(realData);
	if (devState.bSignCollectionCycleEnd)
		RecordAndCalc();
	if (devState.bSignTestEnd)
		AfterPauseTest();

	bool localTestState = std::stoi(App().GetItemConfig(STRING_RUNTIME, STRING_ISTEST));
	if (realData.addr216 > 0 && localTestState)
	{
		App().SetItemConfig(STRING_RUNTIME, STRING_ISTEST, "0");
		LOGINFO("PAUSE TEST(protect)");
		MysqlClient::InsertOperation("PAUSE TEST(protect)");
		App().CleanProcedure(2, false);
	}

	if (localTestState != devState.bSignTesting)
	{
		if (localTestState)
			AfterPauseTest();
		StartProcessing(localTestState);
	}
}

void ModbusDevice::HardStates(const CollectionData& realData)
{
	bool bErrorSecurityValveOpen = std::bitset<16>(realData.addr211)[0];
	bool bErrorSecurityValveClose = std::bitset<16>(realData.addr211)[1];
	bool bErrorPurgeValveOpen = std::bitset<16>(realData.addr211)[2];
	bool bErrorPurgeValveClose = std::bitset<16>(realData.addr211)[3];
	bool bErrorReturnValveMudOpen = std::bitset<16>(realData.addr211)[4];
	bool bErrorReturnValveMudClose = std::bitset<16>(realData.addr211)[5];
	bool bErrorReturnValveDetergentOpen = std::bitset<16>(realData.addr211)[6];
	bool bErrorReturnValveDetergentClose = std::bitset<16>(realData.addr211)[7];
	bool bErrorWashingPumpOpen = std::bitset<16>(realData.addr211)[8];
	bool bErrorWashingPumpClose = std::bitset<16>(realData.addr211)[9];
	bool bErrorCirculatingPump = std::bitset<16>(realData.addr211)[10];

	bool bErrorChoke = std::bitset<16>(realData.addr212)[4];
	bool bErrorPT1 = std::bitset<16>(realData.addr212)[5];
	bool bErrorPT2 = std::bitset<16>(realData.addr212)[6];
	bool bErrorPT3 = std::bitset<16>(realData.addr212)[7];
	bool bErrorPT4 = std::bitset<16>(realData.addr212)[8];
	bool bErrorMFM = std::bitset<16>(realData.addr212)[9];

	if (bErrorSecurityValveOpen != devState.bErrorSecurityValveOpen)
		MakeDiagData(L"Hardware", L"A0100", L"Security Valve Open Failure", bErrorSecurityValveOpen);
	if (bErrorSecurityValveClose != devState.bErrorSecurityValveClose)
		MakeDiagData(L"Hardware", L"A0101", L"Security Valve Close Failure", bErrorSecurityValveClose);
	if (bErrorPurgeValveOpen != devState.bErrorPurgeValveOpen)
		MakeDiagData(L"Hardware", L"A0102", L"Purging Valve Open Failure", bErrorPurgeValveOpen);
	if (bErrorPurgeValveClose != devState.bErrorPurgeValveClose)
		MakeDiagData(L"Hardware", L"A0103", L"Purging Valve Close Failure", bErrorPurgeValveClose);
	if (bErrorReturnValveMudOpen != devState.bErrorReturnValveMudOpen)
		MakeDiagData(L"Hardware", L"A0104", L"Return Valve_Mud Open Failure", bErrorReturnValveMudOpen);
	if (bErrorReturnValveMudClose != devState.bErrorReturnValveMudClose)
		MakeDiagData(L"Hardware", L"A0105", L"Return Valve_Mud Close Failure", bErrorReturnValveMudClose);
	if (bErrorReturnValveDetergentOpen != devState.bErrorReturnValveDetergentOpen)
		MakeDiagData(L"Hardware", L"A0106", L"Return Valve_Detergent Open Failure", bErrorReturnValveDetergentOpen);
	if (bErrorReturnValveDetergentClose != devState.bErrorReturnValveDetergentClose)
		MakeDiagData(L"Hardware", L"A0107", L"Return Valve_Detergent Close Failure", bErrorReturnValveDetergentClose);
	if (bErrorWashingPumpOpen != devState.bErrorWashingPumpOpen)
		MakeDiagData(L"Hardware", L"A0108", L"Washing Pump Open Failure or no air source", bErrorWashingPumpOpen);
	if (bErrorWashingPumpClose != devState.bErrorWashingPumpClose)
		MakeDiagData(L"Hardware", L"A0109", L"Washing Pump Close Failure", bErrorWashingPumpClose);
	if (bErrorCirculatingPump != devState.bErrorCirculatingPump)
		MakeDiagData(L"Hardware", L"A0110", L"Circulating Pump Failure", bErrorCirculatingPump);

	if (bErrorChoke != devState.bErrorChoke)
		MakeDiagData(L"Hardware", L"A0120", L"Choke Malfunction", bErrorChoke);
	if (bErrorPT1 != devState.bErrorPT1)
		MakeDiagData(L"Hardware", L"A0121", L"PT1 Malfunction", bErrorPT1);
	if (bErrorPT2 != devState.bErrorPT2)
		MakeDiagData(L"Hardware", L"A0122", L"PT2 Malfunction", bErrorPT2);
	if (bErrorPT3 != devState.bErrorPT3)
		MakeDiagData(L"Hardware", L"A0123", L"PT3 Malfunction", bErrorPT3);
	if (bErrorPT4 != devState.bErrorPT4)
		MakeDiagData(L"Hardware", L"A0124", L"PT4 Malfunction", bErrorPT4);
	if (bErrorMFM != devState.bErrorMFM)
		MakeDiagData(L"Hardware", L"A0125", L"MFM Malfunction", bErrorMFM);


	devState.bErrorSecurityValveOpen = bErrorSecurityValveOpen;			//隔离阀开故障
	devState.bErrorSecurityValveClose = bErrorSecurityValveClose;			//
	devState.bErrorPurgeValveOpen = bErrorPurgeValveOpen;				//吹扫阀开故障
	devState.bErrorPurgeValveClose = bErrorPurgeValveClose;				//
	devState.bErrorReturnValveMudOpen = bErrorReturnValveMudOpen;			//回油阀开故障
	devState.bErrorReturnValveMudClose = bErrorReturnValveMudClose;			//
	devState.bErrorReturnValveDetergentOpen = bErrorReturnValveDetergentOpen;	//清洗阀开故障
	devState.bErrorReturnValveDetergentClose = bErrorReturnValveDetergentClose;	//
	devState.bErrorWashingPumpOpen = bErrorWashingPumpOpen;				//清洗泵开故障
	devState.bErrorWashingPumpClose = bErrorWashingPumpClose;			//
	devState.bErrorCirculatingPump = bErrorCirculatingPump;			//循环泵故障
	devState.bErrorChoke = bErrorChoke;					//调节阀故障
	devState.bErrorPT1 = bErrorPT1;						//压力变送器1故障
	devState.bErrorPT2 = bErrorPT2;						//压力变送器2故障
	devState.bErrorPT3 = bErrorPT3;						//压力变送器3故障
	devState.bErrorPT4 = bErrorPT4;						//压力变送器4故障
	devState.bErrorMFM = bErrorMFM;			//质量流量计故障

	//附加app使用硬件报警状态
	devState.bAppHardware = bErrorSecurityValveOpen || bErrorSecurityValveClose || bErrorPurgeValveOpen || bErrorPurgeValveClose
		|| bErrorReturnValveMudOpen || bErrorReturnValveMudClose || bErrorReturnValveDetergentOpen
		|| bErrorReturnValveDetergentClose || bErrorWashingPumpOpen || bErrorWashingPumpClose
		|| bErrorCirculatingPump || bErrorChoke || bErrorPT1 || bErrorPT2 || bErrorPT3 || bErrorPT4 || bErrorMFM;
}

void ModbusDevice::CommunicatedStates(const CollectionData& realData)
{
	bool bErrorPLCConfiguration = std::bitset<16>(realData.addr212)[14];
	bool bErrorPLCIOCommunication = std::bitset<16>(realData.addr212)[15];

	bool bErrorDIDO = std::bitset<16>(realData.addr213)[0];
	bool bErrorAIAO = std::bitset<16>(realData.addr213)[1];
	bool bErrorMFMComm = std::bitset<16>(realData.addr213)[2];
	bool bErrorKBOComm = std::bitset<16>(realData.addr213)[3];
	bool bErrorMudLevelComm = std::bitset<16>(realData.addr213)[4];


	if (bErrorPLCConfiguration != devState.bErrorPLCConfiguration)
		MakeDiagData(L"Communication", L"A0130",
			realData.nPLCType == 2 ?
			L"IO Communication Failure" : L"PLC Configuration Failure",
			bErrorPLCConfiguration);
	if (bErrorPLCIOCommunication != devState.bErrorPLCIOCommunication)
		MakeDiagData(L"Communication", L"A0131",
			realData.nPLCType == 2 ?
			L"IO Self-diagnosis Failure" : L"PLC IO Communication Failure",
			bErrorPLCIOCommunication);
	if (bErrorDIDO != devState.bErrorDIDO)
		MakeDiagData(L"Communication", L"A0132",
			realData.nPLCType == 2 ?
			L"IO Mismatch Failure" : L"DIDO Failure",
			bErrorDIDO);
	if (bErrorAIAO != devState.bErrorAIAO)
		MakeDiagData(L"Communication", L"A0133", L"AIAO Failure", bErrorAIAO);
	if (bErrorMFMComm != devState.bErrorMFMComm)
		MakeDiagData(L"Communication", L"A0134", L"MFM Comm Failure", bErrorMFMComm);
	if (bErrorKBOComm != devState.bErrorKBOComm)
		MakeDiagData(L"Communication", L"A0135", L"KBO Comm Failure", bErrorKBOComm);
	if (bErrorMudLevelComm != devState.bErrorMudLevelComm)
		MakeDiagData(L"Communication", L"A0136", L"Mud Level Comm Failure", bErrorMudLevelComm);

	devState.bErrorPLCConfiguration = bErrorPLCConfiguration;
	devState.bErrorPLCIOCommunication = bErrorPLCIOCommunication;
	devState.bErrorDIDO = bErrorDIDO;
	devState.bErrorAIAO = bErrorAIAO;
	devState.bErrorMFMComm = bErrorMFMComm;
	devState.bErrorKBOComm = bErrorKBOComm;
	devState.bErrorMudLevelComm = bErrorMudLevelComm;
	devState.bAppNetwork = bErrorPLCConfiguration || bErrorPLCIOCommunication || bErrorDIDO || bErrorAIAO
		|| bErrorMFMComm || bErrorKBOComm || bErrorMudLevelComm;
}

void ModbusDevice::ProtectStates(const CollectionData& realData)
{
	bool bProtectHHPumpPressure = std::bitset<16>(realData.addr213)[12];
	bool bProtectHHDiffPressure = std::bitset<16>(realData.addr213)[13];
	bool bProtectHHTemperature = std::bitset<16>(realData.addr213)[14];
	bool bProtectLLPumpPressure = std::bitset<16>(realData.addr213)[15];

	bool bProtectCirculatingPumpOverload = std::bitset<16>(realData.addr214)[0];
	bool bProtectLLMudLevel = std::bitset<16>(realData.addr214)[1];


	if (bProtectHHPumpPressure != devState.bProtectHHPumpPressure)
		MakeDiagData(L"Process Fault", L"A0144", L"HH Pump Pressure Being to Stop Equipment", bProtectHHPumpPressure);
	if (bProtectHHDiffPressure != devState.bProtectHHDiffPressure)
		MakeDiagData(L"Process Fault", L"A0145", L"HH Differential Pressure Being to Stop Equipment", bProtectHHDiffPressure);
	if (bProtectHHTemperature != devState.bProtectHHTemperature)
		MakeDiagData(L"Process Fault", L"A0146", L"HH Temperature Being to Stop Equipment", bProtectHHTemperature);
	if (bProtectLLPumpPressure != devState.bProtectLLPumpPressure)
		MakeDiagData(L"Process Fault", L"A0147", L"LL Pump Pressure Being to Stop Equipment", bProtectLLPumpPressure);
	if (bProtectCirculatingPumpOverload != devState.bProtectCirculatingPumpOverload)
		MakeDiagData(L"Process Fault", L"A0148", L"Circulating Pump Overload Being to Stop Equipment", bProtectCirculatingPumpOverload);
	if (bProtectLLMudLevel != devState.bProtectLLMudLevel)
		MakeDiagData(L"Process Fault", L"A0149", L"LL Mud Level Being to Stop Equipment", bProtectLLMudLevel);


	devState.bProtectHHPumpPressure = bProtectHHPumpPressure;
	devState.bProtectHHDiffPressure = bProtectHHDiffPressure;
	devState.bProtectHHTemperature = bProtectHHTemperature;
	devState.bProtectLLPumpPressure = bProtectLLPumpPressure;
	devState.bProtectCirculatingPumpOverload = bProtectCirculatingPumpOverload;
	devState.bProtectLLMudLevel = bProtectLLMudLevel;
	devState.bAppProtected = bProtectHHPumpPressure || bProtectHHDiffPressure || bProtectHHTemperature || bProtectLLPumpPressure
		|| bProtectCirculatingPumpOverload || bProtectLLMudLevel;
}

void ModbusDevice::WarnStates(const CollectionData& realData)
{
	bool bWarnPumpHighPressure = std::bitset<16>(realData.addr214)[12];
	bool bWarnPumpLowPressure = std::bitset<16>(realData.addr214)[13];
	bool bWarnHighDiffPressure = std::bitset<16>(realData.addr214)[14];
	bool bWarnHighTemperature = std::bitset<16>(realData.addr214)[15];
	bool bWarnDetergentHighDensity = std::bitset<16>(realData.addr215)[0];
	bool bWarnLowMudLevel = std::bitset<16>(realData.addr215)[1];
	bool bWarnMudHighDensity = std::bitset<16>(realData.addr215)[2];
	bool bWarnMeasuring = std::bitset<16>(realData.addr215)[3];
	bool bWarnMudViscosityJump = std::bitset<16>(realData.addr215)[4];
	bool bWarnMudDensityJump = std::bitset<16>(realData.addr215)[5];
	bool bWarnCleanInvalid = std::bitset<16>(realData.addr215)[6];

	if (bWarnPumpHighPressure != devState.bWarnPumpHighPressure)
		MakeDiagData(L"Process Warning", L"W0100", L"Pump High Pressure Warning", bWarnPumpHighPressure);
	if (bWarnPumpLowPressure != devState.bWarnPumpLowPressure)
		MakeDiagData(L"Process Warning", L"W0101", L"Pump Low Pressure Warning", bWarnPumpLowPressure);
	if (bWarnHighDiffPressure != devState.bWarnHighDiffPressure)
		MakeDiagData(L"Process Warning", L"W0102", L"High Differential Pressure Warning", bWarnHighDiffPressure);
	if (bWarnHighTemperature != devState.bWarnHighTemperature)
		MakeDiagData(L"Process Warning", L"W0103", L"High Temperature Warning", bWarnHighTemperature);
	if (bWarnDetergentHighDensity != devState.bWarnDetergentHighDensity)
		MakeDiagData(L"Process Warning", L"W0104", L"Detergent High Density Warning", bWarnDetergentHighDensity);
	if (bWarnLowMudLevel != devState.bWarnLowMudLevel)
		MakeDiagData(L"Process Warning", L"W0105", L"Low Mud Level Warning", bWarnLowMudLevel);
	if (bWarnMudHighDensity != devState.bWarnMudHighDensity)
		MakeDiagData(L"Process Warning", L"W0106", L"Mud High Density Warning", bWarnMudHighDensity);
	if (bWarnMeasuring != devState.bWarnMeasuring)
		MakeDiagData(L"Process Warning", L"W0107", L"Measuring Failure", bWarnMeasuring);
	if (bWarnMudViscosityJump != devState.bWarnMudViscosityJump)
		MakeDiagData(L"Process Warning", L"W0108", L"Mud Viscosity Jump Warning", bWarnMudViscosityJump);
	if (bWarnMudDensityJump != devState.bWarnMudDensityJump)
		MakeDiagData(L"Process Warning", L"W0109", L"Mud Density Jump Warning", bWarnMudDensityJump);
	if (bWarnCleanInvalid != devState.bWarnCleanInvalid)
		MakeDiagData(L"Process Warning", L"W0110", L"One-click Cleaning Invalid Warning", bWarnCleanInvalid);


	devState.bWarnPumpHighPressure = bWarnPumpHighPressure;
	devState.bWarnPumpLowPressure = bWarnPumpLowPressure;
	devState.bWarnHighDiffPressure = bWarnHighDiffPressure;
	devState.bWarnHighTemperature = bWarnHighTemperature;
	devState.bWarnDetergentHighDensity = bWarnDetergentHighDensity;
	devState.bWarnLowMudLevel = bWarnLowMudLevel;
	devState.bWarnMudHighDensity = bWarnMudHighDensity;
	devState.bWarnMeasuring = bWarnMeasuring;
	devState.bWarnMudViscosityJump = bWarnMudViscosityJump;
	devState.bWarnMudDensityJump = bWarnMudDensityJump;
	devState.bWarnCleanInvalid = bWarnCleanInvalid;
	devState.bAppWarn = bWarnPumpHighPressure || bWarnPumpLowPressure || bWarnHighDiffPressure || bWarnHighTemperature
		|| bWarnDetergentHighDensity || bWarnLowMudLevel || bWarnMudHighDensity || bWarnMeasuring
		|| bWarnMudViscosityJump || bWarnMudDensityJump || bWarnCleanInvalid;
}


void ModbusDevice::MakeDiagData(const SStringT&& title, const SStringT&& code, const SStringT&& des, bool bInsert)
{
	DiagDataItem info;
	info.type_ = title;
	info.code_ = code;
	info.des_ = des;
	if (bInsert)
	{
		info.timeStart_ = S_CA2T(Utils::GetDateTime().c_str());
	}
	else
	{
		info.timeEnd_ = S_CA2T(Utils::GetDateTime().c_str());
	}
	MysqlClient::InsertWellDiags(std::move(info));
}



void ModbusDevice::RunCurrentInstruction()
{
	if (!connectReady) return;
	std::unique_lock<std::mutex> lock_local(m_mtxIns);
	if (m_lvCurIns.size() > 0)
	{//查询当前指令 plc写入寄存器地址及值
		auto curIter = m_lvCurIns.begin();
		while (curIter != m_lvCurIns.end())
		{
			auto ins = *curIter;
			auto insIter = m_mapInsInfo.find(ins);
			if (insIter != m_mapInsInfo.end())
			{
				if (OnRigsterWrite(insIter->second.addr, insIter->second.sigVal))
				{
					LOGINFO("write ins={}", static_cast<int>(ins));
					curIter = m_lvCurIns.erase(curIter);
					continue;
				}
			}
			++curIter;
		}

	}
	lock_local.unlock();
	OnRigstersWrite();
}

bool ModbusDevice::OnRigsterWrite(AddrType addr, uint16_t buf)
{
	if (buf == 0 || addr == addr40003)
	{
		if (modbus_write_register(m_pmbs, addr, buf) == -1) return false;
	}
	else
	{
		switch (addr)
		{
		case addr40001:
		{
			buf |= m_val40001;
			if (modbus_write_register(m_pmbs, addr, buf) == -1) return false;
			buf &= std::bitset<16>().set(4, 1).to_ulong();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			if (modbus_write_register(m_pmbs, addr, buf) == -1) return false;
		}
		break;
		case addr40002:
		{
			if (modbus_write_register(m_pmbs, addr, buf) == -1) return false;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			if (modbus_write_register(m_pmbs, addr, 0) == -1) return false;
		}
		break;
		default:
			break;
		}
	}
	return true;
}


bool ModbusDevice::OnRigstersWrite()
{
	auto jsConfig = App().GetItemConfig("Parameter");
	modbus_set_float(std::stof(jsConfig["ev_set"].asString()), &m_vFrom40009to40026[0]);
	modbus_set_float(curRecordData_.r600, &m_vFrom40009to40026[2]);
	modbus_set_float(std::stof(jsConfig["fr"].asString()), &m_vFrom40009to40026[4]);
	modbus_set_float(std::stof(jsConfig["ev0_flow"].asString()), &m_vFrom40009to40026[6]);
	modbus_set_float(std::stof(jsConfig["evn_flow"].asString()), &m_vFrom40009to40026[8]);
	modbus_set_float(std::stof(jsConfig["evn_press"].asString()), &m_vFrom40009to40026[10]);
	modbus_set_float(std::stof(jsConfig["lt"].asString()), &m_vFrom40009to40026[12]);
	modbus_set_float(std::stof(jsConfig["set_ev_v0"].asString()), &m_vFrom40009to40026[14]);
	//modbus_set_float(std::stof(mapFlowmeterType_[jsConfig["flowmeter_choose"].asString()]), &m_vFrom40009to40026[16]);
	m_vFrom40009to40026[16] = std::stoi(mapFlowmeterType_[jsConfig["flowmeter_choose"].asString()]);
	if (modbus_write_registers(m_pmbs, addr40009, 18, m_vFrom40009to40026) == -1) return false;
	return true;
}

