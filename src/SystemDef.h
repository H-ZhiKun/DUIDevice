#pragma once
#include <vector>
#include <list>
#include <string>
#include <map>
#include <atomic>
#include <json.h>
#include "Logger.h"
#define STRING_RUNTIME  "Runtime"
#define STRING_USERNAME "userName"
#define STRING_WELLNAME "wellName"
#define STRING_ISEXIT	"isExit"
#define STRING_ISTEST	"isTest"
#define STRING_SAVEFILE	"saveFilePath"
#define STRING_LANGUAGE	"language"
#define STRING_UNIT		"unit"

struct RealF
{
	double x = 0;
	double y = 0;
	RealF(double ix, double iy) :x(ix), y(iy) {}
};

enum PageSwitch
{
	MAIN_PAGE = 0,
	SYSTEM_PAGE,
	FLUID_PAGE
};

enum WndMsgType
{
	WM_BEGIN = WM_USER + 101,
	WM_TIMER_UPDATE,
	WM_TIMER_CLEAN,
	WM_NETWORK_UPDATE,
	WM_WELLNAME_CHANGE,
	WM_WNDSTATE_UPDATE,
	WM_WNDREAL_UPDATE,
	WM_WNDCALC_UPDATE,
	WM_WNDUNIT_UPDATE,
	WM_FLUID_UPDATE,
	WM_PUMPSTATE_UPDATE,
	WM_MSG_INFO,
	WM_END
};
enum WndStateType
{
	WND_STATE_DEF,
	WND_STATE_INIT,
	WND_STATE_IDLE,
	WND_STATE_TEST,
	WND_STATE_CLEAN,
	WND_STATE_WARN,
	WND_STATE_POP
};

enum DeviceIns
{
	default_type = 0,
	save_complete,		//数据保存完成
	test_start,			//开始自动测试
	test_pause,			//停止自动测试
	auto_clean,			//一键清洗
	soft_stop,			//急停
	soft_restore,		//急停恢复
	pump_reset,			//脱扣故障复位
	calc_error,			//计算错误
	calc_complete,		//工艺计算完成
	circulating_pump_1,			//循环泵开
	circulating_pump_0,			//循环泵关
	security_valve_1,			//隔离阀开
	security_valve_0,			//隔离阀关
	purge_valve_1,				//吹扫阀开
	purge_valve_0,				//吹扫阀关
	washing_pump_1,				//清洗泵开
	washing_pump_0,				//清洗泵关
	return_valve_mud_1,			//回油阀开
	return_valve_mud_0,			//回油阀关
	return_valve_detergent_1,   //清洗阀开
	return_valve_detergent_0,   //清洗阀关
	debug_mode_1,					//手动模式
	debug_mode_0					//自动模式
};


enum TransportType
{
	default = 0
};
struct TransportMsg
{
	TransportType EVENT_ID = default;
	std::string  info_;
};

struct SystemOptionsInfo
{
	SStringT wellName_;
	SStringT date_;
	SStringT location_;
	SStringT field_;
	SStringT operator_;
	SStringT rig_;

	SStringT lan_;
	SStringT units_;
	SStringT savePath_;
	SystemOptionsInfo& operator =(const SystemOptionsInfo& obj)
	{
		wellName_ = obj.wellName_;
		date_ = obj.date_;
		location_ = obj.location_;
		field_ = obj.field_;
		operator_ = obj.operator_;
		rig_ = obj.rig_;
		lan_ = obj.lan_;
		units_ = obj.units_;
		savePath_ = obj.savePath_;
		return *this;
	}
	bool operator ==(const SystemOptionsInfo& obj)
	{
		return (wellName_ == obj.wellName_)
			&& (date_ == obj.date_)
			&& (location_ == obj.location_)
			&& (field_ == obj.field_)
			&& (operator_ == obj.operator_)
			&& (rig_ == obj.rig_);
	}
};

struct FluidInfo
{
	SStringT	name_;
	SStringT	type_;
	float		density_ = 0;
	float		temp_ = 0;
	SStringT	source_;
	SStringT	date_;
	FluidInfo& operator =(const FluidInfo& obj)
	{
		name_ = obj.name_;
		type_ = obj.type_;
		density_ = obj.density_;
		temp_ = obj.temp_;
		source_ = obj.source_;
		date_ = obj.date_;
		return *this;
	}
	bool operator ==(const FluidInfo& obj)
	{
		return (name_ == obj.name_ &&
			type_ == obj.type_ &&
			density_ == obj.density_ &&
			temp_ == obj.temp_ &&
			source_ == obj.source_ &&
			date_ == obj.date_);
	}
};

struct CollectionData
{
	uint16_t addr001 = 0;
	uint16_t addr201 = 0;
	uint16_t addr202 = 0;
	uint16_t addr211 = 0;
	uint16_t addr212 = 0;
	uint16_t addr213 = 0;
	uint16_t addr214 = 0;
	uint16_t addr215 = 0;
	uint16_t addr216 = 0;
	uint16_t addr221 = 0;

	uint16_t nSampleIndex = 0;
	float fPumpPressure = 0.0f;             //管道压力
	float fUpstreamPressure = 0.0f;		 //测管前压力
	float fDownstreamPressure = 0.0f;		 //测管后压力
	float fOutPressure = 0.0f;		   //测管出口压力
	float fOpening = 0.0f;					 //阀门开度
	float fMassFlow = 0.0f;				 //测管质量流量
	float fVolFlow = 0.0f;				 //测管体积流量
	float fTemperature = 0.0f;				 //测管温度
	float fDensity = 0.0f;					 //介质密度
	float fDiffPressure = 0.0f;			 //测管差压
	float fDiffPressureStr = 0.0f;			 //测管差压str
	float fEV0 = 0.0f;						//初阀位
	float fEVn = 0.0f;                    //终阀位
	float fMudLevel = 0.0f;                 //液位
	uint16_t nPLCType = 0;			//plc 类型
	float fPH = 0.0f;                 //PH值
	float fCl = 0.0f;                 //氯离子含量
	float fES = 0;
	float fPHManual = 0.0f;                 //PH值
	float fClManual = 0.0f;                 //氯离子含量
	float fESManual = 0;
	CollectionData& operator=(const CollectionData& data)
	{
		addr001 = data.addr001;
		addr201 = data.addr201;
		addr202 = data.addr202;
		addr211 = data.addr211;
		addr212 = data.addr212;
		addr213 = data.addr213;
		addr214 = data.addr214;
		addr215 = data.addr215;
		addr216 = data.addr216;
		addr221 = data.addr221;
		nSampleIndex = data.nSampleIndex;
		fPumpPressure = data.fPumpPressure;
		fUpstreamPressure = data.fUpstreamPressure;
		fDownstreamPressure = data.fDownstreamPressure;
		fOutPressure = data.fOutPressure;
		fOpening = data.fOpening;
		fMassFlow = data.fMassFlow;
		fVolFlow = data.fVolFlow;
		fTemperature = data.fTemperature;
		fDensity = data.fDensity;
		fDiffPressure = data.fDiffPressure;
		fDiffPressureStr = data.fDiffPressureStr;
		fEV0 = data.fEV0;
		fEVn = data.fEVn;
		fMudLevel = data.fMudLevel;
		nPLCType = data.nPLCType;
		fPH = data.fPH;
		fCl = data.fCl;
		fES = data.fES;
		fPHManual = data.fPHManual;
		fClManual = data.fClManual;
		fESManual = data.fESManual;
		return *this;
	}
};
struct OpLogsData
{
	std::wstring time;
	std::wstring user;
	std::wstring event;
};


struct RecordData
{
	std::string model;
	std::string time;
	float nline = 0;
	float nbh = 0;
	float kbh = 0;
	float max_temp = 0.0f;
	float min_temp = 1000.0f;
	float avg_temp = 0.0f;
	float max_pressure = 0.0f;
	float min_pressure = 10000.0f;
	float avg_pressure = 0.0f;
	float max_density = 0.0f;
	float min_density = 10000.0f;
	float avg_density = 0.0f;
	float r600 = 0.0f;
	float r300 = 0.0f;
	float r200 = 0.0f;
	float r100 = 0.0f;
	float r60 = 0.0f;
	float r30 = 0.0f;
	float r20 = 0.0f;
	float r10 = 0.0f;
	float r6 = 0.0f;
	float r3 = 0.0f;
	float r0 = 0.0f;
	float av = 0.0f;
	float pv = 0.0f;
	float yp = 0.0f;
	float npl = 0.0f;
	float kpl = 0.0f;
	float nhb = 0.0f;
	float khb = 0.0f;
	float tau0 = 0.0f;
	double R2 = -100;
	RecordData& operator=(const RecordData& data)
	{
		model = data.model;
		time = data.time;
		max_temp = data.max_temp;
		min_temp = data.min_temp;
		avg_temp = data.avg_temp;
		max_pressure = data.max_pressure;
		min_pressure = data.min_pressure;
		avg_pressure = data.avg_pressure;
		max_density = data.max_density;
		min_density = data.min_density;
		avg_density = data.avg_density;
		r600 = data.r600;
		r300 = data.r300;
		r200 = data.r200;
		r100 = data.r100;
		r60 = data.r60;
		r30 = data.r30;
		r20 = data.r20;
		r10 = data.r10;
		r6 = data.r6;
		r3 = data.r3;
		r0 = data.r0;
		av = data.av;
		pv = data.pv;
		yp = data.yp;
		npl = data.npl;
		kpl = data.kpl;
		nhb = data.nhb;
		khb = data.khb;
		tau0 = data.tau0;
		nline = data.nline;
		nbh = data.nbh;
		kbh = data.kbh;
		R2 = data.R2;
		return *this;
	}
};

struct SensorsRecord
{
	std::string typeName;
	std::string time;
	float value;
};

struct MachineState
{

	//设备状态
	std::atomic_bool bCirculatingPumpOperation{ false };		//循环泵运行

	std::atomic_bool bSecurityValveOpen			{ false };			//隔离阀开反馈
	std::atomic_bool bSecurityValveClose		{ false };			//
	std::atomic_bool bPurgeValveOpen			{ false };				//吹扫阀开反馈
	std::atomic_bool bPurgeValveClose			{ false };				//
	std::atomic_bool bReturnValveMudOpen		{ false };			//回油阀开反馈
	std::atomic_bool bReturnValveMudClose		{ false };			//
	std::atomic_bool bReturnValveDetergentOpen	{ false };		//清洗阀开反馈
	std::atomic_bool bReturnValveDetergentClose { false };	//

	std::atomic_bool bCirculatingPumpTrip	{ false };		//循环泵脱扣
	std::atomic_bool bEStop					{ false };						//急停
	std::atomic_bool bWashingPumpOperation	{ false };			//清洗泵运行


	std::atomic_bool bSignManual	{ false };					//手动标志
	std::atomic_bool bSignAuto		{ false };						//自动标志

	//应用附加状态
	std::atomic_bool bAppNeedClean{ false };						//需要清洗
	std::atomic_bool bAppHardware{ false };							//硬件故障
	std::atomic_bool bAppNetwork{ false };							//网络错误
	std::atomic_bool bAppProtected{ false };						//保护警告
	std::atomic_bool bAppWarn{ false };								//警告报警
	//测试过程状态
	std::atomic_bool bSignTesting			{ false };					//自动测试标志
	std::atomic_bool bSignTestEnd			{ false };					//自动测试结束
	std::atomic_bool bSignCleaning			{ false };					//一键清洗标志
	std::atomic_bool bSignCleanEnd			{ false };					//清洗结束标志
	std::atomic_bool bSignDataCollection	{ false };			//数据保存标志
	std::atomic_bool bSignCollectionCycleing { false };		//周期测试标志
	std::atomic_bool bSignCollectionCycleEnd { false };		//周期测试结束标志
	//报警状态
	////1：硬件故障
	std::atomic_bool bErrorSecurityValveOpen	{ false };			//隔离阀开故障
	std::atomic_bool bErrorSecurityValveClose	{ false };			//
	std::atomic_bool bErrorPurgeValveOpen		{ false };				//吹扫阀开故障
	std::atomic_bool bErrorPurgeValveClose		{ false };				//
	std::atomic_bool bErrorReturnValveMudOpen	{ false };			//回油阀开故障
	std::atomic_bool bErrorReturnValveMudClose	{ false };			//
	std::atomic_bool bErrorReturnValveDetergentOpen		{ false };	//清洗阀开故障
	std::atomic_bool bErrorReturnValveDetergentClose	{ false };	//
	std::atomic_bool bErrorWashingPumpOpen	{ false };				//清洗泵开故障
	std::atomic_bool bErrorWashingPumpClose	{ false };			//
	std::atomic_bool bErrorCirculatingPump	{ false };			//循环泵故障
	std::atomic_bool bErrorChoke{ false };					//调节阀故障
	std::atomic_bool bErrorPT1 { false };						//压力变送器1故障
	std::atomic_bool bErrorPT2 { false };						//压力变送器2故障
	std::atomic_bool bErrorPT3 { false };						//压力变送器3故障
	std::atomic_bool bErrorPT4 { false };						//压力变送器4故障
	std::atomic_bool bErrorMFM { false };						//质量流量计故障

	////2: 通讯故障
	std::atomic_bool bErrorPLCConfiguration		{ false };		//PLC硬件组态错误
	std::atomic_bool bErrorPLCIOCommunication	{ false };		//PLC IO访问错误
	std::atomic_bool bErrorDIDO					{ false };					//8DI/8DO模块出错
	std::atomic_bool bErrorAIAO					{ false };					//4AI/2AO模块出错
	std::atomic_bool bErrorMFMComm				{ false };					//质量流量计通讯错误
	std::atomic_bool bErrorKBOComm				{ false };					//KBO通讯错误
	std::atomic_bool bErrorMudLevelComm			{ false };			//液位计通讯故障
	////3: 工艺保护
	std::atomic_bool bProtectHHPumpPressure				{ false };		//管道压力超高即将保护停机
	std::atomic_bool bProtectHHDiffPressure				{ false };		//压差超高即将保护停机
	std::atomic_bool bProtectHHTemperature				{ false };			//介质温度超高即将保护停机
	std::atomic_bool bProtectLLPumpPressure				{ false };		//管道压力超低即将保护停机
	std::atomic_bool bProtectCirculatingPumpOverload	{ false };		//循环泵过载保护停机
	std::atomic_bool bProtectLLMudLevel					{ false };			//液位超低即将保护停机
	////4: 工艺警告
	std::atomic_bool bWarnPumpHighPressure		{ false };		//管道压力高警示
	std::atomic_bool bWarnPumpLowPressure		{ false };		//管道压力低警示
	std::atomic_bool bWarnHighDiffPressure		{ false };		//管道压差高警示
	std::atomic_bool bWarnHighTemperature		{ false };		//介质温度高警示
	std::atomic_bool bWarnDetergentHighDensity	{ false };		//清洗液密度高报警
	std::atomic_bool bWarnLowMudLevel			{ false };				//液位低报警
	std::atomic_bool bWarnMudHighDensity		{ false };			//泥浆浓度太大
	std::atomic_bool bWarnMeasuring				{ false };			//工艺计算错误
	std::atomic_bool bWarnMudViscosityJump		{ false };			//泥浆粘度突变
	std::atomic_bool bWarnMudDensityJump		{ false };			//泥浆密度突变
	std::atomic_bool bWarnCleanInvalid			{ false };			//自动清洗失效
};

struct SampleData
{
	std::string sampleTime;
	float pumpPressure = 0;
	float fOpening = 0;
	float fVolFlow = 0;
	float fMudLevel = 0;
	float fUpstreamPressure = 0;
	float fDownstreamPressure = 0;
	float diffPressure = 0;
	float massFlow = 0;
	float temperature = 0;
	float density = 0;
	SampleData() :sampleTime(""),
		pumpPressure(0.0f),
		fOpening(0.0f),
		fVolFlow(0.0f),
		fMudLevel(0.0f),
		fUpstreamPressure(0.0f),
		fDownstreamPressure(0.0f),
		diffPressure(0.0f),
		massFlow(0.0f),
		temperature(0.0f),
		density(0.0f) {}

	SampleData(std::string isampleTime,
		float ipumpPressure,
		float ifOpening,
		float ifVolFlow,
		float ifMudLevel,
		float ifUpstreamPressure,
		float ifDownstreamPressure,
		float idiffPressure,
		float imassFlow,
		float itemperature,
		float idensity) :sampleTime(isampleTime),
		pumpPressure(ipumpPressure),
		fOpening(ifOpening),
		fVolFlow(ifVolFlow),
		fMudLevel(ifMudLevel),
		fUpstreamPressure(ifUpstreamPressure),
		fDownstreamPressure(ifDownstreamPressure),
		diffPressure(idiffPressure),
		massFlow(imassFlow),
		temperature(itemperature),
		density(idensity) {}

	SampleData& operator=(const SampleData& data)
	{
		sampleTime = data.sampleTime;
		pumpPressure = data.pumpPressure;
		fVolFlow = data.fVolFlow;
		fUpstreamPressure = data.fUpstreamPressure;
		fDownstreamPressure = data.fDownstreamPressure;
		diffPressure = data.diffPressure;
		massFlow = data.massFlow;
		temperature = data.temperature;
		density = data.density;
		return *this;
	}
};

struct DiagDataItem
{
	DiagDataItem() {}
	DiagDataItem(SStringT timeStart, SStringT timeEnd, SStringT code, SStringT des)
		:timeStart_(timeStart), timeEnd_(timeEnd), code_(code), des_(des) {}
	SStringT timeStart_;
	SStringT timeEnd_;
	SStringT type_;
	SStringT code_;
	SStringT des_;
};


struct ABValue
{
	float a = 0.0f;
	float b = 0.0f;
};
struct ABInfo
{
	std::list<ABValue>  listAB;
	float fDensityRange0 = 0.0f;
	float fDensityRange1 = 0.0f;
	float fR600fann35 = 0.0f;
	float fR300fann35 = 0.0f;
};

struct AnalysisConfig
{
	std::string model_;					//计算模式字符串
	std::vector<ABInfo> lvABInfo_;  	//AB配置参数列表
	float fLength = 0;					//弯管长度
	float fdtc = 0;						//弯管内径
	float fdc = 0;						//弯管螺旋直径
	AnalysisConfig& operator=(const AnalysisConfig& obj)
	{
		model_ = obj.model_;
		lvABInfo_ = obj.lvABInfo_;
		fLength = obj.fLength;
		fdtc = obj.fdtc;
		fdc = obj.fdc;
	}
};

struct AdvanceData
{
	std::map<SStringT, std::list<float>> mapValue;
	std::list <std::string>	vTime;
	void RemoveOne()
	{
		for (auto& it : mapValue)
		{
			it.second.pop_front();
		}
		vTime.pop_front();
	}
};

struct CommentsData
{
	std::string time;
	std::string data;
};