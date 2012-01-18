
#ifndef REGISTERADMIN_H_
#define REGISTERADMIN_H_

#include <map>
#include "GlobalDef.h"
#include "Symbol.h"

namespace MIEC {

typedef BYTE tRegNr;

class RegisterAdmin {
public:
	RegisterAdmin(size_t const nrRegs);
	~RegisterAdmin();

	tRegNr const GetRegister();
	tRegNr const GetRegister(Symbol const*const pSym);
	void AssignRegister(tRegNr const regNr, Symbol const*const pSym);

private:
	typedef std::pair<tRegNr, Symbol const*> tRegEntry;
	typedef std::map<tRegNr, Symbol const*> tRegList;
	tRegList mRegList;
};

} // namespace MIEC

#endif // #ifndef REGISTERADMIN_H_
