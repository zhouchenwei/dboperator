#include "SqliteOpt.h"

CSqliteOpt::CSqliteOpt()
{
}


CSqliteOpt::~CSqliteOpt()
{
}

bool CSqliteOpt::open()
{
	close();
	if (sqlite3_open_v2(_host.c_str(), &_sqlite, 
 		SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE|SQLITE_OPEN_NOMUTEX, nullptr) != SQLITE_OK){
		//logprocess::Instance()->writeLog("[db]Failed to opem local database",_host);   //打开本地数据库失败
		return false; 
	}
	setConnect(true);
	return true;
}

void CSqliteOpt::close()
{
	setConnect(false);
	if (_sqlite) {
		sqlite3_close(_sqlite);
		_sqlite = nullptr;
	}
}

bool CSqliteOpt::query(const std::string& sql, DBResultPtr& ptr)
{
	std::lock_guard<std::mutex> lg(_mutex);
	while (!isConnected()) {
		if (open()) break;
// 		ostringstream oslog;
// 		oslog << "Reconnect database failed。" << _host;  //重连数据库失败
// 		logprocess::Instance()->writeLog(oslog.str());
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
	bool success = true;
	char **reslut = nullptr;
	char* err = nullptr;
	int row = 0, col = 0;
	int r = sqlite3_get_table(_sqlite, sql.c_str(), &reslut, &row, &col, &err);
	if (r!= SQLITE_OK){
		success = false;
// 		ostringstream oslog;
// 		oslog << "Execute statement failed:" << r << "->" << sql;  //执行语句失败
// 		logprocess::Instance()->writeLog(oslog.str());
		if (err) {
// 			ostringstream oslog;
// 			oslog << "Wrong Reason" << err;  //错误原因
// 			logprocess::Instance()->writeLog(oslog.str());
		}
		if (reslut)
			sqlite3_free_table(reslut);
	}
	if (reslut && success)
		ptr = std::make_shared<CSqliteResult>(reslut, row, col);
	
	return success;
}

bool CSqliteOpt::execute(const std::string& sql)
{
	std::lock_guard<std::mutex> lg(_mutex);
	while (!isConnected()) {
		if (open()) break;
// 		ostringstream oslog;
// 		oslog << "Reconnect database failed。" << _host;  //重连数据库失败
// 		logprocess::Instance()->writeLog(oslog.str());
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
	bool success = true;
	char* err = nullptr;
	int r = sqlite3_exec(_sqlite, sql.c_str(), nullptr, nullptr, &err);
	if (r != SQLITE_OK){
		success = false;
// 		ostringstream oslog;
// 		oslog << "Execute statement failed:" << r << "->" << sql;   //执行语句失败
// 		logprocess::Instance()->writeLog(oslog.str());
		if (err) {
// 			ostringstream oslog;
// 			oslog << "Wrong Reason" << err;   //错误原因
// 			logprocess::Instance()->writeLog(oslog.str());
		}
	}
	return success;
}

const std::string CSqliteOpt::getLastError(long& errCode)
{
	errCode = sqlite3_errcode(_sqlite);
	const char *perr = sqlite3_errmsg(_sqlite);
	return perr == nullptr ? "" : perr;
}

