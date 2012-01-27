#include <assert.h>
#include "Parser.h"
#include "DACGenerator.h"

namespace MIEC {

DACGenerator::DACGenerator(Parser* const pParser)
	: mpParser(pParser), mpDACList(new tDACList), mpCurrLabel(0), mLabelNumber(0) 
{
	assert(mpParser != 0);
	assert(mpDACList != 0);
}

DACGenerator::~DACGenerator()
{
	// Don't delete twice!
	// DACSymbols (all added to SymbolTable) will be deleted by SymbolTable destructor.
	/*
	tDACList::iterator itor = mpDACList->begin();
	for (; itor != mDACList->end(); itor++) {
		delete *itor;
	}
	*/
	delete mpDACList;
}

wchar_t* CreateString(wchar_t* prefix, size_t number)
{
	// get destination string length
	size_t const strLength = swprintf(0, 0, L"%s%d", prefix, number) + 1;
	wchar_t* pName = new wchar_t[strLength];

	// create string by concatenating prefix and number 
	swprintf(pName, strLength, L"%s%d", prefix, number);

	return pName;
}

DACLabel* const DACGenerator::GetNewLabel() 
{
	DataType* pType = 0; //UnknownType;
	wchar_t* pName = CreateString(L"$L", mLabelNumber++);

	// create new DACLabel symbol
	DACLabel* const pLabel = new DACLabel(pType, pName);

	coco_string_delete(pName);
	return pLabel;
}

DACLabel* const DACGenerator::AddLabel(DACLabel* pLabel)
{
	if (pLabel == 0) { return 0; }

	// register label - don't override registered label
	if (mpCurrLabel == 0) {
		mpCurrLabel = pLabel;
	}

	// return registered label
	return mpCurrLabel;
}

DACSymbol* const DACGenerator::AddStat(DACSymbol::OpKind op, Symbol* pArg1, Symbol* pArg2)
{
	assert(mpParser != 0);
	assert(mpDACList != 0);

	size_t err = 0;	// error counter

	DataType* pDataType = 0; //UnknownType;

	if (op == DACSymbol::eUnknown) {
		mpParser->Err(L"AddStat: invalid operation");
		return 0;
	}
	else if (op == DACSymbol::eExit) {
	}
	else {
		if (pArg1 == 0) {
			switch (op) {
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
			err++;	// count error
		}
		else { 
			pDataType = pArg1->GetDataType();
		}

		switch (op) {
			case DACSymbol::eAssign:
				if (pArg2 == 0) { mpParser->Err(L"AddStat: invalid assignment source"); err++; break; }
				break;
			case DACSymbol::eAdd: case DACSymbol::eSubtract: case DACSymbol::eMultiply: case DACSymbol::eDivide:
				if (pArg2 == 0) { mpParser->Err(L"AddStat: invalid right parameter"); err++; break; }
				if (pDataType != pArg2->GetDataType()) { mpParser->Err(L"AddStat: incompatible types"); err++; break; }
				break;
			case DACSymbol::eIsEqual: case DACSymbol::eIsNotEqual: case DACSymbol::eIsLessEqual: case DACSymbol::eIsGreaterEqual: case DACSymbol::eIsLess: case DACSymbol::eIsGreater:
				if (pArg2 == 0) { mpParser->Err(L"AddStat: invalid right parameter"); err++; break; }
				// TODO: pDataType = boolean;
				break;
			case DACSymbol::eIfJump: case DACSymbol::eIfFalseJump:
				// TODO: if (pDataType != boolean) { mpParser->Err(L"AddStat: invalid condition"); err++; break; }
				if (pArg2 == 0) { mpParser->Err(L"AddStat: invalid jump destination"); err++; break; }
				break;
			default:
				if (pArg2 != 0) { mpParser->Err(L"AddStat: too much parameters"); err++; }
		}

		if (err > 0) { return 0; }	// don't create DAC with errors
	}

	// get temporary variable name for DAC result
	wchar_t* pName = CreateString(L"$t", mpDACList->size());

	// create new DACSymbol and link DACLabel (if one is registered)
	DACSymbol* stat = new DACSymbol(pDataType, pName, op, pArg1, pArg2, mpCurrLabel);
	// add new DACSymbol to DACList
	mpDACList->push_back(stat);

	// registered DACLabel now linked with new DACSymbol -> unregister
	mpCurrLabel = 0;

	coco_string_delete(pName);
	return stat;
}

tDACList const*const DACGenerator::GetDACList() const
{
	return mpDACList;
}

} // namespace MIEC
