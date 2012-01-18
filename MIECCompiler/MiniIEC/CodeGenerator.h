#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include "DACGenerator.h"
#include "CodeGenProl16.h"

namespace MIEC {

class CodeGenerator {
public:
	CodeGenerator(tDACList const * apDacList);
	~CodeGenerator();

	void GenerateCode(std::wstring const& arFileName);

private:
	typedef tDACList::size_type DACPosition;

	void OperationAdd(DACSymbol* apDacSym, DACPosition aDacPos);
	void OperationSubtract(DACSymbol* apDacSym, DACPosition aDacPos);
	void OperationMultiply(DACSymbol* apDacSym);
	void OperationDivide(DACSymbol* apDacSym);
	void OperationAssign(DACSymbol* apDacSym, DACPosition aDacPos);
	void OperationJump(DACSymbol* apDacSym, std::map<WORD, DACLabel*>& arUnresolvedJumps);
	void OperationConditionalJump(DACSymbol* apDacSym, std::map<WORD, DACLabel*>& arUnresolvedJumps, DACPosition aDacPos);
	void OperationPrint(DACSymbol* apDacSym, DACPosition aDacPos);

	//private members
	const tDACList* mpDacList;
	CodeGenProl16* mpGenProl16;
	RegisterAdmin* mpRegAdmin;
};

} // MIEC

#endif // CODEGENERATOR_H