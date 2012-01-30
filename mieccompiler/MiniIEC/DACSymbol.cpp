#include <assert.h>
#include "Scanner.h"
#include "DACSymbol.h"

namespace MIEC {

DACLabel::DACLabel(wchar_t* const name)
	: Symbol(Symbol::eLabel, name, 0)
{
	assert(name != 0 && !coco_string_equal(name, L""));
}

size_t const DACLabel::GetAddr() const
{
	return mAddr;
}

void DACLabel::SetAddr(size_t const addr)
{
	mAddr = addr;
}


DACSymbol::DACSymbol(DataType* const pDataType, wchar_t* const name, OpKind const operation, Symbol* const pArg1, Symbol* const pArg2, DACLabel*const pLabel)
	: Symbol(Symbol::eTempVar, name, pDataType), mOperation(operation), mpArg1(pArg1), mpArg2(pArg2), mpLabel(pLabel)
{
	if (pDataType == 0) {
		printf("test");
	}
	assert(name != 0 && !coco_string_equal(name, L""));
}

DACLabel* const DACSymbol::GetLabel() const
{
	return mpLabel;
}

DACSymbol::OpKind const DACSymbol::GetOperation() const
{
	return mOperation;
}

Symbol* const DACSymbol::GetArgument1() const
{
	return mpArg1;
}

Symbol* const DACSymbol::GetArgument2() const
{
	return mpArg2;
}

const wchar_t* const DACSymbol::Print()
{
		const wchar_t* operation;

		switch(this->mOperation) {
		case DACSymbol::eAdd :
			operation = coco_string_create(L"ADD"); break;
		case DACSymbol::eSubtract :
			operation = coco_string_create(L"SUB"); break;
		case DACSymbol::eMultiply :
			operation = coco_string_create(L"MULT"); break;
		case DACSymbol::eDivide :
			operation = coco_string_create(L"DIV"); break;
		case DACSymbol::eAssign :
			operation = coco_string_create(L"ASSIGN"); break;
		case DACSymbol::eIfFalseJump :
			operation = coco_string_create(L"IFFALSEJUMP"); break;
		case DACSymbol::eIsNotEqual :
			operation = coco_string_create("NOTEQUAL"); break;
		case DACSymbol::eJump :
			operation = coco_string_create(L"JUMP"); break;
		case DACSymbol::eIfJump :
			operation = coco_string_create(L"IFJUMP"); break;
		case DACSymbol::ePrint :
			operation = coco_string_create(L"PRINT"); break;
		case DACSymbol::eIsEqual :
			operation = coco_string_create(L"EQUAL"); break;
		case DACSymbol::eIsGreater :
			operation = coco_string_create(L"GREATER"); break;
		case DACSymbol::eIsGreaterEqual :
			operation = coco_string_create(L"GREATEREQUAL"); break;
		case DACSymbol::eIsLess :
			operation = coco_string_create(L"LESS"); break;
		case DACSymbol::eIsLessEqual : 
			operation = coco_string_create(L"LESSEQUAL"); break;
		case DACSymbol::eExit :
			operation = coco_string_create(L"EXIT"); break;
		default:
			operation = coco_string_create(L"UNKOWN"); break;
		}

		const wchar_t* returnVal = coco_string_create(this->GetName());
		returnVal = coco_string_create_append(returnVal, L" =\n" );

		if ( this->mpArg1 != NULL ) {
			if ( wmemcmp( this->mpArg1->GetName(), L"$", 1) == 0 )
			{
				returnVal = coco_string_create_append(returnVal, this->mpArg1->GetName() );
			} 
			else
			{
				returnVal = coco_string_create_append(returnVal, L"Arg1: " );
				returnVal = coco_string_create_append(returnVal, this->mpArg1->Print() );
				returnVal = coco_string_create_append(returnVal, L" " );
			}
		}

		returnVal = coco_string_create_append(returnVal, L" OP: ");
		returnVal = coco_string_create_append(returnVal, operation );
		returnVal = coco_string_create_append(returnVal, L" ");

		if ( this->mpArg2 != NULL ) {
			if ( wmemcmp( this->mpArg2->GetName(), L"$", 1) == 0 )
			{
				returnVal = coco_string_create_append(returnVal, this->mpArg2->GetName() );
			} 
			else
			{
				returnVal = coco_string_create_append(returnVal, L"Arg2: " );
				returnVal = coco_string_create_append(returnVal, this->mpArg2->Print() );
				returnVal = coco_string_create_append(returnVal, L" " );
			}
		}

		delete operation, operation = 0;

		return returnVal;
}

} // namespace MIEC
