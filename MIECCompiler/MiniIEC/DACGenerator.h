#ifndef DACGENERATOR_H_
#define DACGENERATOR_H_

#include <list>
#include "DACSymbol.h"

namespace MIEC {

class Parser;

typedef std::list<DACSymbol*> tDACList;

class DACGenerator
{
public:
	DACGenerator(Parser* const pParser);
	virtual ~DACGenerator();

	DACLabel* const GetNewLabel();

	DACLabel* const AddLabel(DACLabel* pLabel);
	DACSymbol* const AddStat(DACSymbol::OpKind op, Symbol* pArg1, Symbol* pArg2);

	tDACList const*const GetDACList() const;

private:
	DACGenerator();
	DACGenerator(const DACGenerator&);
	DACGenerator& operator= (const DACGenerator&);

	Parser* const mpParser;
	tDACList* const mpDACList;

	size_t mLabelNumber;
	DACLabel* mpCurrLabel;
};

} // namespace MIEC

#endif // #ifndef DACGENERATOR_H_
