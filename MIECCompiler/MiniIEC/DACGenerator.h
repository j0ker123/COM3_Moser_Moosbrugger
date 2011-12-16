#ifndef DACGENERATOR_H_
#define DACGENERATOR_H_

#include <list>
#include "DACSymbol.h"

namespace MIEC {

class Parser;

class DACGenerator
{
public:
	typedef std::list<DACSymbol*> tDACList;

	DACGenerator(Parser* const pParser);
	~DACGenerator();

	DACSymbol* const AddStat(DACSymbol::OpKind op, Symbol* pArg1, Symbol* pArg2);

private:
	Parser* mpParser;
	tDACList mDACList;
};

} // MIEC

#endif // DACGENERATOR_H_