#ifndef SYMBOL_H_
#define SYMBOL_H_

#include "Scanner.h"

namespace MIEC {

class SymbolType
{
public:
	SymbolType(size_t const size)
		: mSize(size) { }
	size_t const GetSize() { return mSize; }

protected:
	SymbolType() { mSize = 0; }
	void SetSize(size_t const size) { mSize = size; }

private:
	size_t mSize;
};

class BaseType : public SymbolType
{
public:
	enum eKind { eInt };

	BaseType(eKind const kind)
		: SymbolType(), mKind(kind)
	{
		switch (kind)
		{
		case eInt:
			SymbolType::SetSize(2); break;
		default:
			break;
		}
	}
	~BaseType() { }

private:
	BaseType();

	eKind const mKind;
};

class LabelType : public SymbolType
{
public:
private:
};

class Symbol
{
public:
	~Symbol() { coco_string_delete(mName); }

	SymbolType* const GetType() { return mpType; }
	wchar_t* const GetName() { return mName; }

protected:
	Symbol(SymbolType* const pType, wchar_t* const name)
		: mpType(pType), mName(coco_string_create(name)) { }

private:
	Symbol();

	SymbolType* const mpType;
	wchar_t* mName;
};

class ConstSym : public Symbol
{
public:
	ConstSym(SymbolType* const pType, wchar_t* const name, int const val)
		: Symbol(pType, name), mVal(val) { }
	int GetVal() { return mVal; }
private:
	int const mVal;
};

class VarSym : public Symbol
{
public:
	VarSym(SymbolType* const pType, wchar_t* const name, size_t const addr)
		: Symbol(pType, name), mAddr(addr) { }
	int GetAddr() { return mAddr; }
private:
	size_t const mAddr;
};

}

#endif // SYMBOL_H_