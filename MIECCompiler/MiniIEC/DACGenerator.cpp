#include <assert.h>
#include <stdarg.h>
#include "Parser.h"
#include "DACGenerator.h"

namespace MIEC {

DACGenerator::DACGenerator(Parser* const pParser)
	: mpParser(pParser), mpCurrLabel(0), mTempVarNumber(0), mLabelNumber(0), mErrorLine(0), mErrorCounter(0)
{
	assert(mpParser != 0);

	// initialization...
	// register base data types
	this->AddType(new VoidType());
	this->AddType(new BooleanType());
	this->AddType(new IntegerType());
}

DACGenerator::~DACGenerator()
{
	// Don't delete Symbols in mDACList!
	// Symbols, DACLabels and DACSymbols (all added to SymbolTable) ...
	// will be deleted by mSymbolTable destructor.
}

wchar_t* DACGenerator::CreateString(wchar_t* const prefix, size_t const number)
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

Symbol* const DACGenerator::AddSymbol(Symbol* pSymbol)
{
	assert(mpParser != 0);

	if (pSymbol == 0) {
		this->Err(L"AddSymbol: invalid symbol"); 
		return 0;
	}
	wchar_t* const pName = pSymbol->GetName();
	if (pName == 0 || coco_string_equal(pName, L"")) {
		this->Err(L"AddSymbol: invalid symbol name");
		delete pSymbol; pSymbol = 0;
		return 0;
	}
	DataType* const pDataType = pSymbol->GetDataType();
	if (pDataType == 0) {
		this->Err(L"AddSymbol: invalid data type");
		delete pSymbol; pSymbol = 0;
		return 0;
	}

	// check if symbol already declared (in SymbolTable)
	if (pSymbol->GetType() != Symbol::eConst && mSymbolTable.FindSymbol(pName)) {
		this->Err(L"AddSymbol: symbol '%ls' already defined", pName);
	}

	// add Symbol to SymbolTable (new or already added symbol returned)
	return mSymbolTable.AddSymbol(pSymbol);
}

DataType* const DACGenerator::AddType(DataType* pType)
{
	assert(mpParser != 0);

	if (pType == 0) {
		this->Err(L"AddType: invalid datatype"); 
		return 0;
	}
	wchar_t* pName = pType->GetName();
	if (pName == 0 || coco_string_equal(pName, L"")) {
		this->Err(L"AddType: invalid datatype name");
		delete pType; pType = 0;
		return 0;
	}

	// add DataType to SymbolTable (new or already added symbol returned)
	return (DataType*)(mSymbolTable.AddSymbol(pType));
}

DACLabel* const DACGenerator::AddLabel(DACLabel* pLabel)
{
	assert(mpParser != 0);

	if (pLabel == 0) {
		this->Err(L"AddLabel: invalid label"); 
		return 0;
	}
	wchar_t* pName = pLabel->GetName();
	if (pName == 0 || coco_string_equal(pName, L"")) {
		this->Err(L"AddLabel: invalid label name");
		delete pLabel; pLabel = 0;
		return 0;
	}

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
	DataType* pDataType = 0;	// unknown DataType;

	if (op == DACSymbol::eUnknown) {
		this->Err(L"AddStat: unknown operation");
		err++;	// count error
	}
	else {
		// check left parameter...
		switch (op) {
			case DACSymbol::eAssign:
				if (pArg1 == 0) { this->Err(L"Assignment: invalid assignment destination"); err++; break; }
				if (pArg1->GetType() != Symbol::eVar) { this->Err(L"AddStat: invalid assignment destination '%ls' (Variable expected)", pArg1->GetName()); err++; break; }
				pDataType = pArg1->GetDataType();	// set DataType of DAC result
				break;
			case DACSymbol::eAdd: case DACSymbol::eSubtract: case DACSymbol::eMultiply: case DACSymbol::eDivide:
				if (pArg1 == 0) { this->Err(L"Arithmetic: invalid left parameter"); err++; break; }
				pDataType = pArg1->GetDataType();	// set DataType of DAC result
				break;
			case DACSymbol::eIsEqual: case DACSymbol::eIsNotEqual: case DACSymbol::eIsLessEqual: case DACSymbol::eIsGreaterEqual: case DACSymbol::eIsLess: case DACSymbol::eIsGreater:
				if (pArg1 == 0) { this->Err(L"Comparison: invalid left parameter"); err++; break; }
				pDataType = this->GetType(BooleanType::Name);	// set DataType of DAC result
				break;
			case DACSymbol::eIfJump: case DACSymbol::eIfFalseJump:
				if (pArg1 == 0) { this->Err(L"Jump: invalid branch condition"); err++; break; }
				if (pArg1->GetDataType() != this->GetType(BooleanType::Name)) { this->Err(L"AddStat: invalid condition (type Boolean expected)"); err++; break; }
				pDataType = this->GetType(VoidType::Name);	// set DataType of DAC result
				break;
			case DACSymbol::eJump:
				if (pArg1 == 0) { this->Err(L"Jump: invalid jump destination"); err++; break; }
				if (pArg1->GetType() != Symbol::eLabel) { this->Err(L"AddStat: invalid jump destination (Label expected)"); err++; break; }
				pDataType = this->GetType(VoidType::Name);	// set DataType of DAC result
				break;
			case DACSymbol::ePrint:
				if (pArg1 == 0) { this->Err(L"Statement: invalid parameter"); err++; break; }
				pDataType = this->GetType(VoidType::Name);	// set DataType of DAC result
				break;
			case DACSymbol::eExit:
				if (pArg1 != 0) { this->Err(L"Statement: too many parameters"); err++; break; }
				pDataType = this->GetType(VoidType::Name);	// set DataType of DAC result
				break;
			default:
				this->Err(L"AddStat: unhandled operation");
				break;
		}

		// check right parameter...
		switch (op) {
			case DACSymbol::eAssign:
				if (pArg2 == 0) { this->Err(L"Assignment: invalid assignment"); err++; break; }
				//break;	// continue to check datatypes
			case DACSymbol::eAdd: case DACSymbol::eSubtract: case DACSymbol::eMultiply: case DACSymbol::eDivide:
				if (pArg2 == 0) { this->Err(L"Arithmetic: invalid right parameter"); err++; break; }
				if (pDataType && pDataType != pArg2->GetDataType()) { this->Err(L"AddStat: incompatible types (left is '%ls', right is '%ls')", pDataType->GetName(), pArg2->GetDataType()->GetName()); err++; break; }
				break;
			case DACSymbol::eIsEqual: case DACSymbol::eIsNotEqual: case DACSymbol::eIsLessEqual: case DACSymbol::eIsGreaterEqual: case DACSymbol::eIsLess: case DACSymbol::eIsGreater:
				if (pArg2 == 0) { this->Err(L"Comparison: invalid right parameter"); err++; break; }
				break;
			case DACSymbol::eIfJump: case DACSymbol::eIfFalseJump:
				if (pArg2 == 0) { this->Err(L"Jump: invalid jump destination"); err++; break; }
				if (pArg2->GetType() != Symbol::eLabel) { this->Err(L"AddStat: invalid jump destination (Label expected)"); err++; break; }
				break;
			case DACSymbol::eJump: case DACSymbol::ePrint:
				if (pArg2 != 0) { this->Err(L"Statement: too many parameters"); err++; break; }
				break;
			default:
				break;
		}
	}

	// if an error occured...
	if (err > 0) {
		return 0;	// don't create DAC with errors
	}

	// get unique consecutive name for DAC result containing temporary variable
	wchar_t* pName = CreateString(L"$t", mTempVarNumber++);

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
	assert(mpParser != 0);

	if (pName == 0 || coco_string_equal(pName, L"")) {
		this->Err(L"FindSymbol: invalid symbol name");
		return 0;
	}

	// return Symbol found in SymbolTable
	return mSymbolTable.FindSymbol(pName);
}

Symbol* const DACGenerator::GetSymbol(wchar_t* const pName)
{
	assert(mpParser != 0);

	if (pName == 0 || coco_string_equal(pName, L"")) {
		this->Err(L"GetSymbol: invalid symbol name");
		return 0;
	}

	// search for symbol in SymbolTable
	Symbol* pSymbol = mSymbolTable.FindSymbol(pName);
	if (pSymbol == 0) {
		this->Err(L"GetSymbol: undefined symbol '%ls'", pName);
	}

	return pSymbol;
}

DataType* const DACGenerator::GetType(wchar_t* const pName)
{
	assert(mpParser != 0);

	if (pName == 0 || coco_string_equal(pName, L"")) {
		this->Err(L"GetType: invalid type name");
		return 0;
	}

	// search for symbol in SymbolTable
	Symbol* pSymbol = mSymbolTable.FindSymbol(pName);
	if (pSymbol == 0) {
		this->Err(L"GetType: undefined type '%ls'", pName);
	}

	return (DataType*)pSymbol;
}

const SymbolTable& DACGenerator::GetSymbolList() const
{
	return mSymbolTable;
}

const tDACList& DACGenerator::GetDACList() const
{
	return mDACList;
}

void DACGenerator::SetLine(size_t const line)
{
	mErrorLine = line;
}

void DACGenerator::Err(wchar_t* const format, ...)
{
	assert(mpParser != 0);

	if (format == 0) { return; }

	// get optional parameters
	va_list paramList;
	va_start(paramList, format);

	// get message length
	size_t const msgLength = vswprintf(0, 0, format, paramList) + 1;
	wchar_t* msg = new wchar_t[msgLength];

	// create message depending on format
	vswprintf(msg, msgLength, format, paramList);

	// print error message
	//mpParser->errors->Error(mpParser->la->line, mpParser->la->col, msg);
	// more precise error localization by using mErrorLine
	wprintf(L"-- line %i: %ls\n", mErrorLine, msg);

	// count semantic error
	mErrorCounter++;

	coco_string_delete(msg);
}

size_t const DACGenerator::GetErrorCounter() const
{
	assert(mpParser != 0);

	return mErrorCounter;
}

} // namespace MIEC
