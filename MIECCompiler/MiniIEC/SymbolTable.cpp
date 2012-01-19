
#include "Parser.h"
#include "SymbolTable.h"

namespace MIEC {

SymbolTable::SymbolTable(Parser* const pParser)
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
	if (pSymbol == 0) { 
		//mpParser->Err(L"AddSymbol: invalid symbol"); 
		return 0;
	}
	wchar_t* pName = pSymbol->GetName();
	if (pName == 0 || pName == L"") {
		mpParser->Err(L"AddSymbol: invalid symbol name");
		return 0;
	}

	std::pair<tSymbolList::iterator, bool> ret = mSymbolList.insert(tSymbolEntry(pName, pSymbol));
	if (ret.second == false)
	{
		//mpParser->Err(L"AddSymbol: symbol already defined");

		//delete pSymbol;
		//pSymbol = ret.first->second;
	}
		
	return ret.first->second;
}

Symbol* const SymbolTable::FindSymbol(wchar_t* const pName) 
{
	if (pName == 0 || pName == L"") { 
		mpParser->Err(L"FindSymbol: invalid symbol name");
		return 0; 
	}

	tSymbolList::const_iterator ret = mSymbolList.find(pName);
	if (ret == mSymbolList.end()) { 
		//mpParser->Err(L"FindSymbol: undefined symbol");
		return 0; 
	}

	return ret->second;
}

} // MIEC
