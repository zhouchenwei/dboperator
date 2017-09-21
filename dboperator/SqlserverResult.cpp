#include "SqlserverResult.h"

#ifdef _WIN32
CSqlserverResult::CSqlserverResult(_RecordsetPtr rst)
	: _resultPtr(rst)
{
}

CSqlserverResult::~CSqlserverResult()
{
	_resultPtr->Close();
}

bool CSqlserverResult::IsEof()
{
	return (_resultPtr->adoEOF == VARIANT_TRUE);
}

void CSqlserverResult::MoveNext()
{
	_resultPtr->MoveNext();
}

std::string CSqlserverResult::GetValue(size_t index)
{
	std::string val;
 	_variant_t	vtValue = _resultPtr->GetCollect(_variant_t(static_cast<long>(index)));
	if (vtValue.vt != VT_NULL)
		val = std::string((char*)bstr_t(vtValue));

	vtValue.Clear();
	return val;
}

size_t CSqlserverResult::GetValueAsInt(size_t index)
{
	string val = GetValue(index);
	return atoi(val.c_str());
}

std::string CSqlserverResult::GetValueByName(const std::string& field)
{
	std::string val;
	_variant_t	vtValue = _resultPtr->GetCollect(field.c_str());
	if (vtValue.vt != VT_NULL)
		val = std::string((char*)bstr_t(vtValue));

	vtValue.Clear();
	return val;
}

size_t CSqlserverResult::GetValueByNameAsInt(const std::string& field)
{
	size_t val;
	_variant_t	vtValue = _resultPtr->GetCollect(field.c_str());
	val = vtValue.intVal;

	vtValue.Clear();
	return val;
}

#endif // _WIN32