/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "analis.y"

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


/* Line 268 of yacc.c  */
#line 543 "analis.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUMBER = 258,
     NAME = 259,
     REF = 260,
     COLON = 261,
     SEMICOLON = 262,
     STRINGCONST = 263,
     STRING = 264,
     INTEGER = 265,
     COMA = 266,
     RETURN = 267,
     ASSIGN = 268,
     IF = 269,
     WHILE = 270,
     LOOP = 271,
     POOL = 272,
     READ = 273,
     WRITE = 274,
     NEQ = 275,
     LABEL = 276,
     RECOPENBRACE = 277,
     RECCLOSEBRACE = 278,
     OBLOCK = 279,
     CBLOCK = 280,
     THEN = 281,
     ELSE = 282,
     GOTO = 283,
     OBRACE = 284,
     CBRACE = 285,
     DELETE = 286,
     GLOBAL = 287,
     DEFINE = 288,
     INIT = 289,
     AS = 290,
     QUOTE = 291,
     PRINT = 292,
     SYMBOL = 293,
     ERASE = 294,
     QUEST = 295,
     OR = 296,
     AND = 297,
     GE = 298,
     LE = 299,
     GT = 300,
     LT = 301,
     EQ = 302,
     SUB = 303,
     ADD = 304,
     DIV = 305,
     MUL = 306,
     NOT = 307
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 487 "analis.y"

	int number;
	const char *string;



/* Line 293 of yacc.c  */
#line 638 "analis.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 650 "analis.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   148

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  53
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  48
/* YYNRULES -- Number of states.  */
#define YYNSTATES  113

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   307

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     6,     9,    11,    14,    16,    23,    29,
      32,    34,    36,    38,    40,    42,    44,    46,    48,    50,
      55,    57,    61,    65,    69,    73,    77,    81,    85,    89,
      93,    96,    98,   100,   104,   110,   114,   120,   128,   136,
     140,   147,   152,   158,   162,   166,   175,   179,   180
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      54,     0,    -1,    66,    54,    -1,    65,    54,    -1,    55,
      -1,    55,    56,    -1,    56,    -1,    21,     3,     7,    57,
      64,    67,    -1,    21,     3,     7,    57,    64,    -1,    57,
      58,    -1,    58,    -1,    59,    -1,    71,    -1,    62,    -1,
      67,    -1,    63,    -1,    66,    -1,    68,    -1,    69,    -1,
       4,    13,    60,     7,    -1,    61,    -1,    61,    49,    61,
      -1,    61,    48,    61,    -1,    61,    41,    61,    -1,    61,
      42,    61,    -1,    61,    47,    61,    -1,    61,    46,    61,
      -1,    61,    44,    61,    -1,    61,    45,    61,    -1,    61,
      43,    61,    -1,    52,    61,    -1,     3,    -1,     4,    -1,
      29,    61,    30,    -1,    34,     4,    13,    60,     7,    -1,
      34,     4,     7,    -1,    37,    29,     8,    30,     7,    -1,
      37,    29,    38,    11,     3,    30,     7,    -1,    37,    29,
      60,    11,     4,    30,     7,    -1,    28,    60,     7,    -1,
      34,    32,     4,    13,    60,     7,    -1,    34,    32,     4,
       7,    -1,    33,     4,    35,     4,     7,    -1,    31,     4,
       7,    -1,    39,     4,     7,    -1,     4,    13,     4,    40,
       3,     6,     3,     7,    -1,    24,    57,    25,    -1,    -1,
      14,    29,    60,    72,    30,    70,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   493,   493,   494,   495,   497,   498,   500,   504,   508,
     509,   511,   512,   513,   514,   515,   516,   517,   518,   520,
     571,   578,   583,   588,   593,   598,   603,   608,   613,   618,
     623,   642,   656,   671,   676,   699,   706,   716,   720,   734,
     745,   759,   768,   775,   782,   789,   798,   803,   802
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUMBER", "NAME", "REF", "COLON",
  "SEMICOLON", "STRINGCONST", "STRING", "INTEGER", "COMA", "RETURN",
  "ASSIGN", "IF", "WHILE", "LOOP", "POOL", "READ", "WRITE", "NEQ", "LABEL",
  "RECOPENBRACE", "RECCLOSEBRACE", "OBLOCK", "CBLOCK", "THEN", "ELSE",
  "GOTO", "OBRACE", "CBRACE", "DELETE", "GLOBAL", "DEFINE", "INIT", "AS",
  "QUOTE", "PRINT", "SYMBOL", "ERASE", "QUEST", "OR", "AND", "GE", "LE",
  "GT", "LT", "EQ", "SUB", "ADD", "DIV", "MUL", "NOT", "$accept",
  "program", "files", "file", "commands", "command", "assign", "expr",
  "int_expr", "decl", "print", "goto", "globals", "define", "delete",
  "erase", "tern", "block", "condition", "$@1", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    53,    54,    54,    54,    55,    55,    56,    56,    57,
      57,    58,    58,    58,    58,    58,    58,    58,    58,    59,
      60,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    62,    62,    63,    63,    63,    64,
      65,    65,    66,    67,    68,    69,    70,    72,    71
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     1,     2,     1,     6,     5,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     1,     1,     3,     5,     3,     5,     7,     7,     3,
       6,     4,     5,     3,     3,     8,     3,     0,     6
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     4,     6,     0,     0,     0,
       0,     0,     1,     5,     3,     2,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    10,    11,    13,    15,
      16,    14,    17,    18,    12,     0,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     9,     8,    42,    31,    32,
       0,     0,     0,    20,    32,     0,    47,    43,    35,     0,
       0,     0,     0,    44,     0,     7,     0,    30,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,    39,    33,    23,    24,    29,
      27,    28,    26,    25,    22,    21,     0,     0,    34,    36,
       0,     0,     0,     0,    48,     0,     0,     0,     0,    37,
      38,    45,    46
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     6,    25,    26,    27,    52,    53,    28,
      29,    46,     7,    30,    31,    32,    33,   104,    34,    80
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -36
static const yytype_int16 yypact[] =
{
     -12,     2,    14,    -6,    44,    25,   -36,   -12,   -12,    28,
      18,    43,   -36,   -36,   -36,   -36,    37,    50,    12,    46,
      31,    58,    60,    38,    65,     6,   -36,   -36,   -36,   -36,
     -36,   -36,   -36,   -36,   -36,    66,   -36,     0,     4,     0,
      68,    35,    -2,    73,     0,   -36,    41,   -36,   -36,   -36,
       0,     0,    74,    61,    86,   120,   -36,   -36,   -36,     0,
      98,   118,   119,   -36,   124,   -36,    52,   -36,   -36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   129,   -36,
     103,   127,   128,   133,   134,   -36,   -36,    69,    76,    30,
      30,    30,    30,   -35,   -36,   -36,   131,   115,   -36,   -36,
     110,   111,   139,    37,   -36,   136,   137,   138,    24,   -36,
     -36,   -36,   -36
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -36,    23,   -36,   141,    45,   -25,   -36,   -27,    15,   -36,
     -36,   -36,   -36,    16,   101,   -36,   -36,   -36,   -36,   -36
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      45,    48,    49,    48,    49,     9,    60,    48,    54,     1,
      19,    55,    56,    76,    77,    62,     8,    64,    10,    36,
      20,     2,     3,     8,     8,    37,    11,    50,    19,    50,
      14,    15,    81,    50,    44,    16,    61,    21,    20,     2,
      22,    19,    58,    23,    12,    24,     1,    18,    59,   112,
      51,    20,    51,    17,    35,    21,    51,     2,    22,    38,
      39,    23,    40,    24,    41,    66,    67,    42,    21,    43,
       2,    22,    21,    47,    23,    57,    24,    75,    76,    77,
      63,    68,    86,    45,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    70,    71,    72,    73,    74,    75,    76,    77,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    82,    83,
      84,    85,    96,    97,    98,    99,   100,   102,   101,   103,
     105,   106,   107,   109,   110,   111,    13,    65,   108
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-36))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_uint8 yycheck[] =
{
      25,     3,     4,     3,     4,     3,     8,     3,     4,    21,
       4,    38,    39,    48,    49,    42,     0,    44,     4,     7,
      14,    33,    34,     7,     8,    13,    32,    29,     4,    29,
       7,     8,    59,    29,    28,     7,    38,    31,    14,    33,
      34,     4,     7,    37,     0,    39,    21,     4,    13,    25,
      52,    14,    52,    35,     4,    31,    52,    33,    34,    13,
      29,    37,     4,    39,     4,    50,    51,    29,    31,     4,
      33,    34,    31,     7,    37,     7,    39,    47,    48,    49,
       7,     7,    30,   108,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    42,    43,    44,    45,    46,    47,    48,    49,    43,
      44,    45,    46,    47,    48,    49,    40,     7,    30,    11,
      11,     7,     3,    30,     7,     7,     3,     6,     4,    24,
      30,    30,     3,     7,     7,     7,     5,    46,   103
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    21,    33,    34,    54,    55,    56,    65,    66,     3,
       4,    32,     0,    56,    54,    54,     7,    35,     4,     4,
      14,    31,    34,    37,    39,    57,    58,    59,    62,    63,
      66,    67,    68,    69,    71,     4,     7,    13,    13,    29,
       4,     4,    29,     4,    28,    58,    64,     7,     3,     4,
      29,    52,    60,    61,     4,    60,    60,     7,     7,    13,
       8,    38,    60,     7,    60,    67,    61,    61,     7,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    40,     7,
      72,    60,    30,    11,    11,     7,    30,    61,    61,    61,
      61,    61,    61,    61,    61,    61,     3,    30,     7,     7,
       3,     4,     6,    24,    70,    30,    30,     3,    57,     7,
       7,     7,    25
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 7:

/* Line 1806 of yacc.c  */
#line 502 "analis.y"
    { for (int i = 0; i < 80; i++) { cout << '*'; } cout << endl; }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 506 "analis.y"
    { for (int i = 0; i < 80; i++) { cout << '*'; } cout << endl; }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 521 "analis.y"
    {
			if (mem.ifs.empty())
			{
				if (mem.isDefine((yyvsp[(1) - (4)].string)) || mem.isGlobal((yyvsp[(1) - (4)].string)))
				{
					string tmp = string((yyvsp[(1) - (4)].string));
					string *int_name = new string(mem.getInnerName(tmp));  //memory leak
					if (isN)
					{
						asmr.move(exprNumber, int_name->data());
					}
					else { asmr.move((yyvsp[(3) - (4)].string), int_name->data()); }
					if (!isV) { mem.free_cell((yyvsp[(3) - (4)].string)); }
				}
				else
				{
					string tmp((yyvsp[(1) - (4)].string));
					if (isV)
					{
						tmp = string((yyvsp[(1) - (4)].string));
						string *int_name = new string(mem.getInnerName(tmp));  //memory leak
						asmr.move((yyvsp[(3) - (4)].string), int_name->data());
					}
					else
					{
						if (isN)
						{
							string *int_name = new string(mem.getInnerName(tmp));  //memory leak
							asmr.move(exprNumber, int_name->data());
							mem.free_cell((yyvsp[(3) - (4)].string));
						}
						else {
						try { mem.free_mem(tmp); }
						catch (int e) { Error::error(e); break; }
						string *int_name = new string((yyvsp[(3) - (4)].string));
						mem.define(tmp, *int_name);
						}
					}
				}
			}
			else
			{
				string tmp((yyvsp[(1) - (4)].string));
				string *int_name = new string(mem.getInnerName(tmp));  //memory leak
				asmr.move((yyvsp[(3) - (4)].string), int_name->data());
				if (!isV) { mem.free_cell((yyvsp[(3) - (4)].string)); }
			}
			isV = false; isN = false;
		}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 572 "analis.y"
    {
			isV = isVar((yyvsp[(1) - (1)].string));
			if (isV) { char *s = strdup((yyvsp[(1) - (1)].string)); delSht(s); (yyvsp[(1) - (1)].string) = s; }  //memory leak
			(yyval.string) = (yyvsp[(1) - (1)].string);	
		}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 579 "analis.y"
    {
			inExpr((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyval.string));
			asmr.add((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyval.string));
		}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 584 "analis.y"
    {
			inExpr((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyval.string));
			asmr.sub((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyval.string));		
		}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 589 "analis.y"
    {			
			inExpr((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyval.string));
			asmr._or((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyval.string));
		}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 594 "analis.y"
    {			
			inExpr((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyval.string));
			asmr._and((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyval.string));
		}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 599 "analis.y"
    {			
			inExpr((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyval.string));
			asmr.eq((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyval.string));
		}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 604 "analis.y"
    {			
			inExpr((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyval.string));
			asmr.lt((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyval.string));
		}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 609 "analis.y"
    {			
			inExpr((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyval.string));
			asmr.le((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyval.string));
		}
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 614 "analis.y"
    {			
			inExpr((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyval.string));
			asmr.gt((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyval.string));
		}
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 619 "analis.y"
    {			
			inExpr((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyval.string));
			asmr.ge((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyval.string));
		}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 624 "analis.y"
    {
			bool isV = isVar((yyvsp[(2) - (2)].string));
			string new_mem;
			if (isV) { char *s = strdup((yyvsp[(2) - (2)].string)); delSht(s); (yyvsp[(2) - (2)].string) = s; }  //memory leak
			if (!isV)
			{
				asmr._not((yyvsp[(2) - (2)].string), (yyvsp[(2) - (2)].string));
				(yyval.string) = (yyvsp[(2) - (2)].string);
			}
			else
			{
				try { new_mem = mem.numberToInnerName(mem.alloc()); }
				catch (int e) { Error::error(e); }
				char *res = strdup(new_mem.data());
				asmr._not((yyvsp[(2) - (2)].string), res);
				(yyval.string) = res;
			}
		}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 643 "analis.y"
    {
			string *str = new string(mem.numberToInnerName(mem.alloc()));  //memory leak
			char *memory = (char *)malloc(33*sizeof(char));
			string tmp = string("IMOVE(") + string(mem.itoa((yyvsp[(1) - (1)].number), memory)) + string(", ") + string(str->data()) + string(")\n");			
			free(memory);
			if (exprNum != 0) { tmp = string(exprNum) + tmp; }
			string *left = new string(tmp.data());  //memory leak
			exprNum = left->data();  //memory leak
			exprName = str->data();
			//asmr.move($<number>1, str->data());
			(yyval.string) = str->data();
			isN = true; exprNumber = (yyvsp[(1) - (1)].number);
		}
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 657 "analis.y"
    {
			/*string ext_name($<string>1);
			string *int_name;
			try { int_name = new string(mem.getInnerName(ext_name)); }  //memory leak
			catch (int e) { Error::error(e); break; }
			string *new_mem = new string(mem.numberToInnerName(mem.alloc()));  //memory leak
			asmr.move(int_name->data(), new_mem->data());*/
			//$<string>$ = new_mem->data();
			string str = string((yyvsp[(1) - (1)].string));
			string p = mem.getInnerName(str);
			string s = p + string("$");
			string *res = new string(s.data());  //memory leak
			(yyval.string) = res->data();			
		}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 672 "analis.y"
    {
			(yyval.string) = (yyvsp[(2) - (3)].string);
		}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 677 "analis.y"
    {
			string *new_mem;
			string str((yyvsp[(2) - (5)].string));
			if (isV)
			{
				try { new_mem = new string(mem.alloc(str)); }  //memory leak
				catch (int e) { Error::error(e); break; }
				asmr.move((yyvsp[(4) - (5)].string), new_mem->data());	
			}
			else
			{
				string *ext_name = new string((yyvsp[(2) - (5)].string));  //memory leak
				string *int_name = new string((yyvsp[(4) - (5)].string));  //memory leak
				mem.define(*ext_name, *int_name);
			}
			isV = false;
			/*string *new_mem;
			try { new_mem = new string(mem.alloc(str)); }  //memory leak
			catch (int e) { Error::error(e); break; }
			asmr.move($<string>4, new_mem->data());
			mem.free_cell($<string>4);*/
		}
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 700 "analis.y"
    {
			string str((yyvsp[(2) - (3)].string));
			try { mem.alloc(str); }
			catch (int e) { Error::error(e); break; }
		}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 707 "analis.y"
    {
			char *what = strdup((yyvsp[(3) - (5)].string));
			if (strlen(what) != 4) { Error::error(8); }
			for (int i = 0; i < 4; i++)
			{
				print(what[i], i);
			}
			free(what);
		}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 717 "analis.y"
    {
			print(*((yyvsp[(3) - (7)].string)), (yyvsp[(5) - (7)].number));
		}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 721 "analis.y"
    {
			asmr.cd((yyvsp[(3) - (7)].string), (yyvsp[(5) - (7)].string));
			if (!isV) { mem.free_cell((yyvsp[(3) - (7)].string)); }
			isV = false;
		}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 735 "analis.y"
    {
			if (isN)
			{
				asmr.move(exprNumber, "NETLIST_SELECT");
			}
			else { asmr.move((yyvsp[(2) - (3)].string), "NETLIST_SELECT"); }
			if (!isV) { mem.free_cell((yyvsp[(2) - (3)].string)); }
			isV = false; isN = false;
		}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 746 "analis.y"
    {
			string *ext_name = new string((yyvsp[(3) - (6)].string));  //memory leak
			string *int_name = new string((yyvsp[(5) - (6)].string));  //memory leak
			mem.define(*ext_name, *int_name);
			mem.makeGlobal(*int_name);
			/*string str($<string>3);
			string *int_name;
			try { int_name = new string(mem.alloc(str)); }  //memory leak
			catch (int e) { Error::error(e); break; }
			asmr.move($<string>5, int_name->data());
			mem.free_cell($<string>5);
			mem.makeGlobal(*int_name);*/
		}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 760 "analis.y"
    {
			string str((yyvsp[(3) - (4)].string));
			string *int_name;
			try { int_name = new string(mem.alloc(str)); }  //memory leak
			catch (int e) { Error::error(e); break; }
			mem.makeGlobal(*int_name);
		}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 769 "analis.y"
    {
			string *ext_name = new string((yyvsp[(2) - (5)].string));  //memory leak
			string *int_name = new string((yyvsp[(4) - (5)].string));  //memory leak
			mem.addDef((yyvsp[(2) - (5)].string), (yyvsp[(4) - (5)].string));
		}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 776 "analis.y"
    {
			string ext_name = string((yyvsp[(2) - (3)].string));	
			try { mem.free_mem(ext_name); }
			catch (int e) { Error::error(e); break; }
		}
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 783 "analis.y"
    {
			string ext_name((yyvsp[(2) - (3)].string));
			try { mem.erase(ext_name); }
			catch (int e) { Error::error(e); }
		}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 790 "analis.y"
    {
			string res((yyvsp[(1) - (8)].string));
			string cond((yyvsp[(3) - (8)].string));
			string s1 = mem.getInnerName(res);
			string s2 = mem.getInnerName(cond);
			asmr.imux2(s2.data(), s1.data(), (yyvsp[(7) - (8)].number), (yyvsp[(5) - (8)].number));
		}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 803 "analis.y"
    {
			string tmp((yyvsp[(3) - (3)].string));
			tmp = string((yyvsp[(3) - (3)].string));
			string *int_name = new string(tmp.data());
			string *new_mem;
			if (!isV)
			{
				new_mem = new string(int_name->data());
			}
			else
			{			
				new_mem = new string(mem.numberToInnerName(mem.alloc()));  //for stack
				//asmr.move(int_name->data(), new_mem->data());  //may be should be returned
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
			isV = false;
		}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 830 "analis.y"
    {
			mem.free_cell(mem.ifs.top()->data());
			delete mem.ifs.top();
			mem.ifs.pop();
		}
    break;



/* Line 1806 of yacc.c  */
#line 2426 "analis.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 836 "analis.y"


