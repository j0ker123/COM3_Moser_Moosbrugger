#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include <map>
#include <functional>
#include <ostream>
#include "Scanner.h"
#include "Symbol.h"

namespace MIEC {

class SymbolTable
{
public:
	class NameCompare : public std::binary_function<wchar_t*, wchar_t*, bool> {
	public:
		bool operator()(wchar_t* pName1, wchar_t* pName2) const {
			return (coco_string_compareto(pName1, pName2) < 0); 
		}
	};

	typedef std::pair<wchar_t*, Symbol*> tSymbolEntry;
	typedef std::map<wchar_t*, Symbol*, NameCompare> tSymbolList;

	SymbolTable();
	virtual ~SymbolTable();

	Symbol* const AddSymbol(Symbol* pSymbol);
	Symbol* const FindSymbol(wchar_t* const pName);

	void PrintTable(std::wostream& out) const;

private:
	SymbolTable(const SymbolTable&);
	SymbolTable& operator= (const SymbolTable&);

	tSymbolList mSymbolList;
};

} // namespace MIEC

#endif // #ifndef SYMBOLTABLE_H_
