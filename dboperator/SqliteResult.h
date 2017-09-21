#pragma once
#include "DBResultBase.h"
#include <sqlite3/sqlite3.h>
#ifdef _WIN32
#pragma comment(lib, "sqlite3.lib")
#endif // !_WIN32

class CSqliteResult;
typedef std::shared_ptr<CSqliteResult> DBSqliteResultPtr;

class CSqliteResult
	: public CDBResultBase
{
public:
	explicit CSqliteResult(char** rst, int row, int col);
	~CSqliteResult();

	bool	IsEof();
	void	MoveNext();
	std::string	GetValue(size_t index);
	size_t	GetValueAsInt(size_t index);
	std::string	GetValueByName(const std::string& field);
	size_t	GetValueByNameAsInt(const std::string& field);

private:
	char**		_result = nullptr;
	size_t		_row = 0;
	size_t		_col = 0;
	size_t		_nowRow = 0;
};

