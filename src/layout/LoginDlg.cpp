#include "stdafx.h"
#include "LoginDlg.h"
#include "SWinEdit.h"
LoginDlg::LoginDlg() : SHostWnd(_T("LAYOUT:xml_login"))
{
	m_bLayoutInited = FALSE;
}

LoginDlg::~LoginDlg()
{
}

int LoginDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetMsgHandled(FALSE);
	return 0;
}

BOOL LoginDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	return 0;
}

void LoginDlg::OnClose()
{
}

void LoginDlg::OnBtnOK()
{

}
