
#include <stdio.h>
#include <string>
#include <iostream>

#include "Scanner.h"
#include "Parser.h"
#include "CodeGenerator.h"

using namespace std;

int main(int argc, wchar_t* argv[])
{	
	if (argc < 2) {
		cout << "Usage: <program>.exe <path-to-miec>.miec {<path-to-miec>.miec}" << endl;
		return EXIT_FAILURE;
	}

	FILE* pFile = 0;
	for( int i = 1; i < argc; i++ )
	{
		wstring fileName = argv[i];
		cout << "parse file: " << fileName.c_str() << endl;

		pFile = _wfopen(fileName.c_str(), L"r");
		if (pFile == 0) 
		{
			cout << "Error opening file!" << endl;
		}
		else
		{
			MIEC::Scanner* pScanner = new MIEC::Scanner( pFile );
			MIEC::Parser* pParser = new MIEC::Parser( pScanner );

			pParser->pList = new MIEC::SymbolTable( pParser );
			pParser->pDACGen = new MIEC::DACGenerator( pParser );
			pParser->Parse();
			cout << "Parse errors: " << pParser->errors->count << endl;

			MIEC::CodeGenerator codeGen(pParser->pDACGen->GetDACList());
			codeGen.GenerateCode(fileName);

			delete pParser->pList; pParser->pList = 0;
			delete pParser->pDACGen; pParser->pDACGen = 0;
			delete pParser; pParser = 0;
			delete pScanner; pScanner = 0;
		}
		cout << endl;
	}

	cout << "Press any key . . .";
	getchar();

	return 0;
}
