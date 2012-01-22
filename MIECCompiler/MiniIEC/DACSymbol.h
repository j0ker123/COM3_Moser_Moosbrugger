#ifndef DACSYMBOL_H_
#define DACSYMBOL_H_

#include "Symbol.h"

namespace MIEC {

class DACLabelSymbol : public Symbol
{
public:
	DACLabelSymbol(DataType* pType, wchar_t* name, size_t addr)
		: Symbol(pType, name), mAddr(addr) { }
	int GetAddr() { return mAddr; }
private:
	size_t mAddr;
};

class DACSymbol : public Symbol
{
public:
	enum OpKind { eUnknown, 
		eAdd, eSubtract, eMultiply, eDivide,
		eIsEqual, eIsNotEqual, eIsLessEqual, eIsGreaterEqual, eIsLess, eIsGreater,
		eJump, eIfJump, eIfFalseJump, 
		eAssign, ePrint, eExit };

	DACSymbol(DataType* pType, wchar_t* name, OpKind operation, Symbol* pArg1, Symbol* pArg2, DACLabelSymbol* pLabel)
		: Symbol(pType, name), mOperation(operation), mpArg1(pArg1), mpArg2(pArg2), mpLabel(pLabel) {}
	DACLabelSymbol* const GetLabel() { return mpLabel; }
	OpKind const GetOperation() { return mOperation; }
	Symbol* const GetArgument1() { return mpArg1; }
	Symbol* const GetArgument2() { return mpArg2; }
private:
	DACLabelSymbol* mpLabel;
	OpKind mOperation;
	Symbol* mpArg1;
	Symbol* mpArg2;
};

} // MIEC

#endif // DACSYMBOL_H_