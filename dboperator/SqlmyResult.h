#pragma once
#include "DBResultBase.h"
#include <mysql.h>


class CSqlmyResult;
typedef std::shared_ptr<CSqlmyResult> DBSqlmyResultPtr;

class CSqlmyResult
	: public CDBResultBase
{
public:
	explicit CSqlmyResult(MYSQL_RES* rst);
	~CSqlmyResult();

	bool		IsEof();
	void		MoveNext();
	std::string	GetValue(size_t index);
	size_t		GetValueAsInt(size_t index);
	std::string	GetValueByName(const std::string& field);
	size_t		GetValueByNameAsInt(const std::string& field);

private:
	MYSQL_RES*		_resultPtr;
	MYSQL_ROW		_resultRow;
	my_ulonglong	_cols = 0;
};
