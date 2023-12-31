
#pragma once
#include "SKeyEdit.h"
#include <string>
class KeyboardDlg : public SHostWnd
{
public:
	KeyboardDlg();
	~KeyboardDlg();

	void OnClose();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void SetEdit(SKeyEdit* edit);
	
protected:
	void Key1();
	void Key2();
	void Key3();
	void Key4();
	void Key5();
	void Key6();
	void Key7();
	void Key8();
	void Key9();
	void Key0();
	void KeyDot();
	void KeyBack();
	void KeyEnter();
	void InputChar(std::string str);
	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_key1", Key1)
		EVENT_NAME_COMMAND(L"btn_key2", Key2)
		EVENT_NAME_COMMAND(L"btn_key3", Key3)
		EVENT_NAME_COMMAND(L"btn_key4", Key4)
		EVENT_NAME_COMMAND(L"btn_key5", Key5)
		EVENT_NAME_COMMAND(L"btn_key6", Key6)
		EVENT_NAME_COMMAND(L"btn_key7", Key7)
		EVENT_NAME_COMMAND(L"btn_key8", Key8)
		EVENT_NAME_COMMAND(L"btn_key9", Key9)
		EVENT_NAME_COMMAND(L"btn_key0", Key0)
		EVENT_NAME_COMMAND(L"btn_dot", KeyDot)
		EVENT_NAME_COMMAND(L"btn_bk", KeyBack)
		EVENT_NAME_COMMAND(L"btn_enter", KeyEnter)
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
	EVENT_MAP_END()
		
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(KeyboardDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
private:
	BOOL			m_bLayoutInited;
	SKeyEdit* pEdit_ = nullptr;
};
