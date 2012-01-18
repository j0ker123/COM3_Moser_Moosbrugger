
#include <assert.h>
#include "RegisterAdmin.h"

namespace MIEC {

RegisterAdmin::RegisterAdmin(size_t const nrRegs)
{
}

RegisterAdmin::~RegisterAdmin()
{
}

tRegNr const RegisterAdmin::GetRegister()
{
	// allocate new unused register
	tRegNr regNr = mRegList.size();
	mRegList.insert(tRegEntry(regNr, 0));
	return regNr;
}

tRegNr const RegisterAdmin::GetRegister(Symbol const*const pSym)
{
	assert(pSym != 0);
	if (pSym == 0) { return GetRegister(); }

	// search register assigned to pSym
	tRegList::const_iterator itor = mRegList.begin();
	for (; itor != mRegList.end(); itor++) {
		if (itor->second == pSym) { return itor->first; }
	}
	// if not found allocate new (unused) register
	return GetRegister();
}

void RegisterAdmin::AssignRegister(tRegNr const regNr, Symbol const*const pSym)
{
	assert(pSym != 0);

	// assign regNr to pSym
	tRegList::iterator ret = mRegList.find(regNr);
	ret->second = pSym;
}

void RegisterAdmin::FreeRegister(tRegNr const regNr)
{
	// free (set unused) regNr
	mRegList.erase(regNr);
}

} // namespace MIEC
