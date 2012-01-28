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

	DACLabel* const GetNewLabel();

	DataType* const AddType(DataType* pType);
	Symbol* const AddSymbol(Symbol* pSymbol);
	DACLabel* const AddLabel(DACLabel* const pLabel);
	DACSymbol* const AddStat(DACSymbol::OpKind const op, Symbol* const pArg1, Symbol* const pArg2);

	Symbol* const FindSymbol(wchar_t* const pName);
	Symbol* const GetSymbol(wchar_t* const pName);

	const SymbolTable& GetSymbolList() const;
	const tDACList& GetDACList() const;

	void Err(wchar_t* const format, ...) const;
	size_t const GetErrorCounter() const;

private:
	DACGenerator();
	DACGenerator(const DACGenerator&);
	DACGenerator& operator= (const DACGenerator&);

	Parser* const mpParser;
	SymbolTable mSymbolTable;
	tDACList mDACList;

	size_t mLabelNumber;
	DACLabel* mpCurrLabel;
	size_t mErrorCounter;
};

} // namespace MIEC

#endif // #ifndef DACGENERATOR_H_
