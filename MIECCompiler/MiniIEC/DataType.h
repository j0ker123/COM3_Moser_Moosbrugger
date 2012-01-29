#ifndef DATATYPE_H_
#define DATATYPE_H_

#include "Symbol.h"

namespace MIEC {

class DataType : public Symbol
{
public:
	virtual ~DataType();

	size_t const GetSize();

protected:
	DataType(wchar_t* const name, size_t const size);

private:
	DataType();

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
	static wchar_t* const Name;

	VoidType();
	virtual ~VoidType();

private:
};

class BooleanType : public BaseType
{
public:
	static wchar_t* const Name;

	BooleanType();
	virtual ~BooleanType();

private:
};

class IntegerType : public BaseType
{
public:
	static wchar_t* const Name;

	IntegerType();
	virtual ~IntegerType();

private:
};

} // namespace MIEC

#endif // DATATYPE_H_
