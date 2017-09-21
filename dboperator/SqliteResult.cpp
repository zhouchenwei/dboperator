#include "SqliteResult.h"

CSqliteResult::CSqliteResult(char** rst, int row, int col)
	: _result(rst), _row(row), _col(col)
{
}

CSqliteResult::~CSqliteResult()
{
	if (_result)
		sqlite3_free_table(_result);
}

bool CSqliteResult::IsEof()
{
	return (_nowRow >= _row || _row == 0);
}

void CSqliteResult::MoveNext()
{
	_nowRow++;
}

std::string CSqliteResult::GetValue(size_t index)
{
	if (index >= _col)
		return "";
	int npos = (_col*(_nowRow + 1)) + index;
	return _result[npos] ? string(_result[npos]) : "";
}

size_t CSqliteResult::GetValueAsInt(size_t index)
{
	string val = GetValue(index);
	return atoi(val.c_str());
}

std::string CSqliteResult::GetValueByName(const std::string& field)
{
	for (size_t i = 0; i < _col; i++) {
		//就是这个字段
		if (string(_result[i]) == field.c_str())
			return GetValue(i);
	}
	return "";
}

size_t CSqliteResult::GetValueByNameAsInt(const std::string& field)
{
	string val = GetValueByName(field);
	return atoi(val.c_str());
}

