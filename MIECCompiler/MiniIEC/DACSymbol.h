#ifndef DACSYMBOL_H_
#define DACSYMBOL_H_

#include "Symbol.h"

namespace MIEC {

class DACLabelSymbol : public Symbol
{
public:
	DACLabelSymbol(SymbolType* pType, wchar_t* name, size_t addr)
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

	DACSymbol(SymbolType* pType, wchar_t* name, OpKind operation, Symbol* pArg1, Symbol* pArg2, DACLabelSymbol* pLabel)
		: Symbol(pType, name), mOperation(operation), mpArg1(pArg1), mpArg2(pArg2), mpLabel(pLabel) {}
private:
	DACLabelSymbol* mpLabel;
	OpKind mOperation;
	Symbol* mpArg1;
	Symbol* mpArg2;
};

} // MIEC

#endif // DACSYMBOL_H_