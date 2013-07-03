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
#include <cstdlib>
#include <fstream>

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
			case 8: cout << "Error: argument of print must have 4 symbols only" << endl; break;
			case 9: cout << "Error: nowhere to print; call LED0 - LED4" << endl; break;
			case 10: cout << "Memory::erase: error: imposible to delete an undeclared definition" << endl; break;
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
	bool isGlobal(const char *name)
	{
		string str(name);
		int num = innerNameToNumber(names[str]);
		return isGlobal(num);
	}
	void makeGlobal(int number)
	{
		globals[number] = 1;
	}
	void makeGlobal(string &int_name)
	{
		globals[innerNameToNumber(int_name)] = 1;
	}
	void define(string &ext_name, string &int_name)
	{
		names[ext_name] = int_name;
	}
	bool isDefine(const char *name)
	{
		string str(name);
		return defs.find(name) != defs.end();
	}
	void addDef(const char *name, const char *im)
	{
		string s(name);
		defs.insert(s);
		string Im(im);
		names[s] = Im;
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
	void erase(string &ext_name) throw(int)
	{
		if (names.find(ext_name) == names.end())  //not found
		{ throw 10; }
		names.erase(ext_name);
		if (isDefine(ext_name.data())) { defs.erase(ext_name.data()); }
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
	stack<string *> ifs;
	set<string> defs;
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
};

class Assembler
{
public:
	Assembler(Memory *mem)
	{
		this->mem = mem;
			
	}
	~Assembler() { }
	void assignOutput(int number, filebuf *fb)
	{
		/*char *tmp = (char *)malloc(33*sizeof(char));
		tmp = itoa(number, tmp);
		string s = string(tmp) + string(".nlt");
		free(tmp);
		fb.open(s.data(), std::ios::out);
		os(&fb);*/
	}
	void move(const char *var1, const char *var2)
	{
		if (!mem->ifs.empty())
		{
			string *int_name = mem->ifs.top();
			mux2(int_name->data(), var2, var2, var1);
		}
		else { binary("MOVE", var1, var2); }
	}
	void move(int var1, const char *var2)
	{
		/*if (!mem->ifs.empty())
		{
			string *int_name = mem->ifs.top();
			string new_mem = mem->numberToInnerName(mem->alloc());
			binary("IMOVE", var1, new_mem.data());
			mux2(int_name->data(), var2, var2, new_mem.data());
		}
		else { binary("IMOVE", var1, var2); }*/
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
	void cl(const char *what, const char *where)
	{
		binary("CL", what, where);
	}
	void cd(const char *what, const char *where)
	{
		binary("CD", what, where);
	}
	void mux2(const char *op1, const char *op2, const char *op3, const char *op4)
	{
		quadro("MUX2", op1, op2, op3, op4);
	}
	void imux2(const char *op1, const char *op2, int op3, int op4)
	{
		cout << "IMUX2" << "(" << op1 << ", " << op2 << ", " << op3 << ", " << op4 << ")" << endl;
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
	void quadro(const char *cmd, const char *op1, const char *op2, const char *op3, const char *op4)
	{
		cout << cmd << "(" << op1 << ", " << op2 << ", " << op3 << ", " << op4 << ")" << endl;
	}
	void binary(const char *cmd, const char *op1, const char *op2)
	{
		cout << cmd << "(" << op1 << ", " << op2 << ")" << endl;
	}
	void binary(const char *cmd, int op1, const char *op2)
	{
		cout << cmd << "(" << op1 << ", " << op2 << ")" << endl;
	}
	Memory *mem;
	//ostream os;
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

Memory mem(30);
Assembler asmr(&mem);
bool isV = false;
bool isN = false;
int exprNumber = 0;
const char *exprNum = 0;  //it prints
const char *exprName = 0;

void print(char c, int number)
{
	string where("LED");
	char *tmp = (char *)malloc(33*sizeof(char));
	where = where + string(mem.itoa(number, tmp));
	free(tmp);
	string what(mem.numberToInnerName(mem.alloc()));
	asmr.move(c, what.data());
	asmr.cl(what.data(), where.data());
	mem.free_cell(what.data());
}
int indexOfSht(const char *str) { return strlen(str) - 1; }
bool isVar(const char *str) { return str[indexOfSht(str)] == '$'; }
void swap(const char *&x, const char *&y) { const char *z = x; x = y; y = z; }
void delSht(char *str) { str[indexOfSht(str)] = 0; }
void setSht(const char *&str)
{
	cout << "lol";
	char *s = (char *)malloc((strlen(str)+2)*sizeof(char)); 
	int i = 0;
	while (str[i] != '0') { s[i] = str[i]; i++; }
	s[i] = '$'; i++; s[i] = 0;
	str = s;
}
void inExpr(const char *&int_name1, const char *&int_name2, const char *&res)
{
	bool b1 = isVar(int_name1);
	bool b2 = isVar(int_name2);
	char *var1 = strdup(int_name1);  //memory leak
	char *var2 = strdup(int_name2);  //memory leak	
	if (b1) { delSht(var1); }
	if (b2) { delSht(var2); }
	/*string s1(var1);
	string s2(var2);
	const char *int_name1 = mem.getInnerName(s1).data();
	const char *int_name2 = mem.getInnerName(s2).data();*/
	//cout << var1 << " ! " << var2 << endl;
	string *new_mem;
	if (b1 && b2)
	{
		new_mem = new string(mem.numberToInnerName(mem.alloc()));  //memory leak
		res = new_mem->data();
	}
	else if (b1|| b2)
	{
		if (b2) { swap(var1, var2); bool tmp = b1; b1 = b2; b2 = tmp; }  //now b1 == true && b2 == false
		res = var2;
	}
	else
	{
		res = var2;
		mem.free_cell(var1);
	}
	int_name1 = var1;
	int_name2 = var2;
	if (isN)
	{
		cout << exprNum;
		exprNum = 0;
	}
	isN = false;
}

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
%token GLOBAL DEFINE INIT AS QUOTE PRINT SYMBOL ERASE QUEST

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
program :	define program
		| globals program
		| files
		;
files :		files file
		| file
		;
file :		LABEL NUMBER SEMICOLON
		commands goto delete
		{ for (int i = 0; i < 80; i++) { cout << '*'; } cout << endl; }
		|
		LABEL NUMBER SEMICOLON
		commands goto
		{ for (int i = 0; i < 80; i++) { cout << '*'; } cout << endl; }
		;
commands :	commands command
		| command
		;
command :	assign
		| condition
		| decl
		| delete
		| print
		| define
		| erase
		| tern
		;
assign :	NAME ASSIGN expr SEMICOLON
		{
			if (mem.ifs.empty())
			{
				if (mem.isDefine($<string>1) || mem.isGlobal($<string>1))
				{
					string tmp = string($<string>1);
					string *int_name = new string(mem.getInnerName(tmp));  //memory leak
					if (isN)
					{
						asmr.move(exprNumber, int_name->data());
					}
					else { asmr.move($<string>3, int_name->data()); }
					if (!isV) { mem.free_cell($<string>3); }
				}
				else
				{
					string tmp($<string>1);
					if (isV)
					{
						tmp = string($<string>1);
						string *int_name = new string(mem.getInnerName(tmp));  //memory leak
						asmr.move($<string>3, int_name->data());
					}
					else
					{
						if (isN)
						{
							string *int_name = new string(mem.getInnerName(tmp));  //memory leak
							asmr.move(exprNumber, int_name->data());
							mem.free_cell($<string>3);
						}
						else {
						try { mem.free_mem(tmp); }
						catch (int e) { Error::error(e); break; }
						string *int_name = new string($<string>3);
						mem.define(tmp, *int_name);
						}
					}
				}
			}
			else
			{
				string tmp($<string>1);
				string *int_name = new string(mem.getInnerName(tmp));  //memory leak
				if (isN)
				{
					cout << exprNum; exprNum = 0;
				}
				asmr.move($<string>3, int_name->data());
				if (!isV) { mem.free_cell($<string>3); }
			}
			isV = false; isN = false;
		}
		;
expr :		int_expr
		{
			isV = isVar($<string>1);
			if (isV) { char *s = strdup($<string>1); delSht(s); $<string>1 = s; }  //memory leak
			$<string>$ = $<string>1;	
		}
		;
int_expr:	int_expr ADD int_expr
		{
			inExpr($<string>1, $<string>3, $<string>$);
			asmr.add($<string>1, $<string>3, $<string>$);
		}
		| int_expr SUB int_expr
		{
			inExpr($<string>1, $<string>3, $<string>$);
			asmr.sub($<string>1, $<string>3, $<string>$);		
		}
		| int_expr OR int_expr
		{			
			inExpr($<string>1, $<string>3, $<string>$);
			asmr._or($<string>1, $<string>3, $<string>$);
		}
		| int_expr AND int_expr
		{			
			inExpr($<string>1, $<string>3, $<string>$);
			asmr._and($<string>1, $<string>3, $<string>$);
		}
		| int_expr EQ int_expr
		{			
			inExpr($<string>1, $<string>3, $<string>$);
			asmr.eq($<string>1, $<string>3, $<string>$);
		}
		| int_expr LT int_expr
		{			
			inExpr($<string>1, $<string>3, $<string>$);
			asmr.lt($<string>1, $<string>3, $<string>$);
		}
		| int_expr LE int_expr
		{			
			inExpr($<string>1, $<string>3, $<string>$);
			asmr.le($<string>1, $<string>3, $<string>$);
		}
		| int_expr GT int_expr
		{			
			inExpr($<string>1, $<string>3, $<string>$);
			asmr.gt($<string>1, $<string>3, $<string>$);
		}
		| int_expr GE int_expr
		{			
			inExpr($<string>1, $<string>3, $<string>$);
			asmr.ge($<string>1, $<string>3, $<string>$);
		}
		| NOT int_expr
		{
			bool isV = isVar($<string>2);
			string new_mem;
			if (isV) { char *s = strdup($<string>2); delSht(s); $<string>2 = s; }  //memory leak
			if (!isV)
			{
				asmr._not($<string>2, $<string>2);
				$<string>$ = $<string>2;
			}
			else
			{
				try { new_mem = mem.numberToInnerName(mem.alloc()); }
				catch (int e) { Error::error(e); }
				char *res = strdup(new_mem.data());
				asmr._not($<string>2, res);
				$<string>$ = res;
			}
		}
		| NUMBER
		{
			string *str = new string(mem.numberToInnerName(mem.alloc()));  //memory leak
			char *memory = (char *)malloc(33*sizeof(char));
			string tmp = string("IMOVE(") + string(mem.itoa($<number>1, memory)) + string(", ") + string(str->data()) + string(")\n");			
			free(memory);
			if (exprNum != 0) { tmp = string(exprNum) + tmp; }
			string *left = new string(tmp.data());  //memory leak
			exprNum = left->data();  //memory leak
			exprName = str->data();
			//asmr.move($<number>1, str->data());
			$<string>$ = str->data();
			isN = true; exprNumber = $<number>1;
		}
		| NAME
		{
			/*string ext_name($<string>1);
			string *int_name;
			try { int_name = new string(mem.getInnerName(ext_name)); }  //memory leak
			catch (int e) { Error::error(e); break; }
			string *new_mem = new string(mem.numberToInnerName(mem.alloc()));  //memory leak
			asmr.move(int_name->data(), new_mem->data());*/
			//$<string>$ = new_mem->data();
			string str = string($<string>1);
			string p = mem.getInnerName(str);
			string s = p + string("$");
			string *res = new string(s.data());  //memory leak
			$<string>$ = res->data();			
		}
		| OBRACE int_expr CBRACE
		{
			$<string>$ = $<string>2;
		}
		;
decl:		INIT NAME ASSIGN expr SEMICOLON
		{
			string *new_mem;
			string str($<string>2);
			if (isN)
			{
				cout << exprNum;
				exprNum = 0;
				string *ext_name = new string($<string>2);  //memory leak
				string *int_name = new string($<string>4);  //memory leak
				mem.define(*ext_name, *int_name);
			}
			else
			{
				if (isV)
				{
					try { new_mem = new string(mem.alloc(str)); }  //memory leak
					catch (int e) { Error::error(e); break; }
					asmr.move($<string>4, new_mem->data());	
				}
				else
				{
					string *ext_name = new string($<string>2);  //memory leak
					string *int_name = new string($<string>4);  //memory leak
					mem.define(*ext_name, *int_name);
				}
			}
			isV = false; isN = false;
		}
		| INIT NAME SEMICOLON
		{
			string str($<string>2);
			try { mem.alloc(str); }
			catch (int e) { Error::error(e); break; }
		}
		;
print:		PRINT OBRACE STRINGCONST CBRACE SEMICOLON
		{
			char *what = strdup($<string>3);
			if (strlen(what) != 4) { Error::error(8); }
			for (int i = 0; i < 4; i++)
			{
				print(what[i], i);
			}
			free(what);
		}
		| PRINT OBRACE SYMBOL COMA NUMBER CBRACE SEMICOLON
		{
			print(*($<string>3), $<number>5);
		}
		| PRINT OBRACE expr COMA NAME CBRACE SEMICOLON
		{
			asmr.cd($<string>3, $<string>5);
			if (!isV) { mem.free_cell($<string>3); }
			isV = false; isN = false;
		}
		/*| PRINT OBRACE NAME COMA NUMBER CBRACE SEMICOLON
		{
			char *tmp = (char *)malloc(33*sizeof(char));
			string where = string("LED") + string(mem.itoa($<number>5, tmp));
			free(tmp);
			asmr.cd($<string>3, where.data());
		}*/
		;
goto :		GOTO expr SEMICOLON
		{
			if (isN)
			{
				asmr.move(exprNumber, "NETLIST_SELECT");
			}
			else { asmr.move($<string>2, "NETLIST_SELECT"); }
			if (!isV) { mem.free_cell($<string>2); }
			isV = false; isN = false;
		}
		;
globals:	INIT GLOBAL NAME SEMICOLON
		{
			string str($<string>3);
			string *int_name;
			try { int_name = new string(mem.alloc(str)); }  //memory leak
			catch (int e) { Error::error(e); break; }
			mem.makeGlobal(*int_name);
		}
		;
define:		DEFINE NAME AS NAME SEMICOLON
		{
			string *ext_name = new string($<string>2);  //memory leak
			string *int_name = new string($<string>4);  //memory leak
			mem.addDef($<string>2, $<string>4);
		}
		;
delete:		DELETE NAME SEMICOLON
		{
			string ext_name = string($<string>2);	
			try { mem.free_mem(ext_name); }
			catch (int e) { Error::error(e); break; }
		}
		;
erase :		ERASE NAME SEMICOLON
		{
			string ext_name($<string>2);
			try { mem.erase(ext_name); }
			catch (int e) { Error::error(e); }
		}
		;
tern :		NAME ASSIGN NAME QUEST NUMBER COLON NUMBER SEMICOLON
		{
			string res($<string>1);
			string cond($<string>3);
			string s1 = mem.getInnerName(res);
			string s2 = mem.getInnerName(cond);
			asmr.imux2(s2.data(), s1.data(), $<number>7, $<number>5);
		}
		;
block :		OBLOCK commands CBLOCK
		//| command
		;	
condition :	//IF OBRACE expr CBRACE block ELSE block
		IF OBRACE expr
		{
			string tmp($<string>3);
			string *int_name = new string(tmp.data());
			string *new_mem;  //for stack
			if (!isV)
			{
				new_mem = new string(int_name->data());
			}
			else
			{			
				new_mem = new string(mem.numberToInnerName(mem.alloc()));  //for stack
				asmr.move(int_name->data(), new_mem->data());  //may be should be returned
			}
			if (mem.ifs.empty())
			{
				mem.ifs.push(new_mem);
			}
			else
			{
				string *top = mem.ifs.top();
				asmr._and(top->data(), int_name->data(), new_mem->data());
				mem.ifs.push(new_mem);
			}
			isV = false; isN = false;
		}
		CBRACE block
		{
			mem.free_cell(mem.ifs.top()->data());
			delete mem.ifs.top();
			mem.ifs.pop();
		}
		;
%%
