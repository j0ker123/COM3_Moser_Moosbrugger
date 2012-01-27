#include <assert.h>
#include "GlobalDef.h"
#include "DataType.h"

namespace MIEC {

DataType::DataType(wchar_t* const name, size_t const size)
	: Symbol(Symbol::eType, name, 0), mSize(size)
{
	assert(name != 0 && name != L"");
	assert(size > 0);
}

DataType::~DataType()
{
}

size_t const DataType::GetSize()
{
	return mSize;
}


BaseType::BaseType(eKind const kind, wchar_t* const name, size_t const size)
	: DataType(name, size), mKind(kind)
{
	assert(name != 0 && name != L"");
	assert(size > 0);
}

BaseType::~BaseType()
{
}

BaseType::eKind const BaseType::GetKind() const
{
	return mKind;
}


Integer::Integer()
	: BaseType(BaseType::eInt, L"Integer", sizeof(WORD))
{
}

} // namespace MIEC
