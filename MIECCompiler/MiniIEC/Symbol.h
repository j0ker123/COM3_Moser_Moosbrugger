#ifndef SYMBOL_H_
#define SYMBOL_H_

#include <string>

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

class Symbol
{
public:
	Symbol(SymbolType* pType, std::string const & name)
		: mpType(pType), mName(name) { }
	SymbolType* GetType() { return mpType; }
	std::string const & GetName() { return mName; }
private:
	Symbol();

	SymbolType* mpType;
	std::string mName;
};

class ConstSym : public Symbol
{
public:
	ConstSym(SymbolType* pType, std::string const & name, int val)
		: Symbol(pType, name), mVal(val) { }
	int GetVal() { return mVal; }
private:
	int mVal;
};

class VarSym : public Symbol
{
public:
	VarSym(SymbolType* pType, std::string const & name, size_t addr)
		: Symbol(pType, name), mAddr(addr) { }
	int GetAddr() { return mAddr; }
private:
	size_t mAddr;
};

#endif // SYMBOL_H_