#ifndef DACGENERATOR_H_
#define DACGENERATOR_H_

#include <list>
#include "DataType.h"
#include "Symbol.h"
#include "DACSymbol.h"
#include "SymbolTable.h"

namespace MIEC {

class Parser;

typedef std::list<DACSymbol*> tDACList;

class DACGenerator
{
public:
	DACGenerator(Parser* const pParser);
	virtual ~DACGenerator();

	static wchar_t* CreateString(wchar_t* const prefix, size_t const number);

	DACLabel* const GetNewLabel();

	Symbol* const AddSymbol(Symbol* pSymbol);
	DataType* const AddType(DataType* pType);
	DACLabel* const AddLabel(DACLabel* pLabel);
	DACSymbol* const AddStat(DACSymbol::OpKind const op, Symbol* const pArg1, Symbol* const pArg2);

	Symbol* const FindSymbol(wchar_t* const pName);
	Symbol* const GetSymbol(wchar_t* const pName);
	DataType* const GetType(wchar_t* const pName);

	const SymbolTable& GetSymbolList() const;
	const tDACList& GetDACList() const;

	void SetLine(size_t const line);
	void Err(wchar_t* const format, ...);
	size_t const GetErrorCounter() const;

private:
	DACGenerator();
	DACGenerator(const DACGenerator&);
	DACGenerator& operator= (const DACGenerator&);

	Parser* const mpParser;
	SymbolTable mSymbolTable;
	tDACList mDACList;

	size_t mLabelNumber;	// current label number
	size_t mTempVarNumber;
	DACLabel* mpCurrLabel;

	size_t mErrorLine;		// current line parsed
	size_t mErrorCounter;	// syntax error counter
};

} // namespace MIEC

#endif // #ifndef DACGENERATOR_H_
