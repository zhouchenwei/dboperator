#pragma	once 
#include "DBOptBase.h"
#include "SqlserverResult.h"
#ifdef _WIN32

class CSqlserverOpt;
typedef std::shared_ptr<CSqlserverOpt> SqlServerOptPtr;
class CSqlserverOpt
	: public CDBOptBase
{
public:
	CSqlserverOpt();
	~CSqlserverOpt();

	virtual bool	open();
	virtual void	close();
	virtual	bool	query(const std::string& sql, DBResultPtr& ptr);
	virtual	bool	execute(const std::string& sql);

private:
	const std::string getLastError(long& errCode);

private:
	_ConnectionPtr		_connPtr;
	std::mutex			_mutex;
};

typedef std::shared_ptr<CSqlserverOpt> SqlServerOptPtr;
#endif