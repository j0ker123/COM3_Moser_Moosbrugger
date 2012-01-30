
// enable memory leak detection
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <stdio.h>
#include <string>
#include <iostream>

#include "Scanner.h"
#include "Parser.h"
#include "CodeGenerator.h"

using namespace std;

void printDACStack(MIEC::Parser* pParser) {
	const MIEC::tDACList* const mpDacList = &pParser->pDACGen->GetDACList();
	
	MIEC::tDACList::const_iterator itor = mpDacList->begin();
	for (; itor != mpDacList->end(); itor++) 
	{
		MIEC::DACSymbol* sym = *itor;
		wcout << "-----new DAC-----\n\n" << sym->Print() << "\n" << endl;;
	}
}

int main(int argc, char* argv[])
{	
	if (argc < 2) {
		cout << "Usage: <program>.exe <path-to-miec>.miec {<path-to-miec>.miec}" << endl;
		return EXIT_FAILURE;
	}

	FILE* pFile = 0;
	for( int i = 1; i < argc; i++ )
	{
		wchar_t* fileName = coco_string_create(argv[i]);
		wcout << "Input file: " << fileName << endl;

		pFile = _wfopen(fileName, L"r");
		if (pFile == 0) 
		{
			wcout << "Error opening file!" << endl;
		}
		else
		{
			MIEC::Scanner* pScanner = new MIEC::Scanner( pFile );
			MIEC::Parser* pParser = new MIEC::Parser( pScanner );

			// front-end (analysis)
			pParser->Parse();
			size_t const nrSyntaxErrors = pParser->errors->count;
			size_t const nrSemanticErrors = pParser->pDACGen->GetErrorCounter();
			wcout << "Syntax errors: " << nrSyntaxErrors << endl;
			wcout << "Semantic errors: " << nrSemanticErrors << endl;

			if (nrSyntaxErrors == 0) {
				size_t const cNrRegisters = 8;
				coco_string_merge(fileName, L".iex");
				MIEC::CodeGenerator codeGen(&(pParser->pDACGen->GetDACList()), cNrRegisters);

				// back-end (synthesis)
				codeGen.GenerateCode(fileName);
				wcout << "Code generated: " << fileName << endl;
			}

			printDACStack( pParser );

			delete pParser->pDACGen; pParser->pDACGen = 0;
			delete pParser; pParser = 0;
			delete pScanner; pScanner = 0;

			fclose(pFile);
		}

		coco_string_delete(fileName);
		cout << endl;
	}

	cout << "Press any key . . .";
	getchar();

	// memory leak dump
	_CrtDumpMemoryLeaks();

	return 0;
}
