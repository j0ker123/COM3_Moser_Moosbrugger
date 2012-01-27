#include <assert.h>
#include "CodeGenerator.h"

namespace MIEC {

CodeGenerator::CodeGenerator(const tDACList* const apDacList, const size_t nrRegisters)
	: mpDacList(apDacList), mpGenProl16(new CodeGenProl16()), mpRegAdmin(new RegisterAdmin(mpGenProl16, nrRegisters))
{
}

CodeGenerator::~CodeGenerator()
{
	delete mpGenProl16;
	delete mpRegAdmin;
}

void CodeGenerator::GenerateCode(const std::wstring& arFileName)
{
	assert(mpDacList != 0);
	assert(mpGenProl16 != 0);

	tJumpLblList jumpLblList;

	// run through DAC list - for each entry ...
	tDACList::const_iterator itor = mpDacList->begin();
	for (; itor != mpDacList->end(); itor++) 
	{
		// get DAC
		DACSymbol* pSym = *itor;
		// get code position
		WORD const codePos = mpGenProl16->GetCodePosition();

		// get label linked with DAC (if there is one)
		DACLabel* pLabel = pSym->GetLabel();
		if (pLabel != 0) {
			// set label address
			pLabel->SetAddr(codePos);
			// search for bookmarked code positions using label
			tJumpLblList::iterator ret = jumpLblList.find(pLabel);
			if (ret != jumpLblList.end()) {
				// write current code position as label address to program code
				mpGenProl16->SetAddress(ret->second, codePos);
				jumpLblList.erase(ret);
			}
			else {
				// bookmark current code position as label address
				jumpLblList.insert(tJumpLblEntry(pLabel, codePos));
			}
		}

		// get DAC operation
		DACSymbol::OpKind op = pSym->GetOperation();
		// create program code depending on kind of operation
		switch (op) {
		case DACSymbol::eAdd:
			OperationAdd(pSym); break;
		case DACSymbol::eSubtract:
			OperationSubtract(pSym); break;
		case DACSymbol::eMultiply:
			OperationMultiply(pSym); break;
		case DACSymbol::eDivide:
			OperationDivide(pSym); break;
		case DACSymbol::eAssign:
			OperationAssign(pSym); break;
		case DACSymbol::eJump:
			OperationJump(pSym, jumpLblList); break;
		case DACSymbol::eIfJump: case DACSymbol::eIfFalseJump:
			OperationConditionalJump(pSym, jumpLblList); break;
		case DACSymbol::ePrint:
			OperationPrint(pSym); break;
		case DACSymbol::eExit:
			mpGenProl16->Sleep();
		default:
			// TODO: ...
			break;
		}
	}

	// export compiled program code
	mpGenProl16->WriteIex(arFileName);
}


////////////////////////////////////////////////////////////////////////////
// Addition
//
// result = addend1
// result += addend2
//
////////////////////////////////////////////////////////////////////////////
void CodeGenerator::OperationAdd(DACSymbol* apDacSym)
{
	tRegNr regA = mpRegAdmin->GetRegister(apDacSym->GetArgument1());	// addend1
	tRegNr regB = mpRegAdmin->GetRegister(apDacSym->GetArgument2());	// addend2
	tRegNr regResult = mpRegAdmin->GetRegister();	// result

	mpGenProl16->Move(regResult, regA);
	mpGenProl16->Add(regResult, regB);

	// assign result register to DAC-Symbol
	mpRegAdmin->AssignRegister(regResult, apDacSym);

	// free all other registers
	mpRegAdmin->FreeRegister(regA);
	mpRegAdmin->FreeRegister(regB);
}

////////////////////////////////////////////////////////////////////////////
// Subtraction
//
// result = muniend
// result -= subtrahend
//
////////////////////////////////////////////////////////////////////////////
void CodeGenerator::OperationSubtract(DACSymbol* apDacSym)
{
	tRegNr regA = mpRegAdmin->GetRegister(apDacSym->GetArgument1());	// minuend
	tRegNr regB = mpRegAdmin->GetRegister(apDacSym->GetArgument2());	// subtrahend
	tRegNr regResult = mpRegAdmin->GetRegister();	// result

	mpGenProl16->Move(regResult, regA);
	mpGenProl16->Sub(regResult, regB);

	// assign result register to DAC-Symbol
	mpRegAdmin->AssignRegister(regResult, apDacSym);

	// free all other registers
	mpRegAdmin->FreeRegister(regA);
	mpRegAdmin->FreeRegister(regB);
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
	tRegNr regA = mpRegAdmin->GetRegister(apDacSym->GetArgument1()); // multiplikand
	tRegNr regB = mpRegAdmin->GetRegister(apDacSym->GetArgument2()); // multiplier
	tRegNr regResult = mpRegAdmin->GetRegister(); // will contain result
	tRegNr regJmp = mpRegAdmin->GetRegister(); // used for jumps

	mpGenProl16->LoadI(regResult, 0); //init
	WORD codePosStart = mpGenProl16->GetCodePosition();
	tRegNr regHelp = mpRegAdmin->GetRegister();
	mpGenProl16->LoadI(regHelp, 0);
	mpGenProl16->Comp(regB, regHelp); //compare: multiplier != 0
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
	mpRegAdmin->FreeRegister(regHelp);
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
	tRegNr regA = mpRegAdmin->GetRegister(apDacSym->GetArgument1()); // dividend
	tRegNr regB = mpRegAdmin->GetRegister(apDacSym->GetArgument2()); // divisor
	tRegNr regJmp = mpRegAdmin->GetRegister(); // used for jumps

	tRegNr regRemainder = mpRegAdmin->GetRegister(); // remainder
	mpGenProl16->LoadI(regRemainder, 0);
	tRegNr regBits = mpRegAdmin->GetRegister(); // bit counter
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
	tRegNr regHelp = mpRegAdmin->GetRegister();
	mpGenProl16->LoadI(regHelp, 1);
	mpGenProl16->Or(regA, regHelp);
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
	mpRegAdmin->FreeRegister(regHelp);
}

////////////////////////////////////////////////////////////////////////////
// Assignment
//
// result = source
//
////////////////////////////////////////////////////////////////////////////
void CodeGenerator::OperationAssign(DACSymbol* apDacSym)
{
	tRegNr regA = mpRegAdmin->GetRegister(apDacSym->GetArgument2());	// source
	tRegNr regB = mpRegAdmin->GetRegister();	// destination address

	VarSym* const pDestination = (VarSym*)(apDacSym->GetArgument1());
	mpGenProl16->LoadI(regB, pDestination->GetAddr());
	mpGenProl16->Store(regA, regB);

	// assign result register to DAC-Symbol
	//mpRegAdmin->AssignRegister(regA, pDestination);
	mpRegAdmin->FreeRegister(regA);

	// free all other registers
	mpRegAdmin->FreeRegister(regB);
}

////////////////////////////////////////////////////////////////////////////
// Jump
//
// 
//
////////////////////////////////////////////////////////////////////////////
void CodeGenerator::OperationJump(DACSymbol* apDacSym, tJumpLblList& arUnresolvedJumps)
{
	tRegNr regJump = mpRegAdmin->GetRegister();

	DACLabel* const pLabel = (DACLabel*)(apDacSym->GetArgument1());
	tJumpLblList::iterator ret = arUnresolvedJumps.find(pLabel);
	if (ret == arUnresolvedJumps.end()) {
		WORD jumpNext = mpGenProl16->LoadI(regJump, 0);
		// label address unknown -> bookmark code position of jump destination to be replaced later
		arUnresolvedJumps.insert(tJumpLblEntry(pLabel, jumpNext));
	}
	else {
		mpGenProl16->LoadI(regJump, ret->second);
		arUnresolvedJumps.erase(ret);
	}
	mpGenProl16->Jump(regJump);

	// free all registers
	mpRegAdmin->FreeRegister(regJump);
}

////////////////////////////////////////////////////////////////////////////
// Conditional Jump
//
// 
//
////////////////////////////////////////////////////////////////////////////
void CodeGenerator::OperationConditionalJump(DACSymbol* apDacSym, tJumpLblList& arUnresolvedJumps)
{
	DACSymbol* const pBranchCond = (DACSymbol*)(apDacSym->GetArgument1());
	tRegNr regA = mpRegAdmin->GetRegister(pBranchCond->GetArgument1());
	tRegNr regB = mpRegAdmin->GetRegister(pBranchCond->GetArgument2());

	tRegNr regJump = mpRegAdmin->GetRegister();
	WORD jumpElse = mpGenProl16->LoadI(regJump, 0);

	DACSymbol::OpKind op = pBranchCond->GetOperation();
	if (apDacSym->GetOperation() == DACSymbol::eIfFalseJump) {
		switch (op) {
			case DACSymbol::eIsEqual:
				op = DACSymbol::eIsNotEqual; break;
			case DACSymbol::eIsNotEqual:
				op = DACSymbol::eIsEqual; break;
			case DACSymbol::eIsLessEqual:
				op = DACSymbol::eIsGreater; break;
			case DACSymbol::eIsGreaterEqual:
				op = DACSymbol::eIsLess; break;
			case DACSymbol::eIsLess:
				op = DACSymbol::eIsGreaterEqual; break;
			case DACSymbol::eIsGreater:
				op = DACSymbol::eIsLessEqual; break;
			default: 
				break;
		}
	}

	switch (op) {
		case DACSymbol::eIsEqual:
			mpGenProl16->Comp(regA, regB);
			mpGenProl16->JumpZ(regJump);
			break;
		case DACSymbol::eIsNotEqual:
			mpGenProl16->Comp(regA, regB);
			mpGenProl16->JumpZ(regJump);
			mpGenProl16->SetAddress(jumpElse, mpGenProl16->GetCodePosition() + 3*sizeof(WORD));
			jumpElse = mpGenProl16->LoadI(regJump, 0);
			mpGenProl16->Jump(regJump);
			mpRegAdmin->FreeRegister(regJump);
			break;
		case DACSymbol::eIsLessEqual:
			mpGenProl16->Comp(regA, regB);
			mpGenProl16->JumpC(regJump);
			mpGenProl16->JumpZ(regJump);
			break;
		case DACSymbol::eIsGreaterEqual:
			mpGenProl16->Comp(regB, regA);
			mpGenProl16->JumpC(regJump);
			mpGenProl16->JumpZ(regJump);
			break;
		case DACSymbol::eIsLess:
			mpGenProl16->Comp(regA, regB);
			mpGenProl16->JumpC(regJump);
			break;
		case DACSymbol::eIsGreater:
			mpGenProl16->Comp(regB, regA);
			mpGenProl16->JumpC(regJump);
			break;
		default: 
			break;
	}

	// label address unknown -> bookmark code position of jump destination to be replaced later
	arUnresolvedJumps.insert(tJumpLblEntry((DACLabel*)(apDacSym->GetArgument2()), jumpElse));

	// free all registers
	mpRegAdmin->FreeRegister(regA);
	mpRegAdmin->FreeRegister(regB);
	mpRegAdmin->FreeRegister(regJump);
}

////////////////////////////////////////////////////////////////////////////
// Print
//
// 
//
////////////////////////////////////////////////////////////////////////////
void CodeGenerator::OperationPrint(DACSymbol* apDacSym) 
{
	tRegNr regA = mpRegAdmin->GetRegister(apDacSym->GetArgument1());	// source
	
	mpGenProl16->PrintInt(regA);

	// free all registers
	mpRegAdmin->FreeRegister(regA);
}

} // namespace MIEC
