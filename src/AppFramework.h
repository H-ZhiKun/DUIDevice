#pragma once
#include "stdafx.h"
#include "NonCopyable.h"
#include "SystemDef.h"
#include "MysqlConnectionPool.h"

namespace realology
{
	class AppFramework : public NonCopyable
	{
	public:
		virtual ~AppFramework() = default;
		/**
		 * @brief ��ȡӦ�ó����ܵ�Ψһʵ��
		 * @details �ú�������Ӧ�ó����ܵ�Ψһʵ�����ǵ���ģʽ��ʵ�֡��ú����������κβ�����
		 * @return ����AppFramework���͵����ã���ʾӦ�ó����ܵ�Ψһʵ����
		 */
		static AppFramework& instance();
		/**
		 * @brief ���к���������Ӧ�ó���
		 *
		 * @param theApp ָ��Ӧ�ó���ʵ����ָ��
		 * @return �������н����0 ��ʾ�ɹ�������ֵ��ʾʧ��
		 */
		virtual int Run(SApplication* theApp) = 0;
		/**
		 * @brief �˳�Ӧ�ó���Ĵ��麯��
		 */
		virtual void Quit() = 0;
		/**
		 * @brief ��ʾ������ϵͳ���̵Ľӿ�
		 * @param bShow �Ƿ���ʾϵͳ����
		 */
		virtual void ShowWinKeyboard(bool bShow) = 0;
		/**
		 * @brief ��ʾ�������Զ�����̵Ľӿ�
		 * @param bShow �Ƿ���ʾ�Զ������
		 * @param obj �Զ�����̵�ָ��
		 */
		virtual void ShowCustomKeyboard(bool bShow, SEdit* obj) = 0;
		/**
		 * @brief ��ȡ����·���Ľӿ�
		 * @return ����·�����ַ���
		 */
		virtual std::string GetWorkPath() = 0;
		/**
		 * @brief �����첽�¼��Ľӿ�
		 * @param iEventId �¼�ID
		 * @param wParam ���Ӳ���
		 * @param lParam ������Ϣ
		 */
		virtual void AsyncEvent(int iEventId, int wParam = 0, TransportMsg* lParam = 0) = 0;
		/**
		 * @brief �����첽����Ľӿ�
		 * @param task �첽������
		 */
		virtual void AsyncTask(const std::function<void()>& task) = 0;
		/**
		 * @brief ��ʾ��Ϣ��Ľӿ�
		 * @param title ��Ϣ�����
		 * @param des ��Ϣ������
		 * @param task �����Ϣ���Ҫִ�е�������
		 */
		virtual void ShowMessage(const SStringT&& title, const SStringT&& des, std::function<void(void)> task = nullptr) = 0;
		/**
		 * @brief ҳ���л��Ľӿ�
		 * @param page Ҫ�л���ҳ��
		 */
		virtual void DoPageSwitch(PageSwitch page) = 0;
		/**
		 * @brief ��ȡָ����Ŀ��������Ϣ�Ĵ��麯��
		 * @param itemName ��Ŀ����
		 * @param itemKey ��Ŀ�Ĺؼ���
		 * @return ������Ϣ���ַ���
		 */
		virtual std::string GetItemConfig(const std::string& itemName, const std::string& itemKey) = 0;
		/**
		 * @brief ��ȡָ����Ŀ��������Ϣ�Ľӿ�
		 * @param itemName ��Ŀ����
		 * @return ������Ϣ��Json����
		 */
		virtual Json::Value GetItemConfig(const std::string& itemName) = 0;
		/**
		 * @brief ��ȡ������Ŀ��������Ϣ�Ľӿ�
		 * @return ������Ŀ��������Ϣ��ӳ���
		 */
		virtual std::map<std::string, Json::Value> GetItemConfig() = 0;
		/**
		 * @brief ����ָ����Ŀ��������Ϣ�Ľӿ�
		 * @param title ��Ŀ����
		 * @param item ��Ŀ����
		 * @param val ��Ŀֵ
		 */
		virtual void SetItemConfig(const std::string& title, const std::string& item, const std::string& val) = 0;
		/**
		 * @brief ��ȡʵʱ�������ݽӿ�
		 * @return ʵʱ���ݶ���
		 */
		virtual const AdvanceData& GetAdvanceReal() = 0;
		/**
		 * @brief ��ȡԭʼ�ɼ�����
		 * @return ����ԭʼ�ɼ�����
		 */
		virtual const CollectionData& GetCollection() = 0;
		/**
		 * @brief ��ȡ��������¼����
		 * @return ���ؼ�����������
		 */
		virtual const RecordData& GetRecord() = 0;
		/**
		 * @brief ��ȡ�ɼ����������ݵ�
		 * @return ���زɼ����������ݵ�
		 */
		virtual const std::vector<RealF>& GetPoints() = 0;
		/**
		 * @brief ���ø߼����Կ���
		 * @param bDebug �Ƿ����߼�����ģʽ
		 */
		virtual void SetAdvanceDebug(bool bDebug) = 0;
		/**
		 * @brief ����豸�Ƿ�����
		 * @return ����豸�����ӣ��򷵻� true�����򷵻� false
		 */
		virtual bool DeviceIsConnected() = 0;
		/**
		 * @brief ��ȡ�豸״̬
		 * @return �����豸״̬���������
		 */
		virtual const MachineState& DeviceStates() = 0;
		/**
		 * @brief ��ȡ����������Ϣ
		 * @return ���ط������ö���
		 */
		virtual AnalysisConfig GetAnalysisConfig() = 0;
		/**
		 * @brief �������ݵ�������
		 */
		virtual void SendToServer() = 0;
		/**
		 * @brief �豸ָ�����
		 * @param cmd �豸ָ��
		 */
		virtual void DeviceTodo(DeviceIns cmd) = 0;
		/**
		 * @brief ��ϴ����
		 * @param cleanTimes ��ϴ����
		 * @param bTest �Ƿ����
		 */
		virtual void CleanProcedure(int cleanTimes, bool bTest) = 0;
		/**
		 * @brief ��������
		 */
		virtual void DataReload() = 0;
		/**
		 * @brief ��ȡMySQL����
		 * @return ����MySQL���Ӷ���ָ��
		 */
		virtual Connection* GetMysqlConnection() = 0;
		/**
		 * @brief ����MySQL����
		 * @param pConnect MySQL���Ӷ���ָ��
		 */
		virtual void PushMysqlConnection(Connection* pConnect) = 0;
		/**
		 * @brief ����������Ϣ
		 * @param info ������Ϣ����
		 */
		virtual void SetFluidInfo(const FluidInfo& info) = 0;
		/**
		 * @brief ��ȡ������Ϣ
		 * @return ����������Ϣ���������
		 */
		virtual const FluidInfo& GetFluidInfo() = 0;
		/**
		 * @brief �����ļ�·��
		 * @param filePath �ļ�·��
		 */
		virtual void MakePaths(std::string filePath) = 0;
		/**
		 * @brief �ύ�澯
		 */
		virtual void PostAlarm() = 0;
	};
	inline AppFramework& App()
	{
		return AppFramework::instance();
	}
}

