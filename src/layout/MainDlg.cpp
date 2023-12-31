// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
#include "Utils.h"
#include "SKeyEdit.h"
#include "SWinEdit.h"
#include "MysqlClient.h"
#include "AppFramework.h"
#include "SComboViewAdapter.hpp"
#include <bitset>
#include "SChartLine.h"
#include "SChartTime.h"
#include "SMutiImage.h"
#include "SChartLogs.h"
#include "OpLogsAdapter.hpp"
#include "DatalogsAdapter.hpp"
#include "SensorsAdapter.hpp"
#include <xlnt/xlnt.hpp>
#include "Logger.h"

using namespace realology;

CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:xml_main"))
{
	m_bLayoutInited = FALSE;
	mapWndStateInfo_ = {
		{WND_STATE_INIT, WndStateInfo(L"INIT", L"#A1A1A1")},
		{WND_STATE_IDLE, WndStateInfo(L"IDLE", L"#A1A1A1")},
		{WND_STATE_TEST, WndStateInfo(L"IN TEST", L"#222277")},
		{WND_STATE_CLEAN, WndStateInfo(L"CLEANING", L"#86AF17")},
		{WND_STATE_WARN, WndStateInfo(L"WARNING", L"#C73838")},
	};
}

CMainDlg::~CMainDlg()
{
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;

	{//子窗口初始化
		pDiagDlg_ = new DiagDlg;
		pDiagDlg_->Create(m_hWnd);
		pDiagDlg_->SendMessage(WM_INITDIALOG);
		pDiagDlg_->CenterWindow(m_hWnd);
		pDiagDlg_->SetWindowPos(HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

		pFluidInfoDlg_ = new FluidInfoDlg;
		pFluidInfoDlg_->Create(m_hWnd);
		pFluidInfoDlg_->SendMessage(WM_INITDIALOG);
		pFluidInfoDlg_->CenterWindow(m_hWnd);
		pFluidInfoDlg_->SetWindowPos(HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

		pSystemOptionsDlg_ = new SystemOptionsDlg;
		pSystemOptionsDlg_->Create(m_hWnd);
		pSystemOptionsDlg_->SendMessage(WM_INITDIALOG);
		pSystemOptionsDlg_->CenterWindow(m_hWnd);
		pSystemOptionsDlg_->SetWindowPos(HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

		pRestoreDlg_ = new RestoreDlg;
		pRestoreDlg_->Create(m_hWnd);
		pRestoreDlg_->SendMessage(WM_INITDIALOG);
		pRestoreDlg_->CenterWindow(m_hWnd);
		pRestoreDlg_->SetWindowPos(HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	}
	{
		MapCheckBtnTask systemTask = {
			{ R.name.system_chk_home,std::bind(&CMainDlg::OnPageHome,this,std::placeholders::_1)},
			{ R.name.system_chk_logs,std::bind(&CMainDlg::OnPageLogs,this,std::placeholders::_1)},
			{ R.name.system_chk_adv,std::bind(&CMainDlg::OnPageAdvance,this,std::placeholders::_1)} };

		FindChildByName(R.name.system_chk_home)->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::CheckBtnChange, this));
		FindChildByName(R.name.system_chk_logs)->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::CheckBtnChange, this));
		FindChildByName(R.name.system_chk_adv)->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::CheckBtnChange, this));

		MapCheckBtnTask logsTask = {
			{ R.name.logs_chk_datalogs,std::bind(&CMainDlg::OnPageDatalogs,this,std::placeholders::_1)},
			{ R.name.logs_chk_oplogs,std::bind(&CMainDlg::OnPageOplogs,this,std::placeholders::_1)} };
		FindChildByName(R.name.logs_chk_datalogs)->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::CheckBtnChange, this));
		FindChildByName(R.name.logs_chk_oplogs)->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::CheckBtnChange, this));

		MapCheckBtnTask advanceTask = {
		{ R.name.advance_chk_debug,std::bind(&CMainDlg::OnPageAdvanceDebug,this,std::placeholders::_1)},
		{ R.name.advance_chk_init,std::bind(&CMainDlg::OnPageAdvanceInit,this,std::placeholders::_1)} };
		FindChildByName(R.name.advance_chk_debug)->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::CheckBtnChange, this));
		FindChildByName(R.name.advance_chk_init)->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::CheckBtnChange, this));

		mapCheckBtn_[L"system"] = std::move(systemTask);
		mapCheckBtn_[L"logs"] = std::move(logsTask);
		mapCheckBtn_[L"advance"] = std::move(advanceTask);
	}


	std::map<std::string, Json::Value> configMap = App().GetItemConfig();



	for (const auto& titleValue : configMap)
	{
		if (titleValue.first == "Other")
		{
			std::string clean_start_time = titleValue.second["clean_start_time"].asString();
			std::string clean_enable = titleValue.second["enable_cyclic_clean"].asString();
			std::string interval = titleValue.second["clean_interval"].asString();
			std::string times = titleValue.second["clean_times"].asString();

			auto pDateClean = FindChildByName2<SDateTimePickerEx>("clean_start_time");
			auto pCleanInterval = FindChildByName2<SKeyEdit>("clean_interval");
			auto pCleanTimes = FindChildByName2<SKeyEdit>("clean_times");

			pDateClean->SetUpdater(std::bind(&CMainDlg::CleanDateUpdate, this));
			pCleanInterval->SetCallBack(std::bind(&CMainDlg::KeyEditUpdate, this,
				std::placeholders::_1, std::placeholders::_2),
				"Other", "clean_interval");
			pCleanTimes->SetCallBack(std::bind(&CMainDlg::KeyEditUpdate, this,
				std::placeholders::_1, std::placeholders::_2),
				"Other", "clean_times");

			if (interval.empty())
				interval = "1";
			if (times.empty())
				times = "1";

			if (!clean_start_time.empty())
				pDateClean->SetWindowTextW(S_CA2W(clean_start_time.c_str()));
			if (pCleanInterval->GetWindowTextW().IsEmpty())
				pCleanInterval->SetWindowTextW(L"1");

			pCleanInterval->SetWindowTextW(S_CA2T(interval.c_str()));

			pCleanTimes->SetWindowTextW(S_CA2T(times.c_str()));
			FindChildByName2<SCheckBoxEx>("enable_cyclic_clean")
				->SetCheck(clean_enable == "1" ? TRUE : FALSE);
			CleanDateUpdate();
		}
		else
		{
			auto memberNames = titleValue.second.getMemberNames();
			for (auto iter = memberNames.begin(); iter != memberNames.end(); iter++)
			{
				auto obj = FindChildByName2<SKeyEdit>((*iter).c_str());
				if (obj != nullptr)
				{
					obj->SetWindowTextW(S_CA2T(titleValue.second[*iter].asString().c_str()));
					obj->SetCallBack(std::bind(&CMainDlg::KeyEditUpdate, this, std::placeholders::_1, std::placeholders::_2), titleValue.first, *iter);
				}
			}
		}
	}
	FindChildByName2<SKeyEdit>("sensors_Cl-")->SetCallBack(std::bind(&CMainDlg::KeyEditUpdate, this, std::placeholders::_1, std::placeholders::_2), "Sensors", "sensors_Cl-");
	FindChildByName2<SKeyEdit>("sensors_PH")->SetCallBack(std::bind(&CMainDlg::KeyEditUpdate, this, std::placeholders::_1, std::placeholders::_2), "Sensors", "sensors_PH");
	FindChildByName2<SKeyEdit>("sensors_ES")->SetCallBack(std::bind(&CMainDlg::KeyEditUpdate, this, std::placeholders::_1, std::placeholders::_2), "Sensors", "sensors_ES");


	auto pCharLine = FindChildByName2<SChartLine>("chart_line");
	pCharLine->CreatedLine(L"Real", 1);
	pCharLine->CreatedLine(L"NT");
	pCharLine->CreatedLine(L"BH");
	pCharLine->CreatedLine(L"PL");
	pCharLine->CreatedLine(L"HB");

	FindChildByName2<SCheckBoxEx>("checked_NT")->GetEventSet()->subscribeEvent(EVT_LBUTTONUP, Subscriber(&CMainDlg::CheckedLine, this));
	FindChildByName2<SCheckBoxEx>("checked_BH")->GetEventSet()->subscribeEvent(EVT_LBUTTONUP, Subscriber(&CMainDlg::CheckedLine, this));
	FindChildByName2<SCheckBoxEx>("checked_PL")->GetEventSet()->subscribeEvent(EVT_LBUTTONUP, Subscriber(&CMainDlg::CheckedLine, this));
	FindChildByName2<SCheckBoxEx>("checked_HB")->GetEventSet()->subscribeEvent(EVT_LBUTTONUP, Subscriber(&CMainDlg::CheckedLine, this));


	FindChildByName2<SCheckBoxEx>("checked_press")->GetEventSet()->subscribeEvent(EVT_LBUTTONUP, Subscriber(&CMainDlg::CheckedAdvanceReal, this));
	FindChildByName2<SCheckBoxEx>("checked_vflow")->GetEventSet()->subscribeEvent(EVT_LBUTTONUP, Subscriber(&CMainDlg::CheckedAdvanceReal, this));
	FindChildByName2<SCheckBoxEx>("checked_diffpress")->GetEventSet()->subscribeEvent(EVT_LBUTTONUP, Subscriber(&CMainDlg::CheckedAdvanceReal, this));
	FindChildByName2<SCheckBoxEx>("checked_opening")->GetEventSet()->subscribeEvent(EVT_LBUTTONUP, Subscriber(&CMainDlg::CheckedAdvanceReal, this));
	FindChildByName2<SCheckBoxEx>("checked_mlevel")->GetEventSet()->subscribeEvent(EVT_LBUTTONUP, Subscriber(&CMainDlg::CheckedAdvanceReal, this));



	auto pChartTime = FindChildByName2<SChartTime>(L"chart_time");
	pChartTime->CreatedLine(L"PV");
	pChartTime->CreatedLine(L"YP");

	auto pChartReal = FindChildByName2<SChartReal>(L"advance_chartreal");
	pChartReal->CreateLine(L"checked_press", RGBA(255, 219, 77, 255));
	pChartReal->CreateLine(L"checked_vflow", RGBA(129, 192, 121, 255));
	pChartReal->CreateLine(L"checked_diffpress", RGBA(237, 109, 132, 255));
	pChartReal->CreateLine(L"checked_opening", RGBA(160, 125, 183, 255));
	pChartReal->CreateLine(L"checked_mlevel", RGBA(106, 106, 212, 255));


	{//comboview type 1 2 3
		SComboViewAdapter* pAdapter = new SComboViewAdapter;
		pAdapter->AddItem(L"Type 1");
		pAdapter->AddItem(L"Type 2");
		pAdapter->AddItem(L"Type 3");
		auto obj = FindChildByName2<SComboView>("cbv_type");
		obj->GetListView()->SetAdapter(pAdapter);
		obj->SetCurSel(0);
		obj->GetEventSet()->subscribeEvent(EVT_CB_SELCHANGE, Subscriber(&CMainDlg::ComboViewChange, this));
		pAdapter->Release();
	}
	{//comboview model BH HB PL
		SComboViewAdapter* pAdapter = new SComboViewAdapter;
		pAdapter->AddItem(L"Bingham");
		pAdapter->AddItem(L"HB");
		pAdapter->AddItem(L"Power Law");
		auto obj = FindChildByName2<SComboView>("cbv_model");
		obj->GetListView()->SetAdapter(pAdapter);
		obj->SetCurSel(0);
		obj->GetEventSet()->subscribeEvent(EVT_CB_SELCHANGE, Subscriber(&CMainDlg::ComboViewChange, this));
		pAdapter->Release();
	}
	{//comboview calc model 
		auto obj = FindChildByName2<SComboView>("cbc_calcmodel");
		SComboViewAdapter* pAdapter = new SComboViewAdapter(obj);
		pAdapter->AddItem(L"AUTO");
		pAdapter->AddItem(L"Herschel-Bulkley");
		pAdapter->AddItem(L"Power Law");
		pAdapter->AddItem(L"BingHam");
		pAdapter->AddItem(L"Newtonian");
		obj->GetListView()->SetAdapter(pAdapter);
		obj->GetEventSet()->subscribeEvent(EVT_CB_SELCHANGE, Subscriber(&CMainDlg::ComboViewChange, this));
		pAdapter->Release();
		pAdapter->SetSel(S_CA2T(configMap["Calculation"]["calcmodel"].asString().c_str()));
	}
	{//comboview flowmeter choose 
		auto obj = FindChildByName2<SComboView>("cbv_flowmeter_choose");
		SComboViewAdapter* pAdapter = new SComboViewAdapter(obj);
		pAdapter->AddItem(L"CDKSD");
		pAdapter->AddItem(L"DFJD");
		pAdapter->AddItem(L"SURE");
		obj->GetListView()->SetAdapter(pAdapter);
		obj->GetEventSet()->subscribeEvent(EVT_CB_SELCHANGE, Subscriber(&CMainDlg::ComboViewChange, this));
		pAdapter->Release();
		pAdapter->SetSel(S_CA2T(configMap["Parameter"]["flowmeter_choose"].asString().c_str()));
	}

	{//comboview chart_time
		auto obj = FindChildByName2<SComboView>("cbv_chart_time");
		SComboViewAdapter* pAdapter = new SComboViewAdapter(obj);
		pAdapter->AddItem(L"24 hr");
		pAdapter->AddItem(L"12 hr");
		pAdapter->AddItem(L"6 hr");
		pAdapter->AddItem(L"3 hr");
		pAdapter->AddItem(L"1 hr");
		pAdapter->AddItem(L"0.5 hr");
		obj->GetListView()->SetAdapter(pAdapter);
		obj->GetEventSet()->subscribeEvent(EVT_CB_SELCHANGE, Subscriber(&CMainDlg::ComboViewChange, this));
		pAdapter->Release();
		pAdapter->SetSel(0);
	}

	{//btn units
		FindChildByName2<SImageButton>(L"btn_temp_units")
			->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::BtnUnits, this));
		FindChildByName2<SImageButton>(L"btn_density_units")
			->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::BtnUnits, this));
		FindChildByName2<SImageButton>(L"btn_pv_units")
			->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::BtnUnits, this));
		FindChildByName2<SImageButton>(L"btn_yp_units")
			->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::BtnUnits, this));
	}
	{//advance btn
		for (const auto& it : mapAdvanceBtnCmd_)
		{
			FindChildByName2<SImageButton>(it.first)
				->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::AdvanceBtn, this));
		}
	}
	{//op logs
		OpLogsAdapter* pAdapter = new OpLogsAdapter();
		FindChildByName2<SListView>(L"lv_oplog")->SetAdapter(pAdapter);
		pAdapter->Release();
		FindChildByName2<SImageButton>(L"oplog_prev")
			->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::OpLogsPageChange, this));
		FindChildByName2<SImageButton>(L"oplog_next")
			->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::OpLogsPageChange, this));
		FindChildByName2<SImageButton>(L"oplog_home")
			->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::OpLogsPageChange, this));
		FindChildByName2<SImageButton>(L"oplog_end")
			->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::OpLogsPageChange, this));
		FindChildByName2<SKeyEdit>("oplog_page")
			->SetWindowTextW(L"1");
		FindChildByName2<SKeyEdit>("oplog_page")
			->SetCallBack(std::bind(&CMainDlg::KeyEditUpdate, this, std::placeholders::_1, std::placeholders::_2),
				"oplog", "oplog_page");
	}
	{//data logs
		FindChildByName2<SImageButton>(L"datalog_search")
			->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::DataLogsBtnSearch, this));
		FindChildByName2<SImageButton>(L"datalog_tableft")
			->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::DataLogsTabLeft, this));
		FindChildByName2<SImageButton>(L"datalog_tabright")
			->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::DataLogsTabRight, this));

		DataLogsAdapter* pAdapter = new DataLogsAdapter();
		FindChildByName2<SListView>(L"datalog_lvdata")->SetAdapter(pAdapter);
		pAdapter->Release();
		SensorsAdapter* pPHAdapter = new SensorsAdapter();
		FindChildByName2<SListView>(L"PH_lvdata")->SetAdapter(pPHAdapter);
		pPHAdapter->Release();
		SensorsAdapter* pClAdapter = new SensorsAdapter();
		FindChildByName2<SListView>(L"Cl-_lvdata")->SetAdapter(pClAdapter);
		pClAdapter->Release();
		SensorsAdapter* pESAdapter = new SensorsAdapter();
		FindChildByName2<SListView>(L"ES_lvdata")->SetAdapter(pESAdapter);
		pESAdapter->Release();

		FindChildByName2<SImageButton>(L"datalog_prev")
			->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::DataLogsPageChange, this));
		FindChildByName2<SImageButton>(L"datalog_next")
			->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::DataLogsPageChange, this));
		FindChildByName2<SImageButton>(L"datalog_home")
			->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::DataLogsPageChange, this));
		FindChildByName2<SImageButton>(L"datalog_end")
			->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&CMainDlg::DataLogsPageChange, this));
		FindChildByName2<SKeyEdit>("datalog_page")
			->SetWindowTextW(L"1");
		FindChildByName2<SKeyEdit>("datalog_page")
			->SetCallBack(std::bind(&CMainDlg::KeyEditUpdate, this, std::placeholders::_1, std::placeholders::_2),
				"datalog", "datalog_page");
		FindChildByName2<SKeyEdit>("datalog_comments")
			->SetCallBack(std::bind(&CMainDlg::CommentsUpdate, this, std::placeholders::_1, std::placeholders::_2),
				"datalog", "datalog_comments");
		FindChildByName2<SKeyEdit>("datalog_comments")
			->SetDBCTask(std::bind(&CMainDlg::CommentsDBClick, this));
		FindChildByName2<SKeyEdit>("datalog_comments")
			->SetESCCallback(std::bind(&CMainDlg::CommentsEsc, this));

		auto pChart = FindChildByName2<SChartLogs>("chart_datalog");
		pChart->SetCommentsCallback(std::bind(&CMainDlg::CommentsCallback, this, std::placeholders::_1, std::placeholders::_2
			, std::placeholders::_3));
		for (const auto& it : vDatalogCheck_)
		{
			FindChildByName2<SCheckBoxEx>(it)
				->GetEventSet()->subscribeEvent(EVT_LBUTTONUP, Subscriber(&CMainDlg::DataLogsChecked, this));
			pChart->CreateLine(it.Mid(it.Find(L"_") + 1));
		}
		for (int i = 0; i <= 6; i++)
		{
			SStringT titleName = SStringT().Format(L"datalog_title%d", i);
			FindChildByName2<SStatic>(titleName)->SetAttribute(L"colorBorder", vDatalogLineColorStr[i]);
		}

		std::vector<std::string> vSensors{ "PH","ES","Cl-" };
		std::vector<std::string> vSensorsPage{ "home","end","prev","next" };
		for (auto& item : vSensors)
		{
			SStringT target = S_CA2T(item.c_str());
			FindChildByName2<SKeyEdit>(target + L"_page")
				->SetCallBack(std::bind(&CMainDlg::KeyEditUpdate, this, std::placeholders::_1, std::placeholders::_2),
					"sensors_page", item + "_page");
			for (int i = 0; i < vSensorsPage.size(); i++)
			{
				FindChildByName2<SImageButton>(std::string(item + "_" + vSensorsPage[i]).c_str())
					->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN,
						Subscriber(&CMainDlg::SensorsPageBtn, this));
			}
		}
	}

	return 0;
}

bool CMainDlg::OnWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto* msgInfo = (TransportMsg*)lParam;
	switch (uMsg)
	{
	case WM_NETWORK_UPDATE:
	{
		DiagDataItem item;
		item.type_ = L"Communication";
		item.code_ = L"A0137";
		item.des_ = L"Network Failure";
		if (!App().DeviceIsConnected())
		{
			LOGINFO("PLC connect failure");
			item.timeStart_ = S_CA2T(Utils::GetDateTime().c_str());
			pDiagDlg_->SetDisplayLights(DiagDlg::comm, 1);
			ChangeWndState(WND_STATE_WARN);
			LOGINFO("PLC connect failure");
		}
		else
		{
			item.timeEnd_ = S_CA2T(Utils::GetDateTime().c_str());
			LOGINFO("PLC connected");
		}
		MysqlClient::InsertWellDiags(std::move(item));
	}
	break;
	case WM_WELLNAME_CHANGE:
	{
		Json::Value jsConfig = App().GetItemConfig(STRING_RUNTIME);
		SStringT wellName = S_CA2T(jsConfig[STRING_WELLNAME].asString().c_str(), CP_UTF8);
		SStringT userName = S_CA2T(jsConfig[STRING_USERNAME].asString().c_str(), CP_UTF8);
		FindChildByName2<SStatic>("user_name")->SetWindowTextW(userName);
		FindChildByName2<SStatic>("well_name")->SetWindowTextW(wellName);
		App().DataReload();
		updateMainValue({ main_speed, main_chart_time,main_chart_line, main_model });
	}
	break;
	case WM_TIMER_UPDATE:
	{
		FindChildByName2<SStatic>("datetime")->SetWindowTextW(S_CA2T(Utils::GetDateTime().c_str()));
		AdvanceRealData();
	}
	break;
	case WM_TIMER_CLEAN:
	{
		CleanDateUpdate();
	}
	break;
	case WM_WNDSTATE_UPDATE:
	{
		ChangeWndState((WndStateType)wParam);
	}
	break;
	case WM_WNDREAL_UPDATE:
	{
		updateMainValue({ main_real });
		AdvanceRealText();
		GetCurrentPlcState();
	}
	break;
	case WM_WNDCALC_UPDATE:
	{
		updateMainValue({ main_speed, main_chart_time,main_chart_line, main_model });
		std::string timeEnd = App().GetRecord().time;
		FindChildByName2<SDateTimePickerEx>("datalog_timeend")->SetWindowTextW(S_CA2W(timeEnd.c_str()));
		AddRecordData(App().GetRecord());
		FindChildByName2<SChartLogs>("chart_datalog")->UpdateData();
		GetDatalogsPage();
	}
	break;
	case WM_FLUID_UPDATE:
	{
		if (App().GetFluidInfo().type_ == L"WBM")
		{
			FindChildByName2<SWindow>("sensors_wbm_wnd")->SetAttribute(L"enable", L"1");
			FindChildByName2<SWindow>("sensors_obm_wnd")->SetAttribute(L"enable", L"0");
			FindChildByName2<SMutiImage>("sensors_wbm")->SetCurCheck(2);
			FindChildByName2<SMutiImage>("sensors_obm")->SetCurCheck(0);
		}
		else
		{
			FindChildByName2<SWindow>("sensors_wbm_wnd")->SetAttribute(L"enable", L"0");
			FindChildByName2<SWindow>("sensors_obm_wnd")->SetAttribute(L"enable", L"1");
			FindChildByName2<SMutiImage>("sensors_wbm")->SetCurCheck(0);
			FindChildByName2<SMutiImage>("sensors_obm")->SetCurCheck(2);
		}
		DataLogsTabUpdate();
		updateMainValue({ main_datasource });
	}
	break;
	case WM_PUMPSTATE_UPDATE:
	{

	}
	break;
	default:
		break;
	}
	if (msgInfo)
	{
		delete msgInfo;
	}
	return true;
}

void CMainDlg::updateMainValue(std::list<MainValueType> lvType)
{
	for (auto& type : lvType)
	{
		switch (type)
		{
		case main_speed:
		{

			SStringT type = FindChildByName2<SComboView>("cbv_type")->GetWindowTextW();
			std::vector<SStringT> speedTitle;
			std::vector<float> speedValue;
			if (type == L"Type 1")
			{
				speedTitle = { L"R 600" , L"R 300" ,L"R 200" ,L"R 100" ,L"R 6" ,L"R 3" ,L"R 0" };
				speedValue = { App().GetRecord().r600 ,App().GetRecord().r300 ,
					App().GetRecord().r200 ,App().GetRecord().r100 ,App().GetRecord().r6 ,
					App().GetRecord().r3 ,App().GetRecord().r0 };
			}
			if (type == L"Type 2")
			{
				speedTitle = { L"R 600" , L"R 300" ,L"R 200" ,L"R 100" ,L"R 60" ,L"R 30" ,L"R 6" ,L"R 3" ,L"R 0" };
				speedValue = { App().GetRecord().r600 ,App().GetRecord().r300 ,App().GetRecord().r200 ,
					App().GetRecord().r100 ,App().GetRecord().r60 ,App().GetRecord().r30 ,
					App().GetRecord().r6 ,App().GetRecord().r3 ,App().GetRecord().r0 };
			}
			if (type == L"Type 3")
			{
				speedTitle = { L"R 600" , L"R 300" ,L"R 200" ,L"R 100" ,L"R 60" ,L"R 30" ,L"R 20" ,L"R 10" ,L"R 6" ,L"R 3" ,L"R 0" };
				speedValue = { App().GetRecord().r600 ,App().GetRecord().r300 ,App().GetRecord().r200 ,
					App().GetRecord().r100 ,App().GetRecord().r60 ,App().GetRecord().r30 ,App().GetRecord().r20 ,
					App().GetRecord().r10 ,App().GetRecord().r6 ,App().GetRecord().r3 ,App().GetRecord().r0 };
			}
			int i = 0;
			for (; i < speedTitle.size(); i++)
			{
				SStringT itemTitle = SStringT().Format(L"speed_%d_title", i);
				SStringT itemValue = SStringT().Format(L"speed_%d_value", i);
				FindChildByName2<SStatic>(itemTitle)->SetWindowTextW(speedTitle[i]);
				FindChildByName2<SStatic>(itemValue)->SetWindowTextW(SStringT().Format(L"%.2f", speedValue[i]));
			}
			for (; i <= 10; i++)
			{
				SStringT itemTitle = SStringT().Format(L"speed_%d_title", i);
				SStringT itemValue = SStringT().Format(L"speed_%d_value", i);
				FindChildByName2<SStatic>(itemTitle)->SetWindowTextW(L"");
				FindChildByName2<SStatic>(itemValue)->SetWindowTextW(L"");
			}
		}
		break;
		case main_chart_line:
		{
			size_t pointCount = App().GetPoints().size();
			if (pointCount > 0)
			{
				auto pCharLine = FindChildByName2<SChartLine>("chart_line");
				pCharLine->Clear();
				pCharLine->SetLineWidth(S_CA2W(App().GetRecord().model.c_str()), 5);
				for (auto& [x, y] : App().GetPoints())
				{
					pCharLine->AddData(L"Real", (float)x, (float)y);
				}


				float XMaxValue = (float)App().GetPoints().back().x + 10.0f;
				if (XMaxValue < 1021.8)
				{
					XMaxValue = 1021.8f;
				}
				float xIndexVal = 0.0f;

				while (xIndexVal <= XMaxValue)
				{
					pCharLine->AddData(L"NT", xIndexVal, App().GetRecord().nline * xIndexVal);
					pCharLine->AddData(L"BH", xIndexVal, App().GetRecord().nbh * xIndexVal + App().GetRecord().kbh);
					pCharLine->AddData(L"PL", xIndexVal, pow(xIndexVal, App().GetRecord().npl) * App().GetRecord().kpl);
					pCharLine->AddData(L"HB", xIndexVal, pow(xIndexVal, App().GetRecord().nhb) * App().GetRecord().khb + App().GetRecord().tau0);
					xIndexVal += 1.0f;
				}
				pCharLine->UpdateData();
			}
		}
		break;
		case main_chart_time:
		{
			RecordData lastRecord = App().GetRecord();
			auto pChart = FindChildByName2<SChartTime>(L"chart_time");
			SStringT strEndTime = S_CA2W(lastRecord.time.c_str());
			SStringT timeRange = FindChildByName2<SComboView>(L"cbv_chart_time")->GetWindowTextW();
			long seconds = std::stol(timeRange.Left(timeRange.Find(L" ")).GetBuffer(0)) * 60 * 60;

			SStringT strStartTime = S_CA2W(Utils::GetDateTimeFrom(lastRecord.time, 0 - seconds).c_str());

			pChart->SetXMaxMin(strEndTime, strStartTime);
			std::vector<RecordData> recordDataList;
			MysqlClient::GetRecordsInTime(recordDataList, S_CW2A(strStartTime).GetBuffer(0), lastRecord.time);
			if (recordDataList.size() > 0)
			{
				pChart->Clear();
				for (auto& it : recordDataList)
				{
					pChart->AddTime(S_CA2W(it.time.c_str()));
					pChart->AddData(L"PV", it.pv);
					pChart->AddData(L"YP", it.yp);
				}
				float LMax = std::stof(S_CW2A(FindChildByName2<SKeyEdit>(L"chart_time_lmax")->GetWindowTextW()).GetBuffer(0));
				float LMin = std::stof(S_CW2A(FindChildByName2<SKeyEdit>(L"chart_time_lmin")->GetWindowTextW()).GetBuffer(0));
				float RMax = std::stof(S_CW2A(FindChildByName2<SKeyEdit>(L"chart_time_rmax")->GetWindowTextW()).GetBuffer(0));
				float RMin = std::stof(S_CW2A(FindChildByName2<SKeyEdit>(L"chart_time_rmin")->GetWindowTextW()).GetBuffer(0));
				pChart->SetYMaxMin(LMax, LMin, RMax, RMin);
				pChart->UpdateData();
			}
		}
		break;
		case main_real:
		{
			RealBlockValSet({ L"temp",L"density" });
		}
		break;
		case main_model:
		{
			SStringT model = FindChildByName2<SComboView>(L"cbv_model")->GetWindowTextW();
			std::list<SStringT> vModel;
			if (curRealModel != model)
			{
				curRealModel = model;
				SStringT lUnit, lAlertUp, lAlertDown;
				SStringT rUnit, rAlertUp, rAlertDown;
				SStringT lTitle, rTitle;
				if (model == L"Bingham")
				{
					lUnit = *vUnitPV.begin();
					rUnit = *vUnitYP.begin();
					lAlertUp = S_CA2W(App().GetItemConfig("MainPageConfig", "pv_alert_up").c_str());
					lAlertDown = S_CA2W(App().GetItemConfig("MainPageConfig", "pv_alert_down").c_str());
					rAlertUp = S_CA2W(App().GetItemConfig("MainPageConfig", "yp_alert_up").c_str());
					rAlertDown = S_CA2W(App().GetItemConfig("MainPageConfig", "yp_alert_down").c_str());
					vModel = { L"pv",L"yp" };
					lTitle = L"Plastic Viscosity";
					rTitle = L"Yield Point";
				}
				if (model == L"Power Law")
				{
					lUnit = L"";
					rUnit = *vUnitK.begin();
					lAlertUp = S_CA2W(App().GetItemConfig("MainPageConfig", "npl_alert_up").c_str());
					lAlertDown = S_CA2W(App().GetItemConfig("MainPageConfig", "npl_alert_down").c_str());
					rAlertUp = S_CA2W(App().GetItemConfig("MainPageConfig", "kpl_alert_up").c_str());
					rAlertDown = S_CA2W(App().GetItemConfig("MainPageConfig", "kpl_alert_down").c_str());
					vModel = { L"npl",L"kpl" };
					lTitle = L"Flow Index";
					rTitle = L"Consistency Index";
				}
				if (model == L"HB")
				{
					lUnit = L"";
					rUnit = *vUnitK.begin();
					lAlertUp = S_CA2W(App().GetItemConfig("MainPageConfig", "nhb_alert_up").c_str());
					lAlertDown = S_CA2W(App().GetItemConfig("MainPageConfig", "nhb_alert_down").c_str());
					rAlertUp = S_CA2W(App().GetItemConfig("MainPageConfig", "khb_alert_up").c_str());
					rAlertDown = S_CA2W(App().GetItemConfig("MainPageConfig", "khb_alert_down").c_str());
					vModel = { L"nhb",L"khb" };
					lTitle = L"Flow Index";
					rTitle = L"Consistency Index";
				}
				FindChildByName2<SImageButton>(L"btn_pv_units")->SetWindowTextW(lUnit);
				FindChildByName2<SImageButton>(L"btn_yp_units")->SetWindowTextW(rUnit);
				FindChildByName2<SKeyEdit>(L"pv_alert_up")->SetWindowTextW(lAlertUp);
				FindChildByName2<SKeyEdit>(L"pv_alert_down")->SetWindowTextW(lAlertDown);
				FindChildByName2<SKeyEdit>(L"yp_alert_up")->SetWindowTextW(rAlertUp);
				FindChildByName2<SKeyEdit>(L"yp_alert_down")->SetWindowTextW(rAlertDown);
				FindChildByName2<SStatic>(L"text_pv")->SetWindowTextW(lTitle);
				FindChildByName2<SStatic>(L"text_yp")->SetWindowTextW(rTitle);
			}
			else
			{
				if (model == L"Bingham")
				{
					vModel = { L"pv",L"yp" };
				}
				if (model == L"Power Law")
				{
					vModel = { L"npl",L"kpl" };
				}
				if (model == L"HB")
				{
					vModel = { L"nhb",L"khb" };
				}
			}
			RealBlockValSet(vModel);
		}
		break;
		case main_datasource:
		{
			auto wndSubBlock = FindChildByName2<SWindow>("wnd_ph_data");
			auto textCl = FindChildByName2<SStatic>("text_cl");
			auto btnUnitsCl = FindChildByName2<SImageButton>("btn_cl_units");
			auto lAlertUp = FindChildByName2<SKeyEdit>("cl_alert_up");
			auto lAlertDown = FindChildByName2<SKeyEdit>("cl_alert_down");
			auto rAlertUp = FindChildByName2<SKeyEdit>("ph_alert_up");
			auto rAlertDown = FindChildByName2<SKeyEdit>("ph_alert_down");
			std::list<SStringT> vModel;
			if (App().GetFluidInfo().type_ == L"WBM")
			{
				wndSubBlock->SetAttribute(L"show", L"1");
				textCl->SetWindowTextW(L"Cl-");
				btnUnitsCl->SetWindowTextW(L"g/L");
				lAlertUp->SetWindowTextW(S_CA2W(App().GetItemConfig("MainPageConfig", "cl_alert_up").c_str()));
				lAlertDown->SetWindowTextW(S_CA2W(App().GetItemConfig("MainPageConfig", "cl_alert_down").c_str()));
				rAlertUp->SetWindowTextW(S_CA2W(App().GetItemConfig("MainPageConfig", "ph_alert_up").c_str()));
				rAlertDown->SetWindowTextW(S_CA2W(App().GetItemConfig("MainPageConfig", "ph_alert_down").c_str()));
				vModel = { L"cl",L"ph" };
			}
			else
			{
				textCl->SetWindowTextW(L"ES");
				wndSubBlock->SetAttribute(L"show", L"0");
				btnUnitsCl->SetWindowTextW(L"V");
				lAlertUp->SetWindowTextW(S_CA2W(App().GetItemConfig("MainPageConfig", "es_alert_up").c_str()));
				lAlertDown->SetWindowTextW(S_CA2W(App().GetItemConfig("MainPageConfig", "es_alert_down").c_str()));
				vModel = { L"es" };
			}
			RealBlockValSet(vModel);
		}
		break;
		default:
			break;
		}
	}
}


void CMainDlg::ChangeWndState(WndStateType state)
{
	auto iter = mapWndStateInfo_.find(state);
	if (iter != mapWndStateInfo_.end())
	{
		FindChildByName2<SWindow>("xml_home")->SetAttribute(L"colorBkgnd", iter->second.color_);
		FindChildByName2<SWindow>("state_title")->SetAttribute(L"colorBkgnd", iter->second.color_);
		FindChildByName2<SWindow>("state_title")->SetWindowTextW(iter->second.title_);
		curWndState = state;
	}
	else
	{
		switch (state)
		{
		case WND_STATE_POP:
			nLayered++;
			break;
		case WND_STATE_DEF:
			nLayered--;
			break;
		default:
			break;
		}
		if (nLayered > 0)
		{
			FindChildByName2<SWindow>("blurWnd")->SetAttribute(L"show", L"1");
		}
		else
		{
			FindChildByName2<SWindow>("blurWnd")->SetAttribute(L"show", L"0");
			nLayered = 0;
		}
	}
}

void CMainDlg::OnDlgDiag()
{
	if (pDiagDlg_)
	{
		pDiagDlg_->ShowWindow(SW_SHOWNORMAL);
		App().AsyncEvent(WM_WNDSTATE_UPDATE, WND_STATE_POP);
		FindChildByName2<SCheckBoxEx>("system_setup")->SetCheck(FALSE);
		OnSystemMenu();
	}
}

void CMainDlg::OnDlgFluidSetup()
{
	if (pFluidInfoDlg_)
	{
		pFluidInfoDlg_->ShowDlg();
		App().AsyncEvent(WM_WNDSTATE_UPDATE, WND_STATE_POP);
		FindChildByName2<SCheckBoxEx>("system_setup")->SetCheck(FALSE);
		OnSystemMenu();
	}
}

void CMainDlg::OnDlgSystemOptions()
{
	if (pSystemOptionsDlg_)
	{
		pSystemOptionsDlg_->ShowWindow(SW_SHOWNORMAL);
		App().AsyncEvent(WM_WNDSTATE_UPDATE, WND_STATE_POP);
		FindChildByName2<SCheckBoxEx>("system_setup")->SetCheck(FALSE);
		OnSystemMenu();
	}
}

void CMainDlg::OnLogout()
{
	App().ShowMessage(L"Logout", L"You Will Logout?", std::bind(&CMainDlg::PageLoginOrMain, this, true));
	FindChildByName2<SCheckBoxEx>("system_setup")->SetCheck(FALSE);
	OnSystemMenu();
}


//TODO:消息映射
void CMainDlg::OnClose()
{
	App().SetItemConfig(STRING_RUNTIME, STRING_ISEXIT, "1");
	App().Quit();
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
	FindChildByName2<SCheckBoxEx>("system_setup")->SetCheck(FALSE);
	OnSystemMenu();
}

void CMainDlg::OnLogin()
{
	std::string temp_user = S_CW2A(FindChildByName2<SWinEdit>("winedit_user")->GetWindowTextW(), CP_UTF8);
	std::string temp_password = S_CW2A(FindChildByName2<SWinEdit>("winedit_password")->GetWindowTextW(), CP_UTF8);
	if (MysqlClient::LoginVerification(temp_user, temp_password))
	{
		App().SetItemConfig(STRING_RUNTIME, STRING_USERNAME, temp_user);
		PageLoginOrMain(false);
		MysqlClient::InsertOperation("login");
		EventArgs args(FindChildByName2<SWindow>(R.name.system_chk_home));
		CheckBtnChange(&args);
		OnDlgSystemOptions();
	}
	else
	{
		App().ShowMessage(L"Error", L"Username or Password error");
	}
}

void CMainDlg::OnSystemMenu()
{
	FindChildByName2<SWindow>("system_menu")->SetAttribute(
		L"show", FindChildByName2<SCheckBoxEx>("system_setup")->IsChecked() ? L"1" : L"0");
}

void CMainDlg::PageLoginOrMain(bool bPageLogin)
{
	FindChildByName2<SWindow>("xml_login")->SetAttribute(L"show", bPageLogin ? L"1" : L"0");
	FindChildByName2<SWindow>("xml_home")->SetAttribute(L"show", bPageLogin ? L"0" : L"1");
}

void CMainDlg::OnPageHome(bool bShow)
{
	FindChildByName2<SWindow>("home_page")->SetAttribute(L"show", bShow ? L"1" : L"0");
	FindChildByName2<SWindow>("system_chk_home")->SetCheck(bShow);
}

void CMainDlg::OnPageLogs(bool bShow)
{
	FindChildByName2<SWindow>("logs_page")->SetAttribute(L"show", bShow ? L"1" : L"0");
	FindChildByName2<SWindow>("system_chk_logs")->SetCheck(bShow);
	if (!bShow) return;
	CheckBtnChange(&EventArgs(FindChildByName(R.name.logs_chk_datalogs)));
}

void CMainDlg::OnPageAdvance(bool bShow)
{
	FindChildByName2<SWindow>("advance_page")->SetAttribute(L"show", bShow ? L"1" : L"0");
	FindChildByName2<SWindow>("system_chk_adv")->SetCheck(bShow);
	if (!bShow) return;
	CheckBtnChange(&EventArgs(FindChildByName(R.name.advance_chk_debug)));
}

void CMainDlg::OnPageDatalogs(bool bShow)
{
	FindChildByName2<SWindow>("page_datalogs")->SetAttribute(L"show", bShow ? L"1" : L"0");
	FindChildByName2<SWindow>("logs_chk_datalogs")->SetCheck(bShow);
	if (!bShow) return;
	std::string timeEnd = App().GetRecord().time;
	std::string timeStart = Utils::GetDateTimeFrom(timeEnd, -(12 * 60 * 60));
	FindChildByName2<SDateTimePickerEx>("datalog_timestart")->SetWindowTextW(S_CA2W(timeStart.c_str()));
	FindChildByName2<SDateTimePickerEx>("datalog_timeend")->SetWindowTextW(S_CA2W(timeEnd.c_str()));
	nDatalogsPage_ = 1;
	nDatalogsTab_ = 1;
	ResetDatalogs({ L"datalog_R600",L"datalog_R300" ,L"datalog_PV",L"datalog_YP" ,L"datalog_nHB",L"datalog_KHB" ,L"datalog_Tau0" });
	DataLogsTabUpdate();
	ChartLogsChecked();
}

void CMainDlg::ResetDatalogs(std::vector<SStringT> checkBtn)
{
	std::vector<RecordData> lvInfo;
	std::vector<CommentsData> lvComments;
	SStringT timeStart = FindChildByName2<SDateTimePickerEx>("datalog_timestart")->GetWindowTextW();
	SStringT timeEnd = FindChildByName2<SDateTimePickerEx>("datalog_timeend")->GetWindowTextW();
	MysqlClient::GetRecordsInTime(lvInfo, S_CW2A(timeStart).GetBuffer(0), S_CW2A(timeEnd).GetBuffer(0));
	MysqlClient::GetCommentsInTime(lvComments, S_CW2A(timeStart).GetBuffer(0), S_CW2A(timeEnd).GetBuffer(0));
	auto pChar = FindChildByName2<SChartLogs>("chart_datalog");
	pChar->Clear();
	if (lvInfo.size())
	{
		pChar->AddComment(lvComments);
		for (auto& data : lvInfo)
		{
			AddRecordData(data);
		}
		nDatalogsTotalPage_ = lvInfo.size() / 15 + (lvInfo.size() % 15 ? 1 : 0);
		FindChildByName2<SStatic>("datalog_total")->SetWindowTextW(std::to_wstring(nDatalogsTotalPage_).c_str());

		if (checkBtn.size())
		{
			for (auto& name : vDatalogCheck_)
			{
				auto obj = FindChildByName2<SCheckBoxEx>(name);
				obj->SetCheck(FALSE);
				DataLogsChecked(&EventArgs(obj));
			}
			for (auto& name : checkBtn)
			{
				auto obj = FindChildByName2<SCheckBoxEx>(name);
				obj->SetCheck(TRUE);
				DataLogsChecked(&EventArgs(obj));
			}
		}
	}
	GetDatalogsPage();
}

void CMainDlg::AddRecordData(const RecordData& inData)
{
	RecordData data = inData;
	auto pChar = FindChildByName2<SChartLogs>("chart_datalog");
	pChar->AddData(L"AvgTemp", data.avg_temp, data.time);
	pChar->AddData(L"AvgPress", data.avg_pressure, data.time);
	pChar->AddData(L"R600", data.r600, data.time);
	pChar->AddData(L"R300", data.r300, data.time);
	pChar->AddData(L"R200", data.r200, data.time);
	pChar->AddData(L"R100", data.r100, data.time);
	pChar->AddData(L"R60", data.r60, data.time);
	pChar->AddData(L"R30", data.r30, data.time);
	pChar->AddData(L"R20", data.r20, data.time);
	pChar->AddData(L"R10", data.r10, data.time);
	pChar->AddData(L"R6", data.r6, data.time);
	pChar->AddData(L"R3", data.r3, data.time);
	pChar->AddData(L"R0", data.r0, data.time);
	pChar->AddData(L"MaxDensity", data.max_density, data.time);
	pChar->AddData(L"AvgDensity", data.avg_density, data.time);
	pChar->AddData(L"MinDensity", data.min_density, data.time);
	pChar->AddData(L"AV", data.av, data.time);
	pChar->AddData(L"PV", data.pv, data.time);
	pChar->AddData(L"YP", data.yp, data.time);
	pChar->AddData(L"nPL", data.npl, data.time);
	pChar->AddData(L"KPL", data.kpl, data.time);
	pChar->AddData(L"nHB", data.nhb, data.time);
	pChar->AddData(L"KHB", data.khb, data.time);
	pChar->AddData(L"Tau0", data.tau0, data.time);
}

void CMainDlg::ChartLogsChecked()
{
	auto pChart = FindChildByName2<SChartLogs>("chart_datalog");
	pChart->CleanLabel();
	int i = 0;
	for (const auto& checked : vDatalogBeChecked)
	{
		SStringT titleName = SStringT().Format(L"datalog_title%d", i);
		SStringT info = checked.Mid(checked.Find(L"_") + 1);
		FindChildByName2<SStatic>(titleName)->SetWindowTextW(info);
		pChart->SetLabelAndColor(info, vDatalogLineColorRGB[i]);
		i++;
	}
	for (; i <= 6; i++)
	{
		SStringT titleName = SStringT().Format(L"datalog_title%d", i);
		FindChildByName2<SStatic>(titleName)->SetWindowTextW(L"");
	}
	pChart->UpdateData();
}

void CMainDlg::GetDatalogsPage()
{
	SStringT timeStart = FindChildByName2<SDateTimePickerEx>("datalog_timestart")->GetWindowTextW();
	SStringT timeEnd = FindChildByName2<SDateTimePickerEx>("datalog_timeend")->GetWindowTextW();
	FindChildByName2<SKeyEdit>("datalog_page")->SetWindowTextW(std::to_wstring(nDatalogsPage_).c_str());
	DataLogsAdapter* adapter = (DataLogsAdapter*)FindChildByName2<SListView>("datalog_lvdata")->GetAdapter();
	std::vector<RecordData> lvInfo;
	MysqlClient::GetRecordPage(lvInfo, S_CW2A(timeStart).GetBuffer(0), S_CW2A(timeEnd).GetBuffer(0), nDatalogsPage_, 15);

	adapter->RemoveAll();
	for (auto& data : lvInfo)
	{
		adapter->Add(data);
	}
	adapter->notifyDataSetChanged();
}


bool CMainDlg::DataLogsChecked(EventArgs* pEvt)
{
	if (pEvt->nameFrom == nullptr) return false;
	SCheckBoxEx* checked = (SCheckBoxEx*)pEvt->sender;
	auto pChart = FindChildByName2<SChartLogs>("chart_datalog");
	SStringT target = pEvt->nameFrom;
	if (checked->IsChecked())
	{
		if (vDatalogBeChecked.size() > 6)
		{
			SStringT popName = vDatalogBeChecked.front();
			vDatalogBeChecked.pop_front();
			FindChildByName2<SCheckBoxEx>(popName)->SetCheck(FALSE);
			auto subText1 = FindChildByName2<SStatic>(popName + L"_text1");
			auto subText2 = FindChildByName2<SStatic>(popName + L"_text2");
			if (subText1)
			{
				subText1->SetAttribute(L"colorText", L"#000000");
			}
			if (subText2)
			{
				subText2->SetAttribute(L"colorText", L"#000000");
			}
		}
		vDatalogBeChecked.emplace_back(pEvt->nameFrom);
		auto subText1 = FindChildByName2<SStatic>(SStringT(pEvt->nameFrom) + L"_text1");
		auto subText2 = FindChildByName2<SStatic>(SStringT(pEvt->nameFrom) + L"_text2");
		if (subText1)
		{
			subText1->SetAttribute(L"colorText", L"#FFFFFF");
		}
		if (subText2)
		{
			subText2->SetAttribute(L"colorText", L"#FFFFFF");
		}

	}
	else
	{
		auto iter = std::find(vDatalogBeChecked.begin(), vDatalogBeChecked.end(), pEvt->nameFrom);
		if (iter != vDatalogBeChecked.end())
		{
			vDatalogBeChecked.erase(iter);
		}
		auto subText1 = FindChildByName2<SStatic>(SStringT(pEvt->nameFrom) + L"_text1");
		auto subText2 = FindChildByName2<SStatic>(SStringT(pEvt->nameFrom) + L"_text2");
		if (subText1)
		{
			subText1->SetAttribute(L"colorText", L"#000000");
		}
		if (subText2)
		{
			subText2->SetAttribute(L"colorText", L"#000000");
		}
	}
	ChartLogsChecked();
	return false;
}

bool CMainDlg::DataLogsPageChange(EventArgs* pEvt)
{
	if (pEvt->nameFrom == nullptr) return false;
	SStringT name = pEvt->nameFrom;
	if (name == L"datalog_prev")
	{
		if (nDatalogsPage_ <= 1) return false;
		nDatalogsPage_ -= 1;
	}
	if (name == L"datalog_next")
	{
		if (nDatalogsPage_ >= nDatalogsTotalPage_) return false;
		nDatalogsPage_ += 1;
	}
	if (name == L"datalog_home")
	{
		nDatalogsPage_ = 1;
	}
	if (name == L"datalog_end")
	{
		nDatalogsPage_ = nDatalogsTotalPage_;
	}
	GetDatalogsPage();
	return true;
}

bool CMainDlg::DataLogsBtnSearch(EventArgs* pEvt)
{
	ResetDatalogs();
	DataLogsTabUpdate();
	ChartLogsChecked();
	return false;
}

bool CMainDlg::DataLogsTabLeft(EventArgs* pEvt)
{
	nDatalogsTab_ = 1;
	DataLogsTabUpdate();
	return false;
}

bool CMainDlg::DataLogsTabRight(EventArgs* pEvt)
{
	nDatalogsTab_ = 2;
	DataLogsTabUpdate();
	return false;
}

void CMainDlg::DataLogsTabUpdate()
{
	switch (nDatalogsTab_)
	{
	case 1:
	{
		FindChildByName2<SWindow>("datalog_record")->SetAttribute(L"show", L"1");
		FindChildByName2<SWindow>("datalog_wbm")->SetAttribute(L"show", L"0");
		FindChildByName2<SWindow>("datalog_obm")->SetAttribute(L"show", L"0");
	}
	break;
	case 2:
	{
		FindChildByName2<SWindow>("datalog_record")->SetAttribute(L"show", L"0");
		if (App().GetFluidInfo().type_ == L"WBM")
		{
			ResetSensorsData(L"PH");
			ResetSensorsData(L"Cl-");
			FindChildByName2<SWindow>("datalog_wbm")->SetAttribute(L"show", L"1");
			FindChildByName2<SWindow>("datalog_obm")->SetAttribute(L"show", L"0");
			auto obj = FindChildByName2<SCheckBoxEx>("datalog_ES");
			obj->SetCheck(FALSE);
			DataLogsChecked(&EventArgs(obj));
		}
		else
		{
			ResetSensorsData(L"ES");
			FindChildByName2<SWindow>("datalog_wbm")->SetAttribute(L"show", L"0");
			FindChildByName2<SWindow>("datalog_obm")->SetAttribute(L"show", L"1");
			auto obj1 = FindChildByName2<SCheckBoxEx>("datalog_PH");
			auto obj2 = FindChildByName2<SCheckBoxEx>("datalog_Cl-");
			obj1->SetCheck(FALSE);
			obj2->SetCheck(FALSE);
			DataLogsChecked(&EventArgs(obj1));
			DataLogsChecked(&EventArgs(obj2));
		}

	}
	break;
	default:
		break;
	}
}

void CMainDlg::ResetSensorsData(const SStringT& sensorName)
{
	auto pChar = FindChildByName2<SChartLogs>("chart_datalog");
	pChar->Clear(sensorName);
	std::vector<SensorsRecord> lvInfo;
	SStringT startTime = FindChildByName2<SDateTimePickerEx>("datalog_timestart")->GetWindowTextW();
	SStringT endTime = FindChildByName2<SDateTimePickerEx>("datalog_timeend")->GetWindowTextW();
	MysqlClient::GetSensorsRecordInTime(S_CW2A(sensorName).GetBuffer(0), lvInfo,
		S_CW2A(startTime).GetBuffer(0), S_CW2A(endTime).GetBuffer(0));
	if (lvInfo.size())
	{
		for (auto& item : lvInfo)
		{
			pChar->AddData(sensorName, item.value, item.time);
		}
	}
	size_t totals = lvInfo.size();
	totals = totals / 15 + ((totals % 15 == 0) ? 0 : 1);
	FindChildByName2<SStatic>(sensorName + L"_total")
		->SetWindowTextW(SStringT().Format(L"%d", totals));
	GetSensorsPage(sensorName);
}

void CMainDlg::GetSensorsPage(const SStringT& sensorName)
{
	int page = std::stoi(S_CW2A(FindChildByName2<SKeyEdit>(sensorName + L"_page")->GetWindowTextW()).GetBuffer(0));
	int total = std::stoi(S_CW2A(FindChildByName2<SStatic>(sensorName + L"_total")->GetWindowTextW()).GetBuffer(0));
	SStringT timeStart = FindChildByName2<SDateTimePickerEx>("datalog_timestart")->GetWindowTextW();
	SStringT timeEnd = FindChildByName2<SDateTimePickerEx>("datalog_timeend")->GetWindowTextW();
	SensorsAdapter* adapter = (SensorsAdapter*)FindChildByName2<SListView>(sensorName + L"_lvdata")->GetAdapter();
	std::vector<SensorsRecord> lvInfo;
	MysqlClient::GetSensorsRecordPage(S_CW2A(sensorName).GetBuffer(0), lvInfo,
		S_CW2A(timeStart).GetBuffer(0), S_CW2A(timeEnd).GetBuffer(0), page, 15);

	adapter->RemoveAll();
	for (auto& data : lvInfo)
	{
		adapter->Add(data);
	}
	adapter->notifyDataSetChanged();
}

bool CMainDlg::SensorsPageBtn(EventArgs* pEvt)
{
	if (pEvt->nameFrom == nullptr) return false;
	SStringT name = pEvt->nameFrom;
	SStringT target = name.Left(name.Find(L"_"));
	SStringT dirc = name.Mid(name.Find(L"_") + 1);
	int page = std::stoi(S_CW2A(FindChildByName2<SKeyEdit>(target + L"_page")->GetWindowTextW()).GetBuffer(0));
	int total = std::stoi(S_CW2A(FindChildByName2<SStatic>(target + L"_total")->GetWindowTextW()).GetBuffer(0));
	if (dirc == L"home")
	{
		page = 1;
	}
	else if (dirc == L"end")
	{
		page = total;
	}
	else if (dirc == L"prev")
	{
		page--;
	}
	else if (dirc == L"next")
	{
		page++;
	}
	if (page < 1 || page > total) return false;
	FindChildByName2<SKeyEdit>(target + L"_page")
		->SetWindowTextW(SStringT().Format(L"%d", page));
	GetSensorsPage(target);
	return false;
}


void CMainDlg::OnPageOplogs(bool bShow)
{
	FindChildByName2<SWindow>("page_oplogs")->SetAttribute(L"show", bShow ? L"1" : L"0");
	FindChildByName2<SWindow>("logs_chk_oplogs")->SetCheck(bShow);
	if (!bShow) return;
	nOplogsPage_ = 1;
	GetPageOplogs();
}

void CMainDlg::GetPageOplogs()
{
	std::list<OpLogsData> OpLogsList;
	size_t totals = 0;
	MysqlClient::GetOplogsCount(totals);
	MysqlClient::GetPageOplogs(OpLogsList, nOplogsPage_, 20);
	nOplogsTotalPage_ = totals / 20 + ((totals % 20) == 0 ? 0 : 1);
	FindChildByName2<SStatic>("oplog_total")->SetWindowTextW(std::to_wstring(nOplogsTotalPage_).c_str());
	FindChildByName2<SKeyEdit>("oplog_page")->SetWindowTextW(std::to_wstring(nOplogsPage_).c_str());
	OpLogsAdapter* adpter = (OpLogsAdapter*)FindChildByName2<SListView>("lv_oplog")->GetAdapter();
	adpter->RemoveAll();
	if (OpLogsList.size())
	{
		for (auto& it : OpLogsList)
		{
			adpter->Add(it.time.c_str(), it.user.c_str(), it.event.c_str());
		}
	}
	adpter->notifyDataSetChanged();
}

bool CMainDlg::OpLogsPageChange(EventArgs* pEvt)
{
	if (pEvt->nameFrom == nullptr) return false;
	SStringT name = pEvt->nameFrom;
	if (name == L"oplog_prev")
	{
		if (nOplogsPage_ == 1) return false;
		nOplogsPage_ -= 1;
	}
	if (name == L"oplog_next")
	{
		if (nOplogsPage_ == nOplogsTotalPage_) return false;
		nOplogsPage_ += 1;
	}
	if (name == L"oplog_home")
	{
		nOplogsPage_ = 1;
	}
	if (name == L"oplog_end")
	{
		nOplogsPage_ = nOplogsTotalPage_;
	}
	GetPageOplogs();
	return true;
}


void CMainDlg::OnPageAdvanceDebug(bool bShow)
{
	FindChildByName2<SWindow>("page_advance_debug")->SetAttribute(L"show", bShow ? L"1" : L"0");
	FindChildByName2<SWindow>("advance_chk_debug")->SetCheck(bShow);
	if (!bShow) return;
	FindChildByName2<SChartReal>("advance_chartreal")->SetHistory(false);
}

void CMainDlg::OnPageAdvanceInit(bool bShow)
{
	FindChildByName2<SWindow>("page_advance_init")->SetAttribute(L"show", bShow ? L"1" : L"0");
	FindChildByName2<SWindow>("advance_chk_init")->SetCheck(bShow);
}

void CMainDlg::GetCurrentPlcState()
{
	bool bIdle = true;
	//报警状态
	////1：硬件故障
	pDiagDlg_->SetDisplayLights(DiagDlg::hardware, App().DeviceStates().bAppHardware ? 1 : 0);
	////2: 通讯故障
	pDiagDlg_->SetDisplayLights(DiagDlg::comm, App().DeviceStates().bAppNetwork ? 1 : 0);
	////3: 工艺保护
	pDiagDlg_->SetDisplayLights(DiagDlg::alarm, App().DeviceStates().bAppProtected ? 1 : 0);
	////4: 工艺警告
	pDiagDlg_->SetDisplayLights(DiagDlg::warning, App().DeviceStates().bAppWarn ? 1 : 0);


	if (App().DeviceStates().bSignCleaning)
	{
		bIdle = false;
		if (curWndState != WND_STATE_CLEAN)
		{
			ChangeWndState(WND_STATE_CLEAN);
		}
	}
	if (App().DeviceStates().bSignTesting)
	{
		bIdle = false;
		if (curWndState != WND_STATE_TEST)
		{
			ChangeWndState(WND_STATE_TEST);
			FindChildByName2<SImageButton>("btn_start")->SetWindowTextW(L"PAUSE");
		}
	}
	if (App().DeviceStates().bAppHardware || 
		App().DeviceStates().bAppNetwork || 
		App().DeviceStates().bAppProtected ||
		App().DeviceStates().bAppWarn)
	{
		bIdle = false;
		if (curWndState != WND_STATE_WARN)
		{
			ChangeWndState(WND_STATE_WARN);
		}
	}

	if (bIdle && curWndState != WND_STATE_IDLE)
	{
		ChangeWndState(WND_STATE_IDLE);
		FindChildByName2<SImageButton>("btn_start")->SetWindowTextW(L"START");
	}

	if (App().DeviceStates().bEStop)
	{
		if (!pRestoreDlg_->IsWindowVisible())
		{
			ChangeWndState(WND_STATE_POP);
			pRestoreDlg_->ShowWindow(SW_SHOWNORMAL);
		}
	}
	else
	{
		if (pRestoreDlg_->IsWindowVisible())
		{
			ChangeWndState(WND_STATE_DEF);
			pRestoreDlg_->ShowWindow(SW_HIDE);
			MysqlClient::InsertOperation("System Restore");
		}
	}
}


void CMainDlg::AdvanceBtnChecked()
{
	pSystemOptionsDlg_->SetManual(App().DeviceStates().bSignManual);
	FindChildByName2<SImageButton>(L"advance_debug")->SetCheck(
		App().DeviceStates().bSignManual);
	pSystemOptionsDlg_->SetPump(App().DeviceStates().bCirculatingPumpOperation);
	FindChildByName2<SImageButton>(L"advance_circulating_pump")->SetCheck(
		App().DeviceStates().bCirculatingPumpOperation);
	FindChildByName2<SImageButton>(L"advance_return_valve_mud")->SetCheck(
		App().DeviceStates().bReturnValveMudOpen);
	FindChildByName2<SImageButton>(L"advance_purge_valve")->SetCheck(
		App().DeviceStates().bPurgeValveOpen);
	FindChildByName2<SImageButton>(L"advance_security_valve")->SetCheck(
		App().DeviceStates().bSecurityValveOpen);
	FindChildByName2<SImageButton>(L"advance_washing_pump")->SetCheck(
		App().DeviceStates().bWashingPumpOperation);
	FindChildByName2<SImageButton>(L"advance_return_valve_detergent")->SetCheck(
		App().DeviceStates().bReturnValveDetergentOpen);

}

bool CMainDlg::CheckBtnChange(EventArgs* pEvt)
{
	std::wstring checkName = pEvt->nameFrom;
	if (checkName.empty()) return true;
	std::wstring groupTitle = checkName.substr(0, checkName.find_first_of(L"_"));
	auto beFindItem = mapCheckBtn_.find(groupTitle);
	if (beFindItem != mapCheckBtn_.end())
	{
		auto checkListIter = beFindItem->second.begin();
		for (auto checkListIter = beFindItem->second.begin(); checkListIter != beFindItem->second.end(); ++checkListIter)
		{
			bool bShow = false;
			if (checkListIter->first == checkName) bShow = true;
			checkListIter->second(bShow);
		}
	}
	return true;
}

bool CMainDlg::CheckedLine(EventArgs* pEvt)
{
	if (pEvt->sender == nullptr) return false;
	SCheckBoxEx* obj = (SCheckBoxEx*)pEvt->sender;
	std::wstring strName = pEvt->nameFrom;
	strName = strName.substr(strName.find_first_of(L"_") + 1);
	FindChildByName2<SChartLine>("chart_line")
		->SetLineShow(strName.c_str(), obj->IsChecked());
	return true;
}

bool CMainDlg::CheckedAdvanceReal(EventArgs* pEvt)
{
	if (pEvt->sender == nullptr) return false;
	SCheckBoxEx* obj = (SCheckBoxEx*)pEvt->sender;
	FindChildByName2<SChartReal>("advance_chartreal")->SetLineShow(pEvt->nameFrom, obj->IsChecked());
	return true;
}

bool CMainDlg::ComboViewChange(EventArgs* pEvt)
{
	if (!m_bLayoutInited) return false;
	SStringT cbvName = pEvt->nameFrom;
	SStringT itemName = cbvName.Mid(cbvName.Find(L"_") + 1);
	if (itemName == L"chart_time")
	{
		updateMainValue({ main_chart_time });
		return true;
	}
	if (itemName == L"type")
	{
		updateMainValue({ main_speed });
		return true;
	}
	if (itemName == L"model")
	{
		updateMainValue({ main_model });
		return true;
	}
	if (itemName == L"flowmeter_choose")
	{
		SComboView* obj = (SComboView*)pEvt->sender;
		std::string val = S_CW2A(obj->GetWindowTextW());
		App().SetItemConfig("Parameter", "flowmeter_choose", val);
		return true;
	}
	if (itemName == L"calcmodel")
	{
		SComboView* obj = (SComboView*)pEvt->sender;
		std::string val = S_CW2A(obj->GetWindowTextW());
		App().SetItemConfig("Calculation", "calcmodel", val);
		return true;
	}
	return true;
}

void CMainDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	SetMsgHandled(FALSE);
	auto item = FindChildByName2<SWindow>("system_menu");
	auto checkBtn = FindChildByName2<SCheckBoxEx>("system_setup");
	auto rect1 = item->GetWindowRect();
	auto rect2 = checkBtn->GetWindowRect();
	if (!rect1.PtInRect(point) && !rect2.PtInRect(point))
	{
		checkBtn->SetCheck(FALSE);
		item->SetAttribute(L"show", L"0");
	}
}

void CMainDlg::KeyEditUpdate(std::string title, std::string item)
{
	std::string val = S_CW2A(FindChildByName(item.c_str())->GetWindowTextW(), CP_UTF8).GetBuffer(0);
	if (val.empty()) val = "0";
	if (title == "oplog")
	{
		size_t page = std::stoi(val);
		if (page > nOplogsTotalPage_ || page <= 0) return;
		nOplogsPage_ = page;
		GetPageOplogs();
		return;
	}
	if (title == "datalog")
	{
		size_t page = std::stoi(val);
		if (page > nDatalogsTotalPage_ || page <= 0) return;
		nDatalogsPage_ = page;
		GetDatalogsPage();
		return;
	}
	if (title == "sensors_page")
	{
		std::string target = item.substr(0, item.find_first_of("_"));
		GetSensorsPage(S_CA2W(target.c_str()));
		return;
	}
	std::string target = item.substr(0, item.find_first_of("_"));
	std::string tail = item.substr(item.find_first_of("_") + 1);
	std::list<MainValueType> lvType;
	if (title == "MainPageConfig")
	{
		if (target == "chart")
		{
			lvType.push_back(main_chart_time);
		}
		if (target == "pv")
		{
			if (curRealModel == L"Power Law") item = "npl_" + tail;
			if (curRealModel == L"HB") item = "nhb_" + tail;
			lvType.push_back(main_model);
		}
		if (target == "yp")
		{
			if (curRealModel == L"Power Law") item = "kpl_" + tail;
			if (curRealModel == L"HB") item = "khb_" + tail;
			lvType.push_back(main_model);
		}
		if (target == "cl")
		{
			if (App().GetFluidInfo().type_ != L"WBM") item = "es_" + tail;
			lvType.push_back(main_datasource);
		}
	}
	else if (title == "Sensors")
	{

		MysqlClient::InsertSensors(tail, std::stof(val));
		lvType.push_back(main_datasource);
	}
	else if (title == "Other")
	{
		CleanDateUpdate();
	}
	App().SetItemConfig(title, item, val);
	updateMainValue(lvType);
}

void CMainDlg::RealBlockValSet(const std::list<SStringT>& lvTarget)
{
	for (auto it : lvTarget)
	{
		float fVal = 0;
		if (it == L"temp")
			fVal = App().GetCollection().fTemperature;
		else if (it == L"density")
			fVal = App().GetCollection().fDensity;
		else if (it == L"pv")
			fVal = App().GetRecord().pv;
		else if (it == L"yp")
			fVal = App().GetRecord().yp;
		else if (it == L"npl")
			fVal = App().GetRecord().npl, it = L"pv";
		else if (it == L"kpl")
			fVal = App().GetRecord().kpl, it = L"yp";
		else if (it == L"nhb")
			fVal = App().GetRecord().nhb, it = L"pv";
		else if (it == L"khb")
			fVal = App().GetRecord().khb, it = L"yp";
		else if (it == L"cl")
		{
			MysqlClient::GetLastSensors("Cl-", fVal);
		}
		else if (it == L"ph")
		{
			MysqlClient::GetLastSensors("PH", fVal);
		}
		else if (it == L"es")
		{
			it = L"cl";
			MysqlClient::GetLastSensors("ES", fVal);
		}
		SStringT unit = FindChildByName2<SImageButton>(SStringT().Format(L"btn_%s_units", it))->GetWindowTextW();
		if (unit == L"°F") fVal = fVal * 1.8f + 32.0f;
		else if (unit == L"K") fVal = fVal + 273.15f;
		else if (unit == L"g/cm³" || unit == L"sg") fVal = fVal / 1000.0f;
		else if (unit == L"ppg") fVal = fVal / 1000.0f * 8.345f;
		else if (unit == L"lb/100ft²") fVal = fVal * 2.08854f;
		else if (unit == L"lb·sⁿ/100ft²") fVal = fVal / 0.4788026f;

		SStringT strAlertUp = FindChildByName2<SKeyEdit>(SStringT().Format(L"%s_alert_up", it))->GetWindowTextW();
		if (strAlertUp.IsEmpty() || strAlertUp == L"-") continue;
		float fAlertUp = std::stof(strAlertUp.GetBuffer(0));
		SStringT strAlertDown = FindChildByName2<SKeyEdit>(SStringT().Format(L"%s_alert_down", it))->GetWindowTextW();
		if (strAlertDown.IsEmpty() || strAlertDown == L"-") continue;
		float fAlertDown = std::stof(strAlertDown.GetBuffer(0));
		auto mutiObj = FindChildByName2<SMutiImage>(SStringT().Format(L"muti_%s", it));
		mutiObj->SetCurCheck(0);
		if (fAlertUp > fAlertDown)
		{
			if (fVal >= fAlertUp)
			{
				mutiObj->SetCurCheck(2);
			}
			if (fVal <= fAlertDown)
			{
				mutiObj->SetCurCheck(1);
			}
		}
		if (it == L"pv" || it == L"yp" || it == L"cl" || it == L"ph")
		{
			mutiObj->SetWindowTextW(SStringT().Format(L"%.2f", fVal));
		}
		else
		{
			mutiObj->SetWindowTextW(SStringT().Format(L"%.1f", fVal));
		}
	}
}

bool CMainDlg::BtnUnits(EventArgs* pEvt)
{
	if (pEvt->nameFrom == nullptr) return false;
	SImageButton* obj = (SImageButton*)pEvt->sender;
	std::wstring target = pEvt->nameFrom;
	target = target.substr(target.find_first_of(L"_") + 1);
	SStringT unit = obj->GetWindowTextW();
	if (target == L"temp_units")
	{
		auto iter = std::find(vUnitTemp.begin(), vUnitTemp.end(), unit);
		iter++;
		if (iter == vUnitTemp.end())
		{
			iter = vUnitTemp.begin();
		}
		obj->SetWindowTextW(*iter);
	}
	if (target == L"density_units")
	{
		auto obj = FindChildByName2<SImageButton>("btn_density_units");
		SStringT unit = obj->GetWindowTextW();
		auto iter = std::find(vUnitDensity.begin(), vUnitDensity.end(), unit);
		iter++;
		if (iter == vUnitDensity.end())
		{
			iter = vUnitDensity.begin();
		}
		obj->SetWindowTextW(*iter);
	}
	if (target == L"pv_units")
	{
		auto obj = FindChildByName2<SImageButton>("btn_pv_units");
		SStringT unit = obj->GetWindowTextW();
		if (unit.IsEmpty()) return false;
		auto iter = std::find(vUnitPV.begin(), vUnitPV.end(), unit);
		iter++;
		if (iter == vUnitPV.end())
		{
			iter = vUnitPV.begin();
		}
		obj->SetWindowTextW(*iter);
		updateMainValue({ main_model });
	}
	if (target == L"yp_units")
	{
		auto obj = FindChildByName2<SImageButton>("btn_yp_units");
		SStringT unit = obj->GetWindowTextW();
		if (unit.IsEmpty()) return false;
		auto iter = std::find(vUnitYP.begin(), vUnitYP.end(), unit);
		if (iter != vUnitYP.end())
		{
			iter++;
			if (iter == vUnitYP.end())
			{
				iter = vUnitYP.begin();
			}
		}
		else
		{
			iter = std::find(vUnitK.begin(), vUnitK.end(), unit);
			iter++;
			if (iter == vUnitK.end())
			{
				iter = vUnitK.begin();
			}
		}
		obj->SetWindowTextW(*iter);
		updateMainValue({ main_model });
	}
	return true;
}

bool CMainDlg::AdvanceBtn(EventArgs* pEvt)
{
	if (pEvt->nameFrom == nullptr) return false;
	SStringT strName = pEvt->nameFrom;
	if (strName == L"advance_debug" && curWndState != WND_STATE_IDLE)
	{
		App().ShowMessage(L"Error", L"can not checked debug mode");
		return false;
	}
	if (strName != L"advance_debug" && !FindChildByName2<SImageButton>(L"advance_debug")->IsChecked())
	{
		App().ShowMessage(L"Error", L"not debug mode");
		return false;
	}
	SImageButton* obj = (SImageButton*)pEvt->sender;
	auto iter = mapAdvanceBtnCmd_.find(strName);
	App().DeviceTodo(
		obj->IsChecked() ? iter->second : DeviceIns(iter->second - 1));
	return true;
}

void CMainDlg::CheckLog()
{
	App().SetAdvanceDebug(FindChildByName2<SCheckBoxEx>("advance_log")->IsChecked());
}


void CMainDlg::BtnStart()
{

	if (App().DeviceStates().bSignManual)
	{
		App().ShowMessage(L"Error", L"manual type");
		return;
	}
	SStringT strTitle = FindChildByName2<SImageButton>("btn_start")->GetWindowTextW();
	if (curWndState != WND_STATE_IDLE && L"START" == strTitle)
	{
		App().ShowMessage(L"Error", L"not idle");
		return;
	}

	bool bTest = true;
	if (strTitle == L"PAUSE")
	{
		bTest = false;
	}
	MysqlClient::InsertOperation(bTest ? "START TEST" : "PAUSE TEST");
	LOGINFO(bTest ? "START TEST" : "PAUSE TEST");
	App().DeviceTodo(bTest ? DeviceIns::test_start : DeviceIns::test_pause);
}

void CMainDlg::BtnClean()
{
	if (App().DeviceStates().bSignManual)
	{
		App().ShowMessage(L"Error", L"manual type");
		return;
	}
	if (curWndState == WND_STATE_IDLE)
	{
		App().ShowMessage(L"Clean", L"Confirm cleaning procedures?", [] {
			MysqlClient::InsertOperation("System Clean");
			LOGINFO("System Clean");
			App().DeviceTodo(DeviceIns::auto_clean);
			});
	}
	else
	{
		App().ShowMessage(L"Error", L"not idle");
	}
}

void CMainDlg::BtnStop()
{
	if (App().DeviceStates().bSignManual)
	{
		App().ShowMessage(L"Error", L"manual type");
		return;
	}

	App().ShowMessage(L"Stop", L"Click OK to Stop System!", [] {
		MysqlClient::InsertOperation("System Stop");
		LOGINFO("System Stop");
		App().DeviceTodo(DeviceIns::soft_stop);
		});
}

void CMainDlg::OnSnapshot()
{
	int dt_nWidth = GetSystemMetrics(SM_CXSCREEN);
	int dt_nHeight = GetSystemMetrics(SM_CYSCREEN);
	CRect rc = GetClientRect();
	ClientToScreen(rc);
	if (rc.left < 0)
	{
		rc.left = 0;
	}
	if (rc.top < 0)
	{
		rc.top = 0;
	}
	if (rc.right > dt_nWidth)
	{
		rc.right = dt_nWidth;
	}
	if (rc.bottom > dt_nHeight)
	{
		rc.bottom = dt_nHeight;
	}
	int nWidth = rc.Width();
	int nHeight = rc.Height();
	HDC hDesktopDC = ::GetDC(NULL);
	HDC hBmpFileDC = CreateCompatibleDC(hDesktopDC);
	HBITMAP hBmpFileBitmap = CreateCompatibleBitmap(hDesktopDC, nWidth, nHeight);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBmpFileDC, hBmpFileBitmap);
	BitBlt(hBmpFileDC, 0, 0, nWidth, nHeight, hDesktopDC, rc.left, rc.top, SRCCOPY | CAPTUREBLT);
	SelectObject(hBmpFileDC, hOldBitmap);
	HDC hdc = NULL;
	FILE* fp = NULL;
	LPVOID pBuf = NULL;
	BITMAPINFO bmpInfo;
	BITMAPFILEHEADER bmpFileHeader;
	hdc = GetDC();
	ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	GetDIBits(hdc, hBmpFileBitmap, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS); //获取bmp信息 

	if (bmpInfo.bmiHeader.biSizeImage <= 0)
		bmpInfo.bmiHeader.biSizeImage = bmpInfo.bmiHeader.biWidth * abs(bmpInfo.bmiHeader.biHeight) * (bmpInfo.bmiHeader.biBitCount + 7) / 8;

	if ((pBuf = malloc(bmpInfo.bmiHeader.biSizeImage)) == NULL)
	{
		return;
		//MessageBox(NULL, _T("Unable to Allocate Bitmap Memory"), _T("Error"), MB_OK | MB_ICONERROR);
	}

	bmpInfo.bmiHeader.biCompression = BI_RGB;
	GetDIBits(hdc, hBmpFileBitmap, 0, bmpInfo.bmiHeader.biHeight, pBuf, &bmpInfo, DIB_RGB_COLORS);



	SStringT timeTemp = S_CA2W(Utils::GetDateTime(true).c_str());
	timeTemp.Replace(L":", L"_");
	timeTemp.Replace(L" ", L"-");
	timeTemp.Replace(L"/", L"_");
	timeTemp.Replace(L".", L"_");
	Json::Value jsConfig = App().GetItemConfig(STRING_RUNTIME);
	SStringT filename = S_CA2W(jsConfig[STRING_SAVEFILE].asString().c_str(), CP_UTF8)
		+ L"\\" + S_CA2W(jsConfig[STRING_WELLNAME].asString().c_str(), CP_UTF8) + L"\\Snapshot\\" + timeTemp + L".png";

	HANDLE hFile = CreateFile(filename.GetBuffer(0),                // name of the write
		GENERIC_WRITE,          // open for writing
		0,                      // do not share
		NULL,                   // default security
		CREATE_ALWAYS,          // overwrite existing
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);                  // no attr. template
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpInfo.bmiHeader.biSizeImage;
	bmpFileHeader.bfType = 'MB';
	bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	DWORD dwWriten;
	WriteFile(hFile, &bmpFileHeader, sizeof(BITMAPFILEHEADER), &dwWriten, NULL);
	WriteFile(hFile, &bmpInfo.bmiHeader, sizeof(BITMAPINFOHEADER), &dwWriten, NULL);
	WriteFile(hFile, pBuf, bmpInfo.bmiHeader.biSizeImage, &dwWriten, NULL);

	CloseHandle(hFile);
	free(pBuf);
}

void CMainDlg::DatalogExport()
{
	SStringT timeStart = FindChildByName2<SDateTimePickerEx>("datalog_timestart")->GetWindowTextW();
	SStringT timeEnd = FindChildByName2<SDateTimePickerEx>("datalog_timeend")->GetWindowTextW();
	App().AsyncTask([timeStart, timeEnd] {
		static bool bExport = false;
		if (bExport) return;
		bExport = true;
		std::vector<RecordData> lvRecords;
		std::vector<CommentsData> lvComments;
		std::vector<SensorsRecord> lvPH, lvCl, lvES;
		MysqlClient::GetRecordsInTime(lvRecords, S_CW2A(timeStart).GetBuffer(0), S_CW2A(timeEnd).GetBuffer(0));
		MysqlClient::GetCommentsInTime(lvComments, S_CW2A(timeStart).GetBuffer(0), S_CW2A(timeEnd).GetBuffer(0));
		MysqlClient::GetSensorsRecordInTime("PH", lvPH, S_CW2A(timeStart).GetBuffer(0), S_CW2A(timeEnd).GetBuffer(0));
		MysqlClient::GetSensorsRecordInTime("Cl-", lvCl, S_CW2A(timeStart).GetBuffer(0), S_CW2A(timeEnd).GetBuffer(0));
		MysqlClient::GetSensorsRecordInTime("ES", lvES, S_CW2A(timeStart).GetBuffer(0), S_CW2A(timeEnd).GetBuffer(0));
		SStringA timeTemp = Utils::GetDateTime(true).c_str();
		timeTemp.Replace(":", "_");
		timeTemp.Replace(" ", "-");
		timeTemp.Replace("/", "_");
		timeTemp.Replace(".", "_");
		Json::Value jsConfig = App().GetItemConfig(STRING_RUNTIME);
		std::string filename = jsConfig[STRING_SAVEFILE].asString()
			+ "\\" + jsConfig[STRING_WELLNAME].asString() + "\\Excel\\" + timeTemp.GetBuffer(0) + ".xlsx";
		xlnt::workbook wb;
		int sheetNumber = 1;
		int recordIndex = 0;
		wb.remove_sheet(wb.active_sheet());
		while (1)
		{
			xlnt::worksheet ws = wb.create_sheet();
			ws.title(std::to_string(sheetNumber));
			ws.cell(1, 1).value("time");
			ws.cell(2, 1).value("max_temp(C)");
			ws.cell(3, 1).value("min_temp(C)");
			ws.cell(4, 1).value("avg_temp(C)");
			ws.cell(5, 1).value("r600");
			ws.cell(6, 1).value("r300");
			ws.cell(7, 1).value("r200");
			ws.cell(8, 1).value("r100");
			ws.cell(9, 1).value("r60");
			ws.cell(10, 1).value("r30");
			ws.cell(11, 1).value("r20");
			ws.cell(12, 1).value("r10");
			ws.cell(13, 1).value("r6");
			ws.cell(14, 1).value("r3");
			ws.cell(15, 1).value("r0");
			ws.cell(16, 1).value("max_density(kg/m^3)");
			ws.cell(17, 1).value("min_density(kg/m^3)");
			ws.cell(18, 1).value("avg_density(kg/m^3)");
			ws.cell(19, 1).value("av(mPas)");
			ws.cell(20, 1).value("pv(mPas)");
			ws.cell(21, 1).value("yp(Pa)");
			ws.cell(22, 1).value("npl");
			ws.cell(23, 1).value("kpl(Pas^n)");
			ws.cell(24, 1).value("nhb");
			ws.cell(25, 1).value("khb(Pas^n)");
			ws.cell(26, 1).value("tau0(Pa)");

			int i = 2;
			while ((i < 1002) && (recordIndex < lvRecords.size()))
			{
				ws.cell(1, i).value(lvRecords[recordIndex].time);
				ws.cell(2, i).value(lvRecords[recordIndex].max_temp);
				ws.cell(3, i).value(lvRecords[recordIndex].min_temp);
				ws.cell(4, i).value(lvRecords[recordIndex].avg_temp);
				ws.cell(5, i).value(lvRecords[recordIndex].r600);
				ws.cell(6, i).value(lvRecords[recordIndex].r300);
				ws.cell(7, i).value(lvRecords[recordIndex].r200);
				ws.cell(8, i).value(lvRecords[recordIndex].r100);
				ws.cell(9, i).value(lvRecords[recordIndex].r60);
				ws.cell(10, i).value(lvRecords[recordIndex].r30);
				ws.cell(11, i).value(lvRecords[recordIndex].r20);
				ws.cell(12, i).value(lvRecords[recordIndex].r10);
				ws.cell(13, i).value(lvRecords[recordIndex].r6);
				ws.cell(14, i).value(lvRecords[recordIndex].r3);
				ws.cell(15, i).value(lvRecords[recordIndex].r0);
				ws.cell(16, i).value(lvRecords[recordIndex].max_density);
				ws.cell(17, i).value(lvRecords[recordIndex].min_density);
				ws.cell(18, i).value(lvRecords[recordIndex].avg_density);
				ws.cell(19, i).value(lvRecords[recordIndex].av);
				ws.cell(20, i).value(lvRecords[recordIndex].pv);
				ws.cell(21, i).value(lvRecords[recordIndex].yp);
				ws.cell(22, i).value(lvRecords[recordIndex].npl);
				ws.cell(23, i).value(lvRecords[recordIndex].kpl);
				ws.cell(24, i).value(lvRecords[recordIndex].nhb);
				ws.cell(25, i).value(lvRecords[recordIndex].khb);
				ws.cell(26, i).value(lvRecords[recordIndex].tau0);
				recordIndex++;
				i++;
			}
			if (recordIndex == lvRecords.size()) break;
			sheetNumber++;
		}
		if (lvComments.size())
		{
			xlnt::worksheet ws = wb.create_sheet();
			ws.title("Comments");
			ws.cell(1, 1).value("time");
			ws.cell(2, 1).value("comments");
			for (int i = 0; i < lvComments.size(); i++)
			{
				ws.cell(1, i + 2).value(lvComments[i].time);
				ws.cell(2, i + 2).value(lvComments[i].data);
			}
		}
		if (lvPH.size() || lvCl.size() || lvES.size())
		{
			xlnt::worksheet ws = wb.create_sheet();
			ws.title("Sensors");
			ws.cell(1, 1).value("time");
			ws.cell(2, 1).value("PH");
			ws.cell(5, 1).value("time");
			ws.cell(6, 1).value("Cl-");
			ws.cell(9, 1).value("time");
			ws.cell(10, 1).value("ES");
			for (int i = 0; i < lvPH.size(); i++)
			{
				ws.cell(1, i + 2).value(lvPH[i].time);
				ws.cell(2, i + 2).value(lvPH[i].value);
			}
			for (int i = 0; i < lvCl.size(); i++)
			{
				ws.cell(5, i + 2).value(lvCl[i].time);
				ws.cell(6, i + 2).value(lvCl[i].value);
			}
			for (int i = 0; i < lvES.size(); i++)
			{
				ws.cell(9, i + 2).value(lvES[i].time);
				ws.cell(10, i + 2).value(lvES[i].value);
			}
		}
		wb.save(filename);
		bExport = false;
		});
}

void CMainDlg::AdvanceRealData()
{
	if (!FindChildByName2<SChartReal>("advance_chartreal")->isHistory())
		FindChildByName2<SChartReal>("advance_chartreal")->AddData(App().GetAdvanceReal());
}

void CMainDlg::AdvanceRealText()
{
	SStringT strEV0_n = SStringT().Format(L"%.1f~%.1f",
		App().GetCollection().fEV0, App().GetCollection().fEVn);
	FindChildByName2<SStatic>(L"advance_ev0n")->SetWindowTextW(strEV0_n);
	FindChildByName2<SStatic>(L"advance_p1")->SetWindowTextW(SStringT().Format(L"%.3f",
		App().GetCollection().fPumpPressure));
	FindChildByName2<SStatic>(L"advance_p2")->SetWindowTextW(SStringT().Format(L"%.3f",
		App().GetCollection().fUpstreamPressure));
	FindChildByName2<SStatic>(L"advance_p3")->SetWindowTextW(SStringT().Format(L"%.3f",
		App().GetCollection().fDownstreamPressure));
	FindChildByName2<SStatic>(L"advance_p4")->SetWindowTextW(SStringT().Format(L"%.3f",
		App().GetCollection().fOutPressure));
	FindChildByName2<SStatic>(L"advance_dfp1")->SetWindowTextW(SStringT().Format(L"%.3f",
		App().GetCollection().fDiffPressure));
	FindChildByName2<SStatic>(L"advance_dfp2")->SetWindowTextW(SStringT().Format(L"%.3f",
		App().GetCollection().fDiffPressureStr > 0? App().GetCollection().fDiffPressureStr: 0));

	FindChildByName2<SStatic>(L"sample_index")->SetWindowTextW(SStringT().Format(L"%d",
		App().GetCollection().nSampleIndex));

	FindChildByName2<SStatic>(L"advance_ev")->SetWindowTextW(SStringT().Format(L"%.2f",
		App().GetCollection().fOpening));
	FindChildByName2<SStatic>(L"advance_density")->SetWindowTextW(SStringT().Format(L"%.1f",
		App().GetCollection().fDensity));
	FindChildByName2<SStatic>(L"advance_temp")->SetWindowTextW(SStringT().Format(L"%.1f",
		App().GetCollection().fTemperature));
	FindChildByName2<SStatic>(L"advance_vflow")->SetWindowTextW(SStringT().Format(L"%.2f",
		App().GetCollection().fVolFlow));
	FindChildByName2<SStatic>(L"advance_mflow")->SetWindowTextW(SStringT().Format(L"%.2f",
		App().GetCollection().fMassFlow));
	FindChildByName2<SStatic>(L"advance_mudlevel")->SetWindowTextW(SStringT().Format(L"%.3f",
		App().GetCollection().fMudLevel / 1000.0));

	AdvanceBtnChecked();
	pDiagDlg_->SetTrip(App().DeviceStates().bCirculatingPumpTrip);
}

void CMainDlg::AdvanceBtnSearch()
{
	std::string startTime = S_CW2A(FindChildByName2<SDateTimePickerEx>("advance_starttime")->GetWindowTextW());
	AdvanceData data;
	MysqlClient::GetAdvance(data, startTime);
	if (data.vTime.size())
	{
		FindChildByName2<SChartReal>("advance_chartreal")->SetHistory(true);
		FindChildByName2<SChartReal>("advance_chartreal")->AddData(data);
	}
}


void CMainDlg::CleanDateUpdate()
{
	auto pEnableClean = FindChildByName2<SCheckBoxEx>("enable_cyclic_clean");
	auto pText = FindChildByName2<SStatic>("next_clean_time");
	if (pEnableClean->IsChecked())
	{
		std::string time_start = S_CW2A(FindChildByName2<SDateTimePickerEx>("clean_start_time")->GetWindowTextW());
		App().SetItemConfig("Other", "clean_start_time", time_start);
		float time_interval = std::stof(S_CW2A(FindChildByName2<SKeyEdit>("clean_interval")->GetWindowTextW()).GetBuffer(0));
		if (time_interval <= 0) return;
		float interval = time_interval * (24 * 60 * 60);
		std::string time = Utils::GetDateTimeFrom(time_start, (long)interval);
		uint32_t i = 1;
		while (time <= Utils::GetDateTime())
		{
			i++;
			interval = i * time_interval * (24 * 60 * 60);
			time = Utils::GetDateTimeFrom(time_start, (long)interval);
		}
		App().SetItemConfig("Other", "clean_end_time", time);
		pText->SetWindowTextW(S_CA2W(time.c_str()));
	}
	else
	{
		pText->SetWindowTextW(L"");
	}
}

void CMainDlg::EnableClean()
{
	int checked = FindChildByName2<SCheckBoxEx>("enable_cyclic_clean")->IsChecked();
	App().SetItemConfig("Other", "enable_cyclic_clean", std::to_string(checked));
	CleanDateUpdate();
}

void CMainDlg::CommentsEsc()
{
	FindChildByName2<SChartLogs>("chart_datalog")->SetCommentsEdit(false);
	FindChildByName2<SDateTimePickerEx>("datalog_comment_date")->EnableWindow(FALSE);
	FindChildByName2<SDateTimePickerEx>("datalog_comment_date")->SetAttribute(L"colorBkgnd", L"#F8F8F800");
}

void CMainDlg::CommentsUpdate(std::string title, std::string item)
{
	FindChildByName2<SChartLogs>("chart_datalog")->SetCommentsEdit(false);
	FindChildByName2<SDateTimePickerEx>("datalog_comment_date")->EnableWindow(FALSE);
	FindChildByName2<SDateTimePickerEx>("datalog_comment_date")->SetAttribute(L"colorBkgnd", L"#F8F8F800");
	std::string val = S_CW2A(FindChildByName(item.c_str())->GetWindowTextW(), CP_UTF8).GetBuffer(0);
	if (val.empty()) return;
	CommentsData data;
	data.data = val;
	data.time = S_CW2A(FindChildByName2<SDateTimePickerEx>("datalog_comment_date")->GetWindowTextW());
	MysqlClient::InsertComments(data);
	std::vector<CommentsData> lvComments;
	SStringT timeStart = FindChildByName2<SDateTimePickerEx>("datalog_timestart")->GetWindowTextW();
	SStringT timeEnd = FindChildByName2<SDateTimePickerEx>("datalog_timeend")->GetWindowTextW();
	MysqlClient::GetCommentsInTime(lvComments, S_CW2A(timeStart).GetBuffer(0), S_CW2A(timeEnd).GetBuffer(0));
	auto pChar = FindChildByName2<SChartLogs>("chart_datalog");
	pChar->AddComment(lvComments);
	pChar->UpdateData();
}

void CMainDlg::CommentsCallback(bool bEdit, std::string data, std::string time)
{
	if (!bEdit)
	{
		FindChildByName2<SKeyEdit>("datalog_comments")->SetWindowTextW(S_CA2W(data.c_str(), CP_UTF8));
		if (!time.empty())
			FindChildByName2<SDateTimePickerEx>("datalog_comment_date")->SetWindowTextW(S_CA2W(time.c_str(), CP_UTF8));
	}
	else
	{
		FindChildByName2<SDateTimePickerEx>("datalog_comment_date")->SetWindowTextW(S_CA2W(time.c_str(), CP_UTF8));
	}
}

void CMainDlg::CommentsDBClick()
{
	FindChildByName2<SChartLogs>("chart_datalog")->SetCommentsEdit(true);
	FindChildByName2<SDateTimePickerEx>("datalog_comment_date")->EnableWindow(TRUE);
	FindChildByName2<SDateTimePickerEx>("datalog_comment_date")->SetAttribute(L"colorBkgnd", L"#F8F8F8");
}

