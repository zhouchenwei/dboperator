#ifndef _CSqlserverResult_h_
#define _CSqlserverResult_h_
#include "DBResultBase.h"

#ifdef _WIN32
// #import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace \
// rename ("EOF", "adoEOF")
#import "../libs/msado15.dll" no_namespace rename ("EOF", "adoEOF")

class CSqlserverResult;
typedef std::shared_ptr<CSqlserverResult> DBSqlserverResultPtr;

class CSqlserverResult
	: public CDBResultBase
{
public:
	explicit CSqlserverResult(_RecordsetPtr rst);
	~CSqlserverResult();

	bool		IsEof();
	void		MoveNext();
	std::string	GetValue(size_t index);
	size_t		GetValueAsInt(size_t index);
	std::string	GetValueByName(const std::string& field);
	size_t		GetValueByNameAsInt(const std::string& field);

private:
	_RecordsetPtr	_resultPtr;
};

#endif
#endif