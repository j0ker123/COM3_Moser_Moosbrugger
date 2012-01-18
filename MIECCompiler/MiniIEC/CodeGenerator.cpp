#include <assert.h>
#include "CodeGenerator.h"

namespace MIEC {

CodeGenerator::CodeGenerator(tDACList const * apDacList)
	: mpDacList(apDacList)
{
	mpGenProl16 = new CodeGenProl16();
}

CodeGenerator::~CodeGenerator()
{
	delete mpGenProl16; mpGenProl16 = 0;
}

void CodeGenerator::GenerateCode(std::wstring const& arFileName)
{
	assert(mpDacList != 0);
	assert(mpGenProl16 != 0);

	tDACList::const_iterator itor = mpDacList.begin();
	for (; itor != mpDacList.end(); itor++) 
	{
		DACSymbol* sym = *itor;
		DACSymbol::OpKind op = sym->GetOperation();

		switch (op) {
		case DACSymbol::eAdd :
			OperationAdd(sym, mpGenProl16->GetCodePosition()); break;
		case DACSymbol::eSubtract :
			OperationSubtract(sym, mpGenProl16->GetCodePosition()); break;
		case DACSymbol::eMultiply :
			OperationMultiply(sym); break;
		case DACSymbol::eDivide :
			OperationDivide(sym); break;
		case DACSymbol::eAssign :
			OperationAssign(sym, mpGenProl16->GetCodePosition()); break;
		case DACSymbol::eJump :
			OperationJump(sym, ...); break;
		case DACSymbol::eIfJump :
			OperationConditionalJump(sym, ...); break;
		case DACSymbol::ePrint :
			OperationPrint(sym, mpGenProl16->GetCodePosition()); break;
		default:
			...
		}
	}

	mpGenProl16->WriteIex(arFileName);
}


////////////////////////////////////////////////////////////////////////////
// Addition
//
// result = addend1
// result += addend2
//
////////////////////////////////////////////////////////////////////////////
void CodeGenerator::OperationAdd(DACSymbol* apDacSym, DACPosition aDacPos)
{
	RegNr regA = mpRegAdmin->GetRegister(apDacSym->GetArgument1());	// addend1
	RegNr regB = mpRegAdmin->GetRegister(apDacSym->GetArgument2()); // addend2
	RegNr regResult = mpRegAdmin->GetRegister(); // result

	mpGenProl16->Move(regResult, regA);
	mpGenProl16->Add(regResult, regB);

	mpRegAdmin->AssignRegister(regResult, apDacSym);	// ???
}

////////////////////////////////////////////////////////////////////////////
// Subtraction
//
// result = muniend
// result -= subtrahend
//
////////////////////////////////////////////////////////////////////////////
void CodeGenerator::OperationSubtract(DACSymbol* apDacSym, DACPosition aDacPos)
{
	RegNr regA = mpRegAdmin->GetRegister(apDacSym->GetArgument1());	// minuend
	RegNr regB = mpRegAdmin->GetRegister(apDacSym->GetArgument2()); // subtrahend
	RegNr regResult = mpRegAdmin->GetRegister(); // result

	mpGenProl16->Move(regResult, regA);
	mpGenProl16->Sub(regResult, regB);

	mpRegAdmin->AssignRegister(regResult, apDacSym);	// ???
}

////////////////////////////////////////////////////////////////////////////
// Multiplication by shift
//
// result = 0
// while (multiplier != 0)
// {
// multiplier = multiplier >> 1
// if (carry != 0)
// {
// result += multiplikand
// }
// multiplikand = multiplikand << 1
// }
//
////////////////////////////////////////////////////////////////////////////
void CodeGenerator::OperationMultiply(DACSymbol* apDacSym)
{
	RegNr regA = mpRegAdmin->GetRegister(apDacSym->GetArgument1()); // multiplikand
	RegNr regB = mpRegAdmin->GetRegister(apDacSym->GetArgument2()); // multiplier
	RegNr regJmp = mpRegAdmin->GetRegister(); // used for jumps
	RegNr regResult = mpRegAdmin->GetRegister(); // will contain result
	mpGenProl16->LoadI(regResult, 0); //init
	WORD codePosStart = mpGenProl16->GetCodePosition();
	RegNr helpReg = mpRegAdmin->GetRegister();
	mpGenProl16->LoadI(helpReg, 0);
	mpGenProl16->Comp(regB, helpReg); //compare: multiplier != 0
	WORD jumpData1 = mpGenProl16->LoadI(regJmp, 0);
	mpGenProl16->JumpZ(regJmp);
	mpGenProl16->ShR(regB);
	WORD jumpData2 = mpGenProl16->LoadI(regJmp, 0);
	mpGenProl16->JumpC(regJmp);
	mpGenProl16->ShL(regA);
	mpGenProl16->LoadI(regJmp, codePosStart);
	mpGenProl16->Jump(regJmp);
	mpGenProl16->SetAddress(jumpData2, mpGenProl16->GetCodePosition());
	mpGenProl16->Add(regResult, regA);
	mpGenProl16->ShL(regA);
	mpGenProl16->LoadI(regJmp, codePosStart);
	mpGenProl16->Jump(regJmp);
	mpGenProl16->SetAddress(jumpData1, mpGenProl16->GetCodePosition());

	// regResult contains result of multiplication -> assign to DAC-symbol
	mpRegAdmin->AssignRegister(regResult, apDacSym);

	// free all other registers
	mpRegAdmin->FreeRegister(regA);
	mpRegAdmin->FreeRegister(regB);
	mpRegAdmin->FreeRegister(regJmp);
	mpRegAdmin->FreeRegister(helpReg);
}

////////////////////////////////////////////////////////////////////////////
// Division by shift
//
// remainder = 0
// bits = 16
// do
// {
// [Logic shift left] dividend
// [Shift left] remainder
// if ((carry != 0) || (remainder >= divisor))
// {
// remainder -= divisor
// dividend |= 0x01
// }
// bits--
// } while (bits > 0)
//
////////////////////////////////////////////////////////////////////////////
void CodeGenerator::OperationDivide(DACSymbol* apDacSym)
{
	RegNr regA = mpRegAdmin->GetRegister(apDacSym->GetArgument1()); // dividend
	RegNr regB = mpRegAdmin->GetRegister(apDacSym->GetArgument2()); // divisor
	RegNr regJmp = mpRegAdmin->GetRegister(); // used for jumps
	RegNr regRemainder = mpRegAdmin->GetRegister(); // remainder
	mpGenProl16->LoadI(regRemainder, 0);
	RegNr regBits = mpRegAdmin->GetRegister(); // bit counter
	mpGenProl16->LoadI(regBits, 16);
	WORD codePosStart = mpGenProl16->GetCodePosition();
	mpGenProl16->ShL(regA);
	mpGenProl16->ShLC(regRemainder);
	WORD jumpData1 = mpGenProl16->LoadI(regJmp, 0);
	mpGenProl16->JumpC(regJmp);
	mpGenProl16->Comp(regB, regRemainder);
	mpGenProl16->JumpC(regJmp);
	mpGenProl16->JumpZ(regJmp);
	WORD jumpData2 = mpGenProl16->LoadI(regJmp, 0);
	mpGenProl16->Jump(regJmp);
	mpGenProl16->SetAddress(jumpData1, mpGenProl16->GetCodePosition());
	mpGenProl16->Sub(regRemainder, regB);
	RegNr helpReg = mpRegAdmin->GetRegister();
	mpGenProl16->LoadI(helpReg, 1);
	mpGenProl16->Or(regA, helpReg);
	mpGenProl16->SetAddress(jumpData2, mpGenProl16->GetCodePosition());
	mpGenProl16->Dec(regBits);
	WORD jumpData3 = mpGenProl16->LoadI(regJmp, 0);
	mpGenProl16->JumpZ(regJmp);
	mpGenProl16->LoadI(regJmp, codePosStart);
	mpGenProl16->Jump(regJmp);
	mpGenProl16->SetAddress(jumpData3, mpGenProl16->GetCodePosition());

	// regA contains result of division -> assign to DAC-symbol
	mpRegAdmin->AssignRegister(regA, apDacSym);

	// free all other registers
	mpRegAdmin->FreeRegister(regB);
	mpRegAdmin->FreeRegister(regJmp);
	mpRegAdmin->FreeRegister(regRemainder);
	mpRegAdmin->FreeRegister(regBits);
	mpRegAdmin->FreeRegister(helpReg);
}

////////////////////////////////////////////////////////////////////////////
// Assignment
//
// result = source
//
////////////////////////////////////////////////////////////////////////////
void CodeGenerator::OperationAssign(DACSymbol* apDacSym, DACPosition aDacPos)
{
	RegNr regA = mpRegAdmin->GetRegister(apDacSym->GetArgument1());	// destination
	RegNr regB = mpRegAdmin->GetRegister(apDacSym->GetArgument2());	// source

	mpGenProl16->Move(regA, regB);

	mpRegAdmin->AssignRegister(regA, apDacSym);	// ???
}

} // namespace MIEC
