#ifndef DACGENERATOR_H_
#define DACGENERATOR_H_

#include <list>
#include "Parser.h"
#include "DACSymbol.h"

class DACGenerator
{
public:
	typedef std::list<DACSymbol*> tDACList;

	DACGenerator(MIEC::Parser* pParser)
		: mpParser(pParser) { }

	DACSymbol* const AddStat(DACSymbol::OpKind op, Symbol* pArg1, Symbol* pArg2)
	{
		if (pArg1 == 0 || pArg2 == 0) { return 0; }

		wchar_t* name = new wchar_t[30];
		swprintf(name, L"$t%d", mDACList.size());

		DACSymbol* stat = new DACSymbol(0, name, op, pArg1, pArg2); 
		mDACList.push_back(stat);

		return stat;
	}

	~DACGenerator()
	{
		/*tDACList::iterator itor = mDACList.begin();
		for (; itor != mDACList.end(); itor++) {
			delete *itor;
		}*/
	}

private:
	MIEC::Parser* mpParser;

	tDACList mDACList;
};

#endif // DACGENERATOR_H_