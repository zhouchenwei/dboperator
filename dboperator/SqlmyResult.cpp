#include "SqlmyResult.h"


CSqlmyResult::CSqlmyResult(MYSQL_RES* rst)
	: _resultPtr(rst)
{
 	_cols = mysql_num_fields(_resultPtr);
	_resultRow = mysql_fetch_row(_resultPtr);
}

CSqlmyResult::~CSqlmyResult()
{
	if (_resultPtr)
		mysql_free_result(_resultPtr);
}

bool CSqlmyResult::IsEof()
{
	return _resultRow == nullptr;
}

void CSqlmyResult::MoveNext()
{
	_resultRow = mysql_fetch_row(_resultPtr);
}

std::string CSqlmyResult::GetValue(size_t index)
{
	if (_resultRow[index])
		return _resultRow[index];
	else
		return "";
}

size_t CSqlmyResult::GetValueAsInt(size_t index)
{
	string val = GetValue(index);
	return atoi(val.c_str());
}

std::string CSqlmyResult::GetValueByName(const std::string& field)
{
	for (size_t i = 0; i < _cols; i++) {
		char * pFiledName = mysql_fetch_field_direct(_resultPtr, i)->name;
		if (pFiledName && field == string(pFiledName))
			return GetValue(i);
	}
	return "";
}

size_t CSqlmyResult::GetValueByNameAsInt(const std::string& field)
{
	string val = GetValueByName(field);
	return atoi(val.c_str());
}
