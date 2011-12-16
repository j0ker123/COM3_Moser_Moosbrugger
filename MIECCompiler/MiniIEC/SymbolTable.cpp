
#include "Parser.h"
#include "SymbolTable.h"

namespace MIEC {

SymbolTable::SymbolTable(Parser* pParser)
	: mpParser(pParser) { }

SymbolTable::~SymbolTable()
{
	tSymbolList::iterator itor = mSymbolList.begin();
	for (; itor != mSymbolList.end(); itor++) {
		delete itor->second;
	}
	mSymbolList.clear();
}

Symbol* const SymbolTable::AddSymbol(Symbol* pSymbol)
{
	if (pSymbol == 0 || pSymbol->GetName() == 0 || pSymbol->GetName() == L"") { return 0; }

	std::pair<tSymbolList::iterator, bool> ret = mSymbolList.insert(tSymbolEntry(pSymbol->GetName(), pSymbol));
	if (ret.second == false) 
	{
		// key name already in list
		delete pSymbol;
		pSymbol = ret.first->second;
	}
		
	return ret.first->second;
}

Symbol* const SymbolTable::FindSymbol(wchar_t* const name) 
{
	if (name == 0 || name == L"") { return 0; }

	tSymbolList::const_iterator ret = mSymbolList.find(name);
	if (ret == mSymbolList.end()) { return 0; }

	return ret->second;
}

} // MIEC
