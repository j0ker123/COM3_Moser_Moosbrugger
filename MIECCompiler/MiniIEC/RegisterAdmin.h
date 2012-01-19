
#ifndef REGISTERADMIN_H_
#define REGISTERADMIN_H_

#include <map>
#include "GlobalDef.h"
#include "Symbol.h"
#include "CodeGenProl16.h"

namespace MIEC {

class RegisterAdmin {
public:
	RegisterAdmin(CodeGenProl16* const pProl16Gen, const size_t nrRegs);
	~RegisterAdmin();

	const size_t GetRegister();
	const size_t GetRegister(const Symbol* const pSym);
	void AssignRegister(const size_t regNr, const Symbol* const pSym);
	void FreeRegister(const size_t regNr);

private:
	CodeGenProl16* const mpProl16Gen;
	const size_t mNrRegisters;

	typedef std::pair<const size_t, const Symbol*> tRegEntry;
	typedef std::map<const size_t, const Symbol*> tRegList;
	tRegList mRegList;
};

} // namespace MIEC

#endif // #ifndef REGISTERADMIN_H_
