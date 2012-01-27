#include <assert.h>
#include "Parser.h"
#include "DACGenerator.h"

namespace MIEC {

DACGenerator::DACGenerator(Parser* const pParser)
	: mpParser(pParser), mSymbolTable(pParser), mpCurrLabel(0), mLabelNumber(0), mErrorCounter(0)
{
	assert(mpParser != 0);
}

DACGenerator::~DACGenerator()
{
	// Don't delete Symbols in mDACList!
	// Symbols, DACLabels and DACSymbols (all added to SymbolTable) ...
	// will be deleted by mSymbolTable destructor.
}

wchar_t* CreateString(wchar_t* const prefix, size_t const number)
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
	// get unique consecutive name for new DACLabel
	wchar_t* pName = CreateString(L"$L", mLabelNumber++);

	// create new DACLabel symbol
	DACLabel* const pLabel = new DACLabel(pName);

	// add DACLabel to SymbolTable
	mSymbolTable.AddSymbol(pLabel);

	coco_string_delete(pName);
	return pLabel;
}

DataType* const DACGenerator::AddType(DataType* pType)
{
	// add DataType to SymbolTable
	return (DataType*)(mSymbolTable.AddSymbol(pType));
}

Symbol* const DACGenerator::AddSymbol(Symbol* pSymbol)
{
	// add Symbol to SymbolTable
	return mSymbolTable.AddSymbol(pSymbol);
}

DACLabel* const DACGenerator::AddLabel(DACLabel* const pLabel)
{
	if (pLabel == 0) { return 0; }

	// register label - don't override registered label
	if (mpCurrLabel == 0) {
		mpCurrLabel = pLabel;
	}

	// return registered label
	return mpCurrLabel;
}

DACSymbol* const DACGenerator::AddStat(DACSymbol::OpKind const op, Symbol* const pArg1, Symbol* const pArg2)
{
	assert(mpParser != 0);

	size_t err = 0;				// local error counter
	DataType* pDataType = 0;	// unknown or no DataType;

	if (op == DACSymbol::eUnknown) {
		mpParser->Err(L"AddStat: invalid operation");
		err++;	// count error
	}
	else if (op == DACSymbol::eExit) {
		pDataType = this->AddType(new Void());	// set DataType of DAC result
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
			pDataType = pArg1->GetDataType();	// set DataType of DAC result
		}

		switch (op) {
			case DACSymbol::eAssign:
				if (pArg2 == 0) { mpParser->Err(L"AddStat: invalid assignment source"); err++; break; }
				if (pDataType != pArg2->GetDataType()) { mpParser->Err(L"AddStat: incompatible types"); err++; break; }
				break;
			case DACSymbol::eAdd: case DACSymbol::eSubtract: case DACSymbol::eMultiply: case DACSymbol::eDivide:
				if (pArg2 == 0) { mpParser->Err(L"AddStat: invalid right parameter"); err++; break; }
				if (pDataType != pArg2->GetDataType()) { mpParser->Err(L"AddStat: incompatible types"); err++; break; }
				break;
			case DACSymbol::eIsEqual: case DACSymbol::eIsNotEqual: case DACSymbol::eIsLessEqual: case DACSymbol::eIsGreaterEqual: case DACSymbol::eIsLess: case DACSymbol::eIsGreater:
				if (pArg2 == 0) { mpParser->Err(L"AddStat: invalid right parameter"); err++; break; }
				pDataType = this->AddType(new Boolean());	// set DataType of DAC result
				break;
			case DACSymbol::eIfJump: case DACSymbol::eIfFalseJump:
				if (pDataType != this->AddType(new Boolean())) { mpParser->Err(L"AddStat: invalid condition (Boolean expected)"); err++; break; }
				if (pArg2 == 0) { mpParser->Err(L"AddStat: invalid jump destination"); err++; break; }
				pDataType = this->AddType(new Void());	// set DataType of DAC result
				break;
			default:
				if (pArg2 != 0) { mpParser->Err(L"AddStat: too much parameters"); err++; break; }
				pDataType = this->AddType(new Void());	// set DataType of DAC result
		}
	}

	// if an error occured...
	if (err > 0) { 
		mErrorCounter += err;	// increase error counter
		return 0;				// don't create DAC with errors
	}

	// get unique consecutive name for DAC result containing temporary variable
	wchar_t* pName = CreateString(L"$t", mDACList.size());

	// create new DACSymbol and link DACLabel (if one is registered)
	DACSymbol* pStat = new DACSymbol(pDataType, pName, op, pArg1, pArg2, mpCurrLabel);
	
	// add DACSymbol to SymbolTable
	mSymbolTable.AddSymbol(pStat);
	// add DACSymbol to DACList
	mDACList.push_back(pStat);

	// registered DACLabel now linked with new DACSymbol -> unregister
	mpCurrLabel = 0;

	coco_string_delete(pName);
	return pStat;
}

Symbol* const DACGenerator::FindSymbol(wchar_t* const pName)
{
	// return Symbol found in SymbolTable
	return mSymbolTable.FindSymbol(pName);
}

const SymbolTable& DACGenerator::GetSymbolList() const
{
	return mSymbolTable;
}

const tDACList& DACGenerator::GetDACList() const
{
	return mDACList;
}

size_t const DACGenerator::GetErrorCounter() const
{
	return mErrorCounter;
}

} // namespace MIEC
