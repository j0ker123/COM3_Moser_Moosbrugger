
#include "Parser.h"
#include "DACGenerator.h"

namespace MIEC {

DACGenerator::DACGenerator(Parser* const pParser)
	: mpParser(pParser), mLabelNumber(0) 
{
}

DACGenerator::~DACGenerator()
{
	// don't delete twice! - already done in SymbolTable destructor
	//tDACList::iterator itor = mDACList.begin();
	//for (; itor != mDACList.end(); itor++) {
	//	delete *itor;
	//}
}

wchar_t* CreateString(wchar_t* prefix, size_t number)
{
	wchar_t* pName = new wchar_t[30];

	swprintf(pName, L"%s%d", prefix, number);

	return pName;
}

DACLabelSymbol* const DACGenerator::GetNewLabel() 
{
	static LabelType* pType = new LabelType();
	wchar_t* pName = CreateString(L"$L", mLabelNumber++);

	return new DACLabelSymbol(pType, pName, /* TODO: address of label in bytecode */ 0);
}

DACSymbol* const DACGenerator::AddStat(DACSymbol::OpKind op, Symbol* pArg1, Symbol* pArg2)
{
	size_t err = 0;	// error counter

	SymbolType* pType = 0; //UnknownType;

	if (op == DACSymbol::eUnknown) {
		mpParser->Err(L"AddStat: invalid operation"); err++;
		//return 0;
	}
	if (pArg1 == 0) {
		switch (op) {
			case DACSymbol::eUnknown:
				break;
			case DACSymbol::eAssign:
				mpParser->Err(L"AddStat: invalid assignment destination");
				break;
			case DACSymbol::ePrint:
				mpParser->Err(L"AddStat: invalid parameter");
				break;
			case DACSymbol::eIfJump: case DACSymbol::eIfFalseJump:
				mpParser->Err(L"AddStat: invalid branch condition");
				break;
			case DACSymbol::eJump:
				mpParser->Err(L"AddStat: invalid jump destination");
				break;
			default:
				mpParser->Err(L"AddStat: invalid left parameter");
		}
		err++;
		//return 0;
	}
	else { 
		pType = pArg1->GetType();
	}

	switch (op) {
		case DACSymbol::eUnknown:
			break;
		case DACSymbol::eAssign:
			if (pArg2 == 0) { mpParser->Err(L"AddStat: invalid assignment source"); err++; break; }
			break;
		case DACSymbol::eAdd: case DACSymbol::eSubtract: case DACSymbol::eMultiply: case DACSymbol::eDivide:
			if (pArg2 == 0) { mpParser->Err(L"AddStat: invalid right parameter"); err++; break; }
			if (pType != pArg2->GetType()) { mpParser->Err(L"AddStat: incompatible types"); err++; break; }
			break;
		case DACSymbol::eIsEqual: case DACSymbol::eIsNotEqual: case DACSymbol::eIsLessEqual: case DACSymbol::eIsGreaterEqual: case DACSymbol::eIsLess: case DACSymbol::eIsGreater:
			if (pArg2 == 0) { mpParser->Err(L"AddStat: invalid right parameter"); err++; break; }
			// TODO: pType = boolean;
			break;
		case DACSymbol::eIfJump: case DACSymbol::eIfFalseJump:
			if (pArg2 == 0) { mpParser->Err(L"AddStat: invalid jump destination"); err++; break; }
		case DACSymbol::eJump: 
			// TODO: if (pType != boolean) { mpParser->Err(L"AddStat: invalid condition"); err++; break; }
			// TODO: pType = LabelType;
			break;
		default:
			if (pArg2 != 0) { mpParser->Err(L"AddStat: too much parameters"); err++; }
	}

	if (err > 0) { return 0; }

	wchar_t* pName = CreateString(L"$t", mDACList.size());

	DACSymbol* stat = new DACSymbol(pType, pName, op, pArg1, pArg2); 
	mDACList.push_back(stat);

	return stat;
}

} // MIEC
