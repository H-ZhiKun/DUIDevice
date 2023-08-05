#pragma once
#include "DiagAdapter.hpp"
class DiagDlg : public SHostWnd
{
public:
	enum DiagTarget
	{
		base = 0,
		hardware,
		alarm,
		warning,
		comm
	};
	DiagDlg();
	~DiagDlg();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnClose();


	void OnBtnCode();
	void OnBtnTime();



	void OnBtnHardware(bool bShow);
	void OnBtnAlarm(bool bShow);
	void OnBtnWarning(bool bShow);
	void OnBtnComm(bool bShow);


	void GetPageInfo();

	void OnBtnPrev();
	void OnBtnNext();
	void OnBtnHome();
	void OnBtnEnd();
	void OnTrip();

	//target:check title set nRedLight(0:green,1:red)
	void SetDisplayLights(DiagTarget target, uint8_t nRedLight);

	//void UpdateDisplayData();
	void SetTrip(bool bEnable);
	void KeyEditUpdate(std::string title, std::string item);
	void ShowDlg();
protected:
	bool CheckBtnChange(EventArgs* pEvt);
	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_trip", OnTrip)
		EVENT_NAME_COMMAND(L"btn_time", OnBtnTime)
		EVENT_NAME_COMMAND(L"btn_code", OnBtnCode)

		EVENT_NAME_COMMAND(L"btn_prev", OnBtnPrev)
		EVENT_NAME_COMMAND(L"btn_next", OnBtnNext)
		EVENT_NAME_COMMAND(L"btn_home", OnBtnHome)
		EVENT_NAME_COMMAND(L"btn_end", OnBtnEnd)
		EVENT_MAP_END()

		//HostWnd真实窗口消息处理
		BEGIN_MSG_MAP_EX(DiagDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
		END_MSG_MAP()
public:
	BOOL			m_bLayoutInited;
	std::string sqlKey_ = "id";
	std::string sqlOrder_;
	std::string dataType_;
	size_t nPage_ = 1;
	size_t nTotalPage_ = 0;
	DiagAdapter* pDiagAdapter_ = new DiagAdapter();
	std::map<DiagTarget, std::string> mapLightsTarget_;
	DiagTarget curTarget_ = base;
	bool bTripEnable = false;
	std::map <SStringT, std::function<void(bool)>> mapCheckBtn;
};