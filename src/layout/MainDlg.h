// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <map>
#include <string>
#include "SystemDef.h"
#include "SCheckBoxEx.h"
#include "DiagDlg.h"
#include "FluidInfoDlg.h"
#include "SystemOptionsDlg.h"
#include "RestoreDlg.h"
#include "MessageDlg.h"
#include "ModbusDevice.h"
#include "SChartReal.h"

enum  MainValueType
{
	main_speed = 0,	 //转速组
	main_real = 1,   //温度密度实时数据更新
	main_model,		 //模型选择
	main_chart_time, //时间曲线
	main_chart_line,  //计算曲线
	main_datasource
};


class CMainDlg : public SHostWnd
{
public:
	struct WndStateInfo
	{
		SStringT color_;
		SStringT title_;
		WndStateInfo(SStringT title, SStringT color)
		{
			title_ = title;
			color_ = color;
		}
	};
	CMainDlg();
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	bool OnWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void updateMainValue(std::list<MainValueType> lvType);
	void ChangeWndState(WndStateType state);

	void OnDlgDiag();
	void OnDlgFluidSetup();
	void OnDlgSystemOptions();
	void OnLogout();

	void OnLogin();

	void OnSystemMenu();

	void PageLoginOrMain(bool bPageLogin);
	void OnPageHome(bool bShow);
	void OnPageLogs(bool bShow);
	void OnPageAdvance(bool bShow);
	void OnPageDatalogs(bool bShow);
	void ResetDatalogs(std::vector<SStringT> checkBtn = {});
	void AddRecordData(const RecordData& data);
	void ChartLogsChecked();
	void GetDatalogsPage();
	bool DataLogsChecked(EventArgs* pEvt);
	bool DataLogsPageChange(EventArgs* pEvt);
	bool DataLogsBtnSearch(EventArgs* pEvt);
	bool DataLogsTabLeft(EventArgs* pEvt);
	bool DataLogsTabRight(EventArgs* pEvt);
	void DataLogsTabUpdate();
	void ResetSensorsData(const SStringT& sensorName);
	void GetSensorsPage(const SStringT& sensorName);
	bool SensorsPageBtn(EventArgs* pEvt);


	void OnPageOplogs(bool bShow);
	void GetPageOplogs();
	bool OpLogsPageChange(EventArgs* pEvt);

	void OnPageAdvanceDebug(bool bShow);
	void OnPageAdvanceInit(bool bShow);


	//实时数据更新入口
	void GetCurrentPlcState();

protected:
	void AdvanceBtnChecked();
	bool CheckBtnChange(EventArgs* pEvt);
	bool CheckedLine(EventArgs* pEvt);
	bool CheckedAdvanceReal(EventArgs* pEvt);
	bool ComboViewChange(EventArgs* pEvt);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void KeyEditUpdate(std::string title, std::string item);
	void RealBlockValSet(const std::list<SStringT>& lvTarget);
	bool BtnUnits(EventArgs* pEvt);
	bool AdvanceBtn(EventArgs* pEvt);
	void CheckLog();
	void BtnStart();
	void BtnClean();
	void BtnStop();
	void OnSnapshot();
	void DatalogExport();

	void AdvanceRealData();
	void AdvanceRealText();
	void AdvanceBtnSearch();

	void CleanDateUpdate();
	void EnableClean();

	void CommentsEsc();
	void CommentsUpdate(std::string title, std::string item);
	void CommentsCallback(bool bEdit, std::string data, std::string time);
	void CommentsDBClick();

	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_fluid_setup", OnDlgFluidSetup)
		EVENT_NAME_COMMAND(L"btn_diag", OnDlgDiag)
		EVENT_NAME_COMMAND(L"btn_system_options", OnDlgSystemOptions)
		EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"btn_logout", OnLogout)
		EVENT_NAME_COMMAND(L"login_btn_close", OnClose)
		EVENT_NAME_COMMAND(L"login_btn_login", OnLogin)
		EVENT_NAME_COMMAND(L"system_setup", OnSystemMenu)
		EVENT_NAME_COMMAND(L"btn_start", BtnStart)
		EVENT_NAME_COMMAND(L"btn_clean", BtnClean)
		EVENT_NAME_COMMAND(L"btn_stop", BtnStop)
		EVENT_NAME_COMMAND(L"btn_snapshot", OnSnapshot)
		EVENT_NAME_COMMAND(L"datalog_export", DatalogExport)

		EVENT_NAME_COMMAND(L"enable_cyclic_clean", EnableClean)

		EVENT_NAME_COMMAND(L"advance_log", CheckLog)
		//advance btn group
		EVENT_NAME_COMMAND(L"advance_search", AdvanceBtnSearch)

		EVENT_MAP_END()

		//HostWnd真实窗口消息处理
		BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MESSAGE_RANGE_HANDLER_EX(WM_BEGIN, WM_END, OnWndMsg)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
		END_MSG_MAP()
private:
	BOOL			m_bLayoutInited;
	using MapCheckBtnTask = std::map < std::wstring, std::function<void(bool)>>;
	std::map < std::wstring, MapCheckBtnTask> mapCheckBtn_;
	DiagDlg* pDiagDlg_;
	FluidInfoDlg* pFluidInfoDlg_;
	SystemOptionsDlg* pSystemOptionsDlg_;
	RestoreDlg* pRestoreDlg_;
	std::map<WndStateType, WndStateInfo> mapWndStateInfo_;
	std::map<SStringT, DeviceIns> mapAdvanceBtnCmd_{ {L"advance_debug",DeviceIns::debug_mode_0},
												 {L"advance_circulating_pump",DeviceIns::circulating_pump_0},
												 {L"advance_return_valve_mud",DeviceIns::return_valve_mud_0},
												 {L"advance_purge_valve",DeviceIns::purge_valve_0},
												 {L"advance_security_valve",DeviceIns::security_valve_0},
												 {L"advance_washing_pump",DeviceIns::washing_pump_0},
												 {L"advance_return_valve_detergent",DeviceIns::return_valve_detergent_0} };
	WndStateType curWndState = WND_STATE_IDLE;
	SStringT curRealModel = L"";
	std::list<SStringT> vUnitPV{ L"mPa·s",L"cp" };
	std::list<SStringT> vUnitYP{ L"Pa",L"lb/100ft²" };
	std::list<SStringT> vUnitK{ L"Pa·sⁿ",L"lb·sⁿ/100ft²" };
	std::list<SStringT> vUnitTemp{ L"°C",L"°F",L"K" };
	std::list<SStringT> vUnitDensity{ L"kg/m³",L"g/cm³",L"sg",L"ppg" };
	std::list<SStringT> vUnitES{ L"V" };
	std::list<SStringT> vUnitCl{ L"g/L" };
	std::list<SStringT> vDatalogCheck_{ L"datalog_AvgTemp", L"datalog_AvgPress",
									L"datalog_R600",L"datalog_R300", L"datalog_R200", L"datalog_R100",
									L"datalog_R60",L"datalog_R30", L"datalog_R20", L"datalog_R10",
									L"datalog_R6",L"datalog_R3", L"datalog_R0",
									L"datalog_MaxDensity",L"datalog_AvgDensity", L"datalog_MinDensity",
									L"datalog_AV",L"datalog_PV",L"datalog_YP", L"datalog_nPL", L"datalog_KPL",
									L"datalog_nHB", L"datalog_KHB", L"datalog_Tau0",
									L"datalog_PH",L"datalog_Cl-",L"datalog_ES" };

	std::list<SStringT> vDatalogBeChecked;

	std::vector<COLORREF> vDatalogLineColorRGB{ RGBA(255,219,77,255),RGBA(129,192,121,255),RGBA(128,189,234,255),
											RGBA(160,125,183,255),RGBA(106,106,212,255),RGBA(140,58,75,255) ,RGBA(244,150,86,255) };
	std::vector<SStringT> vDatalogLineColorStr{ L"#FFDB4D",L"#81C079",L"#80BDEA",
												L"#A07DB7",L"#6A6AD4",L"#8C3A4B",L"#F49656" };
	size_t nDatalogsPage_ = 1;
	size_t nDatalogsTotalPage_ = 1;
	size_t nOplogsPage_ = 1;
	size_t nOplogsTotalPage_ = 1;
	uint8_t nDatalogsTab_ = 1;

	uint8_t nLayered = 0;
};
