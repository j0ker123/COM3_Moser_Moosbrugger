#ifndef SYMBOL_H_
#define SYMBOL_H_

#include <ostream>
#include "DataObject.h"

namespace MIEC {

class DataType;

class Symbol
{
public:
	typedef enum { eType, eConst, eVar, eTempVar, eLabel } tSymbolType;

	virtual ~Symbol();

	tSymbolType const GetType() const;
	wchar_t* const GetName() const;
	DataType* const GetDataType() const;

	virtual void Print(std::wostream& out) const;

protected:
	Symbol(tSymbolType const symbolType, wchar_t* const symbolName, DataType* const pDataType);

private:
	Symbol();
	Symbol(const Symbol&);
	Symbol& operator= (const Symbol&);

	tSymbolType const mType;
	wchar_t* mName;
	DataType* const mpDataType;
};

class ConstSym : public Symbol
{
public:
	ConstSym(wchar_t* const pName, DataType* const pDataType, DataObject* const pVal);
	virtual ~ConstSym();

	DataObject* const GetVal() const;

private:
	ConstSym();
	ConstSym(const ConstSym&);
	ConstSym& operator= (const ConstSym&);

	DataObject* const mpVal;
};

class VarSym : public Symbol
{
public:
	VarSym(wchar_t* const pName, DataType* const pDataType, size_t const addr);
	virtual ~VarSym();

	size_t const GetAddr() const;

private:
	VarSym();
	VarSym(const VarSym&);
	VarSym& operator= (const VarSym&);

	size_t const mAddr;
};

} // namespace MIEC

#endif // #ifndef SYMBOL_H_
