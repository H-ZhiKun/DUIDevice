#include "stdafx.h"
#include <ShlObj.h>
#include "SystemOptionsDlg.h"
#include "SWinEdit.h"
#include "MysqlClient.h"
#include "ModbusDevice.h"
#include "Logger.h"
#include "AppFramework.h"
using namespace realology;


SystemOptionsDlg::SystemOptionsDlg() : SHostWnd(_T("LAYOUT:xml_system_options"))
{
	m_bLayoutInited = FALSE;
}

SystemOptionsDlg::~SystemOptionsDlg()
{
}

int SystemOptionsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetMsgHandled(FALSE);
	return 0;
}

BOOL SystemOptionsDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	Json::Value jsSysConfig = App().GetItemConfig(STRING_RUNTIME);
	{//comboview well name
		auto obj = FindChildByName2<SComboView>("cbv_name");
		pWellNameAdapter_ = new SComboViewAdapter(obj);
		obj->GetListView()->SetAdapter(pWellNameAdapter_);
		obj->GetEventSet()->subscribeEvent(EVT_CB_SELCHANGE, Subscriber(&SystemOptionsDlg::ComboViewChange, this));
		pWellNameAdapter_->Release();
		std::vector<std::string> vNameList;
		MysqlClient::GetWellNames(vNameList);
		if (vNameList.size() > 0)
		{
			for (auto& it : vNameList)
			{
				pWellNameAdapter_->AddItem(S_CA2T(it.c_str(), CP_UTF8));
			}
		}
		std::string wellName = jsSysConfig[STRING_WELLNAME].asString();
		if (wellName.empty())
			pWellNameAdapter_->SetSel(pWellNameAdapter_->getCount() - 1);
		else 
			pWellNameAdapter_->SetSel(S_CA2T(wellName.c_str(), CP_UTF8));

		MysqlClient::GetWellOptions(curSystemOptionsInfo_);
		//FindChildByName2<SComboView>("cbv_name")->SetWindowTextW(curSystemOptionsInfo_.wellName_);
		FindChildByName2<SDateTimePickerEx>("edit_date")->SetWindowTextW(curSystemOptionsInfo_.date_);
		FindChildByName2<SWinEdit>("edit_location")->SetWindowTextW(curSystemOptionsInfo_.location_);
		FindChildByName2<SWinEdit>("edit_field")->SetWindowTextW(curSystemOptionsInfo_.field_);
		FindChildByName2<SWinEdit>("edit_operator")->SetWindowTextW(curSystemOptionsInfo_.operator_);
		FindChildByName2<SWinEdit>("edit_rig")->SetWindowTextW(curSystemOptionsInfo_.rig_);

	}
	{//comboview well MGT
		auto obj = FindChildByName2<SComboView>("cbv_mgt");
		obj->GetListView()->SetAdapter(pWellNameAdapter_);
	}
	{//comboview Language
		auto obj = FindChildByName2<SComboView>("cbv_lan");
		pLanAdapter_ = new SComboViewAdapter(obj);
		obj->GetListView()->SetAdapter(pLanAdapter_);
		pLanAdapter_->AddItem(L"English");
		pLanAdapter_->AddItem(L"简体中文");
		pLanAdapter_->SetSel(S_CA2T(jsSysConfig[STRING_LANGUAGE].asString().c_str(), CP_UTF8));
		pLanAdapter_->Release();
	}
	{//comboview units
		auto obj = FindChildByName2<SComboView>("cbv_units");
		pUnitsAdapter_ = new SComboViewAdapter(obj);
		obj->GetListView()->SetAdapter(pUnitsAdapter_);
		pUnitsAdapter_->AddItem(L"SI");
		pUnitsAdapter_->AddItem(L"Oilfield");
		pUnitsAdapter_->SetSel(S_CA2T(jsSysConfig[STRING_UNIT].asString().c_str(), CP_UTF8));
		pUnitsAdapter_->Release();
	}
	SStringA savePath = jsSysConfig[STRING_SAVEFILE].asString().c_str();
	if (savePath.IsEmpty())
	{
		savePath = App().GetWorkPath().c_str();
	}
	else
	{
		savePath.Replace("/", "\\");
	}
	SStringT path = S_CA2W(savePath, CP_UTF8);;
	FindChildByName2<SEdit>("save_path")->SetWindowTextW(path);
	return 0;
}

void SystemOptionsDlg::OnClose()
{
	if (curSystemOptionsInfo_.wellName_.IsEmpty())
	{
		App().ShowMessage(L"ERROR", L"Not Choose Well Name");
		return;
	}
	FindChildByName2<SComboView>("cbv_name")->SetWindowTextW(curSystemOptionsInfo_.wellName_);
	FindChildByName2<SDateTimePickerEx>("edit_date")->SetWindowTextW(curSystemOptionsInfo_.date_);
	FindChildByName2<SWinEdit>("edit_location")->SetWindowTextW(curSystemOptionsInfo_.location_);
	FindChildByName2<SWinEdit>("edit_field")->SetWindowTextW(curSystemOptionsInfo_.field_);
	FindChildByName2<SWinEdit>("edit_operator")->SetWindowTextW(curSystemOptionsInfo_.operator_);
	FindChildByName2<SWinEdit>("edit_rig")->SetWindowTextW(curSystemOptionsInfo_.rig_);

	pLanAdapter_->SetSel(curSystemOptionsInfo_.lan_);
	pUnitsAdapter_->SetSel(curSystemOptionsInfo_.units_);
	FindChildByName2<SEdit>("save_path")->SetWindowTextW(curSystemOptionsInfo_.savePath_);

	FindChildByName2<SWinEdit>("edit_name")->SetAttribute(L"show", L"0");
	ShowWindow(SW_HIDE);
	App().AsyncEvent(WM_WNDSTATE_UPDATE, WND_STATE_DEF);
}

void SystemOptionsDlg::OnBtnOK()
{
	auto editNameObj = FindChildByName2<SWinEdit>("edit_name");
	if (editNameObj->IsVisible() && !editNameObj->GetWindowTextW().IsEmpty())
	{
		pWellNameAdapter_->AddItem(editNameObj->GetWindowTextW());
		pWellNameAdapter_->SetSel(editNameObj->GetWindowTextW());
		editNameObj->SetAttribute(L"show", L"0");
	}
	curSystemOptionsInfo_.wellName_ = FindChildByName2<SComboView>("cbv_name")->GetWindowTextW();
	if (curSystemOptionsInfo_.wellName_.IsEmpty())
	{
		App().ShowMessage(L"ERROR", L"Not Choose Well Name");
		return;
	}
	curSystemOptionsInfo_.date_ = FindChildByName2<SDateTimePickerEx>("edit_date")->GetWindowTextW();
	curSystemOptionsInfo_.location_ = FindChildByName2<SWinEdit>("edit_location")->GetWindowTextW();
	curSystemOptionsInfo_.field_ = FindChildByName2<SWinEdit>("edit_field")->GetWindowTextW();
	curSystemOptionsInfo_.operator_ = FindChildByName2<SWinEdit>("edit_operator")->GetWindowTextW();
	curSystemOptionsInfo_.rig_ = FindChildByName2<SWinEdit>("edit_rig")->GetWindowTextW();
	curSystemOptionsInfo_.lan_ = FindChildByName2<SComboView>("cbv_lan")->GetWindowTextW();
	curSystemOptionsInfo_.units_ = FindChildByName2<SComboView>("cbv_units")->GetWindowTextW();
	curSystemOptionsInfo_.savePath_ = FindChildByName2<SEdit>("save_path")->GetWindowTextW();
	MysqlClient::UpsertWellOptions(curSystemOptionsInfo_);
	LOGINFO("system options change");
	MysqlClient::InsertOperation("system options change");
	std::string strWellNameUtf8 = S_CW2A(curSystemOptionsInfo_.wellName_, CP_UTF8);
	App().SetItemConfig(STRING_RUNTIME, STRING_WELLNAME, strWellNameUtf8);
	App().SetItemConfig(STRING_RUNTIME, STRING_LANGUAGE, S_CW2A(curSystemOptionsInfo_.lan_, CP_UTF8).GetBuffer(0));
	App().SetItemConfig(STRING_RUNTIME, STRING_UNIT, S_CW2A(curSystemOptionsInfo_.units_, CP_UTF8).GetBuffer(0));
	SStringT savePath = curSystemOptionsInfo_.savePath_;
	savePath.Replace(L"\\", L"/");
	App().SetItemConfig(STRING_RUNTIME, STRING_SAVEFILE, S_CW2A(savePath, CP_UTF8).GetBuffer(0));

	App().MakePaths(S_CW2A(curSystemOptionsInfo_.savePath_, CP_ACP).GetBuffer(0));
	MysqlClient::CreateWellTableGroup(strWellNameUtf8);

	ShowWindow(SW_HIDE);
	App().AsyncEvent(WM_WNDSTATE_UPDATE, WND_STATE_DEF);
	App().AsyncEvent(WM_WELLNAME_CHANGE);
	App().DoPageSwitch(FLUID_PAGE);
}

void SystemOptionsDlg::OnFile()
{
	
	static TCHAR szDir[MAX_PATH] = { 0 };
	static BROWSEINFO bi = { 0 };
	bi.lpszTitle = L"Select a folder";
	bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_USENEWUI | BIF_RETURNONLYFSDIRS;

	// 使用异步操作来调用 SHBrowseForFolder
	std::thread t([]()
		{
			LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
			if (pidl != 0) {
				SHGetPathFromIDList(pidl, szDir);
				CoTaskMemFree(pidl);
			}
		});

	// 等待异步操作完成
	if (t.joinable())
	{
		t.join();
	}

	// 将选择的文件夹路径设置到编辑框中
	if (szDir[0] != '\0') {
		FindChildByName2<SEdit>("save_path")->SetWindowText(szDir);
	}


}

void SystemOptionsDlg::OnNewOption()
{
	FindChildByName2<SWinEdit>("edit_name")->SetAttribute(L"show", L"1");
	FindChildByName2<SWinEdit>("edit_name")->SetWindowTextW(L"");
	FindChildByName2<SDateTimePickerEx>("edit_date")->SetWindowTextW(L"");
	FindChildByName2<SWinEdit>("edit_location")->SetWindowTextW(L"");
	FindChildByName2<SWinEdit>("edit_field")->SetWindowTextW(L"");
	FindChildByName2<SWinEdit>("edit_operator")->SetWindowTextW(L"");
	FindChildByName2<SWinEdit>("edit_rig")->SetWindowTextW(L"");
}

void SystemOptionsDlg::SetManual(bool bManual)
{
	FindChildByName2<SImageButton>("btn_debug")->SetCheck(bManual);
	FindChildByName2<SStatic>("pump_title")->SetAttribute(L"colorText", bManual ? L"#232323" : L"#2323236F");
}

void SystemOptionsDlg::SetPump(bool bPump)
{
	FindChildByName2<SImageButton>("btn_pump")->SetCheck(bPump);
}

void SystemOptionsDlg::BtnPump()
{
	bool bCheck = FindChildByName2<SImageButton>("btn_pump")->IsChecked();
	if (!App().DeviceStates().bSignManual && !bCheck)
	{
		App().ShowMessage(L"Error",L"not manual flag");
		return;
	}
	App().DeviceTodo(
		bCheck ? DeviceIns::circulating_pump_0: DeviceIns::circulating_pump_1);
}

void SystemOptionsDlg::BtnDebug()
{
	App().DeviceTodo(
		FindChildByName2<SImageButton>("btn_debug")->IsChecked() 
		? DeviceIns::debug_mode_0 : DeviceIns::debug_mode_1);
}

bool SystemOptionsDlg::ComboViewChange(EventArgs* pEvt)
{
	auto* obj = (SComboView*)pEvt->sender;

	if (obj == nullptr) return true;

	MysqlClient::GetWellOptions(curSystemOptionsInfo_);
	//FindChildByName2<SComboView>("cbv_name")->SetWindowTextW(curSystemOptionsInfo_.wellName_);
	FindChildByName2<SDateTimePickerEx>("edit_date")->SetWindowTextW(curSystemOptionsInfo_.date_);
	FindChildByName2<SWinEdit>("edit_location")->SetWindowTextW(curSystemOptionsInfo_.location_);
	FindChildByName2<SWinEdit>("edit_field")->SetWindowTextW(curSystemOptionsInfo_.field_);
	FindChildByName2<SWinEdit>("edit_operator")->SetWindowTextW(curSystemOptionsInfo_.operator_);
	FindChildByName2<SWinEdit>("edit_rig")->SetWindowTextW(curSystemOptionsInfo_.rig_);

	return false;
}

