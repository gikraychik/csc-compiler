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
			case 7: cout << "Memory::getInnerName: error: undeclared identifier in expression" << endl; break;	
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
	string getInnerName(string &ext_name) throw(int)
	{
		if (names.find(ext_name) == names.end())  //not found
		{
			throw 7;
			//return string("");
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
	void free_cell(const char *int_name) throw(int)
	{
		string str(int_name);
		int number = innerNameToNumber(str);
		if (globals[number] == 1) { throw 6; }
		allocated[number] = 0;
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
		cout << cmd << "(" << op1 << ", " << op2 << ", " << op3 << ")" << endl;
	}
	void ternary(const char *cmd, int op1, const char *op2, const char *op3)
	{
		cout << cmd << "(" << op1 << ", " << op2 << ", " << op3 << ")" << endl;
	}
	void binary(const char *cmd, const char *op1, const char *op2)
	{
		cout << cmd << "(" << op1 << ", " << op2 << ")" << endl;
	}
	void binary(const char *cmd, int op1, const char *op2)
	{
		cout << cmd << "(" << op1 << ", " << op2 << ")" << endl;
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

int main()
{
	yyparse();
	mem.print();
}
%}

%token NUMBER NAME REF COLON SEMICOLON STRINGCONST STRING INTEGER COMA RETURN ASSIGN
%token IF WHILE LOOP POOL READ WRITE NEQ
%token LABEL RECOPENBRACE RECCLOSEBRACE
%token OBLOCK CBLOCK THEN ELSE GOTO OBRACE CBRACE DELETE
%token GLOBAL DEFINE INIT AS

%left OR
%left AND
%left LT GT LE GE
%left EQ
%left ADD SUB
%left MUL DIV
%left NOT

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
int_expr:	int_expr ADD int_expr
		{
			asmr.add($<string>1, $<string>3, $<string>1);
			mem.free_cell($<string>3);
			$<string>$ = $<string>1;
		}
		| int_expr SUB int_expr
		{			
			asmr.sub($<string>1, $<string>3, $<string>1);
			mem.free_cell($<string>3);
			$<string>$ = $<string>1;
		}
		| int_expr OR int_expr
		{			
			asmr._or($<string>1, $<string>3, $<string>1);
			mem.free_cell($<string>3);
			$<string>$ = $<string>1;
		}
		| int_expr AND int_expr
		{			
			asmr._and($<string>1, $<string>3, $<string>1);
			mem.free_cell($<string>3);
			$<string>$ = $<string>1;
		}
		| int_expr EQ int_expr
		{			
			asmr.eq($<string>1, $<string>3, $<string>1);
			mem.free_cell($<string>3);
			$<string>$ = $<string>1;
		}
		| int_expr LT int_expr
		{			
			asmr.lt($<string>1, $<string>3, $<string>1);
			mem.free_cell($<string>3);
			$<string>$ = $<string>1;
		}
		| int_expr LE int_expr
		{			
			asmr.le($<string>1, $<string>3, $<string>1);
			mem.free_cell($<string>3);
			$<string>$ = $<string>1;
		}
		| int_expr GT int_expr
		{			
			asmr.gt($<string>1, $<string>3, $<string>1);
			mem.free_cell($<string>3);
			$<string>$ = $<string>1;
		}
		| int_expr GE int_expr
		{			
			asmr.ge($<string>1, $<string>3, $<string>1);
			mem.free_cell($<string>3);
			$<string>$ = $<string>1;
		}
		| NOT int_expr
		{
			asmr._not($<string>2, $<string>2);
			$<string>$ = $<string>2;
		}
		| NUMBER
		{
			string *str = new string(mem.numberToInnerName(mem.alloc()));  //memory leak
			asmr.move($<number>1, str->data());
			$<string>$ = str->data();
		}
		| NAME
		{
			string ext_name($<string>1);
			string *int_name;
			try { int_name = new string(mem.getInnerName(ext_name)); }  //memory leak
			catch (int e) { Error::error(e); break; }
			string *new_mem = new string(mem.numberToInnerName(mem.alloc()));  //memory leak
			asmr.move(int_name->data(), new_mem->data());
			$<string>$ = new_mem->data();
		}
		| OBRACE int_expr CBRACE
		{
			$<string>$ = $<string>2;
		}
		;
decl:		INIT NAME ASSIGN expr SEMICOLON
		{
			string str($<string>2);
			string *new_mem;
			try { new_mem = new string(mem.alloc(str)); }  //memory leak
			catch (int e) { Error::error(e); break; }
			asmr.move($<string>4, new_mem->data());
			mem.free_cell($<string>4);
		}
		| INIT NAME SEMICOLON
		{
			string str($<string>2);
			try { mem.alloc(str); }
			catch (int e) { Error::error(e); break; }
		}
		| INIT GLOBAL NAME ASSIGN SEMICOLON
		| INIT GLOBAL NAME SEMICOLON
		{
			string str($<string>3);
			string *int_name;
			try
			{			
				int_name = new string(mem.alloc(str));  //memory leak
			}
			catch (int e)
			{
				Error::error(e);
				break;
			}
			mem.makeGlobal(*int_name);
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
