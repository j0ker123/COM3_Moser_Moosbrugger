#ifndef DACSYMBOL_H_
#define DACSYMBOL_H_

#include "Symbol.h"

namespace MIEC {

class DACSymbol : public Symbol
{
public:
	enum OpKind { 
		eAdd, eSubtract, eMultiply, eDivide,
		eIsEqual, eIsNotEqual, eIsLessEqual, eIsGreaterEqual, eIsLess, eIsGreater,
		eAssign, eJump, eIfJump, eIfFalseJump, ePrint, eExit };

	DACSymbol(SymbolType* pType, wchar_t* name, OpKind operation, Symbol* pArg1, Symbol* pArg2)
		: Symbol(pType, name), mOperation(operation), mpArg1(pArg1), mpArg2(pArg2) {}
private:
	OpKind mOperation;
	Symbol* mpArg1;
	Symbol* mpArg2;
};

class DACLabelSymbol : public Symbol
{
public:
	DACLabelSymbol(SymbolType* pType, wchar_t* name, size_t addr)
		: Symbol(pType, name), mAddr(addr) { }
	int GetAddr() { return mAddr; }
private:
	size_t mAddr;
};

} // MIEC

#endif // DACSYMBOL_H_