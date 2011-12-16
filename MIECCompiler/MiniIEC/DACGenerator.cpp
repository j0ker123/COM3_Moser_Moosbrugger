
#include "Parser.h"
#include "DACGenerator.h"

namespace MIEC {

DACGenerator::DACGenerator(Parser* const pParser)
	: mpParser(pParser) { }

DACGenerator::~DACGenerator()
{
	//tDACList::iterator itor = mDACList.begin();
	//for (; itor != mDACList.end(); itor++) {
	//	delete *itor;
	//}
}

DACSymbol* const DACGenerator::AddStat(DACSymbol::OpKind op, Symbol* pArg1, Symbol* pArg2)
{
	if (pArg1 == 0) { return 0; }
	SymbolType* pType = pArg1->GetType();

	switch (op) {
		case DACSymbol::ePrint:
			break;
		case DACSymbol::eAdd: case DACSymbol::eSubtract: case DACSymbol::eMultiply: case DACSymbol::eDivide:
			//if (pArg2 == 0) { return 0; }
			if (pArg2 == 0 || pType != pArg2->GetType()) {
				mpParser->Err(L"AddStat: incompatible types");
				return 0;
			}
			break;
		case DACSymbol::eIsEqual: case DACSymbol::eIsLessEqual: case DACSymbol::eIsGreaterEqual: case DACSymbol::eIsLess: case DACSymbol::eIsGreater:
		default:
			// TODO: pType = boolean;
			if (pArg2 == 0) { return 0; }
			break;
	}

	wchar_t* pName = new wchar_t[30];
	swprintf(pName, L"$t%d", mDACList.size());

	DACSymbol* stat = new DACSymbol(pType, pName, op, pArg1, pArg2); 
	mDACList.push_back(stat);

	return stat;
}

} // MIEC
