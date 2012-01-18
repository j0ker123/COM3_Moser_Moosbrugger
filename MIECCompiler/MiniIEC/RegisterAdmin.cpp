
#include <assert.h>
#include "RegisterAdmin.h"

namespace MIEC {

RegisterAdmin::RegisterAdmin(CodeGenProl16* const pProl16Gen, const size_t nrRegs)
	: mpProl16Gen(pProl16Gen), mNrRegisters(nrRegs)
{
}

RegisterAdmin::~RegisterAdmin()
{
}

tRegNr const RegisterAdmin::GetRegister()
{
	// allocate new unused register
	tRegNr regNr = mRegList.size();
	const Symbol* const pSym = 0;
	mRegList.insert(tRegEntry(regNr, pSym));
	return regNr;
}

tRegNr const RegisterAdmin::GetRegister(const Symbol* const pSym)
{
	assert(pSym != 0);
	
	if (pSym != 0 && mRegList.size() > 0) {

		// search register assigned to pSym
		tRegList::const_iterator itor = mRegList.begin();
		for (; itor != mRegList.end(); itor++) {
			if (itor->second == pSym) { return itor->first; }
		}
	}

	// if not found allocate new (unused) register
	tRegNr regNr = GetRegister();
	mpProl16Gen->LoadI(regNr, ((ConstSym*)pSym)->GetVal());
	return regNr;
}

void RegisterAdmin::AssignRegister(const tRegNr regNr, const Symbol* const pSym)
{
	assert(pSym != 0);

	// assign regNr to pSym
	tRegList::iterator ret = mRegList.find(regNr);
	ret->second = pSym;
}

void RegisterAdmin::FreeRegister(const tRegNr regNr)
{
	// free (set unused) regNr
	mRegList.erase(regNr);
}

} // namespace MIEC
