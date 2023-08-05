#include "stdafx.h"
#include "Utils.h"
#include <fstream>
#include <vector>
#include "Logger.h"
#include <tlhelp32.h>
#include <shellapi.h>

std::string Utils::Base64Encode(const unsigned char* str, int bytes)
{
	std::string _base64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int num = 0, bin = 0;
	std::string _encode_result;
	const unsigned char* current;
	current = str;
	while (bytes > 2) {
		_encode_result += _base64_table[current[0] >> 2];
		_encode_result += _base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
		_encode_result += _base64_table[((current[1] & 0x0f) << 2) + (current[2] >> 6)];
		_encode_result += _base64_table[current[2] & 0x3f];

		current += 3;
		bytes -= 3;
	}
	if (bytes > 0)
	{
		_encode_result += _base64_table[current[0] >> 2];
		if (bytes % 3 == 1) {
			_encode_result += _base64_table[(current[0] & 0x03) << 4];
			_encode_result += "==";
		}
		else if (bytes % 3 == 2) {
			_encode_result += _base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
			_encode_result += _base64_table[(current[1] & 0x0f) << 2];
			_encode_result += "=";
		}
	}
	return _encode_result;
}

std::string Utils::Base64Decode(const char* str, int length)
{
	char base64_pad = '=';
	const char DecodeTable[] =
	{
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -2, -2, -1, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 62, -2, -2, -2, 63,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -2, -2, -2, -2, -2, -2,
		-2,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -2, -2, -2, -2, -2,
		-2, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2
	};
	int bin = 0, i = 0, pos = 0;
	std::string _decode_result;
	const char* current = str;
	char ch;
	while ((ch = *current++) != '\0' && length-- > 0)
	{
		if (ch == base64_pad) { // 当前一个字符是“=”号
			/*
			先说明一个概念：在解码时，4个字符为一组进行一轮字符匹配。
			两个条件：
				1、如果某一轮匹配的第二个是“=”且第三个字符不是“=”，说明这个带解析字符串不合法，直接返回空
				2、如果当前“=”不是第二个字符，且后面的字符只包含空白符，则说明这个这个条件合法，可以继续。
			*/
			if (*current != '=' && (i % 4) == 1) {
				return NULL;
			}
			continue;
		}
		ch = DecodeTable[ch];
		//这个很重要，用来过滤所有不合法的字符
		if (ch < 0) { /* a space or some other separator character, we simply skip over */
			continue;
		}
		switch (i % 4)
		{
		case 0:
			bin = ch << 2;
			break;
		case 1:
			bin |= ch >> 4;
			_decode_result += bin;
			bin = (ch & 0x0f) << 4;
			break;
		case 2:
			bin |= ch >> 2;
			_decode_result += bin;
			bin = (ch & 0x03) << 6;
			break;
		case 3:
			bin |= ch;
			_decode_result += bin;
			break;
		}
		i++;
	}
	return _decode_result;
}

std::string Utils::GetDateTime(bool bMillisecond)
{
	SYSTEMTIME s;
	GetLocalTime(&s);
	std::string time;
	if (bMillisecond)
	{
		time = SStringA().Format("%4d/%02d/%02d %02d:%02d:%02d.%03d",
			s.wYear, s.wMonth, s.wDay, s.wHour, s.wMinute, s.wSecond, s.wMilliseconds)
			.GetBuffer(0);
	}
	else
	{
		time = SStringA().Format("%4d/%02d/%02d %02d:%02d:%02d",
			s.wYear, s.wMonth, s.wDay, s.wHour, s.wMinute, s.wSecond)
			.GetBuffer(0);
	}
	return time;
}

std::string Utils::GetDateTimeFrom(const std::string& dateTime, long intervalSeconds)
{
	if (dateTime.empty())
	{
		return "";
	}
	struct tm* p;
	tm temptm;
	memset(&temptm, 0, sizeof(temptm));
	auto rc = sscanf(dateTime.c_str(), "%d/%d/%d %d:%d:%d",
		&temptm.tm_year, &temptm.tm_mon, &temptm.tm_mday,
		&temptm.tm_hour, &temptm.tm_min, &temptm.tm_sec);
	temptm.tm_year -= 1900;
	temptm.tm_mon--;
	time_t curTick = mktime(&temptm);

	curTick += intervalSeconds;

	p = localtime(&curTick);
	return SStringA().Format("%4d/%02d/%02d %02d:%02d:%02d",
		p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec)
		.GetBuffer(0);
}

SYSTEMTIME Utils::DateTimeToStruct(const std::string& dateTime)
{
	SYSTEMTIME s{0};
	if (!dateTime.empty())
	{
		auto ret = sscanf(dateTime.c_str(), "%hd/%hd/%hd %hd:%hd:%hd",
			&s.wYear, &s.wMonth, &s.wDay,
			&s.wHour, &s.wMinute, &s.wSecond);
	}
	return s;
}


Json::Value Utils::StrToJson(const std::string& jsString)
{
	Json::Value root;
	JSONCPP_STRING errs;
	Json::CharReaderBuilder readerBuilder;
	std::unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());
	jsonReader->parse(jsString.c_str(), jsString.c_str() + jsString.length(), &root, &errs);
	return root;
}

std::string Utils::JsonToStr(const Json::Value& jsVal)
{
	static Json::Value def = []() {
		Json::Value def;
		Json::StreamWriterBuilder::setDefaults(&def);
		def["emitUTF8"] = true;
		return def;
	}();

	std::ostringstream stream;
	Json::StreamWriterBuilder stream_builder;
	stream_builder.settings_ = def;//Config emitUTF8
	std::unique_ptr<Json::StreamWriter> writer(stream_builder.newStreamWriter());
	writer->write(jsVal, &stream);
	return stream.str();
}

Json::Value Utils::ReadJsonFile(const std::string& filePath)
{
	Json::Value jsVal;
	std::ifstream is(filePath);
	if (!is.is_open()) return jsVal;
	std::ostringstream osBuf;
	char ch;
	while (osBuf && is.get(ch)) osBuf.put(ch);
	jsVal = StrToJson(osBuf.str());
	is.close();
	return jsVal;
}

bool Utils::WriteJsonFile(const std::string& filePath, const Json::Value& jsVal)
{
	std::ofstream outfile(filePath);
	if (!outfile.is_open()) return false;
	std::string jsString = JsonToStr(jsVal);
	outfile << jsString;
	outfile.close();
	return true;
}


bool Utils::FindProcess(const SStringT& strProcessName)
{
	//查找进程
	DWORD nPid;
	STARTUPINFO st;
	PROCESS_INFORMATION pi;
	PROCESSENTRY32 ps;
	HANDLE hSnapshot;
	memset(&st, 0, sizeof(STARTUPINFO));
	st.cb = sizeof(STARTUPINFO);
	memset(&ps, 0, sizeof(PROCESSENTRY32));
	ps.dwSize = sizeof(PROCESSENTRY32);
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));
	// 遍历进程 
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return false;
	if (!Process32First(hSnapshot, &ps))
		return false;
	do {
		if (strProcessName == ps.szExeFile)
		{
			//找到制定的程序
			nPid = ps.th32ProcessID;
			CloseHandle(hSnapshot);
			return true;
		}
	} while (Process32Next(hSnapshot, &ps));
	CloseHandle(hSnapshot);
	return false;
}

bool Utils::StartProcess(SStringT strProcessName)
{
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	if (CreateProcess(NULL, (LPWSTR)strProcessName.GetBuffer(0), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
	}
	return true;
}

std::vector<std::string> Utils::FindAllFiles(const std::string& path)
{
	std::vector<std::string> vFiles;
	_finddata_t file_info;
	//可以定义后面的后缀为*.exe，*.txt等来查找特定后缀的文件，*.*是通配符，匹配所有类型,路径连接符最好是左斜杠/，可跨平台
	std::string current_path = path + "\\*.*";
	int64_t handle = _findfirst(current_path.c_str(), &file_info);
	//返回值为-1则查找失败  
	if (-1 != handle)
	{
		do
		{
			if (file_info.attrib != _A_SUBDIR) //是目录  
				vFiles.emplace_back(file_info.name);
		} while (!_findnext(handle, &file_info));
		//关闭文件句柄  
		_findclose(handle);
	}
	return vFiles;
}

bool Utils::RemoveOutdatedLog(const std::string& currentTime, const std::string& path)
{
	std::string lastTime = GetDateTimeFrom(currentTime, -(5 * 24 * 60 * 60));
	lastTime = lastTime.substr(0, lastTime.find_first_of(' '));
	SStringA date = lastTime.c_str();
	date.Replace('/', '-');
	lastTime = SStringA().Format("daily_%s.log", date).GetBuffer(0);
	std::vector<std::string> files = FindAllFiles(path);
	for (auto& item:files)
	{
		if (item <= lastTime)
		{
			std::string file = path + "\\" + item;
			std::remove(file.c_str());
		}
	}
	return false;
}
