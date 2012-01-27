#ifndef SYMBOL_H_
#define SYMBOL_H_

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
	ConstSym(DataType* const pDataType, wchar_t* const name, double const val);

	double const GetVal() const;

private:
	ConstSym();
	ConstSym(const ConstSym&);
	ConstSym& operator= (const ConstSym&);

	double const mVal;
};

class VarSym : public Symbol
{
public:
	VarSym(DataType* const pDataType, wchar_t* const name, size_t const addr);

	size_t const GetAddr() const;

private:
	VarSym();
	VarSym(const VarSym&);
	VarSym& operator= (const VarSym&);

	size_t const mAddr;
};

} // namespace MIEC

#endif // #ifndef SYMBOL_H_
