#ifndef DATATYPE_H_
#define DATATYPE_H_

#include "Symbol.h"

namespace MIEC {

class DataType : public Symbol
{
public:
	DataType(wchar_t* const name, size_t const size);
	virtual ~DataType();

	size_t const GetSize();

private:
	size_t const mSize;
};

class BaseType : public DataType
{
public:
	enum eKind { eVoid, eBool, eInt };

	virtual ~BaseType();

	eKind const GetKind() const;

protected:
	BaseType(eKind const kind, wchar_t* const name, size_t const size);
	
private:
	BaseType();

	eKind const mKind;
};

class VoidType : public BaseType
{
public:
	VoidType();

private:
};

class BooleanType : public BaseType
{
public:
	BooleanType();

private:
};

class IntegerType : public BaseType
{
public:
	IntegerType();

private:
};

} // namespace MIEC

#endif // DATATYPE_H_
