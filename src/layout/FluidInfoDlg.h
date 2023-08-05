#pragma once
#include "SArrayAdapter.hpp"
#include "SystemDef.h"
class FluidInfoDlg : public SHostWnd
{
public:
	FluidInfoDlg();
	~FluidInfoDlg();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnClose();
	void OnBtnOK();
	void ShowDlg();


protected:

	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_ok", OnBtnOK)
	EVENT_MAP_END()

	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(FluidInfoDlg)
	MSG_WM_CREATE(OnCreate)
	MSG_WM_INITDIALOG(OnInitDialog)
	CHAIN_MSG_MAP(SHostWnd)
	REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
private:
	BOOL m_bLayoutInited;
	int testIndex = 644286;
};