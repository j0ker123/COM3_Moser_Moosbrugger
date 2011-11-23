

#include <wchar.h>
#include "Parser.h"
#include "Scanner.h"


namespace MIEC {


void Parser::SynErr(int n) {
	if (errDist >= minErrDist) errors->SynErr(la->line, la->col, n);
	errDist = 0;
}

void Parser::SemErr(const wchar_t* msg) {
	if (errDist >= minErrDist) errors->Error(t->line, t->col, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		t = la;
		la = scanner->Scan();
		if (la->kind <= maxT) { ++errDist; break; }

		if (dummyToken != t) {
			dummyToken->kind = t->kind;
			dummyToken->pos = t->pos;
			dummyToken->col = t->col;
			dummyToken->line = t->line;
			dummyToken->next = NULL;
			coco_string_delete(dummyToken->val);
			dummyToken->val = coco_string_create(t->val);
			t = dummyToken;
		}
		la = t;
	}
}

void Parser::Expect(int n) {
	if (la->kind==n) Get(); else { SynErr(n); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::MIEC() {
		Expect(3);
		Expect(1);
		if (la->kind == 6) {
			VarDecl();
		}
		Expect(4);
		Statements();
		Expect(5);
}

void Parser::VarDecl() {
		Expect(6);
		Expect(1);
		Expect(7);
		Expect(8);
		Expect(9);
		while (la->kind == 1) {
			Get();
			Expect(7);
			Expect(8);
			Expect(9);
		}
		Expect(10);
}

void Parser::Statements() {
		Stat();
		while (StartOf(1)) {
			Stat();
		}
}

void Parser::Stat() {
		if (la->kind == 1) {
			Get();
			Expect(11);
			Expr();
			Expect(9);
		} else if (la->kind == 12) {
			Get();
			Expect(13);
			Expr();
			Expect(14);
			Expect(9);
		} else if (la->kind == 15) {
			Get();
			Condition();
			Expect(16);
			Statements();
			Expect(5);
		} else if (la->kind == 17) {
			Get();
			Condition();
			Expect(18);
			Statements();
			Expect(19);
			Statements();
			Expect(5);
		} else SynErr(31);
}

void Parser::Expr() {
		Term();
		while (la->kind == 20 || la->kind == 21) {
			if (la->kind == 20) {
				Get();
			} else {
				Get();
			}
			Term();
		}
}

void Parser::Condition() {
		Expr();
		Relop();
		Expr();
}

void Parser::Term() {
		Fact();
		while (la->kind == 22 || la->kind == 23) {
			if (la->kind == 22) {
				Get();
			} else {
				Get();
			}
			Fact();
		}
}

void Parser::Fact() {
		if (la->kind == 1) {
			Get();
		} else if (la->kind == 2) {
			Get();
		} else if (la->kind == 13) {
			Get();
			Expr();
			Expect(14);
		} else SynErr(32);
}

void Parser::Relop() {
		switch (la->kind) {
		case 24: {
			Get();
			break;
		}
		case 25: {
			Get();
			break;
		}
		case 26: {
			Get();
			break;
		}
		case 27: {
			Get();
			break;
		}
		case 28: {
			Get();
			break;
		}
		case 29: {
			Get();
			break;
		}
		default: SynErr(33); break;
		}
}



void Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	MIEC();

	Expect(0);
}

Parser::Parser(Scanner *scanner) {
	maxT = 30;

	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	errors = new Errors();
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[2][32] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x},
		{x,T,x,x, x,x,x,x, x,x,x,x, T,x,x,T, x,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	delete errors;
	delete dummyToken;
}

Errors::Errors() {
	count = 0;
}

void Errors::SynErr(int line, int col, int n) {
	wchar_t* s;
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"ident expected"); break;
			case 2: s = coco_string_create(L"number expected"); break;
			case 3: s = coco_string_create(L"\"program\" expected"); break;
			case 4: s = coco_string_create(L"\"BEGIN\" expected"); break;
			case 5: s = coco_string_create(L"\"END\" expected"); break;
			case 6: s = coco_string_create(L"\"BEGIN_VAR\" expected"); break;
			case 7: s = coco_string_create(L"\":\" expected"); break;
			case 8: s = coco_string_create(L"\"Integer\" expected"); break;
			case 9: s = coco_string_create(L"\";\" expected"); break;
			case 10: s = coco_string_create(L"\"END_VAR\" expected"); break;
			case 11: s = coco_string_create(L"\":=\" expected"); break;
			case 12: s = coco_string_create(L"\"print\" expected"); break;
			case 13: s = coco_string_create(L"\"(\" expected"); break;
			case 14: s = coco_string_create(L"\")\" expected"); break;
			case 15: s = coco_string_create(L"\"WHILE\" expected"); break;
			case 16: s = coco_string_create(L"\"DO\" expected"); break;
			case 17: s = coco_string_create(L"\"IF\" expected"); break;
			case 18: s = coco_string_create(L"\"THEN\" expected"); break;
			case 19: s = coco_string_create(L"\"ELSE\" expected"); break;
			case 20: s = coco_string_create(L"\"+\" expected"); break;
			case 21: s = coco_string_create(L"\"-\" expected"); break;
			case 22: s = coco_string_create(L"\"*\" expected"); break;
			case 23: s = coco_string_create(L"\"/\" expected"); break;
			case 24: s = coco_string_create(L"\"=\" expected"); break;
			case 25: s = coco_string_create(L"\"<=\" expected"); break;
			case 26: s = coco_string_create(L"\">=\" expected"); break;
			case 27: s = coco_string_create(L"\"!=\" expected"); break;
			case 28: s = coco_string_create(L"\"<\" expected"); break;
			case 29: s = coco_string_create(L"\">\" expected"); break;
			case 30: s = coco_string_create(L"??? expected"); break;
			case 31: s = coco_string_create(L"invalid Stat"); break;
			case 32: s = coco_string_create(L"invalid Fact"); break;
			case 33: s = coco_string_create(L"invalid Relop"); break;

		default:
		{
			wchar_t format[20];
			coco_swprintf(format, 20, L"error %d", n);
			s = coco_string_create(format);
		}
		break;
	}
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	coco_string_delete(s);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
}

void Errors::Warning(const wchar_t *s) {
	wprintf(L"%ls\n", s);
}

void Errors::Exception(const wchar_t* s) {
	wprintf(L"%ls", s); 
	exit(1);
}

} // namespace


