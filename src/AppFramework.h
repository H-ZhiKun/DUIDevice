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
		 * @brief 获取应用程序框架的唯一实例
		 * @details 该函数返回应用程序框架的唯一实例，是单例模式的实现。该函数不接收任何参数。
		 * @return 返回AppFramework类型的引用，表示应用程序框架的唯一实例。
		 */
		static AppFramework& instance();
		/**
		 * @brief 运行函数，启动应用程序
		 *
		 * @param theApp 指向应用程序实例的指针
		 * @return 返回运行结果，0 表示成功，非零值表示失败
		 */
		virtual int Run(SApplication* theApp) = 0;
		/**
		 * @brief 退出应用程序的纯虚函数
		 */
		virtual void Quit() = 0;
		/**
		 * @brief 显示或隐藏系统键盘的接口
		 * @param bShow 是否显示系统键盘
		 */
		virtual void ShowWinKeyboard(bool bShow) = 0;
		/**
		 * @brief 显示或隐藏自定义键盘的接口
		 * @param bShow 是否显示自定义键盘
		 * @param obj 自定义键盘的指针
		 */
		virtual void ShowCustomKeyboard(bool bShow, SEdit* obj) = 0;
		/**
		 * @brief 获取工作路径的接口
		 * @return 工作路径的字符串
		 */
		virtual std::string GetWorkPath() = 0;
		/**
		 * @brief 触发异步事件的接口
		 * @param iEventId 事件ID
		 * @param wParam 附加参数
		 * @param lParam 附加消息
		 */
		virtual void AsyncEvent(int iEventId, int wParam = 0, TransportMsg* lParam = 0) = 0;
		/**
		 * @brief 触发异步任务的接口
		 * @param task 异步任务函数
		 */
		virtual void AsyncTask(const std::function<void()>& task) = 0;
		/**
		 * @brief 显示消息框的接口
		 * @param title 消息框标题
		 * @param des 消息框描述
		 * @param task 点击消息框后要执行的任务函数
		 */
		virtual void ShowMessage(const SStringT&& title, const SStringT&& des, std::function<void(void)> task = nullptr) = 0;
		/**
		 * @brief 页面切换的接口
		 * @param page 要切换的页面
		 */
		virtual void DoPageSwitch(PageSwitch page) = 0;
		/**
		 * @brief 获取指定项目的配置信息的纯虚函数
		 * @param itemName 项目名称
		 * @param itemKey 项目的关键字
		 * @return 配置信息的字符串
		 */
		virtual std::string GetItemConfig(const std::string& itemName, const std::string& itemKey) = 0;
		/**
		 * @brief 获取指定项目的配置信息的接口
		 * @param itemName 项目名称
		 * @return 配置信息的Json对象
		 */
		virtual Json::Value GetItemConfig(const std::string& itemName) = 0;
		/**
		 * @brief 获取所有项目的配置信息的接口
		 * @return 所有项目的配置信息的映射表
		 */
		virtual std::map<std::string, Json::Value> GetItemConfig() = 0;
		/**
		 * @brief 设置指定项目的配置信息的接口
		 * @param title 项目标题
		 * @param item 项目名称
		 * @param val 项目值
		 */
		virtual void SetItemConfig(const std::string& title, const std::string& item, const std::string& val) = 0;
		/**
		 * @brief 获取实时分析数据接口
		 * @return 实时数据对象
		 */
		virtual const AdvanceData& GetAdvanceReal() = 0;
		/**
		 * @brief 获取原始采集数据
		 * @return 返回原始采集数据
		 */
		virtual const CollectionData& GetCollection() = 0;
		/**
		 * @brief 获取计算结果记录数据
		 * @return 返回计算结果的引用
		 */
		virtual const RecordData& GetRecord() = 0;
		/**
		 * @brief 获取采集计算结果数据点
		 * @return 返回采集计算结果数据点
		 */
		virtual const std::vector<RealF>& GetPoints() = 0;
		/**
		 * @brief 设置高级调试开关
		 * @param bDebug 是否开启高级调试模式
		 */
		virtual void SetAdvanceDebug(bool bDebug) = 0;
		/**
		 * @brief 检查设备是否连接
		 * @return 如果设备已连接，则返回 true，否则返回 false
		 */
		virtual bool DeviceIsConnected() = 0;
		/**
		 * @brief 获取设备状态
		 * @return 返回设备状态对象的引用
		 */
		virtual const MachineState& DeviceStates() = 0;
		/**
		 * @brief 获取分析配置信息
		 * @return 返回分析配置对象
		 */
		virtual AnalysisConfig GetAnalysisConfig() = 0;
		/**
		 * @brief 发送数据到服务器
		 */
		virtual void SendToServer() = 0;
		/**
		 * @brief 设备指令操作
		 * @param cmd 设备指令
		 */
		virtual void DeviceTodo(DeviceIns cmd) = 0;
		/**
		 * @brief 清洗过程
		 * @param cleanTimes 清洗次数
		 * @param bTest 是否测试
		 */
		virtual void CleanProcedure(int cleanTimes, bool bTest) = 0;
		/**
		 * @brief 数据重载
		 */
		virtual void DataReload() = 0;
		/**
		 * @brief 获取MySQL连接
		 * @return 返回MySQL连接对象指针
		 */
		virtual Connection* GetMysqlConnection() = 0;
		/**
		 * @brief 返还MySQL连接
		 * @param pConnect MySQL连接对象指针
		 */
		virtual void PushMysqlConnection(Connection* pConnect) = 0;
		/**
		 * @brief 设置流体信息
		 * @param info 流体信息对象
		 */
		virtual void SetFluidInfo(const FluidInfo& info) = 0;
		/**
		 * @brief 获取流体信息
		 * @return 返回流体信息对象的引用
		 */
		virtual const FluidInfo& GetFluidInfo() = 0;
		/**
		 * @brief 生成文件路径
		 * @param filePath 文件路径
		 */
		virtual void MakePaths(std::string filePath) = 0;
		/**
		 * @brief 提交告警
		 */
		virtual void PostAlarm() = 0;
	};
	inline AppFramework& App()
	{
		return AppFramework::instance();
	}
}

