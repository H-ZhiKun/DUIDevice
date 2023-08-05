#pragma once
#include "AppFramework.h"
#include "MainDlg.h"
#include "KeyboardDlg.h"
#include "MessageDlg.h"
#include "TcpClient.hpp"
#include "ConcurrentTaskQueue.h"
namespace realology
{
	class AppFrameworkImpl final : public AppFramework
	{
	public:
		AppFrameworkImpl();
		~AppFrameworkImpl() noexcept override;
		inline static AppFrameworkImpl& instance()
		{
			static AppFrameworkImpl instance;
			return instance;
		}

		int Run(SApplication* theApp) override;
		void Quit() override;
		void ShowWinKeyboard(bool bShow) override;
		void ShowCustomKeyboard(bool bShow, SEdit* obj) override;
		std::string GetWorkPath() override;
		void AsyncEvent(int iEventId, int wParam = 0, TransportMsg* lParam = 0) override;
		void AsyncTask(const std::function<void()>& task) override;
		void ShowMessage(const SStringT&& title, const SStringT&& des, std::function<void(void)> task = nullptr) override;
		void DoPageSwitch(PageSwitch page) override;
		std::string GetItemConfig(const std::string& itemName, const std::string& itemKey) override;
		Json::Value GetItemConfig(const std::string& itemName) override;
		std::map<std::string, Json::Value> GetItemConfig() override;
		void SetItemConfig(const std::string& title, const std::string& item, const std::string& val) override;
		const AdvanceData& GetAdvanceReal() override;
		const CollectionData& GetCollection() override;
		const RecordData& GetRecord() override;
		const std::vector<RealF>& GetPoints() override;
		void SetAdvanceDebug(bool bDebug) override;
		bool DeviceIsConnected()override;
		AnalysisConfig GetAnalysisConfig() override;
		void SendToServer() override;
		void DeviceTodo(DeviceIns cmd) override;
		const MachineState& DeviceStates() override;
		void CleanProcedure(int cleanTimes, bool bTest) override;
		void DataReload() override;
		Connection* GetMysqlConnection() override;
		void PushMysqlConnection(Connection* pConnect) override;
		void SetFluidInfo(const FluidInfo& info) override;
		const FluidInfo& GetFluidInfo() override;
		void MakePaths(std::string filePath) override;
		void PostAlarm() override;
	private:
		//私有接口区
		/**
		 * @brief 判断Windows键盘是否打开
		 */
		bool IsWindosKeyboardOpen();
		/**
		 * @brief 打开键盘函数
		 */
		void OpenKeyboard();
		/**
		 * @brief 判断自定义键盘是否显示
		 */
		bool IsShowWinKeyboard();
		/**
		 * @brief 创建窗口函数
		 */
		void CreateWnd();
		/**
		 * @brief 初始化项目配置函数
		 */
		void InitItemConfig();
		/**
		 * @brief 初始化计算信息
		 */
		void InitCalcInfo();

		/**
		 * @brief 启动定时器服务函数
		 */
		void StartTimerService();
		/**
		 * @brief 启动TCP客户端函数
		 */
		void StartTcpClient();
		/**
		 * @brief 启动守护进程函数
		 */
		void StartGuardian();
		/**
		 * @brief 恢复入口函数
		 */
		void RecoveredEntry();

		//系统数据区域
		std::vector<std::thread> globalWorker_;
		std::atomic_bool workHolder_{ true };
		std::string strWorkPath_;												//工作目录

		std::mutex mtxItemConfig_;												//项目配置信息表mutex
		std::map<std::string, Json::Value> mapItemConfig_;						//项目配置信息表

		std::mutex mtxFluidSetup_;
		FluidInfo curFluidSetupInfo_;											//当前fluidsetup


		AnalysisConfig analysisConfig_;											//配置计算参数

		//窗口对象区域
		CMainDlg* pMainDlg_{nullptr};											//主窗口对象
		KeyboardDlg* pKeyboardDlg_{ nullptr };									//数字键盘
		MessageDlg* pMessageDlg_{ nullptr };									//通知消息窗口
		//other
		std::unique_ptr<ModbusDevice> pMbsDevice_;								//modbus设备
		boost::shared_ptr<talk_to_svr> pTcpClient_{ nullptr };					//tcpClient
		std::unique_ptr<ConcurrentTaskQueue> pTaskMgr_{ nullptr };				//event loop
		std::unique_ptr<MysqlConnectionPool> pMysqlPool_;						//mysql连接池对象
	};
}
