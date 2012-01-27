#ifndef CODEGENERATOR_H_
#define CODEGENERATOR_H_

#include <map>
#include "DACGenerator.h"
#include "CodeGenProl16.h"
#include "RegisterAdmin.h"

namespace MIEC {

class CodeGenerator {
public:
	typedef BYTE tRegNr;

	CodeGenerator(const tDACList* const apDacList, const size_t nrRegisters);
	~CodeGenerator();

	void GenerateCode(const std::wstring& arFileName);

private:
	CodeGenerator();
	CodeGenerator(const CodeGenerator&);
	CodeGenerator& operator= (const CodeGenerator&);

	typedef tDACList::size_type tDACPosition;
	typedef std::pair<DACLabel*, WORD> tJumpLblEntry;
	typedef std::map<DACLabel*, WORD> tJumpLblList;

	void OperationAdd(DACSymbol* apDacSym, tDACPosition aDacPos);
	void OperationSubtract(DACSymbol* apDacSym, tDACPosition aDacPos);
	void OperationMultiply(DACSymbol* apDacSym);
	void OperationDivide(DACSymbol* apDacSym);
	void OperationAssign(DACSymbol* apDacSym, tDACPosition aDacPos);
	void OperationJump(DACSymbol* apDacSym , tJumpLblList& arUnresolvedJumps);
	void OperationConditionalJump(DACSymbol* apDacSym, tJumpLblList& arUnresolvedJumps, tDACPosition aDacPos);
	void OperationPrint(DACSymbol* apDacSym, tDACPosition aDacPos);

	//private members
	const tDACList* const mpDacList;
	CodeGenProl16* const mpGenProl16;
	RegisterAdmin* const mpRegAdmin;
};

} // namespace MIEC

#endif // #ifndef CODEGENERATOR_H_
