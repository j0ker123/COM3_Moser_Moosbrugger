#include <assert.h>
#include "Scanner.h"
#include "DataType.h"

namespace MIEC {

DataType::DataType(wchar_t* const name, size_t const size)
	: Symbol(Symbol::eType, name, 0), mSize(size)
{
	assert(name != 0 && !coco_string_equal(name, L""));
	//assert(size > 0);
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
	assert(name != 0 && !coco_string_equal(name, L""));
	//assert(size > 0);
}

BaseType::~BaseType()
{
}

BaseType::eKind const BaseType::GetKind() const
{
	return mKind;
}

wchar_t* const VoidType::Name = L"Void";

VoidType::VoidType()
	: BaseType(BaseType::eVoid, VoidType::Name, 0)
{
}

VoidType::~VoidType()
{
}

wchar_t* const BooleanType::Name = L"Boolean";

BooleanType::BooleanType()
	: BaseType(BaseType::eBool, BooleanType::Name, 1)
{
}

BooleanType::~BooleanType()
{
}

wchar_t* const IntegerType::Name = L"Integer";

IntegerType::IntegerType()
	: BaseType(BaseType::eInt, IntegerType::Name, 2)
{
}

IntegerType::~IntegerType()
{
}

} // namespace MIEC
