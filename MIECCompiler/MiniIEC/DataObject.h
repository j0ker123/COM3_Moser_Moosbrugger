#ifndef DATAOBJECT_H_
#define DATAOBJECT_H_

#include "GlobalDef.h"

namespace MIEC {

class DataObject
{
public:
	virtual ~DataObject() { }

	virtual operator WORD() const = 0;

protected:
	DataObject() { }

private:
};

class BooleanObject : public DataObject
{
public:
	BooleanObject(bool const val) : mValue(val) { }
	BooleanObject(const BooleanObject& obj) : mValue(obj.GetValue()) { }
	BooleanObject& operator= (const BooleanObject& obj) { mValue = obj.GetValue(); }
	virtual ~BooleanObject() { }

	bool const GetValue() const { return mValue; }
	void SetValue(bool const val) { mValue = val; }

	operator WORD() const { return (WORD)mValue; }

private:
	bool mValue;
};

class IntegerObject : public DataObject
{
public:
	IntegerObject(int const val) : mValue(val) { }
	IntegerObject(const IntegerObject& obj) : mValue(obj.GetValue()) { }
	IntegerObject& operator= (const IntegerObject& obj) { mValue = obj.GetValue(); }
	virtual ~IntegerObject() { }

	int const GetValue() const { return mValue; }
	void SetValue(int const val) { mValue = val; }

	operator WORD() const { return (WORD)mValue; }

private:
	int mValue;
};

} // namespace MIEC

#endif // #ifndef DATAOBJECT_H_
