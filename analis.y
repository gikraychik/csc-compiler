%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <sstream>
#include <iostream>
#include <vector>
#include <stack>
#include <set>

using namespace std;

extern "C"
{
	int yyparse(void);
	int yylex(void);
	int yywrap()
	{
		return 1;
	}
}
void yyerror(const char *s)
{
	fprintf(stderr, "Unexpected error: %s\n", s);
}

/*int yywrap()
{
	return 1;
}*/


int main()
{
	yyparse();
}
%}

%union
{
	int number;
	char *string;
}

%token <number>NUMBER <string>NAME REF COLON SEMICOLON <string>STRINGCONST STRING INTEGER COMA RETURN ASSIGN
%token IF WHILE LOOP POOL READ WRITE
%token LABEL RECOPENBRACE RECCLOSEBRACE
%token BLOCK KCOLB THEN ELSE GOTO OBRACE CBRACE

%left AND OR
%left NOT
%left LT GT LE GE EQ NE
%left ADD SUB
%left MUL DIV

%%
program :	/* empty */
		| commands
		;
commands :	commands command
		| command
		;
command :	assign
		| condition
		;
assign :	NAME ASSIGN expr
		;
expr :		expr ADD expr
		| expr SUB expr
		| NUMBER
		| NAME
		;
condition :	IF OBRACE expr CBRACE BLOCK commands KCOLB ELSE BLOCK commands KCOLB
		| IF OBRACE expr CBRACE BLOCK commands KCOLB
		;
%%
