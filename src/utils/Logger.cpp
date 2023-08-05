#include "stdafx.h"
#include "Logger.h"

void Logger::InitLogger(std::string logPath)
{
	// ���ļ���С
	//auto file_logger = spdlog::rotating_logger_mt("file_log", "log/log.log", 1024 * 1024 * 100, 3);
	// ÿ��2:00 am �½�һ����־�ļ�
	auto logger = spdlog::daily_logger_mt("daily_logger", logPath + "/daily.log", 2, 30);
	// ����warn flush��־����ֹ��ʧ
	logger->flush_on(spdlog::level::warn);
	//ÿ����ˢ��һ��
	spdlog::flush_every(std::chrono::seconds(3));

	// Set the default logger to file logger
	auto console = spdlog::stdout_color_mt("console");
	spdlog::set_default_logger(console);
	spdlog::set_level(spdlog::level::debug); // Set global log level to debug

	// change log pattern
	// %s���ļ���
	// %#���к�
	// %!��������
	spdlog::set_pattern("%Y-%m-%d %H:%M:%S [%l] [%t] - <%s>|<%#>|<%!>,%v");
}

void Logger::CloseLogger()
{
	// Release and close all loggers
	spdlog::drop_all();
}