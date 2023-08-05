#pragma once
class LoginDlg : public SHostWnd
{
public:
	LoginDlg();
	~LoginDlg();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnClose();
	void OnBtnOK();

protected:

	//soui��Ϣ
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"login_btn_close", OnClose)
		EVENT_NAME_COMMAND(L"login_btn_login", OnBtnOK)
	EVENT_MAP_END()

	//HostWnd��ʵ������Ϣ����
	BEGIN_MSG_MAP_EX(LoginDlg)
	MSG_WM_CREATE(OnCreate)
	MSG_WM_INITDIALOG(OnInitDialog)
	CHAIN_MSG_MAP(SHostWnd)
	REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
private:
	BOOL m_bLayoutInited;
};