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
		//˽�нӿ���
		/**
		 * @brief �ж�Windows�����Ƿ��
		 */
		bool IsWindosKeyboardOpen();
		/**
		 * @brief �򿪼��̺���
		 */
		void OpenKeyboard();
		/**
		 * @brief �ж��Զ�������Ƿ���ʾ
		 */
		bool IsShowWinKeyboard();
		/**
		 * @brief �������ں���
		 */
		void CreateWnd();
		/**
		 * @brief ��ʼ����Ŀ���ú���
		 */
		void InitItemConfig();
		/**
		 * @brief ��ʼ��������Ϣ
		 */
		void InitCalcInfo();

		/**
		 * @brief ������ʱ��������
		 */
		void StartTimerService();
		/**
		 * @brief ����TCP�ͻ��˺���
		 */
		void StartTcpClient();
		/**
		 * @brief �����ػ����̺���
		 */
		void StartGuardian();
		/**
		 * @brief �ָ���ں���
		 */
		void RecoveredEntry();

		//ϵͳ��������
		std::vector<std::thread> globalWorker_;
		std::atomic_bool workHolder_{ true };
		std::string strWorkPath_;												//����Ŀ¼

		std::mutex mtxItemConfig_;												//��Ŀ������Ϣ��mutex
		std::map<std::string, Json::Value> mapItemConfig_;						//��Ŀ������Ϣ��

		std::mutex mtxFluidSetup_;
		FluidInfo curFluidSetupInfo_;											//��ǰfluidsetup


		AnalysisConfig analysisConfig_;											//���ü������

		//���ڶ�������
		CMainDlg* pMainDlg_{nullptr};											//�����ڶ���
		KeyboardDlg* pKeyboardDlg_{ nullptr };									//���ּ���
		MessageDlg* pMessageDlg_{ nullptr };									//֪ͨ��Ϣ����
		//other
		std::unique_ptr<ModbusDevice> pMbsDevice_;								//modbus�豸
		boost::shared_ptr<talk_to_svr> pTcpClient_{ nullptr };					//tcpClient
		std::unique_ptr<ConcurrentTaskQueue> pTaskMgr_{ nullptr };				//event loop
		std::unique_ptr<MysqlConnectionPool> pMysqlPool_;						//mysql���ӳض���
	};
}
