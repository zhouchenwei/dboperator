#ifndef _CDBOptBase_h_
#define _CDBOptBase_h_
#include "DBResultBase.h"

class CDBOptBase
{
public:
	CDBOptBase() = default;
	~CDBOptBase() = default;  

	inline void	setConnectParameter(const std::string& host, const std::string& port = "",
		const std::string& dbname = "", const std::string& user = "", const std::string& pass = "") {
		_host = host;
		_port = port;
		_name = dbname;
		_user = user;
		_pass = pass;
	}

	virtual bool	open() = 0;
	virtual	void	close() = 0;
	virtual bool	execute(const std::string& sql) = 0;
	virtual	bool	query(const std::string& sql, DBResultPtr& ptr) = 0;

	bool			isConnected() {
		std::lock_guard<std::mutex> lg(_mutexBase);
		return _connected;
	}
	void			setConnect(bool stat) {
		std::lock_guard<std::mutex> lg(_mutexBase);
		_connected = stat;
	}

protected:
	std::string	_host;
	std::string	_port;
	std::string	_name;
	std::string	_user;
	std::string	_pass;

private:
	std::mutex	_mutexBase;
	bool		_connected = false;
};

#endif