#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include <map>
#include <functional>
#include "Symbol.h"

class SymbolTable
{
public:
	class NameCompare : public std::binary_function<wchar_t*, wchar_t*, bool> {
	public:
		bool operator()(wchar_t* name1, wchar_t* name2) const {
			return (coco_string_compareto(name1, name2) < 0); 
		}
	};

	typedef std::pair<wchar_t*, Symbol*> tSymbolEntry;
	typedef std::map<wchar_t*, Symbol*, NameCompare> tSymbolList;

	~SymbolTable()
	{
		tSymbolList::iterator itor = mSymbolList.begin();
		for (; itor != mSymbolList.end(); itor++) {
			delete itor->second;
		}
		mSymbolList.clear();
	}

	Symbol* AddSymbol(Symbol* pSymbol)
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

	Symbol* FindSymbol(wchar_t* const name) 
	{
		if (name == 0 || name == L"") { return 0; }

		tSymbolList::const_iterator ret = mSymbolList.find(name);
		if (ret == mSymbolList.end()) { return 0; }

		return ret->second;
	}

private:
	tSymbolList mSymbolList;
};

#endif // SYMBOLTABLE_H_