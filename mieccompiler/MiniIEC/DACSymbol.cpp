#include <assert.h>
#include "DACSymbol.h"

namespace MIEC {

DACLabel::DACLabel(DataType* pDataType, wchar_t* name)
	: Symbol(Symbol::eLabel, name, pDataType)
{
	//assert(pDataType != 0);
	assert(name != 0 && name != L"");
}

size_t const DACLabel::GetAddr() const
{
	return mAddr;
}

void DACLabel::SetAddr(size_t const addr)
{
	mAddr = addr;
}


DACSymbol::DACSymbol(DataType* pDataType, wchar_t* name, OpKind operation, Symbol* pArg1, Symbol* pArg2, DACLabel* pLabel)
	: Symbol(Symbol::eTempVar, name, pDataType), mOperation(operation), mpArg1(pArg1), mpArg2(pArg2), mpLabel(pLabel)
{
	//assert(pDataType != 0);
	assert(name != 0 && name != L"");
}

DACLabel* const DACSymbol::GetLabel() const
{
	return mpLabel;
}

DACSymbol::OpKind const DACSymbol::GetOperation() const
{
	return mOperation;
}

Symbol* const DACSymbol::GetArgument1() const
{
	return mpArg1;
}

Symbol* const DACSymbol::GetArgument2() const
{
	return mpArg2;
}

} // namespace MIEC
