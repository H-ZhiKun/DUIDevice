#include "stdafx.h"
#include "AppFrameworkImpl.h"
#include <filesystem>
#include <initguid.h>
#include <shlObj.h>
#include "HttpClient.hpp"

DEFINE_GUID(CLSID_UIHostNoLaunch, 0x4CE576FA, 0x83DC, 0x4f88, 0x95, 0x1C, 0x9D, 0x07, 0x82, 0xB4, 0xE3, 0x76);
DEFINE_GUID(IID_ITipInvocation, 0x37c994e7, 0x432b, 0x4834, 0xa2, 0xf7, 0xdc, 0xe1, 0xf1, 0x3b, 0x83, 0x4b);
struct ITipInvocation : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE Toggle(HWND wnd) = 0;
};

EXTERN_C const IID IID_IFrameworkInputPane;


MIDL_INTERFACE("5752238B-24F0-495A-82F1-2FD593056796")
IFrameworkInputPane : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE Advise(
		/* [in] */ __RPC__in_opt IUnknown * pWindow,
		/* [in] */ __RPC__in_opt IFrameworkInputPaneHandler * pHandler,
		/* [out] */ __RPC__out DWORD * pdwCookie) = 0;

	virtual HRESULT STDMETHODCALLTYPE AdviseWithHWND(
		/* [in] */ __RPC__in HWND hwnd,
		/* [in] */ __RPC__in_opt IFrameworkInputPaneHandler* pHandler,
		/* [out] */ __RPC__out DWORD* pdwCookie) = 0;

	virtual HRESULT STDMETHODCALLTYPE Unadvise(
		/* [in] */ DWORD dwCookie) = 0;

	virtual HRESULT STDMETHODCALLTYPE Location(
		/* [out] */ __RPC__out RECT* prcInputPaneScreenLocation) = 0;

};



using namespace realology;

AppFramework& AppFramework::instance()
{
	return AppFrameworkImpl::instance();
}

realology::AppFrameworkImpl::AppFrameworkImpl() :
	strWorkPath_(std::filesystem::current_path().string()),
	pMysqlPool_(new MysqlConnectionPool()),
	pMainDlg_(new CMainDlg()),
	pKeyboardDlg_(new KeyboardDlg()),
	pMessageDlg_(new MessageDlg()),
	pTaskMgr_(new ConcurrentTaskQueue(std::thread::hardware_concurrency(), "task")),
	pMbsDevice_(new ModbusDevice())
{

}

realology::AppFrameworkImpl::~AppFrameworkImpl() noexcept
{
	workHolder_ = false;
	for (auto& it : globalWorker_)
		it.join();
}

int realology::AppFrameworkImpl::Run(SApplication* theApp)
{
	Logger::InitLogger(strWorkPath_ + "\\logs");
	if (!IsWindosKeyboardOpen())
	{
		OpenKeyboard();
	}
	InitItemConfig();
	InitCalcInfo();
	CreateWnd();
	StartTimerService();
	StartTcpClient();
	StartGuardian();
	pMbsDevice_->Work();
	RecoveredEntry();
	return theApp->Run(pMainDlg_->m_hWnd);
}

void realology::AppFrameworkImpl::Quit()
{
	workHolder_ = false;
	HWND hwnd = FindWindow(nullptr, L"realology_guardian");
	if (hwnd != 0)
	{
		::SendMessage(hwnd, WM_CLOSE, NULL, NULL);
	}
	LOGINFO("System Close");
	Logger::CloseLogger();
	TerminateProcess(GetCurrentProcess(), 0);
}

void realology::AppFrameworkImpl::ShowWinKeyboard(bool bShow)
{
	if (IsShowWinKeyboard() != bShow)
	{
		ITipInvocation* tip_invocation_ = nullptr;
		HRESULT hr = ::CoCreateInstance(CLSID_UIHostNoLaunch, 0, CLSCTX_INPROC_HANDLER | CLSCTX_LOCAL_SERVER,
			IID_ITipInvocation, (LPVOID*)(&tip_invocation_));
		if (tip_invocation_ == nullptr)
		{
			return;
		}
		if (hr == S_OK)
		{
			tip_invocation_->Toggle(GetDesktopWindow());
		}
		tip_invocation_->Release();

	}
}

void realology::AppFrameworkImpl::ShowCustomKeyboard(bool bShow, SEdit* obj)
{
	if (bShow)
	{
		pKeyboardDlg_->SetEdit(dynamic_cast<SKeyEdit*>(obj));
		pKeyboardDlg_->ShowWindow(SW_SHOW);
	}
	else
	{
		pKeyboardDlg_->ShowWindow(SW_HIDE);
	}
}

std::string realology::AppFrameworkImpl::GetWorkPath()
{
	static std::mutex mtx;
	std::lock_guard<std::mutex> lock(mtx);
	return strWorkPath_;
}

void realology::AppFrameworkImpl::AsyncEvent(int iEventId, int wParam, TransportMsg* lParam)
{
	if (pMainDlg_ == nullptr)
	{
		return;
	}
	::PostMessage(pMainDlg_->m_hWnd, iEventId, wParam, LPARAM(lParam));
}

void realology::AppFrameworkImpl::AsyncTask(const std::function<void()>& task)
{
	pTaskMgr_->runTaskInQueue(task);
}

void realology::AppFrameworkImpl::ShowMessage(const SStringT&& title, const SStringT&& des, std::function<void(void)> task)
{
	pMessageDlg_->ResetInfo(title, des, task);
	pMessageDlg_->ShowWindow(SW_SHOWNORMAL);
	AsyncEvent(WM_WNDSTATE_UPDATE, WND_STATE_POP);
}

void realology::AppFrameworkImpl::DoPageSwitch(PageSwitch page)
{
	switch (page)
	{
	case MAIN_PAGE:
		break;
	case SYSTEM_PAGE:
		break;
	case FLUID_PAGE:
		pMainDlg_->OnDlgFluidSetup();
		break;
	default:
		break;
	}
}

std::string realology::AppFrameworkImpl::GetItemConfig(const std::string& itemName, const std::string& itemKey)
{
	std::string ret;
	std::lock_guard<std::mutex> autoLock(mtxItemConfig_);
	auto iter = mapItemConfig_.find(itemName);
	if (iter != mapItemConfig_.end())
	{
		ret = std::move(iter->second[itemKey].asString());
	}
	return ret;
}

Json::Value realology::AppFrameworkImpl::GetItemConfig(const std::string& itemName)
{
	Json::Value ret;
	std::lock_guard<std::mutex> autoLock(mtxItemConfig_);
	auto iter = mapItemConfig_.find(itemName);
	if (iter != mapItemConfig_.end())
	{
		ret = iter->second;
	}
	return ret;
}

std::map<std::string, Json::Value> realology::AppFrameworkImpl::GetItemConfig()
{
	std::lock_guard<std::mutex> autoLock(mtxItemConfig_);
	return std::map<std::string, Json::Value>(mapItemConfig_);
}

void realology::AppFrameworkImpl::SetItemConfig(const std::string& title, const std::string& item, const std::string& val)
{
	std::lock_guard<std::mutex> autoLock(mtxItemConfig_);
	auto iter = mapItemConfig_.find(title);
	if (iter != mapItemConfig_.end())
	{
		iter->second[item] = val;
		MysqlClient::UpdateItemConfig(title, iter->second);
	}
}

const AdvanceData& realology::AppFrameworkImpl::GetAdvanceReal()
{
	return pMbsDevice_->GetAdvanceReal();
}

const CollectionData& realology::AppFrameworkImpl::GetCollection()
{
	return pMbsDevice_->GetCollection();
}

const RecordData& realology::AppFrameworkImpl::GetRecord()
{
	return pMbsDevice_->GetRecord();
}

const std::vector<RealF>& realology::AppFrameworkImpl::GetPoints()
{
	return pMbsDevice_->GetPoints();
}

void realology::AppFrameworkImpl::SetAdvanceDebug(bool bDebug)
{
	pMbsDevice_->SetAdvanceDebug(bDebug);
}

bool realology::AppFrameworkImpl::DeviceIsConnected()
{
	return pMbsDevice_->IsConnected();
}

AnalysisConfig realology::AppFrameworkImpl::GetAnalysisConfig()
{
	return analysisConfig_;
}

void realology::AppFrameworkImpl::SendToServer()
{
	std::map<std::string, std::string> map_data;
	map_data["0003"] = SStringA().Format("%.3f", pMbsDevice_->GetRecord().avg_density / 1000.0).GetBuffer(0);
	map_data["0004"] = SStringA().Format("%.1f", pMbsDevice_->GetRecord().avg_temp).GetBuffer(0);
	map_data["0009"] = SStringA().Format("%.2f", pMbsDevice_->GetRecord().av).GetBuffer(0);
	map_data["0010"] = SStringA().Format("%.2f", pMbsDevice_->GetRecord().pv).GetBuffer(0);
	map_data["0011"] = SStringA().Format("%.2f", pMbsDevice_->GetRecord().yp).GetBuffer(0);
	map_data["0014"] = SStringA().Format("%.2f", pMbsDevice_->GetRecord().yp / pMbsDevice_->GetRecord().pv).GetBuffer(0);
	map_data["0015"] = SStringA().Format("%.2f", pMbsDevice_->GetRecord().nhb).GetBuffer(0);
	map_data["0016"] = SStringA().Format("%.2f", pMbsDevice_->GetRecord().khb).GetBuffer(0);
	map_data["0017"] = SStringA().Format("%.2f", pMbsDevice_->GetRecord().r600).GetBuffer(0);
	map_data["0018"] = SStringA().Format("%.2f", pMbsDevice_->GetRecord().r300).GetBuffer(0);
	map_data["0019"] = SStringA().Format("%.2f", pMbsDevice_->GetRecord().r200).GetBuffer(0);
	map_data["0020"] = SStringA().Format("%.2f", pMbsDevice_->GetRecord().r100).GetBuffer(0);
	map_data["0021"] = SStringA().Format("%.2f", pMbsDevice_->GetRecord().r6).GetBuffer(0);
	map_data["0022"] = SStringA().Format("%.2f", pMbsDevice_->GetRecord().r3).GetBuffer(0);
	map_data["0033"] = SStringA().Format("%.2f", pMbsDevice_->GetRecord().avg_pressure).GetBuffer(0);
	std::string strFormat;
	if (map_data.size())
	{
		strFormat += "&&\r\n";
		for (auto& it : map_data)
		{
			strFormat += it.first + it.second + "\r\n";
		}
		strFormat += "!!\r\n";
	}
	if (pTcpClient_ != nullptr)
	{
		LOGINFO("TCP data reporting");
		pTcpClient_->do_write(strFormat);
	}
}

void realology::AppFrameworkImpl::DeviceTodo(DeviceIns cmd)
{
	pMbsDevice_->PushInstruction(cmd);
}

const MachineState& realology::AppFrameworkImpl::DeviceStates()
{
	return pMbsDevice_->DeviceStates();
}

void realology::AppFrameworkImpl::CleanProcedure(int cleanTimes, bool bTest)
{
	pMbsDevice_->CleanProcedure(cleanTimes, bTest);
}

void realology::AppFrameworkImpl::DataReload()
{
	pMbsDevice_->DataReload();
}

Connection* realology::AppFrameworkImpl::GetMysqlConnection()
{
	return pMysqlPool_->GetConnection();
}

void realology::AppFrameworkImpl::PushMysqlConnection(Connection* pConnect)
{
	pMysqlPool_->ReleaseConnection(pConnect);
}

void realology::AppFrameworkImpl::SetFluidInfo(const FluidInfo& info)
{
	std::lock_guard<std::mutex> lock(mtxFluidSetup_);
	curFluidSetupInfo_ = info;
}

const FluidInfo& realology::AppFrameworkImpl::GetFluidInfo()
{
	std::lock_guard<std::mutex> lock(mtxFluidSetup_);
	return curFluidSetupInfo_;
}

void realology::AppFrameworkImpl::MakePaths(std::string filePath)
{
	std::string save_path = filePath + "\\" + mapItemConfig_[STRING_RUNTIME][STRING_WELLNAME].asString();
	std::filesystem::create_directories(save_path);
	std::filesystem::create_directories(save_path + "\\Excel");
	std::filesystem::create_directories(save_path + "\\Snapshot");
	std::filesystem::create_directories(save_path + "\\Temp");
}

void realology::AppFrameworkImpl::PostAlarm()
{
	AsyncTask([this] {
		std::string wellName = GetItemConfig(STRING_RUNTIME, STRING_WELLNAME);
		std::string host = GetItemConfig("Communication", "alarm_ip");
		std::string port = GetItemConfig("Communication", "alarm_port");
		std::string page = "/alarm/trigger";
		Json::Value jsRoot;
		jsRoot["well_name"] = wellName;

		std::string reponse_data;
		LOGINFO("well alarm");
		int ret = HttpClient::post(host, port, page, Utils::JsonToStr(jsRoot), reponse_data);
		if (ret == 0)
		{
			Json::Reader jsRead;
			Json::Value js_ret;
			if (jsRead.parse(reponse_data, js_ret))
			{
				int code = js_ret["code"].as<int>();

				if (code != 1)
				{
					SStringA tempEvent = SStringA().Format("Remote Alert Failed,code=%d", code);
					MysqlClient::InsertOperation(tempEvent.GetBuffer(0));
				}
			}
		}
		else
		{
			MysqlClient::InsertOperation("Remote Alert Failed");
		}
		});

}


bool realology::AppFrameworkImpl::IsWindosKeyboardOpen()
{
	bool ret = false;
	LPCWSTR WINDOW_CLASS = L"IPTip_Main_Window";
	if (::FindWindowEx(nullptr, nullptr, WINDOW_CLASS, nullptr))
	{
		ret = true;
	}
	return ret;
}

void realology::AppFrameworkImpl::OpenKeyboard()
{
	auto res = CoInitialize(nullptr);
	TCHAR filePath[MAX_PATH];
	if (!::SHGetSpecialFolderPath(nullptr, filePath, CSIDL_PROGRAM_FILES, false))
	{
		return;
	}
	std::wstring path = filePath;
	std::wstring process = path.substr(0, path.find_first_of(L'\\')) + L" / Program Files / Common Files / Microsoft Shared / ink / tabtip.exe";
	static PROCESS_INFORMATION Stat;
	static STARTUPINFO SU = { sizeof SU };
	SetEnvironmentVariable(L"__compat_layer", L"RunAsInvoker");
	if (!CreateProcess(0, (LPWSTR)process.c_str(), 0, 0, 1, 0, 0, 0, &SU, &Stat)) {
		return;
	}
	CloseHandle(Stat.hProcess);
	CloseHandle(Stat.hThread);
	CoUninitialize();
}

bool realology::AppFrameworkImpl::IsShowWinKeyboard()
{
	IFrameworkInputPane* pInputPane = nullptr;
	RECT prcInputPaneScreenLocation = { 0,0,0,0 };

	HRESULT hr = ::CoCreateInstance(CLSID_FrameworkInputPane, NULL, CLSCTX_INPROC_SERVER, IID_IFrameworkInputPane, (LPVOID*)&pInputPane);
	if (FAILED(hr))
	{
		return false;
	}

	hr = pInputPane->Location(&prcInputPaneScreenLocation);
	if (FAILED(hr))
	{
		return false;
	}
	pInputPane->Release();

	if (prcInputPaneScreenLocation.right - prcInputPaneScreenLocation.left > 5)
	{
		return true;
	}
	return false;
}

void realology::AppFrameworkImpl::CreateWnd()
{
	pMainDlg_ = new CMainDlg;
	pMainDlg_->Create(nullptr);
	pMainDlg_->SendMessage(WM_INITDIALOG);
	pMainDlg_->CenterWindow(pMainDlg_->m_hWnd);
	pMainDlg_->ShowWindow(SW_SHOWNORMAL);
	pMainDlg_->SetWindowPos(HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	pKeyboardDlg_ = new KeyboardDlg;
	pKeyboardDlg_->Create(nullptr);
	pKeyboardDlg_->SendMessage(WM_INITDIALOG);
	pKeyboardDlg_->SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);

	pMessageDlg_ = new MessageDlg;
	pMessageDlg_->Create(nullptr);
	pMessageDlg_->SendMessage(WM_INITDIALOG);
	pMessageDlg_->CenterWindow(pMainDlg_->m_hWnd);
	pMessageDlg_->SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}


void realology::AppFrameworkImpl::InitItemConfig()
{
	MysqlClient::GetItemConfig(mapItemConfig_);
	MysqlClient::GetFluidInfo(curFluidSetupInfo_);
	if (mapItemConfig_[STRING_RUNTIME][STRING_ISEXIT].asString() == "0" && mapItemConfig_[STRING_RUNTIME][STRING_ISTEST].asString() == "1")
	{
		LOGINFO("START TEST(AUTO)");
		pMbsDevice_->PushInstruction(DeviceIns::test_start);
		MysqlClient::InsertOperation("START TEST(AUTO)");
	}
	SetItemConfig(STRING_RUNTIME, STRING_ISEXIT, "0");
}

void realology::AppFrameworkImpl::InitCalcInfo()
{
	Json::Value jsConfig = Utils::ReadJsonFile(strWorkPath_ + "\\viscosity.json");
	analysisConfig_.fLength = jsConfig["length(m)"].asFloat();
	analysisConfig_.fdc = jsConfig["dc(m)"].asFloat();
	analysisConfig_.fdtc = jsConfig["dtc(m)"].asFloat();
	for (uint32_t i = 0; i < jsConfig["ABList"].size(); i++)
	{
		ABInfo info;
		info.fDensityRange0 = jsConfig["ABList"][i]["DensityRange0"].asFloat();
		info.fDensityRange1 = jsConfig["ABList"][i]["DensityRange1"].asFloat();
		info.fR600fann35 = jsConfig["ABList"][i]["R600fann35"].asFloat();
		info.fR300fann35 = jsConfig["ABList"][i]["R300fann35"].asFloat();
		for (uint32_t j = 0; j < jsConfig["ABList"][i]["ABVal"].size(); j++)
		{
			ABValue AB;
			AB.a = jsConfig["ABList"][i]["ABVal"][j]["a"].asFloat();
			AB.b = jsConfig["ABList"][i]["ABVal"][j]["b"].asFloat();
			info.listAB.emplace_back(AB);
		}
		analysisConfig_.lvABInfo_.emplace_back(info);
	}

}

void realology::AppFrameworkImpl::StartTimerService()
{
	globalWorker_.emplace_back(std::thread([this] {
		std::string lastTime = "2021/01/01 00:00:00";//¼ÍÄîÈÕÆÚ
		while (workHolder_)
		{
			std::string currentTime = Utils::GetDateTime();
			if (lastTime != currentTime)
			{
				std::string lastDate = lastTime.substr(0, lastTime.find_first_of(' '));
				std::string curDate = currentTime.substr(0, currentTime.find_first_of(' '));
				if (lastDate != curDate)
				{
					AsyncTask([this, currentTime] {
						MysqlClient::CreateWellTableGroup(GetItemConfig(STRING_RUNTIME, STRING_WELLNAME));
						std::string path = GetWorkPath() + "logs";
						Utils::RemoveOutdatedLog(currentTime, path);
						});
				}
				lastTime = currentTime;
				AsyncEvent(WM_TIMER_UPDATE);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		}));
}

void realology::AppFrameworkImpl::StartTcpClient()
{
	globalWorker_.emplace_back(std::thread([this]
		{
			while (workHolder_)
			{
				boost::shared_ptr<io_service> service(new io_service);
				std::string ip = GetItemConfig("Communication", "wits_ip");
				uint16_t port = atoi(GetItemConfig("Communication", "wits_port").c_str());
				ip::tcp::endpoint ep(ip::address::from_string(ip), port);
				pTcpClient_ = talk_to_svr::start(ep, "", service);
				service->run();
				service->reset();
				pTcpClient_.reset();
				pTcpClient_ = nullptr;
				std::this_thread::sleep_for(std::chrono::seconds(5));
			}
		}));
}

void realology::AppFrameworkImpl::StartGuardian()
{
	globalWorker_.emplace_back(std::thread([this]
		{
			SStringT guardIns = S_CA2W(strWorkPath_.c_str(), CP_UTF8) + L"\\Guardian.exe";
			SStringT dataIns = S_CA2W(strWorkPath_.c_str(), CP_UTF8) + L"\\DataService.exe";
			while (workHolder_)
			{
				if (!Utils::FindProcess(L"Guardian.exe"))
				{
					Utils::StartProcess(guardIns);
				}
				if (!Utils::FindProcess(L"DataService.exe"))
				{
					Utils::StartProcess(dataIns);
				}
				std::this_thread::sleep_for(std::chrono::seconds(30));
			}
		}));
}

void realology::AppFrameworkImpl::RecoveredEntry()
{
	if (mapItemConfig_[STRING_RUNTIME][STRING_ISTEST].asString() == "1")
		pMainDlg_->PageLoginOrMain(false);
	AsyncTask([this]
		{
			std::this_thread::sleep_for(std::chrono::seconds(2));
			AsyncEvent(WM_WELLNAME_CHANGE);
			AsyncEvent(WM_FLUID_UPDATE);
			AsyncEvent(WM_WNDCALC_UPDATE);
		});
}

