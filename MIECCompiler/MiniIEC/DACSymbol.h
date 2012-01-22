#ifndef DACSYMBOL_H_
#define DACSYMBOL_H_

#include "Symbol.h"

namespace MIEC {

class DACLabel : public Symbol
{
public:
	DACLabel(DataType* pDataType, wchar_t* name, size_t addr)
		: Symbol(Symbol::eLabel, name, pDataType), mAddr(addr) { }

	int const GetAddr() const { return mAddr; }

private:
	size_t const mAddr;
};

class DACSymbol : public Symbol
{
public:
	enum OpKind { eUnknown, 
		eAdd, eSubtract, eMultiply, eDivide,
		eIsEqual, eIsNotEqual, eIsLessEqual, eIsGreaterEqual, eIsLess, eIsGreater,
		eJump, eIfJump, eIfFalseJump, 
		eAssign, ePrint, eExit };

	DACSymbol(DataType* pDataType, wchar_t* name, OpKind operation, Symbol* pArg1, Symbol* pArg2, DACLabel* pLabel)
		: Symbol(Symbol::eTempVar, name, pDataType), mOperation(operation), mpArg1(pArg1), mpArg2(pArg2), mpLabel(pLabel) {}

	DACLabel* const GetLabel() const { return mpLabel; }
	OpKind const GetOperation() const { return mOperation; }
	Symbol* const GetArgument1() const { return mpArg1; }
	Symbol* const GetArgument2() const { return mpArg2; }

private:
	DACLabel* const mpLabel;
	OpKind const mOperation;
	Symbol* const mpArg1;
	Symbol* const mpArg2;
};

} // MIEC

#endif // DACSYMBOL_H_