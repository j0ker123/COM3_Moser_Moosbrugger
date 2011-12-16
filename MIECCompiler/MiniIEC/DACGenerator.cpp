
#include "Parser.h"
#include "DACGenerator.h"

namespace MIEC {

DACGenerator::DACGenerator(Parser* const pParser)
	: mpParser(pParser) { }

DACGenerator::~DACGenerator()
{
	// don't delete twice! - already done in SymbolTable destructor
	//tDACList::iterator itor = mDACList.begin();
	//for (; itor != mDACList.end(); itor++) {
	//	delete *itor;
	//}
}

DACSymbol* const DACGenerator::AddStat(DACSymbol::OpKind op, Symbol* pArg1, Symbol* pArg2)
{
	if (pArg1 == 0) {
		switch (op) {
			case DACSymbol::eAssign:
				mpParser->Err(L"AddStat: invalid assignment destination");
				break;
			case DACSymbol::ePrint:
				mpParser->Err(L"AddStat: invalid parameter");
				break;
			default:
				mpParser->Err(L"AddStat: invalid left parameter");
		}
		return 0;
	}

	SymbolType* pType = pArg1->GetType();

	switch (op) {
		case DACSymbol::eAssign:
			if (pArg2 == 0) { mpParser->Err(L"AddStat: invalid assignment source"); return 0; }
			break;
		case DACSymbol::eAdd: case DACSymbol::eSubtract: case DACSymbol::eMultiply: case DACSymbol::eDivide:
			if (pArg2 == 0) { mpParser->Err(L"AddStat: invalid right parameter"); return 0; }
			if (pType != pArg2->GetType()) { mpParser->Err(L"AddStat: incompatible types"); return 0; }
			break;
		case DACSymbol::eIsEqual: case DACSymbol::eIsNotEqual: case DACSymbol::eIsLessEqual: case DACSymbol::eIsGreaterEqual: case DACSymbol::eIsLess: case DACSymbol::eIsGreater:
			if (pArg2 == 0) { mpParser->Err(L"AddStat: invalid right parameter"); return 0; }
			// TODO: pType = boolean;
			break;
		case DACSymbol::eJump: case DACSymbol::eIfJump: case DACSymbol::eIfFalseJump:
			// TODO: pType = LabelType;
			break;
		default:
			if (pArg2 != 0) { mpParser->Err(L"AddStat: too much parameters"); return 0; }
			break;
	}

	wchar_t* pName = new wchar_t[30];
	swprintf(pName, L"$t%d", mDACList.size());

	DACSymbol* stat = new DACSymbol(pType, pName, op, pArg1, pArg2); 
	mDACList.push_back(stat);

	return stat;
}

} // MIEC
