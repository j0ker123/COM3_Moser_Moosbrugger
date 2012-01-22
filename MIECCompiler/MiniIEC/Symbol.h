#ifndef SYMBOL_H_
#define SYMBOL_H_

#include "DataType.h"
#include "Scanner.h"

namespace MIEC {

class Symbol
{
public:
	typedef enum { eConst, eVar, eTempVar, eLabel } tSymbolType;

	~Symbol() { coco_string_delete(mName); }

	DataType* const GetType() { return mpType; }
	wchar_t* const GetName() { return mName; }

protected:
	Symbol(DataType* const pType, wchar_t* const name)
		: mpType(pType), mName(coco_string_create(name)) { }

private:
	Symbol();

	DataType* const mpType;
	wchar_t* mName;
};

class ConstSym : public Symbol
{
public:
	ConstSym(DataType* const pType, wchar_t* const name, int const val)
		: Symbol(pType, name), mVal(val) { }
	int GetVal() { return mVal; }
private:
	int const mVal;
};

class VarSym : public Symbol
{
public:
	VarSym(DataType* const pType, wchar_t* const name, size_t const addr)
		: Symbol(pType, name), mAddr(addr) { }
	int GetAddr() { return mAddr; }
private:
	size_t const mAddr;
};

}

#endif // SYMBOL_H_