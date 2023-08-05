#include "stdafx.h"
#include "MessageDlg.h"
#include "AppFramework.h"
using namespace realology;
MessageDlg::MessageDlg() : SHostWnd(_T("LAYOUT:xml_message"))
{
	m_bLayoutInited = FALSE;
}

MessageDlg::~MessageDlg()
{
}

int MessageDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetMsgHandled(FALSE);
	return 0;
}

BOOL MessageDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	return 0;
}

void MessageDlg::OnClose()
{
	ShowWindow(SW_HIDE);
	App().AsyncEvent(WM_WNDSTATE_UPDATE, WND_STATE_DEF);
}

void MessageDlg::OnBtnOK()
{
	ShowWindow(SW_HIDE);
	App().AsyncEvent(WM_WNDSTATE_UPDATE, WND_STATE_DEF);
	if (task_)
	{
		task_();
	}
}

void MessageDlg::ResetInfo(const SStringT& title, const SStringT& des, std::function<void(void)> task)
{
	FindChildByName2<SStatic>("title")->SetWindowTextW(title);
	FindChildByName2<SStatic>("des")->SetWindowTextW(des);
	UpdateWindow();
	task_ = task;
}
