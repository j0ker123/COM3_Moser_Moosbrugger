#include <stdio.h>
#include <iostream>

#include "Scanner.h"
#include "Parser.h"


using namespace MIEC;
using namespace std;

int main(int argc, char* argv[])
{	
	if (argc < 2) {
		cout << "Usage: <program>.exe <path-to-miec>.miec {<path-to-miec>.miec}" << endl;
		return EXIT_FAILURE;
	}

	FILE* pFile = 0;
	for( int i = 1; i < argc; i++ )
	{
		cout << "parse file: " << argv[i] << endl;

		pFile = fopen(argv[i], "r");
		if (pFile == 0) 
		{
			cout << "Error opening file!" << endl;
		}
		else
		{
			Parser* parser = new Parser( new Scanner( pFile ) );
			parser->Parse();

			cout << "count errors: " << parser->errors->count << endl;
		}
		cout << endl;
	}

	getchar();

	return 0;
}