#include <iomanip>
#include "SymbolTable.h"

namespace MIEC {

SymbolTable::SymbolTable()
{
}

SymbolTable::~SymbolTable()
{
	// for each entry in symbol table ...
	tSymbolList::iterator itor = mSymbolList.begin();
	for (; itor != mSymbolList.end(); itor++) {
		// delete symbol
		delete itor->second; itor->second = 0;
	}
	// erase all entries
	mSymbolList.clear();
}

Symbol* const SymbolTable::AddSymbol(Symbol* pSymbol)
{
	if (pSymbol == 0) {
		return 0;	// invalid symbol
	}
	wchar_t* pName = pSymbol->GetName();
	if (pName == 0 || coco_string_equal(pName, L"")) {
		delete pSymbol; pSymbol = 0;
		return 0;	// invalid symbol name
	}

	// insert symbol pSymbol to symbol table (if not already added)
	std::pair<tSymbolList::iterator, bool> ret = mSymbolList.insert(tSymbolEntry(pName, pSymbol));
	if (ret.second == false)
	{
		// symbol already added to symbol table
		if (ret.first->second != pSymbol) {
			delete pSymbol;
			pSymbol = ret.first->second;
		}
	}		
	return pSymbol;
}

Symbol* const SymbolTable::FindSymbol(wchar_t* const pName) 
{
	if (pName == 0 || coco_string_equal(pName, L"")) { 
		return 0;	// invalid symbol name
	}

	// search symbol with name pName in symbol table
	tSymbolList::const_iterator ret = mSymbolList.find(pName);
	if (ret == mSymbolList.end()) {
		return 0;	// undefined symbol
	}
	return ret->second;
}

void SymbolTable::PrintTable(std::wostream& out) const
{
	// for each entry...
	tSymbolList::const_iterator itor = mSymbolList.begin();
	for (; itor != mSymbolList.end(); itor++) {
		out << L" " << std::left;
		// print Key
		out << std::setw(10) << itor->first << L"\t: ";
		// print Symbol
		itor->second->Symbol::Print(out);
		out << std::endl;
	}
}

} // namespace MIEC
