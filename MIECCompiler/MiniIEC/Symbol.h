#ifndef SYMBOL_H_
#define SYMBOL_H_

#include "Scanner.h"

namespace MIEC {

class SymbolType
{
public:
	SymbolType(size_t size)
		: mSize(size) { }
	size_t GetSize() { return mSize; }
protected:
	SymbolType() { mSize = 0; }

	size_t mSize;
};

class BaseType : public SymbolType
{
public:
	enum eKind { eInt };

	BaseType(eKind kind)
		: SymbolType(), mKind(kind)
	{
		switch (kind)
		{
		case eInt:
			mSize = 2; break;
		default:
			break;
		}
	}
private:
	BaseType();

	eKind mKind;
};

class LabelType : public SymbolType
{
public:
private:
};

class Symbol
{
public:
	Symbol(SymbolType* pType, wchar_t* name)
		: mpType(pType), mName(coco_string_create(name)) { }
	~Symbol() { coco_string_delete(mName); }

	SymbolType* GetType() { return mpType; }
	wchar_t* GetName() { return mName; }
private:
	Symbol();

	SymbolType* mpType;
	wchar_t* mName;
};

class ConstSym : public Symbol
{
public:
	ConstSym(SymbolType* pType, wchar_t* name, int val)
		: Symbol(pType, name), mVal(val) { }
	int GetVal() { return mVal; }
private:
	int mVal;
};

class VarSym : public Symbol
{
public:
	VarSym(SymbolType* pType, wchar_t* name, size_t addr)
		: Symbol(pType, name), mAddr(addr) { }
	int GetAddr() { return mAddr; }
private:
	size_t mAddr;
};

}

#endif // SYMBOL_H_