// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KeyboardDlg.h"

KeyboardDlg::KeyboardDlg() : SHostWnd(_T("LAYOUT:xml_keyboard"))
{
	m_bLayoutInited = FALSE;
}


KeyboardDlg::~KeyboardDlg()
{
}

int KeyboardDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetMsgHandled(FALSE);
	return 0;
}

BOOL KeyboardDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	return 0;
}

//TODO:消息映射
void KeyboardDlg::OnClose()
{
	pEdit_->OnKeyDown(VK_ESCAPE, 1, 2);
}

void KeyboardDlg::SetEdit(SKeyEdit* edit)
{
	pEdit_ = edit;
}

void KeyboardDlg::Key1()
{
	InputChar("1");
}

void KeyboardDlg::Key2()
{
	InputChar("2");
}

void KeyboardDlg::Key3()
{
	InputChar("3");
}

void KeyboardDlg::Key4()
{
	InputChar("4");
}

void KeyboardDlg::Key5()
{
	InputChar("5");
}

void KeyboardDlg::Key6()
{
	InputChar("6");
}

void KeyboardDlg::Key7()
{
	InputChar("7");
}

void KeyboardDlg::Key8()
{
	InputChar("8");
}

void KeyboardDlg::Key9()
{
	InputChar("9");
}

void KeyboardDlg::Key0()
{
	InputChar("0");
}

void KeyboardDlg::KeyDot()
{
	InputChar(".");
}

void KeyboardDlg::KeyBack()
{
	InputChar("back");
}

void KeyboardDlg::KeyEnter()
{
	InputChar("enter");
}

void KeyboardDlg::InputChar(std::string str)
{
	if (pEdit_ == nullptr) return;
	SStringT strTemp = pEdit_->GetWindowTextW();
	if (str == "back")
	{
		pEdit_->SetWindowTextW(strTemp.Left(strTemp.GetLength() - 1));
		return;
	}
	if (str == "enter")
	{
		pEdit_->OnKeyDown(VK_RETURN, 1, 2);
		return;
	}
	pEdit_->SetWindowTextW(strTemp + S_CA2W(str.c_str()));
}





