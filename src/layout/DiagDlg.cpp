#include "stdafx.h"
#include "DiagDlg.h"
#include "SCheckBoxEx.h"
#include "SKeyEdit.h"
#include "SInvertCheck.h"
#include "SMutiImage.h"
#include "AppFramework.h"
using namespace realology;
DiagDlg::DiagDlg() : SHostWnd(_T("LAYOUT:xml_diag"))
{
	m_bLayoutInited = FALSE;
	mapLightsTarget_ = { {hardware,"hardware_lights"},{alarm,"alarm_lights"},{warning,"warning_lights"} ,{comm,"comm_lights"} };
}

DiagDlg::~DiagDlg()
{
}



int DiagDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetMsgHandled(FALSE);
	return 0;
}

BOOL DiagDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;

	FindChildByName2<SListView>("lv_info")->SetAdapter(pDiagAdapter_);
	pDiagAdapter_->Release();
	mapCheckBtn = {
		{L"check_hardware",std::bind(&DiagDlg::OnBtnHardware,this,std::placeholders::_1)},
		{L"check_alarm",std::bind(&DiagDlg::OnBtnAlarm,this,std::placeholders::_1) },
		{L"check_warning", std::bind(&DiagDlg::OnBtnWarning, this, std::placeholders::_1)},
		{L"check_comm", std::bind(&DiagDlg::OnBtnComm, this, std::placeholders::_1)}
	};


	FindChildByName2<SWindow>(L"check_hardware")->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&DiagDlg::CheckBtnChange, this));
	FindChildByName2<SWindow>(L"check_alarm")->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&DiagDlg::CheckBtnChange, this));
	FindChildByName2<SWindow>(L"check_warning")->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&DiagDlg::CheckBtnChange, this));
	FindChildByName2<SWindow>(L"check_comm")->GetEventSet()->subscribeEvent(EVT_LBUTTONDOWN, Subscriber(&DiagDlg::CheckBtnChange, this));

	FindChildByName2<SKeyEdit>("page")->SetCallBack(std::bind(&DiagDlg::KeyEditUpdate, this, std::placeholders::_1, std::placeholders::_2), "", "page");
	OnBtnHardware(true);
	return 0;
}

void DiagDlg::OnClose()
{
	ShowWindow(SW_HIDE);
	curTarget_ = base;
	App().AsyncEvent(WM_WNDSTATE_UPDATE, WND_STATE_DEF);
}

void DiagDlg::OnBtnCode()
{
	auto obj = FindChildByName2<SInvertCheck>("btn_code");
	obj->SetCheck(TRUE);
	FindChildByName2<SInvertCheck>("btn_time")->SetCheck(FALSE);
	FindChildByName2<SInvertCheck>("btn_time")->SetCurCheck(0);
	obj->SetCurCheck(obj->GetCurState() == 2 ? 3 : 2);
	sqlKey_ = "code";
	sqlOrder_ = (obj->GetCurState() == 2) ? "desc" : "";
	GetPageInfo();
}

void DiagDlg::OnBtnTime()
{
	auto obj = FindChildByName2<SInvertCheck>("btn_time");
	obj->SetCheck(TRUE);
	FindChildByName2<SInvertCheck>("btn_code")->SetCheck(FALSE);
	FindChildByName2<SInvertCheck>("btn_code")->SetCurCheck(0);
	obj->SetCurCheck(obj->GetCurState() == 2 ? 3 : 2);
	sqlKey_ = "id";
	sqlOrder_ = (obj->GetCurState() == 2) ? "desc" : "";
	GetPageInfo();
}

void DiagDlg::OnBtnHardware(bool bShow)
{
	FindChildByName2<SWindow>("check_hardware")->SetCheck(bShow);
	if (!bShow) return;
	curTarget_ = hardware;
	dataType_ = "Hardware";
	nPage_ = 1;
	FindChildByName2<SInvertCheck>("btn_time")->SetCurCheck(0);
	OnBtnTime();
}

void DiagDlg::OnBtnAlarm(bool bShow)
{
	FindChildByName2<SWindow>("check_alarm")->SetCheck(bShow);
	if (!bShow) return;
	curTarget_ = alarm;
	dataType_ = "Process Fault";
	nPage_ = 1;
	FindChildByName2<SInvertCheck>("btn_time")->SetCurCheck(0);
	OnBtnTime();
}

void DiagDlg::OnBtnWarning(bool bShow)
{
	FindChildByName2<SWindow>("check_warning")->SetCheck(bShow);
	if (!bShow) return;
	curTarget_ = warning;
	dataType_ = "Process Warning";
	nPage_ = 1;
	FindChildByName2<SInvertCheck>("btn_time")->SetCurCheck(0);
	OnBtnTime();
}

void DiagDlg::OnBtnComm(bool bShow)
{
	FindChildByName2<SWindow>("check_comm")->SetCheck(bShow);
	if (!bShow) return;
	curTarget_ = comm;
	dataType_ = "Communication";
	nPage_ = 1;
	FindChildByName2<SInvertCheck>("btn_time")->SetCurCheck(0);
	OnBtnTime();
}


void DiagDlg::KeyEditUpdate(std::string title, std::string item)
{
	SStringT text = FindChildByName2<SKeyEdit>(std::string("page").c_str())->GetWindowTextW();
	size_t row = std::stoull(S_CW2A(text).GetBuffer(0));
	if (row <= 0 || row > nTotalPage_)
	{
		return;
	}
	nPage_ = row;
	GetPageInfo();
}

void DiagDlg::ShowDlg()
{
	//UpdateDisplayData();
}

bool DiagDlg::CheckBtnChange(EventArgs* pEvt)
{
	SStringT checkName = pEvt->nameFrom;
	if (checkName.IsEmpty()) return true;

	for (auto checkListIter = mapCheckBtn.begin(); checkListIter != mapCheckBtn.end(); ++checkListIter)
	{
		bool bShow = false;
		if (checkListIter->first == checkName) bShow = true;
		checkListIter->second(bShow);
	}
	return true;
}

void DiagDlg::GetPageInfo()
{
	std::list<DiagDataItem> lvData;
	size_t total = 0;
	MysqlClient::SelectDiagsCount(total, dataType_);
	MysqlClient::SelectDiags(lvData, dataType_, sqlKey_, sqlOrder_, nPage_);
	nTotalPage_ = total / 10 + ((total % 10) == 0 ? 0 : 1);
	if (total == 0) nPage_ = 1;
	FindChildByName2<SStatic>("text_total")->SetWindowTextW(std::to_wstring(nTotalPage_).c_str());
	FindChildByName2<SKeyEdit>("page")->SetWindowTextW(std::to_wstring(nPage_).c_str());
	pDiagAdapter_->RemoveAll();
	if (lvData.size() > 0)
	{
		for (auto& it : lvData)
		{
			pDiagAdapter_->Add(std::move(it));
		}
	}
	pDiagAdapter_->notifyDataSetChanged();
}

void DiagDlg::OnBtnPrev()
{
	if (nPage_ <= 1) return;
	nPage_ -= 1;
	GetPageInfo();
}

void DiagDlg::OnBtnNext()
{
	if (nPage_ >= nTotalPage_) return;
	nPage_ += 1;
	GetPageInfo();
}

void DiagDlg::OnBtnHome()
{
	nPage_ = 1;
	GetPageInfo();
}

void DiagDlg::OnBtnEnd()
{
	if (nTotalPage_ == 0) nPage_ = 1;
	else nPage_ = nTotalPage_;
	GetPageInfo();
}

void DiagDlg::OnTrip()
{
	App().DeviceTodo(pump_reset);
}

void DiagDlg::SetTrip(bool bEnable)
{
	if (bTripEnable != bEnable)
	{
		bTripEnable = bEnable;
		FindChildByName2<SImageButton>("btn_trip")->EnableWindow(bEnable);
		FindChildByName2<SImageButton>("btn_trip")->Invalidate();
	}
}

void DiagDlg::SetDisplayLights(DiagTarget target, uint8_t nRedLight)
{
	auto iter = mapLightsTarget_.find(target);
	//if (curTarget_ == target && IsWindowVisible()) UpdateDisplayData();
	//else curTarget_ = target;
	FindChildByName2<SMutiImage>(iter->second.c_str())->SetCurCheck(nRedLight);
}





