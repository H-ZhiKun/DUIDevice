#pragma once
class RestoreDlg : public SHostWnd
{
public:
	RestoreDlg();
	~RestoreDlg();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnBtnRestore();
protected:

	//soui��Ϣ
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_restore", OnBtnRestore)
	EVENT_MAP_END()

	//HostWnd��ʵ������Ϣ����
	BEGIN_MSG_MAP_EX(RestoreDlg)
	MSG_WM_CREATE(OnCreate)
	MSG_WM_INITDIALOG(OnInitDialog)
	CHAIN_MSG_MAP(SHostWnd)
	REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
private:
	BOOL m_bLayoutInited;
};