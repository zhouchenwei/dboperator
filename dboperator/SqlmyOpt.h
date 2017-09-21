#pragma	once 
#include "DBOptBase.h"
#include "SqlmyResult.h"


class CSqlmyOpt;
typedef std::shared_ptr<CSqlmyOpt> SqlmyOptPtr;

class CSqlmyOpt
	: public CDBOptBase
{
public:
	CSqlmyOpt();
	~CSqlmyOpt();

	virtual bool	open();
	virtual void	close();
	virtual	bool	query(const std::string& sql, DBResultPtr& ptr);
	virtual	bool	execute(const std::string& sql);

private:
	std::string getLastError(unsigned int& errCode);
	void	clean();
private:
	MYSQL*				_connPtr;
	std::mutex			_mutex;

	int m_nReconnectTimes;
};

