
#ifndef REGISTERADMIN_H_
#define REGISTERADMIN_H_

#include <map>
#include "GlobalDef.h"
#include "Symbol.h"
#include "CodeGenProl16.h"

namespace MIEC {

typedef BYTE tRegNr;

class RegisterAdmin {
public:
	RegisterAdmin(CodeGenProl16* const pProl16Gen, const size_t nrRegs);
	~RegisterAdmin();

	const tRegNr GetRegister();
	const tRegNr GetRegister(const Symbol* const pSym);
	void AssignRegister(const tRegNr regNr, const Symbol* const pSym);
	void FreeRegister(const tRegNr regNr);

private:
	CodeGenProl16* const mpProl16Gen;
	const size_t mNrRegisters;

	typedef std::pair<tRegNr, const Symbol*> tRegEntry;
	typedef std::map<tRegNr, const Symbol*> tRegList;
	tRegList mRegList;
};

} // namespace MIEC

#endif // #ifndef REGISTERADMIN_H_
