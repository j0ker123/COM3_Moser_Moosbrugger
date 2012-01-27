#include <assert.h>
#include "RegisterAdmin.h"

namespace MIEC {

RegisterAdmin::RegisterAdmin(CodeGenProl16* const pProl16Gen, const size_t nrRegs)
	: mpProl16Gen(pProl16Gen), mNrRegisters(nrRegs)
{
	assert(pProl16Gen != 0);
}

RegisterAdmin::~RegisterAdmin()
{
}

const size_t RegisterAdmin::GetRegister()
{
	// search unused register
	size_t regNr = 0;
	while (mRegList.find(regNr) != mRegList.end()) {
		regNr++;
	}

	// allocate new unused register
	const Symbol* const pSym = 0;
	mRegList.insert(tRegEntry(regNr, pSym));
	return regNr;
}

const size_t RegisterAdmin::GetRegister(const Symbol* const pSym)
{
	assert(mpProl16Gen != 0);

	if (pSym == 0) {
		// return unused register
		return GetRegister();
	}
	
	if (mRegList.size() > 0) {
		// search register assigned to pSym
		tRegList::const_iterator itor = mRegList.begin();
		for (; itor != mRegList.end(); itor++) {
			if (itor->second == pSym) {
				// return assigned register
				return itor->first;
			}
		}
	}

	// if not found allocate new (unused) register and load value
	size_t addrRegNr = 0;
	size_t regNr = GetRegister();
	switch (pSym->GetType()) {
		case Symbol::eConst:
			// load register with immediate value
			mpProl16Gen->LoadI(regNr, ((ConstSym*)pSym)->GetVal());
			break;
		case Symbol::eVar:
			addrRegNr = GetRegister();
			// load address register with address
			mpProl16Gen->LoadI(addrRegNr, ((VarSym*)pSym)->GetAddr());
			mpProl16Gen->Load(regNr, addrRegNr);
			FreeRegister(addrRegNr);
			break;
		default:
			assert(false);
			break;
	}
	// assign register
	AssignRegister(regNr, pSym);
	return regNr;
}

void RegisterAdmin::AssignRegister(const size_t regNr, const Symbol* const pSym)
{
	assert(pSym != 0);

	// assign regNr to pSym
	tRegList::iterator ret = mRegList.find(regNr);
	ret->second = pSym;
}

void RegisterAdmin::FreeRegister(const size_t regNr)
{
	// free (set unused) regNr
	mRegList.erase(regNr);
}

} // namespace MIEC
