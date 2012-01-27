#ifndef REGISTERADMIN_H_
#define REGISTERADMIN_H_

#include <map>
#include "Symbol.h"
#include "CodeGenProl16.h"

namespace MIEC {

class RegisterAdmin {
public:
	RegisterAdmin(CodeGenProl16* const pProl16Gen, size_t const nrRegs);
	~RegisterAdmin();

	size_t const GetRegister();
	size_t const GetRegister(const Symbol* const pSym);
	void AssignRegister(size_t const regNr, const Symbol* const pSym);
	void FreeRegister(size_t const regNr);

private:
	RegisterAdmin();
	RegisterAdmin(const RegisterAdmin&);
	RegisterAdmin& operator= (const RegisterAdmin&);

	CodeGenProl16* const mpProl16Gen;
	size_t const mNrRegisters;

	typedef std::pair<const size_t, const Symbol*> tRegEntry;
	typedef std::map<const size_t, const Symbol*> tRegList;
	tRegList mRegList;
};

} // namespace MIEC

#endif // #ifndef REGISTERADMIN_H_
