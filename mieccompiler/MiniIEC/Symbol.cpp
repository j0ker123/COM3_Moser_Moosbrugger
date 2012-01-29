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


ConstSym::ConstSym(wchar_t* const pName, DataType* const pDataType, DataObject* const pVal)
	: Symbol(Symbol::eConst, pName, pDataType), mpVal(pVal)
{
	assert(pName != 0 && !coco_string_equal(pName, L""));
	assert(pDataType != 0);
	assert(mpVal != 0);
}

ConstSym::~ConstSym()
{
	delete mpVal;
}

DataObject* const ConstSym::GetVal() const
{
	return mpVal;
}


VarSym::VarSym(wchar_t* const pName, DataType* const pDataType, size_t const addr)
	: Symbol(Symbol::eVar, pName, pDataType), mAddr(addr)
{
	assert(pName != 0 && !coco_string_equal(pName, L""));
	assert(pDataType != 0);
}

VarSym::~VarSym()
{
}

size_t const VarSym::GetAddr() const
{
	return mAddr;
}

} // namespace MIEC
