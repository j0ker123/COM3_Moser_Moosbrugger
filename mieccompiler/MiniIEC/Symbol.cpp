#include <assert.h>
#include "Scanner.h"
#include "Symbol.h"

namespace MIEC {

Symbol::Symbol(Symbol::tSymbolType const symbolType, wchar_t* const symbolName, DataType* const pDataType)
	: mType(symbolType), mName(coco_string_create(symbolName)), mpDataType(pDataType)
{
	assert(mName != 0 && !coco_string_equal(mName, L""));
}

Symbol::~Symbol()
{
	coco_string_delete(mName);
}

Symbol::tSymbolType const Symbol::GetType() const
{
	return mType;
}

wchar_t* const Symbol::GetName() const
{
	return mName;
}

DataType* const Symbol::GetDataType() const
{
	return mpDataType;
}


ConstSym::ConstSym(DataType* const pDataType, wchar_t* const name, double const val)
	: Symbol(Symbol::eConst, name, pDataType), mVal(val)
{
	assert(pDataType != 0);
	assert(name != 0 && !coco_string_equal(name, L""));
}

double const ConstSym::GetVal() const
{
	return mVal;
}


VarSym::VarSym(DataType* const pDataType, wchar_t* const name, size_t const addr)
	: Symbol(Symbol::eVar, name, pDataType), mAddr(addr)
{
	assert(pDataType != 0);
	assert(name != 0 && !coco_string_equal(name, L""));
}

size_t const VarSym::GetAddr() const
{
	return mAddr;
}

} // namespace MIEC
