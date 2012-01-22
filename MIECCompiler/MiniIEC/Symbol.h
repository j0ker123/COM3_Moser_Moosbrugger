#ifndef SYMBOL_H_
#define SYMBOL_H_

#include "DataType.h"
#include "Scanner.h"

namespace MIEC {

class Symbol
{
public:
	virtual ~Symbol() { coco_string_delete(mName); }

	typedef enum { eConst, eVar, eTempVar, eLabel } tSymbolType;
	tSymbolType const GetType() const { return mType; }
	wchar_t* const GetName() const { return mName; }
	DataType* const GetDataType() const { return mpDataType; }

protected:
	Symbol(tSymbolType const symbolType, wchar_t* const symbolName, DataType* const pDataType)
		: mType(symbolType), mName(coco_string_create(symbolName)), mpDataType(pDataType) { }

private:
	Symbol();

	tSymbolType const mType;
	wchar_t* mName;
	DataType* const mpDataType;
};

class ConstSym : public Symbol
{
public:
	ConstSym(DataType* const pDataType, wchar_t* const name, int const val)
		: Symbol(Symbol::eConst, name, pDataType), mVal(val) { }

	int GetVal() const { return mVal; }

private:
	int const mVal;
};

class VarSym : public Symbol
{
public:
	VarSym(DataType* const pDataType, wchar_t* const name, size_t const addr)
		: Symbol(Symbol::eVar, name, pDataType), mAddr(addr) { }

	int GetAddr() const { return mAddr; }

private:
	size_t const mAddr;
};

}

#endif // SYMBOL_H_