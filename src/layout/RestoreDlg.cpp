#include "stdafx.h"
#include "RestoreDlg.h"
#include "AppFramework.h"
using namespace realology;
RestoreDlg::RestoreDlg() : SHostWnd(_T("LAYOUT:xml_restore"))
{
	m_bLayoutInited = FALSE;
}

RestoreDlg::~RestoreDlg()
{
}

int RestoreDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetMsgHandled(FALSE);
	return 0;
}

BOOL RestoreDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	return 0;
}



void RestoreDlg::OnBtnRestore()
{
	App().DeviceTodo(DeviceIns::soft_restore);
}