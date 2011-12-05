#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include <string>
#include <map>

#include "Symbol.h"

class SymbolTable
{
public:
	typedef std::pair<std::string, Symbol*> tSymbolEntry;
	typedef std::map<std::string, Symbol*> tSymbolList;

	Symbol* AddSymbol(Symbol* pSymbol, std::string const & name)
	{
		if (pSymbol == 0 || name == "") { return 0; }

		std::pair<tSymbolList::iterator, bool> ret = mSymbolList.insert(tSymbolEntry(name, pSymbol));
		if (ret.second == false) 
		{
			// key name already in list
			delete pSymbol; pSymbol = 0;
		}
		
		return ret.first->second;
	}

	Symbol* FindSymbol(std::string const & name) 
	{
		if (name == "") { return 0; }

		tSymbolList::const_iterator ret = mSymbolList.find(name);
		if (ret == mSymbolList.end()) { return 0; }

		return ret->second;
	}

private:
	tSymbolList mSymbolList;
};

#endif // SYMBOLTABLE_H_