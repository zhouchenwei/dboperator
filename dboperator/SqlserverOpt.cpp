#include "SqlserverOpt.h"
#include "LogProcess.h"

#ifdef _WIN32
CSqlserverOpt::CSqlserverOpt()
{
}


CSqlserverOpt::~CSqlserverOpt()
{
}

bool CSqlserverOpt::open()
{
	close();
	HRESULT hr;
	if (_connPtr == nullptr){
		/*CoInitialize(nullptr);*/
		hr = _connPtr.CreateInstance("ADODB.Connection");
		if (!SUCCEEDED(hr))
			return false;
	}
	if (_host.empty() && _user.empty() && _pass.empty() && _name.empty()){
		//logprocess::Instance()->writeLog("Database Connection Information Not Configured.");  //未配置数据库连接信息
		return false;
	}

	std::ostringstream connFmt;
	connFmt << "Provider=SQLOLEDB.1;Password=" << _pass << ";Persist Security Info=TRUE;User ID=" << _user << ";Initial Catalog=" << _name << ";Data Source=" << _host << "," << _port;
	hr = _connPtr->raw_Open(_bstr_t(connFmt.str().c_str()), bstr_t(""), _bstr_t(""), adModeUnknown);
	if (!SUCCEEDED(hr)) {
		long e = 0;
// 		ostringstream oslog;
// 		oslog << "Open sqlserver failed." << connFmt.str() << getLastError(e);
// 		logprocess::Instance()->writeLog(oslog.str());
		return false;
	}
	setConnect(true);
	return true;
}

void CSqlserverOpt::close()
{
	setConnect(false);
	if (_connPtr == nullptr)
		return;
	/*if ((_connPtr->State&adStateOpen) == adStateOpen)*/
	_connPtr->raw_Close();

	_connPtr.Release();
	_connPtr = nullptr;
}

bool CSqlserverOpt::query(const std::string& sql, DBResultPtr& ptr)
{
	int nReconnectTimes = 0;
	_RecordsetPtr	mRst = nullptr;
	bool success = true;
	_mutex.lock();
	while (!isConnected()) {
		if (open()) break;
		if (nReconnectTimes > 20)
		{
			//logprocess::Instance()->writeLog("Reconnect too many times, exit!");
			std::this_thread::sleep_for(std::chrono::seconds(5));
			exit(0);//数据库重连20次失败后，退出程序
		}
// 		ostringstream oslog;
// 		oslog << "Reconnect database failed。" << _host; //重连数据库失败
// 		logprocess::Instance()->writeLog(oslog.str());
		nReconnectTimes++;
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
	HRESULT hr = _connPtr->raw_Execute(_bstr_t(sql.c_str()), NULL, adCmdText | adOpenKeyset, &mRst);
	if (!SUCCEEDED(hr)){
		success = false;
		long e = 0;
// 		ostringstream oslog;
// 		oslog << "Query sqlserver failed." << sql << getLastError(e);  
// 		logprocess::Instance()->writeLog(oslog.str());
		if (_connPtr->State == adStateClosed || e == 0x80004005) {
			//logprocess::Instance()->writeLog("Disconnect error。"); //断开连接错误
			close();
		}
	}
	_mutex.unlock();

	if (success && mRst){
		ptr = std::make_shared<CSqlserverResult>(mRst);
		return true;
	}
	return success;
}

bool CSqlserverOpt::execute(const std::string& sql)
{
	std::lock_guard<std::mutex> lg(_mutex);
	while (!isConnected()) {
		if (open()) break;
// 		ostringstream oslog;
// 		oslog << "Reconnect database failed。" << _host; //重连数据库失败
// 		logprocess::Instance()->writeLog(oslog.str());
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
	HRESULT hr = _connPtr->raw_Execute(_bstr_t(sql.c_str()), NULL, adCmdText | adExecuteNoRecords, nullptr);
	if (!SUCCEEDED(hr)) {
		long e = 0;
// 		ostringstream oslog;
// 		oslog << "Execute sqlserver failed." << sql << getLastError(e);
// 		logprocess::Instance()->writeLog(oslog.str());
		if (_connPtr->State ==  adStateClosed || e == 0x80004005)
			close();
	}
	return SUCCEEDED(hr);
}

const std::string CSqlserverOpt::getLastError(long& errCode)
{
	if (_connPtr == nullptr)
		return "";

	std::ostringstream os;
	long	errCounts = _connPtr->Errors->Count;
	for (long i = 0; i < errCounts; i++) {
		Error	*errMsg = NULL;
		_variant_t			vt_index(i, VT_I4);
		HRESULT	hr = _connPtr->Errors->get_Item(vt_index, &errMsg);
		if (SUCCEEDED(hr) && errMsg){
			errCode = errMsg->Number;
			os << "error code:" << errMsg->Number << " " << errMsg->Description << std::endl;
			errMsg->Release();
		}
		_connPtr->Errors->Clear();
	}
	return os.str();
}

#endif // _WIN32