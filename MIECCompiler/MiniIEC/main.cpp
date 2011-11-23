#include <stdio.h>
#include <iostream>

#include "Scanner.h"
#include "Parser.h"


using namespace MIEC;
using namespace std;

int main(int argc, char* argv[])
{	
	//FILE* pFile = fopen(argv[1], "r");

	Parser* parser;
	FILE* pFile;

	for( int i = 1; i < argc; i++ )
	{
		pFile = fopen(argv[i], "r");

		parser = new Parser( new Scanner( pFile ) );
		parser->Parse();

		cout << "parse file: " << argv[i] << endl;
		cout << "count errors: " << parser->errors->count << endl;
	}

	
	//Scanner* scan = new Scanner( pFile );
	//
	//Parser* parser = new Parser( scan );

	//parser->Parse();

	//cout << "count errors: " << parser->errors->count << endl;

	getchar();

	return 0;
}