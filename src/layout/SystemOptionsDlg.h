#pragma once
#include "SCheckBoxEx.h"
#include "SDateTimePickerEx.h"
#include "SArrayAdapter.hpp"
#include "SComboViewAdapter.hpp"
#include "SystemDef.h"


class SystemOptionsDlg : public SHostWnd
{
public:
	SystemOptionsDlg();
	~SystemOptionsDlg();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnClose();
	void OnBtnOK();
	void OnFile();
	void OnNewOption();
	void SetManual(bool bManual);
	void SetPump(bool bPump);
	void BtnPump();
	void BtnDebug();
protected:
	bool ComboViewChange(EventArgs* pEvt);
	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_ok", OnBtnOK)
		EVENT_NAME_COMMAND(L"btn_file", OnFile)
		EVENT_NAME_COMMAND(L"btn_new", OnNewOption)
		EVENT_NAME_COMMAND(L"btn_debug", BtnDebug)
		EVENT_NAME_COMMAND(L"btn_pump", BtnPump)
	EVENT_MAP_END()

	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(SystemOptionsDlg)
	MSG_WM_CREATE(OnCreate)
	MSG_WM_INITDIALOG(OnInitDialog)
	CHAIN_MSG_MAP(SHostWnd)
	REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
private:
	BOOL			m_bLayoutInited;
	SComboViewAdapter* pWellNameAdapter_ = nullptr;
	SComboViewAdapter* pLanAdapter_ = nullptr;
	SComboViewAdapter* pUnitsAdapter_ = nullptr;
	SystemOptionsInfo curSystemOptionsInfo_;
};