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
public:
	Error();
	~Error();
	static void error(int number)
	{
		switch (number)
		{
			case 1: cout << "Run out of memory" << endl; break;
			case 2: cout << "Attempt to alloc memory for the second time; redeclaration of variable" << endl; break;
			case 3: cout << "Memory::isFree: error: wrong int_name, it must have at least 1 digit in the end" << endl; break;
			case 4: cout << "Memory::isFree: error: out of range" << endl;
			case 5: cout << "Memory::free_mem: error: impossible to delete an undeclared variable" << endl; break;
			case 6: cout << "Memory::free_cell: error: impossible to delete global variable" << endl; break;	
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
		globals.resize(cap, 0);
	}
	string &alloc(string &ext_name) throw(int)
	{
		if (names.find(ext_name) == names.end())  //not found
		{
			char *str = (char *)malloc(33*sizeof(char));
			int x;
			try
			{
				x = alloc_mem();
			}
			catch (int e)
			{
				free(str);
				//some actions, if run out of memory
			}
			names[ext_name] = numberToInnerName(x);
			free(str);
		}
		else { throw 2; }
		return names[ext_name];
	}
	int alloc() throw (int)  //allocates memory if no ext_name
	{
		int res;
		try { res = alloc_mem(); }
		catch (int e)
		{
			if (e == 1) { }  //out of memory
			else { Error::error(e); }
		}
		return res;
	}
	bool isFree(int number) throw(int)
	{
		if ((number < 0) || (number >= cap)) { throw 4; }
		return allocated[number] == 0;
	}
	bool isFree(string &int_name) throw(int)
	{
		return isFree(innerNameToNumber(int_name));		
	}
	bool isGlobal(int number)
	{
		return globals[number] == 1;
	}
	void makeGlobal(int number)
	{
		globals[number] = 1;
	}
	void makeGlobal(string &int_name)
	{
		globals[innerNameToNumber(int_name)] = 1;
	}
	string getInnerName(string &ext_name)
	{
		if (names.find(ext_name) == names.end())  //not found
		{
			return string("");
		}
		return names[ext_name];
	}
	int innerNameToNumber(string &int_name)
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
			throw(3);
		}
		int x = atoi(str+i);
		return x;
	}
	string numberToInnerName(int number)
	{
		char *str = (char *)malloc(33*sizeof(char));
		string res = string("TMP") + string(itoa(number, str));
		free(str);
		return res;
	}
	void free_mem(string &ext_name) throw(int)
	{
		if (names.find(ext_name) == names.end())  //not found
		{
			throw 5;
		}
		try { free_cell(ext_name); }
		catch (int e) { throw e; }
	}

	void print()
	{
		for (int i = 0; i < 50; i++) { cout << "*"; } cout << endl;
		cout << "Locals: " << endl;
		for (int i = 0; i < cap; i++)
		{
			cout << allocated[i] << " ";
		}
		cout << endl << "Globals:" << endl;
		for (int i = 0; i < cap; i++)
		{
			cout << globals[i] << " ";
		}
		cout << endl << "Names:" << endl;
		for (map<string , string>::iterator i = names.begin(); i != names.end(); i++)
		{
			cout << (*i).first << " -> " << (*i).second << endl;
		}
		for (int i = 0; i < 50; i++) { cout << "*"; } cout << endl;
	}
private:
	map<string, string> names;
	int cap;  //how much memory we have
	vector<int> globals;  //numbers of global variables
	vector<int> allocated;
	
	int alloc_mem() throw(int)
	{
		int i = 0;
		while (i < cap) { if (allocated[i] == 0) { break; } i++; }
		if (i == cap) { throw (1); }
		allocated[i] = 1;
		return i;
	}
	void free_cell(string &ext_name) throw(int)
	{
		string int_name = names[ext_name];
		int number = innerNameToNumber(int_name);
		if (globals[number] == 1) { throw 6; }
		allocated[number] = 0;
		names.erase(ext_name);
	}		
	
	void reverse(char s[])
	{
	    int i, j;
	    char c;
	    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
		 c = s[i];
		 s[i] = s[j];
		 s[j] = c;
	    }
	}
	char *itoa(int n, char s[])
 	{
     		int i, sign;
     		if ((sign = n) < 0)  /* записываем знак */
        	n = -n;          /* делаем n положительным числом */
    		i = 0;
     		do {       /* генерируем цифры в обратном порядке */
         		s[i++] = n % 10 + '0';   /* берем следующую цифру */
     		} while ((n /= 10) > 0);     /* удаляем */
     		if (sign < 0)
         		s[i++] = '-';
     		s[i] = '\0';
     		reverse(s);
		return s;
	}
	
};

class Assembler
{
public:
	Assembler()
	{	
	}
	~Assembler() { }
	void move(const char *var1, const char *var2)
	{
		binary("MOVE", var1, var2);
	}
	void move(int var1, const char *var2)
	{
		binary("IMOVE", var1, var2);
	}
	void _not(const char *op1, const char *res)
	{
		binary("NOT", op1, res);
	}
	void add(const char *op1, const char *op2, const char *res)
	{
		ternary("ADD", op1, op2, res);
	}
	void sub(const char *op1, const char *op2, const char *res)
	{
		ternary("SUB", op1, op2, res);
	}
	void lt(const char *op1, const char *op2, const char *res)
	{
		ternary("LT", op1, op2, res);
	}
	void le(const char *op1, const char *op2, const char *res)
	{
		ternary("LE", op1, op2, res);
	}
	void gt(const char *op1, const char *op2, const char *res)
	{
		ternary("GT", op1, op2, res);
	}
	void ge(const char *op1, const char *op2, const char *res)
	{
		ternary("GE", op1, op2, res);
	}
	void eq(const char *op1, const char *op2, const char *res)
	{
		ternary("EQ", op1, op2, res);
	}
	void eq(int op1, const char *op2, const char *res)
	{
		ternary("IEQ", op1, op2, res);
	}
	void _or(const char *op1, const char *op2, const char *res)
	{
		ternary("OR", op1, op2, res);
	}
	void _and(const char *op1, const char *op2, const char *res)
	{
		ternary("AND", op1, op2, res);
	}

private:	 
	void ternary(const char *cmd, const char *op1, const char *op2, const char *op3)
	{
		cout << cmd << "(" << op1 << "," << op2 << "," << op3 << ")";
	}
	void ternary(const char *cmd, int op1, const char *op2, const char *op3)
	{
		cout << cmd << "(" << op1 << "," << op2 << "," << op3 << ")";
	}
	void binary(const char *cmd, const char *op1, const char *op2)
	{
		cout << cmd << "(" << op1 << "," << op2 << ")";
	}
	void binary(const char *cmd, int op1, const char *op2)
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
Memory mem(30);
bool isTerm[6] = {false, false, false, false, false, false};

void inTerm(int num, const char *&res, const char *&arg)
{
	cout << "Hello";
	if (isTerm[num])
	{
		string *new_mem = new string(mem.numberToInnerName(mem.alloc()));  //memory leak
		res = new_mem->data();
		asmr.move(arg, new_mem->data());
		isTerm[num] = false;
	}
	else { res = arg; }
}
int main()
{
	cout << "plpl";
	yyparse();
	mem.print();
}
%}

%token NUMBER NAME REF COLON SEMICOLON STRINGCONST STRING INTEGER COMA RETURN ASSIGN
%token IF WHILE LOOP POOL READ WRITE NEQ
%token LABEL RECOPENBRACE RECCLOSEBRACE
%token OBLOCK CBLOCK THEN ELSE GOTO OBRACE CBRACE DELETE
%token GLOBAL DEFINE INIT AS

%token AND OR
%token NOT
%token LT GT LE GE EQ NE
%token ADD SUB
%token MUL DIV

%union
{
	int number;
	const char *string;
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
		| delete
		;
assign :	NAME ASSIGN expr SEMICOLON
		;
expr :		int_expr { $<string>$ = $<string>1; }
		;
int_expr:	term1 { isTerm[0] = true; } OR int_expr
		{
			asmr._or($<string>3, $<string>1, $<string>3);
			$<string>$ = $<string>3;
		}
		| term1 { inTerm(0, $<string>$, $<string>1); }
		;
term1:		term2 { isTerm[1] = true; } AND term1
		{
			asmr._and($<string>3, $<string>1, $<string>3);
			$<string>$ = $<string>1;
		}
		| term2 { inTerm(1, $<string>$, $<string>1); }
		;
term2:		term3 { isTerm[2] = true; } EQ term2
		{
			asmr.eq($<string>3, $<string>1, $<string>3);
			$<string>$ = $<string>3;
		}
		| term3 { inTerm(2, $<string>$, $<string>1); }
		;
term3:		term4 { isTerm[3] = true; } LT term3
		{
			asmr.lt($<string>3, $<string>1, $<string>3);
			$<string>$ = $<string>3;
		}
		| term4 { isTerm[3] = true; } LE term3
		{
			asmr.le($<string>3, $<string>1, $<string>3);
			$<string>$ = $<string>3;
		}
		| term4 { isTerm[3] = true; } GT term3
		{
			asmr.gt($<string>3, $<string>1, $<string>3);
			$<string>$ = $<string>3;
		}
		| term4 { isTerm[3] = true; } GE term3
		{
			asmr.ge($<string>3, $<string>1, $<string>3);
			$<string>$ = $<string>3;
		}
		| term4 { inTerm(3, $<string>$, $<string>1); }
		;
term4:		term6 { isTerm[4] = true; } ADD term4
		{
			asmr.add($<string>3, $<string>1, $<string>3);
			$<string>$ = $<string>3;
		}
		| term6 { isTerm[4] = true; } SUB term4
		{
			asmr.sub($<string>3, $<string>1, $<string>3);
			$<string>$ = $<string>3;
		}
		| term6 { inTerm(4, $<string>$, $<string>1); }
		;
term6:		NOT term7
		{
			string str = mem.numberToInnerName(mem.alloc());
			asmr._not($<string>2, str.data());
			$<string>$ = $<string>2;
		}
		| term7 { $<string>$ = $<string>1; }
		;
term7:		NUMBER
		{
			cout << "lol";
			string str = mem.numberToInnerName(mem.alloc());
			asmr.move($<number>1, str.data());
			$<string>$ = str.data();
		}
		| NAME
		| OBRACE expr CBRACE
		;
decl:		INIT NAME ASSIGN expr SEMICOLON
		| INIT NAME SEMICOLON
		{
			string str($<string>2);
			try
			{			
				mem.alloc(str);
			}
			catch (int e)
			{
				Error::error(e);
				break;
			}
		}
		| INIT GLOBAL NAME ASSIGN SEMICOLON
		| INIT GLOBAL NAME SEMICOLON
		{
			string str($<string>3);
			string int_name;
			try
			{			
				int_name = mem.alloc(str);
			}
			catch (int e)
			{
				Error::error(e);
				break;
			}
			mem.makeGlobal(int_name);
		}
		| DEFINE NAME AS NAME SEMICOLON
		| expr
		;
delete:		DELETE NAME SEMICOLON
		{
			string ext_name = string($<string>2);	
			try { mem.free_mem(ext_name); }
			catch (int e) { Error::error(e); break; }
		}
		;	
condition :	IF OBRACE expr CBRACE OBLOCK commands CBLOCK ELSE OBLOCK commands CBLOCK
		| IF OBRACE expr CBRACE OBLOCK commands CBLOCK
		;
%%
