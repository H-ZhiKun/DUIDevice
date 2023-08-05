#include "stdafx.h"
#include "MysqlClient.h"
#include "AppFramework.h"
#include "Utils.h"
#include "Logger.h"

using namespace realology;

bool MysqlClient::CreateWellTableGroup(const std::string& wellName)
{
	std::string tableName = wellName;
	if (tableName.empty())
		return false;
	LOGINFO("create for well name = {}", tableName);
	CreateFluidInfo(tableName);
	CreateSampleTable(tableName);
	CreateRecordData(tableName);
	CreateCommentsData(tableName);
	CreateAdvanceData(tableName);
	CreateWellDiag(tableName);
	CreateSensorsRecord(tableName);
	return true;
}

bool MysqlClient::CreateFluidInfo(const std::string& wellName)
{
	ExecSql(SStringA().Format("CREATE TABLE IF NOT EXISTS `%s_fluid_info`  (\
			`id` int UNSIGNED NOT NULL AUTO_INCREMENT,\
			`name` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,\
			`type` varchar(10) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,\
			`density(kg/m^3)` float(20, 6) UNSIGNED NULL DEFAULT NULL,\
			`test_temp(C)` float(20, 6) UNSIGNED NULL DEFAULT NULL,\
			`sample_source` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,\
			`test_time` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,\
			`updated_time` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,\
			PRIMARY KEY(`id`) USING BTREE\
			) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic; ",
		wellName.c_str()).GetBuffer(0));
	return true;
}

bool MysqlClient::CreateOriginData(const std::string& wellName)
{
	SYSTEMTIME s;
	GetLocalTime(&s);
	SStringA sql = SStringA().Format("CREATE TABLE IF NOT EXISTS `%s_%04d_%02d_origindata`  (\
			`id` int UNSIGNED NOT NULL AUTO_INCREMENT,\
			`sample_time` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,\
			`sample_index` tinyint UNSIGNED NOT NULL,\
			`diff_pressure(kPa)` float(20, 6) NULL DEFAULT NULL,\
			`mass_flow(kg/h)` float(20, 6) NULL DEFAULT NULL,\
			`test_temp(C)` float(20, 6)  NULL DEFAULT NULL,\
			`test_density(kg/m^3)` float(20, 6)  NULL DEFAULT NULL,\
			`diff_pressure_str(kPa)` float(20, 6)  NULL DEFAULT NULL,\
			`pump_pressure(kPa)` float(20, 6)  NULL DEFAULT NULL,\
			PRIMARY KEY(`id`) USING BTREE\
			) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic;"
		, wellName.c_str(), s.wYear, s.wMonth);
	ExecSql(sql.GetBuffer(0));
	return true;
}

bool MysqlClient::CreateSampleTable(const std::string& wellName)
{
	SYSTEMTIME s;
	GetLocalTime(&s);
	SStringA sql = SStringA().Format("CREATE TABLE IF NOT EXISTS `%s_%04d_%02d_samples`  (\
			`id` int UNSIGNED NOT NULL AUTO_INCREMENT,\
			`sample_time` varchar(30) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,\
			`sample_index` tinyint UNSIGNED NOT NULL,\
			`volume_flow(m^3/hr)` float(20, 6) NULL DEFAULT NULL,\
			`mass_flow(kg/h)` float(20, 6) NULL DEFAULT NULL,\
			`opening` float(20, 6) NULL DEFAULT NULL,\
			`up_pressure(kPa)` float(20, 6) NULL DEFAULT NULL, \
			`down_pressure(kPa)` float(20, 6) NULL DEFAULT NULL, \
			`diff_pressure(kPa)` float(20, 6) NULL DEFAULT NULL, \
			`temperature(C)` float(20, 6)  NULL DEFAULT NULL,\
			`density(kg/m^3)` float(20, 6)  NULL DEFAULT NULL,\
			`pump_pressure(kPa)` float(20, 6)  NULL DEFAULT NULL,\
			`mud_level(m)` float(20, 6) NULL DEFAULT NULL,\
			PRIMARY KEY(`id`) USING BTREE\
			) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic;"
		, wellName.c_str(), s.wYear, s.wMonth);
	ExecSql(sql.GetBuffer(0));
	return true;
}

bool MysqlClient::CreateAuxillaryData(const std::string& wellName)
{
	SYSTEMTIME s;
	GetLocalTime(&s);
	SStringA sql = SStringA().Format("CREATE TABLE IF NOT EXISTS `%s_%04d_%02d_auxillarydata`  (\
			`id` int UNSIGNED NOT NULL AUTO_INCREMENT,\
			`mud_level(mm)` float(20, 6)  DEFAULT NULL,\
			`density(kg/m3)` float(20, 6)  DEFAULT NULL,\
			`temp(C)` float(20, 6)  DEFAULT NULL,\
			`volume_flow(m3/h)` float(20, 6) DEFAULT NULL,\
			`created_time` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL UNIQUE,\
			PRIMARY KEY(`id`) USING BTREE\
			) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic;"
		, wellName.c_str(), s.wYear, s.wMonth);
	ExecSql(sql.GetBuffer(0));
	return true;
}

bool MysqlClient::CreateRecordData(const std::string& wellName)
{
	SYSTEMTIME s;
	GetLocalTime(&s);
	SStringA sql = SStringA().Format("CREATE TABLE IF NOT EXISTS `%s_RecordData`  (\
			`id` int UNSIGNED NOT NULL AUTO_INCREMENT,\
			`sample_time` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL UNIQUE,\
			`max_temp(C)` float(20, 6)  NULL DEFAULT NULL,\
			`min_temp(C)` float(20, 6)  NULL DEFAULT NULL,\
			`avg_temp(C)` float(20, 6)  NULL DEFAULT NULL,\
			`r600` float(20, 6)  NULL DEFAULT NULL,\
			`r300` float(20, 6)  NULL DEFAULT NULL,\
			`r200` float(20, 6)  NULL DEFAULT NULL,\
			`r100` float(20, 6)  NULL DEFAULT NULL,\
			`r60` float(20, 6)  NULL DEFAULT NULL,\
			`r30` float(20, 6)  NULL DEFAULT NULL,\
			`r20` float(20, 6)  NULL DEFAULT NULL,\
			`r10` float(20, 6)  NULL DEFAULT NULL,\
			`r6` float(20, 6)  NULL DEFAULT NULL,\
			`r3` float(20, 6)  NULL DEFAULT NULL,\
			`r0` float(20, 6)  NULL DEFAULT NULL,\
			`max_density(kg/m^3)` float(20, 6)  NULL DEFAULT NULL,\
			`min_density(kg/m^3)` float(20, 6)  NULL DEFAULT NULL,\
			`avg_density(kg/m^3)` float(20, 6)  NULL DEFAULT NULL,\
			`av(mPas)` float(20, 6)  NULL DEFAULT NULL,\
			`pv(mPas)` float(20, 6)  NULL DEFAULT NULL,\
			`yp(Pa)` float(20, 6)  NULL DEFAULT NULL,\
			`npl` float(20, 6)  NULL DEFAULT NULL,\
			`kpl(Pas^n)` float(20, 6)  NULL DEFAULT NULL,\
			`nhb` float(20, 6)  NULL DEFAULT NULL,\
			`khb(Pas^n)` float(20, 6)  NULL DEFAULT NULL,\
			`tau0(Pa)` float(20, 6)  NULL DEFAULT NULL,\
			`model` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,\
			`nline` float(20, 6)  NULL DEFAULT NULL,\
			`nbh` float(20, 6)  NULL DEFAULT NULL,\
			`kbh` float(20, 6)  NULL DEFAULT NULL,\
			`max_pressure(kPa)` float(20, 6)  NULL DEFAULT NULL,\
			`min_pressure(kPa)` float(20, 6)  NULL DEFAULT NULL,\
			`avg_pressure(kPa)` float(20, 6)  NULL DEFAULT NULL,\
			PRIMARY KEY(`id`) USING BTREE\
			) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic;"
		, wellName.c_str());
	ExecSql(sql.GetBuffer(0));
	return true;
}

bool MysqlClient::CreateSensorsRecord(const std::string& wellName)
{
	SYSTEMTIME s;
	GetLocalTime(&s);
	SStringA sql = SStringA().Format("CREATE TABLE IF NOT EXISTS `%s_SensorsRecord`  (\
			`id` int UNSIGNED NOT NULL AUTO_INCREMENT,\
			`type_name` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,\
			`value` float(20, 6)  NULL DEFAULT NULL,\
			`created_time` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL UNIQUE,\
			PRIMARY KEY(`id`) USING BTREE\
			) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic;"
		, wellName.c_str());
	ExecSql(sql.GetBuffer(0));
	return true;
}

bool MysqlClient::CreateCommentsData(const std::string& wellName)
{
	SYSTEMTIME s;
	GetLocalTime(&s);
	SStringA sql = SStringA().Format("CREATE TABLE IF NOT EXISTS `%s_comments`  (\
			`id` int UNSIGNED NOT NULL AUTO_INCREMENT,\
			`comments_time` varchar(30) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL UNIQUE,\
			`comments` TEXT CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,\
			`created_time` varchar(30) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL UNIQUE,\
			PRIMARY KEY(`id`) USING BTREE\
			) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic;"
		, wellName.c_str());
	ExecSql(sql.GetBuffer(0));
	return true;
}

bool MysqlClient::CreateAdvanceData(const std::string& wellName)
{
	SYSTEMTIME s;
	GetLocalTime(&s);
	SStringA sql = SStringA().Format("CREATE TABLE IF NOT EXISTS `%s_advancedata`  (\
			`id` int UNSIGNED NOT NULL AUTO_INCREMENT,\
			`pump_press(MPa)` float(20, 6)  NULL DEFAULT NULL,\
			`volume_flow(m^3/hr)` float(20, 6) NULL DEFAULT NULL,\
			`diff_press(MPa)` float(20, 6)  NULL DEFAULT NULL,\
			`opening` float(20, 6) NULL DEFAULT NULL,\
			`mud_level(m)` float(20, 6) NULL DEFAULT NULL,\
			`created_time` varchar(30) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL UNIQUE,\
			PRIMARY KEY(`id`) USING BTREE\
			) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic;"
		, wellName.c_str());
	ExecSql(sql.GetBuffer(0));
	return true;
}

bool MysqlClient::CreateWellDiag(const std::string& wellName)
{
	SYSTEMTIME s;
	GetLocalTime(&s);
	SStringA sql = SStringA().Format("CREATE TABLE IF NOT EXISTS `%s_diag`  (\
			`id` int UNSIGNED NOT NULL AUTO_INCREMENT,\
			`type`  varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,\
			`code`	varchar(10) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,\
			`description` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,\
			`created_time` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL ,\
			`updated_time` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL ,\
			PRIMARY KEY(id),\
			KEY idx_code(code) USING BTREE\
			) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic; "
		, wellName.c_str());
	ExecSql(sql.GetBuffer(0));
	return true;
}

ResultSet* MysqlClient::SelectSql(const std::string&& sql)
{
	Connection* con = nullptr;
	Statement* state = nullptr;
	ResultSet* result = nullptr;
	con = App().GetMysqlConnection();
	if (con == nullptr)
	{
		return result;
	}
	state = con->createStatement();
	if (state == nullptr)
	{
		return result;
	}
	try
	{
		result = state->executeQuery(sql);
	}
	catch (SQLException& e)
	{
		LOGINFO(SStringA().Format("SelectSql: %s\n", e.what()));
	}
	if (state)
	{
		delete state;
		state = nullptr;
	}
	App().PushMysqlConnection(con);
	return result;
}

int MysqlClient::UpdateSql(const std::string&& sql)
{
	Connection* con = nullptr;
	Statement* state = nullptr;
	int ret = 0;
	con = App().GetMysqlConnection();
	if (con == nullptr)
	{
		return ret;
	}
	state = con->createStatement();
	if (state == nullptr)
	{
		return ret;
	}
	try
	{
		ret = state->executeUpdate(sql);
	}
	catch (SQLException& e)
	{
		LOGINFO(SStringA().Format("UpdateSql: %s\n", e.what()));
	}
	if (state)
	{
		delete state;
		state = nullptr;
	}
	App().PushMysqlConnection(con);
	return ret;
}

bool MysqlClient::ExecSql(const std::string&& sql)
{
	Connection* con = nullptr;
	Statement* state = nullptr;
	bool ret = 0;
	con = App().GetMysqlConnection();
	if (con == nullptr)
	{
		return ret;
	}
	state = con->createStatement();
	if (state == nullptr)
	{
		return ret;
	}
	try
	{
		ret = state->execute(sql);
	}
	catch (SQLException& e)
	{
		LOGINFO(SStringA().Format("ExecSql: %s\n", e.what()));
	}
	if (state)
	{
		delete state;
		state = nullptr;
	}
	App().PushMysqlConnection(con);
	return ret;
}

bool MysqlClient::LoginVerification(const std::string& userName, const std::string& password)
{
	bool ret = false;
	Connection* con = nullptr;
	Statement* state = nullptr;
	con = App().GetMysqlConnection();
	if (con == nullptr)
	{
		return false;
	}
	state = con->createStatement();
	if (state == nullptr)
	{
		return false;
	}
	try
	{
		std::string code = password;
		std::string key = "!R@e#a$l^o&l*o(g)y-";
		size_t keySize = key.length() - 1;
		for (int i = 0; i <= code.length(); i++)
		{
			code[i] = code[i] ^ key[i % keySize];
		}
		code = Utils::Base64Encode((const unsigned char*)code.c_str(), static_cast<int>(code.length()));
		SStringA sql = SStringA().Format("SELECT COUNT(*) as count FROM `system_user` WHERE username='%s' AND password='%s';",
			userName.c_str(), code.c_str());
		auto result = state->executeQuery(sql.GetBuffer(0));
		if (result->next())
		{
			if (result->getInt("count") > 0)
			{
				ret = true;
			}
		}
		if (result != nullptr)
		{
			delete result;
			result = nullptr;
		}
	}
	catch (SQLException& e)
	{
		LOGINFO(SStringA().Format("LoginVerification: %s\n", e.what()));
	}
	if (state)
	{
		delete state;
		state = nullptr;
	}
	App().PushMysqlConnection(con);
	return ret;
}

bool MysqlClient::GetWellNames(std::vector<std::string>& vName)
{
	auto res = SelectSql(SStringA().Format("SELECT * FROM system_options ORDER BY updated_time").GetBuffer(0));
	if (res != nullptr)
	{
		while (res->next())
		{
			vName.push_back(res->getString("well_name"));
		}
		delete res;
	}
	return true;
}

bool MysqlClient::GetWellOptions(SystemOptionsInfo& info)
{
	std::string wellName = App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME);
	auto res = SelectSql(SStringA().Format("SELECT * FROM system_options WHERE well_name='%s'", wellName.c_str()).GetBuffer(0));
	if (res != nullptr)
	{
		if (res->next())
		{
			info.wellName_ = S_CA2T(res->getString("well_name").c_str(), CP_UTF8);
			info.date_ = S_CA2T(res->getString("date").c_str(), CP_UTF8);
			info.location_ = S_CA2T(res->getString("location").c_str(), CP_UTF8);
			info.field_ = S_CA2T(res->getString("field").c_str(), CP_UTF8);
			info.operator_ = S_CA2T(res->getString("operator").c_str(), CP_UTF8);
			info.rig_ = S_CA2T(res->getString("rig_constractor").c_str(), CP_UTF8);
		}
		delete res;
	}
	return true;
}

bool MysqlClient::UpsertWellOptions(const SystemOptionsInfo& info)
{
	ExecSql(SStringA().Format("INSERT INTO `system_options` (well_name, date, location, field, operator,rig_constractor,updated_time) VALUES\
		('%s', '%s', '%s', '%s','%s', '%s', '%s')\
		ON DUPLICATE KEY UPDATE \
		date = VALUES(date),\
		location=VALUES(location),\
		field=VALUES(field),\
		operator=VALUES(operator),\
		rig_constractor=VALUES(rig_constractor)",
		S_CW2A(info.wellName_, CP_UTF8).GetBuffer(0),
		S_CW2A(info.date_, CP_UTF8).GetBuffer(0),
		S_CW2A(info.location_, CP_UTF8).GetBuffer(0),
		S_CW2A(info.field_, CP_UTF8).GetBuffer(0),
		S_CW2A(info.operator_, CP_UTF8).GetBuffer(0),
		S_CW2A(info.rig_, CP_UTF8).GetBuffer(0),
		Utils::GetDateTime().c_str()).GetBuffer(0));
	return true;
}

bool MysqlClient::InsertOperation(const std::string&& info)
{
	ExecSql(SStringA().Format("INSERT INTO `system_operation_logs` (time, username, event) VALUES\
		('%s', '%s', '%s')",
		Utils::GetDateTime().c_str(), App().GetItemConfig(STRING_RUNTIME, STRING_USERNAME).c_str(), info.c_str()).GetBuffer(0));
	return true;
}

bool MysqlClient::GetOplogsCount(size_t& totals)
{
	SStringA sql = SStringA().Format("select count(id) as count from system_operation_logs");
	auto res = SelectSql(sql.GetBuffer(0));
	if (res != nullptr)
	{
		if (res->next())
		{
			totals = std::stoi(res->getString("count"));
		}
		delete res;
	}
	return false;
}

bool MysqlClient::GetPageOplogs(std::list<OpLogsData>& lvInfo, size_t page, size_t count)
{
	SStringA sql = SStringA().Format("select time,username,event from system_operation_logs order by id desc limit %s,%s",
		to_string((page - 1) * count).c_str(), to_string(count).c_str());
	auto res = SelectSql(sql.GetBuffer(0));

	if (res != nullptr)
	{
		while (res->next())
		{
			OpLogsData item;
			item.time = S_CA2W(res->getString("time").c_str());
			item.user = S_CA2W(res->getString("username").c_str());
			item.event = S_CA2W(res->getString("event").c_str());
			lvInfo.push_back(std::move(item));
		}
		delete res;
	}
	return false;
}


bool MysqlClient::InsertSensors(const std::string& sensorType, float value)
{
	SYSTEMTIME s;
	GetLocalTime(&s);
	ExecSql(SStringA().Format("INSERT INTO `%s_SensorsRecord` (`type_name`,`value`,`created_time`) VALUES\
		('%s',%f,'%s')",
		App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME).c_str(),
		sensorType.c_str(), value, Utils::GetDateTime().c_str()).GetBuffer(0));
	return true;
}

bool MysqlClient::GetLastSensors(const std::string& sensorType, float& value)
{
	auto res = SelectSql(SStringA().Format("SELECT * FROM `%s_SensorsRecord` WHERE type_name='%s' ORDER BY id desc LIMIT 1",
		App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME).c_str(),
		sensorType.c_str()).GetBuffer(0));
	if (res != nullptr)
	{
		if (res->next())
		{
			value = static_cast<float>(res->getDouble("value"));
		}
		delete res;
	}
	return true;
}

bool MysqlClient::GetSensorsRecordInTime(const std::string& sensorType, std::vector<SensorsRecord>& info, const std::string& timeStart, const std::string& timeEnd)
{
	auto res = SelectSql(SStringA().Format("SELECT * FROM `%s_SensorsRecord` WHERE created_time >='%s' AND created_time <='%s'AND type_name='%s'",
		App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME).c_str(),
		timeStart.c_str(), timeEnd.c_str(),
		sensorType.c_str()).GetBuffer(0));
	if (res != nullptr)
	{
		while (res->next())
		{
			SensorsRecord data;
			data.time = res->getString("created_time");
			data.typeName = res->getString("type_name");
			data.value = static_cast<float >(res->getDouble("value"));
			info.push_back(data);
		}
		delete res;
	}
	return true;
}

bool MysqlClient::GetSensorsRecordPage(const std::string& sensorType, std::vector<SensorsRecord>& info, const std::string& timeStart, const std::string& timeEnd, size_t page, size_t count)
{
	SStringA sql = SStringA().Format("SELECT * FROM `%s_SensorsRecord` WHERE created_time >='%s' AND created_time <='%s' AND type_name='%s' order by id desc limit %d,%d",
		App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME).c_str(),
		timeStart.c_str(), timeEnd.c_str(), sensorType.c_str(),
		(page - 1) * count, count);
	auto res = SelectSql(sql.GetBuffer(0));
	if (res != nullptr)
	{
		while (res->next())
		{
			SensorsRecord data;
			data.typeName = res->getString("type_name");
			data.time = res->getString("created_time");
			data.value = static_cast<float>(res->getDouble("value"));
			info.push_back(data);
		}
		delete res;
	}
	return true;
}

bool MysqlClient::GetLastRecord(RecordData& data)
{
	std::string wellName = App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME);
	auto res = SelectSql(SStringA().Format("SELECT * FROM `%s_recorddata` ORDER BY id desc LIMIT 1", 
		wellName.c_str()).GetBuffer(0));
	if (res != nullptr)
	{
		if (res->next())
		{
			data.model = res->getString("model");
			data.time = res->getString("sample_time");
			data.max_temp = static_cast<float >(res->getDouble("max_temp(C)"));
			data.min_temp = static_cast<float >(res->getDouble("min_temp(C)"));
			data.avg_temp = static_cast<float >(res->getDouble("avg_temp(C)"));
			data.max_pressure = static_cast<float >(res->getDouble("max_pressure(kPa)"));
			data.min_pressure = static_cast<float >(res->getDouble("min_pressure(kPa)"));
			data.avg_pressure = static_cast<float >(res->getDouble("avg_pressure(kPa)"));
			data.r600 = static_cast<float >(res->getDouble("r600"));
			data.r300 = static_cast<float >(res->getDouble("r300"));
			data.r200 = static_cast<float >(res->getDouble("r200"));
			data.r100 = static_cast<float >(res->getDouble("r100"));
			data.r60 = static_cast<float >(res->getDouble("r60"));
			data.r30 = static_cast<float >(res->getDouble("r30"));
			data.r20 = static_cast<float >(res->getDouble("r20"));
			data.r10 = static_cast<float >(res->getDouble("r10"));
			data.r6 = static_cast<float >(res->getDouble("r6"));
			data.r3 = static_cast<float >(res->getDouble("r3"));
			data.r0 = static_cast<float >(res->getDouble("r0"));
			data.max_density = static_cast<float >(res->getDouble("max_density(kg/m^3)"));
			data.min_density = static_cast<float >(res->getDouble("min_density(kg/m^3)"));
			data.avg_density = static_cast<float >(res->getDouble("avg_density(kg/m^3)"));

			data.av = static_cast<float >(res->getDouble("av(mPas)"));
			data.pv = static_cast<float >(res->getDouble("pv(mPas)"));
			data.yp = static_cast<float >(res->getDouble("yp(Pa)"));
			data.npl = static_cast<float >(res->getDouble("npl"));
			data.kpl = static_cast<float >(res->getDouble("kpl(Pas^n)"));
			data.nhb = static_cast<float >(res->getDouble("nhb"));
			data.khb = static_cast<float >(res->getDouble("khb(Pas^n)"));
			data.tau0 = static_cast<float>(res->getDouble("tau0(Pa)"));
		}
		delete res;
	}
	return true;
}

bool MysqlClient::GetRecordsInTime(std::vector<RecordData>& lvData,
	const std::string& timeStart, const std::string& timeEnd)
{
	std::string wellName = App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME);
	auto res = SelectSql(SStringA().Format("SELECT * FROM `%s_recorddata` WHERE sample_time >='%s' AND sample_time <='%s'",
		wellName.c_str(), timeStart.c_str(), timeEnd.c_str()).GetBuffer(0));
	if (res != nullptr)
	{
		while (res->next())
		{
			RecordData data;
			data.model = res->getString("model");
			data.time = res->getString("sample_time");
			data.max_temp = static_cast<float>(res->getDouble("max_temp(C)"));
			data.min_temp = static_cast<float>(res->getDouble("min_temp(C)"));
			data.avg_temp = static_cast<float>(res->getDouble("avg_temp(C)"));
			data.max_pressure =static_cast<float>(res->getDouble("max_pressure(kPa)"));
			data.min_pressure =static_cast<float>(res->getDouble("min_pressure(kPa)"));
			data.avg_pressure =static_cast<float>(res->getDouble("avg_pressure(kPa)"));
			data.r600 = static_cast<float>(res->getDouble("r600"));
			data.r300 = static_cast<float>(res->getDouble("r300"));
			data.r200 = static_cast<float>(res->getDouble("r200"));
			data.r100 = static_cast<float>(res->getDouble("r100"));
			data.r60 = static_cast<float>(res->getDouble("r60"));
			data.r30 = static_cast<float>(res->getDouble("r30"));
			data.r20 = static_cast<float>(res->getDouble("r20"));
			data.r10 = static_cast<float>(res->getDouble("r10"));
			data.r6 = static_cast<float>(res->getDouble("r6"));
			data.r3 = static_cast<float>(res->getDouble("r3"));
			data.r0 = static_cast<float>(res->getDouble("r0"));
			data.max_density = static_cast<float>(res->getDouble("max_density(kg/m^3)"));
			data.min_density = static_cast<float>(res->getDouble("min_density(kg/m^3)"));
			data.avg_density = static_cast<float>(res->getDouble("avg_density(kg/m^3)"));

			data.av = static_cast<float>(res->getDouble("av(mPas)"));
			data.pv = static_cast<float>(res->getDouble("pv(mPas)"));
			data.yp = static_cast<float>(res->getDouble("yp(Pa)"));
			data.npl = static_cast<float>(res->getDouble("npl"));
			data.kpl = static_cast<float>(res->getDouble("kpl(Pas^n)"));
			data.nhb = static_cast<float>(res->getDouble("nhb"));
			data.khb = static_cast<float>(res->getDouble("khb(Pas^n)"));
			data.tau0 = static_cast<float>(res->getDouble("tau0(Pa)"));
			lvData.push_back(data);
		}
		delete res;
	}
	return true;
}

bool MysqlClient::GetRecordPage(std::vector<RecordData>& lvData,
	const std::string& timeStart, const std::string& timeEnd, size_t page, size_t count)
{
	std::string wellName = App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME);
	SStringA sql = SStringA().Format("SELECT * FROM `%s_recorddata` WHERE sample_time >='%s' AND sample_time <='%s' order by id desc limit %s,%s",
		wellName.c_str(), timeStart.c_str(), timeEnd.c_str(),
		to_string((page - 1) * count).c_str(), to_string(count).c_str());
	auto res = SelectSql(sql.GetBuffer(0));
	if (res != nullptr)
	{
		while (res->next())
		{
			RecordData data;
			data.model = res->getString("model");
			data.time = res->getString("sample_time");
			data.max_temp = static_cast<float>(res->getDouble("max_temp(C)"));
			data.min_temp = static_cast<float>(res->getDouble("min_temp(C)"));
			data.avg_temp = static_cast<float>(res->getDouble("avg_temp(C)"));
			data.max_pressure = static_cast<float>(res->getDouble("max_pressure(kPa)"));
			data.min_pressure = static_cast<float>(res->getDouble("min_pressure(kPa)"));
			data.avg_pressure = static_cast<float>(res->getDouble("avg_pressure(kPa)"));
			data.r600 = static_cast<float>(res->getDouble("r600"));
			data.r300 = static_cast<float>(res->getDouble("r300"));
			data.r200 = static_cast<float>(res->getDouble("r200"));
			data.r100 = static_cast<float>(res->getDouble("r100"));
			data.r60 = static_cast<float>(res->getDouble("r60"));
			data.r30 = static_cast<float>(res->getDouble("r30"));
			data.r20 = static_cast<float>(res->getDouble("r20"));
			data.r10 = static_cast<float>(res->getDouble("r10"));
			data.r6 = static_cast<float>(res->getDouble("r6"));
			data.r3 = static_cast<float>(res->getDouble("r3"));
			data.r0 = static_cast<float>(res->getDouble("r0"));
			data.max_density = static_cast<float>(res->getDouble("max_density(kg/m^3)"));
			data.min_density = static_cast<float>(res->getDouble("min_density(kg/m^3)"));
			data.avg_density = static_cast<float>(res->getDouble("avg_density(kg/m^3)"));

			data.av = static_cast<float>(res->getDouble("av(mPas)"));
			data.pv = static_cast<float>(res->getDouble("pv(mPas)"));
			data.yp = static_cast<float>(res->getDouble("yp(Pa)"));
			data.npl = static_cast<float>(res->getDouble("npl"));
			data.kpl = static_cast<float>(res->getDouble("kpl(Pas^n)"));
			data.nhb = static_cast<float>(res->getDouble("nhb"));
			data.khb = static_cast<float>(res->getDouble("khb(Pas^n)"));
			data.tau0 = static_cast<float>(res->getDouble("tau0(Pa)"));
			lvData.push_back(data);
		}
		delete res;
	}
	return true;
}


bool MysqlClient::GetFluidInfo(FluidInfo& info)
{
	std::string wellName = App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME);
	auto res = SelectSql(SStringA().Format("SELECT * FROM `%s_fluid_info` ORDER BY id desc LIMIT 1", 
		wellName.c_str()).GetBuffer(0));
	if (res != nullptr)
	{
		if (res->next())
		{
			info.name_ = S_CA2T(res->getString("name").c_str(), CP_UTF8);
			info.type_ = S_CA2T(res->getString("type").c_str(), CP_UTF8);
			info.density_ = std::stof(res->getString("density(kg/m^3)").c_str());
			info.temp_ = std::stof(res->getString("test_temp(C)").c_str());
			info.source_ = S_CA2T(res->getString("sample_source").c_str(), CP_UTF8);
			info.date_ = S_CA2T(res->getString("test_time").c_str(), CP_UTF8);
		}
		delete res;
	}
	return true;
}

bool MysqlClient::InsertFluidInfo(const FluidInfo& info)
{
	std::string wellName = App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME);
	ExecSql(SStringA().Format("INSERT INTO `%s_fluid_info` (name,type,`density(kg/m^3)`,`test_temp(C)`,\
			sample_source,test_time,updated_time) VALUES\
		('%s','%s',%f,%f,'%s','%s','%s')",
		wellName.c_str(),
		S_CW2A(info.name_, CP_UTF8).GetBuffer(0),
		S_CW2A(info.type_, CP_UTF8).GetBuffer(0),
		info.density_,
		info.temp_,
		S_CW2A(info.source_, CP_UTF8).GetBuffer(0),
		S_CW2A(info.date_, CP_UTF8).GetBuffer(0),
		Utils::GetDateTime().c_str()).GetBuffer(0));
	return true;
}

bool MysqlClient::InsertSamples(const std::map<uint16_t, std::vector<SampleData>>& mapData)
{
	std::string wellName = App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME);
	SYSTEMTIME s;
	GetLocalTime(&s);
	SStringA sql = SStringA().Format("INSERT INTO `%s_%04d_%02d_samples` (\
		`sample_time`,`sample_index`,`volume_flow(m^3/hr)`,`mass_flow(kg/h)`,`opening`,\
		`up_pressure(kPa)`,`down_pressure(kPa)`,`diff_pressure(kPa)`,`temperature(C)`,`density(kg/m^3)`,\
		`pump_pressure(kPa)`,`mud_level(m)`) VALUES", wellName.c_str(), s.wYear, s.wMonth);

	for (const auto& indexItem : mapData)
	{
		for (const auto& itemData : indexItem.second)
		{
			sql.Append(SStringA().Format("('%s',%d, %f,%f,%f,%f,%f, %f,%f,%f,%f,%f),",
				itemData.sampleTime.c_str(), indexItem.first,
				itemData.fVolFlow, itemData.massFlow, itemData.fOpening, 
				itemData.fUpstreamPressure, itemData.fDownstreamPressure, itemData.diffPressure, itemData.temperature,itemData.density,
				itemData.pumpPressure, itemData.fMudLevel));
		}
	}
	sql.Delete(sql.GetLength() - 1);
	ExecSql(sql.GetBuffer(0));
	return true;
}

bool MysqlClient::GetItemConfig(std::map<std::string, Json::Value>& config)
{
	auto res = SelectSql("select * from `system_config`");
	if (res != nullptr)
	{
		while (res->next())
		{
			config[res->getString("title")] = Utils::StrToJson(res->getString("config_json"));
		}
	}
	return false;
}

bool MysqlClient::UpdateItemConfig(std::string itemName, Json::Value& itemValue)
{
	UpdateSql(SStringA().Format("UPDATE `system_config` SET `config_json`=\'%s\' WHERE `title`=\'%s\'",
		Utils::JsonToStr(itemValue).c_str(), itemName.c_str()).GetBuffer(0));
	return false;
}

bool MysqlClient::InsertAdvance(const CollectionData& data, const std::string& time)
{
	std::string wellName =App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME);
	std::string recordTime = time;
	if (recordTime.empty())
		recordTime = Utils::GetDateTime();
	ExecSql(SStringA().Format("INSERT INTO `%s_advancedata` (`pump_press(MPa)`, `volume_flow(m^3/hr)`,`diff_press(MPa)`,`opening`,`mud_level(m)`,created_time) VALUES\
		(%f,%f,%f,%f,%f,'%s')",
		wellName.c_str(),
		data.fPumpPressure, data.fVolFlow, data.fDiffPressure, data.fOpening / 100.0, data.fMudLevel / 1000.0,
		recordTime.c_str()
	).GetBuffer(0));
	return true;
}

bool MysqlClient::GetAdvance(AdvanceData& data, const std::string& timeStart)
{
	auto res = SelectSql(SStringA().Format("SELECT * FROM `%s_advancedata` WHERE created_time >='%s' limit 1801",
		App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME).c_str(),
		timeStart.c_str()).GetBuffer(0));
	if (res != nullptr)
	{
		while (res->next())
		{
			data.mapValue[L"checked_press"].push_back(static_cast<float>(res->getDouble("pump_press(MPa)")));
			data.mapValue[L"checked_vflow"].push_back(static_cast<float>(res->getDouble("volume_flow(m^3/hr)")));
			data.mapValue[L"checked_diffpress"].push_back(static_cast<float>(res->getDouble("diff_press(MPa)")));
			data.mapValue[L"checked_opening"].push_back(static_cast<float>(res->getDouble("opening")));
			data.mapValue[L"checked_mlevel"].push_back(static_cast<float>(res->getDouble("mud_level(m)")));
			data.vTime.push_back(res->getString("created_time"));
		}
		delete res;
	}
	return true;
}

bool MysqlClient::InsertWellDiags(const DiagDataItem&& Info)
{
	std::string wellName = App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME);
	SStringA sql;
	if (Info.timeStart_.IsEmpty())
	{
		sql = SStringA().Format("UPDATE `%s_diag` set updated_time='%s' where code='%s' and updated_time=''",
			wellName.c_str(), S_CW2A(Info.timeEnd_), S_CW2A(Info.code_));
	}
	else
	{
		sql = SStringA().Format("Select * from `%s_diag` where code='%s' and updated_time=''",
			wellName.c_str(), S_CW2A(Info.code_));
		auto res = SelectSql(sql.GetBuffer(0));
		if (res != nullptr)
		{
			if (res->next())
			{

				return true;
			}
			delete res;
		}
		sql = SStringA().Format("INSERT `%s_diag` (created_time,updated_time,type,code,description) VALUES('%s','%s','%s','%s','%s')",
			wellName.c_str(), S_CW2A(Info.timeStart_), S_CW2A(Info.timeEnd_), S_CW2A(Info.type_), S_CW2A(Info.code_), S_CW2A(Info.des_));
	}
	ExecSql(sql.GetBuffer(0));
	return true;
}

bool MysqlClient::SelectDiags(std::list<DiagDataItem>& lvInfo, const std::string& type, const std::string& key, const std::string& order, size_t page, size_t count)
{
	std::string wellName = App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME);
	SStringA sql = SStringA().Format("select created_time,updated_time,code,description from `%s_diag` where type='%s' order by %s %s limit %s,%s"
		, wellName.c_str(), type.c_str(), key.c_str(), order.c_str(),
		to_string((page - 1) * count).c_str(), to_string(count).c_str());
	auto res = SelectSql(sql.GetBuffer(0));
	if (res != nullptr)
	{
		while (res->next())
		{
			DiagDataItem item;
			item.timeStart_ = S_CA2W(res->getString("created_time").c_str());
			item.timeEnd_ = S_CA2W(res->getString("updated_time").c_str());
			item.code_ = S_CA2W(res->getString("code").c_str());
			item.des_ = S_CA2W(res->getString("description").c_str());
			lvInfo.push_back(std::move(item));
		}
		delete res;
	}


	return false;
}

bool MysqlClient::SelectDiagsCount(size_t& total, const std::string& type)
{
	std::string wellName = App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME);
	SStringA sql = SStringA().Format("select count(id) as count from `%s_diag` where type='%s'"
		, wellName.c_str(), type.c_str());
	auto res = SelectSql(sql.GetBuffer(0));
	if (res != nullptr)
	{
		if (res->next())
		{
			total = std::stoi(res->getString("count"));
		}
		delete res;
	}
	return false;
}


bool MysqlClient::GetCommentsInTime(std::vector<CommentsData>& info, const std::string& timeStart, const std::string& timeEnd)
{
	SStringA sql = SStringA().Format("select * from `%s_comments` where comments_time>='%s' and comments_time<='%s' order by comments_time"
		, App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME).c_str(),
		timeStart.c_str(), timeEnd.c_str());
	auto res = SelectSql(sql.GetBuffer(0));
	if (res != nullptr)
	{
		while (res->next())
		{
			CommentsData data;
			data.time = res->getString("comments_time");
			data.data = res->getString("comments");
			info.push_back(data);
		}
		delete res;
	}
	return false;
}

bool MysqlClient::InsertComments(CommentsData& data)
{
	SStringA sql = SStringA().Format("INSERT INTO `%s_comments` (comments_time,comments,created_time) VALUES \
		('%s', '%s', '%s')\
		ON DUPLICATE KEY UPDATE \
		comments_time = VALUES(comments_time), \
		comments = VALUES(comments), \
		created_time = VALUES(created_time)",
		App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME).c_str(),
		data.time.c_str(), data.data.c_str(), Utils::GetDateTime().c_str());
	ExecSql(sql.GetBuffer(0));
	return true;
}

bool MysqlClient::GetLastOriginData(std::map<uint16_t, std::vector<SampleData>>& pInSample)
{
	SYSTEMTIME s;
	GetLocalTime(&s);
	std::string tableName = SStringA().Format("%s_%04d_%02d_origindata", 
		App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME).c_str(), s.wYear, s.wMonth);
	SStringA sql = SStringA().Format("select * from `%s` ORDER BY id desc limit 500", tableName.c_str());
	auto res = SelectSql(sql.GetBuffer(0));
	if (res != nullptr)
	{
		while (res->next())
		{
			SampleData data;
			data.diffPressure = std::stof(res->getString("diff_pressure(kPa)").c_str());
			data.massFlow = std::stof(res->getString("mass_flow(kg/h)").c_str());
			data.temperature = std::stof(res->getString("test_temp(C)").c_str());
			data.density = std::stof(res->getString("test_density(kg/m^3)").c_str());
			data.pumpPressure = std::stof(res->getString("pump_pressure(kPa)").c_str());
			//data.sampleTime = res->getString("sample_time");
			int index = res->getInt("sample_index");
			pInSample[index].push_back(data);
		}
		delete res;
	}

	return false;
}

bool MysqlClient::GetOriginDataById(std::map<uint16_t, std::vector<SampleData>>& pInSample, int id, int year, int month)
{
	SStringA sql = SStringA().Format("select * from `%s_%04d_%02d_samples` where id >= %d and id < %d",
		App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME).c_str(),
		year, month, id, id + 100);
	auto res = SelectSql(sql.GetBuffer(0));
	if (res != nullptr)
	{
		while (res->next())
		{
			SampleData data;
			data.fVolFlow = std::stof(res->getString("volume_flow(m^3/hr)").c_str());
			data.fUpstreamPressure = std::stof(res->getString("up_pressure(kPa)").c_str());
			data.fDownstreamPressure = std::stof(res->getString("down_pressure(kPa)").c_str());
			data.diffPressure = std::stof(res->getString("diff_pressure(kPa)").c_str());
			data.massFlow = std::stof(res->getString("mass_flow(kg/h)").c_str());
			data.temperature = std::stof(res->getString("temperature(C)").c_str());
			data.density = std::stof(res->getString("density(kg/m^3)").c_str());
			data.pumpPressure = std::stof(res->getString("pump_pressure(kPa)").c_str());
			data.sampleTime = res->getString("sample_time");
			uint16_t index = res->getInt("sample_index");
			pInSample[index].push_back(data);
		}
		delete res;
	}

	return false;
}

bool MysqlClient::InsertRecord(const RecordData& record)
{
	SStringA sql = SStringA().Format(" INSERT INTO `%s_recorddata` VALUES \
		(null, \'%s\', %.6f, %.6f, %.6f, \
		%.6f, %.6f, %.6f, %.6f, %.6f, \
		%.6f, %.6f, %.6f, %.6f, %.6f, \
		%.6f, %.6f, %.6f, %.6f, %.6f, \
		%.6f, %.6f, %.6f, %.6f, %.6f, \
		%.6f, %.6f, \'%s\', %.6f, %.6f,\
		%.6f, %.6f, %.6f, %.6f)"
		, App().GetItemConfig(STRING_RUNTIME, STRING_WELLNAME).c_str(),
		record.time.c_str(), record.max_temp, record.min_temp, record.avg_temp,
		record.r600, record.r300, record.r200, record.r100, record.r60,
		record.r30, record.r20, record.r10, record.r6, record.r3,
		record.r0, record.max_density, record.min_density, record.avg_density, record.av,
		record.pv, record.yp, record.npl, record.kpl, record.nhb,
		record.khb, record.tau0, record.model.c_str(), record.nline, record.nbh,
		record.kbh, record.max_pressure, record.min_pressure, record.avg_pressure);
	ExecSql(sql.GetBuffer(0));
	return true;
}
