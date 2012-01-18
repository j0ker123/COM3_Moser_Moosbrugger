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
	~DACGenerator();

	DACLabelSymbol* const GetNewLabel();

	DACLabelSymbol* const AddLabel(DACLabelSymbol* pLabel);
	DACSymbol* const AddStat(DACSymbol::OpKind op, Symbol* pArg1, Symbol* pArg2);

	tDACList const*const GetDACList() const;

private:
	Parser* const mpParser;
	tDACList* const mpDACList;

	size_t mLabelNumber;
	DACLabelSymbol* mpCurrLabel;
};

} // MIEC

#endif // DACGENERATOR_H_