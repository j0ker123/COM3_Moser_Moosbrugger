#include <assert.h>
#include "Scanner.h"
#include "DACSymbol.h"

namespace MIEC {

DACLabel::DACLabel(wchar_t* const name)
	: Symbol(Symbol::eLabel, name, 0)
{
	assert(name != 0 && !coco_string_equal(name, L""));
}

size_t const DACLabel::GetAddr() const
{
	return mAddr;
}

void DACLabel::SetAddr(size_t const addr)
{
	mAddr = addr;
}


DACSymbol::DACSymbol(DataType* const pDataType, wchar_t* const name, OpKind const operation, Symbol* const pArg1, Symbol* const pArg2, DACLabel*const pLabel)
	: Symbol(Symbol::eTempVar, name, pDataType), mOperation(operation), mpArg1(pArg1), mpArg2(pArg2), mpLabel(pLabel)
{
	if (pDataType == 0) {
		printf("test");
	}
	assert(name != 0 && !coco_string_equal(name, L""));
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
