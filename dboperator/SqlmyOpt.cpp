#include "SqlmyOpt.h"
#include "LogProcess.h"


CSqlmyOpt::CSqlmyOpt()
{
}


CSqlmyOpt::~CSqlmyOpt()
{
}

bool CSqlmyOpt::open()
{
	close();
	_connPtr = mysql_init(nullptr);
	if (_connPtr == nullptr) {
		//logprocess::Instance()->writeLog("[db]Initialization Mysql Failed.");  //[db]初始化mysql失败
		return false;
	}

	if (!mysql_real_connect(_connPtr, _host.c_str(), _user.c_str(),
		_pass.c_str(), _name.c_str(), atoi(_port.c_str()),
		nullptr, CLIENT_MULTI_STATEMENTS)) {

		//logprocess::Instance()->writeLog("[db]Connect Mysql Failed.", _host, mysql_error(_connPtr));  //连接mysql失败
		return false;
	}
	
	mysql_set_character_set(_connPtr, "utf8");
	setConnect(true);
	m_nReconnectTimes = 0;
	return true;
}

void CSqlmyOpt::close()
{
	setConnect(false);
	if (_connPtr){
		mysql_close(_connPtr);
		_connPtr = nullptr;
	}
}

bool CSqlmyOpt::query(const std::string& sql, DBResultPtr& ptr)
{
	std::lock_guard<std::mutex> lg(_mutex);
	bool success = true;
	while (!isConnected()) {
		if (open()) break;
		//logprocess::Instance()->writeLog("[db]Connect Mysql Failed too."); //重连mysql失败
		std::this_thread::sleep_for(std::chrono::seconds(5));
		m_nReconnectTimes++;
		if (m_nReconnectTimes > 20)
		{
			//logprocess::Instance()->writeLog("[db]Connect Mysql Failed too Many times, exit."); //重连mysql失败多次，退出程序
			std::this_thread::sleep_for(std::chrono::seconds(5));
			_exit(0);
		}
	}
	if (mysql_real_query(_connPtr, sql.c_str(), sql.length())){
		success = false;
		//logprocess::Instance()->writeLog("[db]Execute statement failed.", sql);  //执行语句失败
		unsigned int errcode = 0;
		//logprocess::Instance()->writeLog(getLastError(errcode));
		if (errcode == 2006){
			//logprocess::Instance()->writeLog("[db]Disconnect.");  //断开连接
			close();
		}
	}
	if (success){
		MYSQL_RES* res = mysql_store_result(_connPtr);
		if (success && res)
			ptr = std::make_shared<CSqlmyResult>(res);
		//其他结果集释放掉
		while (mysql_next_result(_connPtr) == 0) {
			MYSQL_RES *p = mysql_store_result(_connPtr);
			if (p)
				mysql_free_result(p);
		}
	}
	m_nReconnectTimes = 0;
	return success;
}

bool CSqlmyOpt::execute(const std::string& sql)
{
	std::lock_guard<std::mutex> lg(_mutex);
	bool success = true;
	while (!isConnected()) {
		if (open()) break;
		//logprocess::Instance()->writeLog("[db]Connect Mysql Failed too."); //重连mysql失败
		std::this_thread::sleep_for(std::chrono::seconds(5));
		m_nReconnectTimes++;
		if (m_nReconnectTimes > 20)
		{
			//logprocess::Instance()->writeLog("[db]Connect Mysql Failed too Many times, exit."); //重连mysql失败多次，退出程序
			std::this_thread::sleep_for(std::chrono::seconds(5));
			_exit(0);
		}
	}
	if (mysql_real_query(_connPtr, sql.c_str(), sql.length())) {
		success = false;
		//logprocess::Instance()->writeLog("[db]Execute statement failed.", sql);   //执行语句失败
		unsigned int errcode = 0;
		//logprocess::Instance()->writeLog(getLastError(errcode));
		if (errcode == 2006) {
			//logprocess::Instance()->writeLog("[db]Disconnect.");   //断开连接
			close();
		}
	}
	if (success)
		clean();
	m_nReconnectTimes = 0;
	return success;
}

std::string CSqlmyOpt::getLastError(unsigned int& errCode)
{
	const char* err = mysql_error(_connPtr);
	errCode = mysql_errno(_connPtr);
	if (err)
		return err;
	return "";
}

void CSqlmyOpt::clean()
{
	MYSQL_RES* res = mysql_store_result(_connPtr);
	if (res)
		mysql_free_result(res);
	while (mysql_next_result(_connPtr) == 0)
	{
		res = mysql_store_result(_connPtr);
		if (res)
			mysql_free_result(res);
	}
}
