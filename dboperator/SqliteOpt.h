#pragma	once 
#include "DBOptBase.h"
#include "SqliteResult.h"

class CSqliteOpt;
typedef std::shared_ptr<CSqliteOpt> SqliteOptPtr;

class CSqliteOpt
	: public CDBOptBase
{
public:
	CSqliteOpt();
	~CSqliteOpt();

	virtual bool	open();
	virtual void	close();
	virtual	bool	query(const std::string& sql, DBResultPtr& ptr);
	virtual	bool	execute(const std::string& sql);

private:
	const std::string getLastError(long& errCode);

private:
	sqlite3*			_sqlite;
	std::mutex			_mutex;
};

