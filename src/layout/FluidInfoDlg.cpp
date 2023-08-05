#include "stdafx.h"
#include "FluidInfoDlg.h"
#include "SComboViewAdapter.hpp"
#include "SWinEdit.h"
#include "SDateTimePickerEx.h"
#include "AppFramework.h"
#include "MysqlClient.h"
using namespace realology;

FluidInfoDlg::FluidInfoDlg() : SHostWnd(_T("LAYOUT:xml_fluid_info"))
{
	m_bLayoutInited = FALSE;
}

FluidInfoDlg::~FluidInfoDlg()
{
}

int FluidInfoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetMsgHandled(FALSE);
	return 0;
}

BOOL FluidInfoDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	{
		auto obj = FindChildByName2<SComboView>("cbv_type");
		auto pAdapter_ = new SComboViewAdapter(obj);
		obj->GetListView()->SetAdapter(pAdapter_);
		pAdapter_->AddItem(L"WBM");
		pAdapter_->AddItem(L"OBM");
		pAdapter_->AddItem(L"SBM");
		pAdapter_->SetSel(0);
		pAdapter_->Release();
	}
	{
		auto obj = FindChildByName2<SComboView>("cbv_density");
		auto pAdapter_ = new SComboViewAdapter(obj);
		obj->GetListView()->SetAdapter(pAdapter_);
		pAdapter_->AddItem(L"kg/m³");
		pAdapter_->AddItem(L"g/cm³");
		pAdapter_->AddItem(L"sg");
		pAdapter_->AddItem(L"ppg");
		pAdapter_->SetSel(0);
		pAdapter_->Release();
	}
	{
		auto obj = FindChildByName2<SComboView>("cbv_temp");
		auto pAdapter_ = new SComboViewAdapter(obj);
		obj->GetListView()->SetAdapter(pAdapter_);
		pAdapter_->AddItem(L"°C");
		pAdapter_->AddItem(L"°F");
		pAdapter_->SetSel(0);
		pAdapter_->Release();
	}
	{
		auto obj = FindChildByName2<SComboView>("cbv_active");
		auto pAdapter_ = new SComboViewAdapter(obj);
		obj->GetListView()->SetAdapter(pAdapter_);
		pAdapter_->AddItem(L"Active System");
		pAdapter_->AddItem(L"Return System");
		pAdapter_->AddItem(L"Lab System");
		pAdapter_->SetSel(0);
		pAdapter_->Release();
	}

	return 0;
}

void FluidInfoDlg::OnClose()
{
	if (App().GetFluidInfo().name_.IsEmpty())
	{
		App().ShowMessage(L"ERROR", L"fluid name empty");
		return;
	}
	FindChildByName2<SWinEdit>("edit_name")->SetWindowTextW(App().GetFluidInfo().name_);
	FindChildByName2<SWinEdit>("edit_density")->SetWindowTextW(SStringT().Format(L"%.2f", App().GetFluidInfo().density_));
	FindChildByName2<SWinEdit>("edit_temp")->SetWindowTextW(SStringT().Format(L"%.2f", App().GetFluidInfo().temp_));
	FindChildByName2<SDateTimePickerEx>("date_time")->SetWindowTextW(App().GetFluidInfo().date_);

	ShowWindow(SW_HIDE);
	App().AsyncEvent(WM_WNDSTATE_UPDATE, WND_STATE_DEF);
}

void FluidInfoDlg::OnBtnOK()
{
	FluidInfo info;
	info.name_ = FindChildByName2<SWinEdit>("edit_name")->GetWindowTextW();
	info.type_ = FindChildByName2<SComboView>("cbv_type")->GetWindowTextW();
	info.density_ = std::stof(S_CW2A(FindChildByName2<SWinEdit>("edit_density")->GetWindowTextW()).GetBuffer(0));
	info.temp_ = std::stof(S_CW2A(FindChildByName2<SWinEdit>("edit_temp")->GetWindowTextW()).GetBuffer(0));
	info.source_ = FindChildByName2<SComboView>("cbv_active")->GetWindowTextW();
	info.date_ = FindChildByName2<SDateTimePickerEx>("date_time")->GetWindowTextW();

	if (info.name_.IsEmpty())
	{
		App().ShowMessage(L"ERROR", L"fluid name empty");
		return;
	}

	App().SetFluidInfo(info);
	MysqlClient::InsertFluidInfo(info);
	MysqlClient::InsertOperation("fluid setup change");
	ShowWindow(SW_HIDE);
	App().AsyncEvent(WM_WNDSTATE_UPDATE, WND_STATE_DEF);
	App().AsyncEvent(WM_FLUID_UPDATE);
}

void FluidInfoDlg::ShowDlg()
{
	FindChildByName2<SWinEdit>("edit_name")->SetWindowTextW(App().GetFluidInfo().name_);
	FindChildByName2<SWinEdit>("edit_density")->SetWindowTextW(SStringT().Format(L"%.2f", App().GetFluidInfo().density_));
	FindChildByName2<SWinEdit>("edit_temp")->SetWindowTextW(SStringT().Format(L"%.2f", App().GetFluidInfo().temp_));
	FindChildByName2<SDateTimePickerEx>("date_time")->SetWindowTextW(App().GetFluidInfo().date_);

	SComboViewAdapter* adapter = (SComboViewAdapter*)FindChildByName2<SComboView>("cbv_type")->GetListView()->GetAdapter();
	adapter->SetSel(App().GetFluidInfo().type_);
	adapter = (SComboViewAdapter*)FindChildByName2<SComboView>("cbv_active")->GetListView()->GetAdapter();
	adapter->SetSel(App().GetFluidInfo().source_);
	ShowWindow(SW_SHOW);

}


