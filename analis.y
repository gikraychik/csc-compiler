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
#include <ostream>

using namespace std;

class Error
{
	Error();
	~Error();
	static void error(int number)
	{
		switch (number)
		{
			case 1: cout << "Run out of memory" << endl; break;
			default: cout << "Unexpected error" << endl; break;
		}
	}
};
class Memory
{
public:
	Memory(int capacity)
	{
		cap = capacity;
		allocated.resize(cap, 0);
	}
	string &alloc(string &ext_name)
	{
		if (names.find(ext_name) == names.end())  //not found
		{
			names[ext_name] = 
		}
	}
	bool isFree(int number)
	{
		return allocated.find(number) == allocated.end();
	}
	bool isFree(string &int_name)
	{
		const char *str = int_name.data();
		char i = 0;
		while (str[i] != '\0')
		{
			if ((str[i] >= '0') && (str[i] <= '9')) { break; }
			i++;
		}
		if (str[i] == '\0')
		{
			//yyerror("Memory::isFree: error: wrong int_name, it must have at least 1 digit in the end");
		}
		int x = atoi(str+i);
		return isFree(x);		
	}

private:
	map<string, string> names;
	int cap;  //how much memory we have
	set<int> globals;  //numbers of global variables
	vector<int> allocated;
	
	int alloc_cell() throw(int)
	{
		int i = 0;
		while (i < cap) { if (allocated[i] == 0) { return i; } i++; }
		throw (1);
	}
};

class Assembler
{
public:
	Assembler()
	{	
	}
	~Assembler() { }
	void move(string &var1, string &var2)
	{
		binary("MOVE", var1, var2);
	}
	void move(int var1, string &var2)
	{
		binary("MOVE", var1, var2);
	}
	void _not(string &op1, string &res)
	{
		binary("NOT", op1, res);
	}
	void add(string &op1, string &op2, string &res)
	{
		ternary("ADD", op1, op2, res);
	}
	void sub(string &op1, string &op2, string &res)
	{
		ternary("SUB", op1, op2, res);
	}
	void lt(string &op1, string &op2, string &res)
	{
		ternary("LT", op1, op2, res);
	}
	void le(string &op1, string &op2, string &res)
	{
		ternary("LE", op1, op2, res);
	}
	void gt(string &op1, string &op2, string &res)
	{
		ternary("GT", op1, op2, res);
	}
	void ge(string &op1, string &op2, string &res)
	{
		ternary("GE", op1, op2, res);
	}
	void eq(string &op1, string &op2, string &res)
	{
		ternary("EQ", op1, op2, res);
	}
	void eq(int op1, string &op2, string &res)
	{
		ternary("ADD", op1, op2, res);
	}
	void _or(string &op1, string &op2, string &res)
	{
		ternary("OR", op1, op2, res);
	}
	void _and(string &op1, string &op2, string &res)
	{
		ternary("AND", op1, op2, res);
	}

private:	 
	void ternary(const char *cmd, string &op1, string &op2, string &op3)
	{
		cout << cmd << "(" << op1 << "," << op2 << "," << op3 << ")";
	}
	void ternary(const char *cmd, int op1, string &op2, string &op3)
	{
		cout << cmd << "(" << op1 << "," << op2 << "," << op3 << ")";
	}
	void binary(const char *cmd, string &op1, string &op2)
	{
		cout << cmd << "(" << op1 << "," << op2 << ")";
	}
	void binary(const char *cmd, int op1, string &op2)
	{
		cout << cmd << "(" << op1 << "," << op2 << ")";
	}
};

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
	fprintf(stderr, "Input language compilation error: %s\n", s);
}

/*int yywrap()
{
	return 1;
}*/

Assembler asmr;
Memory mem;

int main()
{
	yyparse();
}
%}

%token NUMBER NAME REF COLON SEMICOLON STRINGCONST STRING INTEGER COMA RETURN ASSIGN
%token IF WHILE LOOP POOL READ WRITE NEQ
%token LABEL RECOPENBRACE RECCLOSEBRACE
%token OBLOCK CBLOCK THEN ELSE GOTO OBRACE CBRACE
%token GLOBAL DEFINE INIT AS

%token AND OR
%token NOT
%token LT GT LE GE EQ NE
%token ADD SUB
%token MUL DIV

%union
{
	int number;
	char *string;
}

%%
program :	/* empty */
		| commands
		;
commands :	commands command
		| command
		;
command :	assign
		| condition
		| decl
		;
assign :	NAME ASSIGN expr
		;
expr :		int_expr
		;
int_expr:	int_expr OR term1
		| term1
		;
term1:		term1 AND term2
		| term2
		;
term2:		term2 EQ term3
		| term2 NEQ term3
		| term3
		;
term3:		term3 LT term4
		| term3 LE term4
		| term3 GT term4
		| term3 GE term4
		| term4
		;
term4:		term4 ADD term5
		| term4 SUB term5
		| term5
		;
term5:		term5 MUL term6
		| term5 DIV term6
		| term6
		;
term6:		NOT term7
		;
term7:		NUMBER
		| NAME
		| OBRACE expr CBRACE
		;
decl:		INIT NAME ASSIGN expr SEMICOLON
		| INIT NAME SEMICOLON
		{
			mem.alloc($<string>2);

		}
		| INIT GLOBAL NAME ASSIGN SEMICOLON
		| INIT GLOBAL NAME SEMICOLON
		| DEFINE NAME AS NAME SEMICOLON
		;

condition :	IF OBRACE expr CBRACE OBLOCK commands CBLOCK ELSE OBLOCK commands CBLOCK
		| IF OBRACE expr CBRACE OBLOCK commands CBLOCK
		;
%%
