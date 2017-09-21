#ifndef _CDBResultBase_h_
#define _CDBResultBase_h_
#include "header.h"

class CDBResultBase;
typedef std::shared_ptr<CDBResultBase> DBResultPtr;

class CDBResultBase
{
public:
	CDBResultBase() = default;
	~CDBResultBase() = default;

	CDBResultBase(CDBResultBase&) = delete;
	CDBResultBase& operator = (const CDBResultBase&) = delete;

	virtual bool	IsEof() = 0;
	virtual void	MoveNext() = 0;
	virtual std::string	GetValue(size_t index) = 0;
	virtual size_t	GetValueAsInt(size_t index) = 0;
	virtual std::string	GetValueByName(const std::string& field) = 0;
	virtual size_t	GetValueByNameAsInt(const std::string& field) = 0;
};

#endif

