#ifndef DACSYMBOL_H_
#define DACSYMBOL_H_

#include "Symbol.h"

namespace MIEC {

class DACLabel : public Symbol
{
public:
	DACLabel(DataType* pDataType, wchar_t* name);

	size_t const GetAddr() const;
	void SetAddr(size_t const addr);

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

	DACSymbol(DataType* pDataType, wchar_t* name, OpKind operation, Symbol* pArg1, Symbol* pArg2, DACLabel* pLabel);

	DACLabel* const GetLabel() const;
	OpKind const GetOperation() const;
	Symbol* const GetArgument1() const;
	Symbol* const GetArgument2() const;

private:
	DACLabel* const mpLabel;
	OpKind const mOperation;
	Symbol* const mpArg1;
	Symbol* const mpArg2;
};

} // namespace MIEC

#endif // #ifndef DACSYMBOL_H_
