#ifndef DATATYPE_H_
#define DATATYPE_H_

namespace MIEC {

class DataType
{
public:
	DataType(size_t const size)
		: mSize(size) { }
	size_t const GetSize() { return mSize; }

protected:
	DataType() { mSize = 0; }
	void SetSize(size_t const size) { mSize = size; }

private:
	size_t mSize;
};

class BaseType : public DataType
{
public:
	enum eKind { eInt };

	BaseType(eKind const kind)
		: DataType(), mKind(kind)
	{
		switch (kind)
		{
		case eInt:
			DataType::SetSize(2); break;
		default:
			break;
		}
	}
	~BaseType() { }

private:
	BaseType();

	eKind const mKind;
};

} // namespace MIEC

#endif // DATATYPE_H
