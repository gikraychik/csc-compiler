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
int main()
{
	yyparse();
	mem.print();
}


/* Line 268 of yacc.c  */
#line 449 "analis.tab.c"

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
     OR = 295,
     AND = 296,
     GE = 297,
     LE = 298,
     GT = 299,
     LT = 300,
     EQ = 301,
     SUB = 302,
     ADD = 303,
     DIV = 304,
     MUL = 305,
     NOT = 306
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 393 "analis.y"

	int number;
	const char *string;



/* Line 293 of yacc.c  */
#line 543 "analis.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 555 "analis.tab.c"

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
#define YYLAST   137

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  45
/* YYNRULES -- Number of states.  */
#define YYNSTATES  105

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   306

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
      45,    46,    47,    48,    49,    50,    51
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     6,     9,    11,    14,    16,    22,    25,
      27,    29,    31,    33,    35,    37,    39,    41,    46,    48,
      52,    56,    60,    64,    68,    72,    76,    80,    84,    87,
      89,    91,    95,   101,   105,   111,   119,   127,   131,   138,
     143,   149,   153,   157,   161,   162
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      53,     0,    -1,    65,    53,    -1,    64,    53,    -1,    54,
      -1,    54,    55,    -1,    55,    -1,    21,     3,     7,    56,
      63,    -1,    56,    57,    -1,    57,    -1,    58,    -1,    69,
      -1,    61,    -1,    66,    -1,    62,    -1,    65,    -1,    67,
      -1,     4,    13,    59,     7,    -1,    60,    -1,    60,    48,
      60,    -1,    60,    47,    60,    -1,    60,    40,    60,    -1,
      60,    41,    60,    -1,    60,    46,    60,    -1,    60,    45,
      60,    -1,    60,    43,    60,    -1,    60,    44,    60,    -1,
      60,    42,    60,    -1,    51,    60,    -1,     3,    -1,     4,
      -1,    29,    60,    30,    -1,    34,     4,    13,    59,     7,
      -1,    34,     4,     7,    -1,    37,    29,     8,    30,     7,
      -1,    37,    29,    38,    11,     3,    30,     7,    -1,    37,
      29,    59,    11,     3,    30,     7,    -1,    28,     3,     7,
      -1,    34,    32,     4,    13,    59,     7,    -1,    34,    32,
       4,     7,    -1,    33,     4,    35,     4,     7,    -1,    31,
       4,     7,    -1,    39,     4,     7,    -1,    24,    56,    25,
      -1,    -1,    14,    29,     4,    70,    30,    68,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   399,   399,   400,   401,   403,   404,   406,   409,   410,
     412,   413,   414,   415,   416,   417,   418,   420,   429,   431,
     437,   443,   449,   455,   461,   467,   473,   479,   485,   490,
     496,   506,   511,   522,   529,   539,   543,   560,   565,   579,
     588,   595,   602,   609,   614,   613
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
  "QUOTE", "PRINT", "SYMBOL", "ERASE", "OR", "AND", "GE", "LE", "GT", "LT",
  "EQ", "SUB", "ADD", "DIV", "MUL", "NOT", "$accept", "program", "files",
  "file", "commands", "command", "assign", "expr", "int_expr", "decl",
  "print", "goto", "globals", "define", "delete", "erase", "block",
  "condition", "$@1", 0
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
     305,   306
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    52,    53,    53,    53,    54,    54,    55,    56,    56,
      57,    57,    57,    57,    57,    57,    57,    58,    59,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    61,    61,    62,    62,    62,    63,    64,    64,
      65,    66,    67,    68,    70,    69
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     1,     2,     1,     5,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     1,
       1,     3,     5,     3,     5,     7,     7,     3,     6,     4,
       5,     3,     3,     3,     0,     6
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     4,     6,     0,     0,     0,
       0,     0,     1,     5,     3,     2,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     9,    10,    12,    14,
      15,    13,    16,    11,     0,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     8,     7,    40,    29,    30,     0,
       0,     0,    18,     0,    44,    41,    33,     0,     0,     0,
       0,    42,     0,     0,    28,    38,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    17,     0,     0,     0,     0,
       0,    37,    31,    21,    22,    27,    25,    26,    24,    23,
      20,    19,     0,    32,    34,     0,     0,     0,    45,     0,
       0,     0,    35,    36,    43
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     6,    25,    26,    27,    51,    52,    28,
      29,    45,     7,    30,    31,    32,    98,    33,    76
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -36
static const yytype_int16 yypact[] =
{
     -14,    23,    11,   -15,    24,    10,   -36,   -14,   -14,    32,
       7,    41,   -36,   -36,   -36,   -36,    26,    44,     3,    42,
      25,    60,   109,    37,   110,     4,   -36,   -36,   -36,   -36,
     -36,   -36,   -36,   -36,   108,   -36,     0,     0,   112,   111,
      21,    -2,   113,   114,   -36,   -36,   -36,   -36,   -36,     0,
       0,   115,    27,   116,   -36,   -36,   -36,     0,    89,   117,
     118,   -36,   119,    47,   -36,   -36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   -36,    91,   120,   123,   121,
     122,   -36,   -36,    55,    62,   -35,   -35,   -35,   -35,    -1,
     -36,   -36,   107,   -36,   -36,   102,   103,    26,   -36,   127,
     128,    19,   -36,   -36,   -36
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -36,   104,   -36,   131,    40,   -25,   -36,   -32,    12,   -36,
     -36,   -36,   -36,    14,   -36,   -36,   -36,   -36,   -36
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      44,    47,    48,    47,    48,    53,    58,     1,    19,    60,
      35,    72,    73,    74,     8,    10,    36,    11,    20,     2,
       3,     8,     8,    19,    12,    77,     9,    49,    56,    49,
      19,     1,    43,    20,    57,    21,    59,     2,    22,    16,
      20,    23,    17,    24,   104,    18,    73,    74,    34,    50,
      21,    50,     2,    22,    38,    37,    23,    21,    24,     2,
      22,    63,    64,    23,    39,    24,    41,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    44,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    67,    68,    69,    70,
      71,    72,    73,    74,    68,    69,    70,    71,    72,    73,
      74,    14,    15,    40,    42,    46,    54,    62,    55,    78,
      61,    92,    65,    75,    95,    96,    81,    93,    79,    80,
      94,    97,    99,   100,   102,   103,    13,   101
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-36))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_uint8 yycheck[] =
{
      25,     3,     4,     3,     4,    37,     8,    21,     4,    41,
       7,    46,    47,    48,     0,     4,    13,    32,    14,    33,
      34,     7,     8,     4,     0,    57,     3,    29,     7,    29,
       4,    21,    28,    14,    13,    31,    38,    33,    34,     7,
      14,    37,    35,    39,    25,     4,    47,    48,     4,    51,
      31,    51,    33,    34,    29,    13,    37,    31,    39,    33,
      34,    49,    50,    37,     4,    39,    29,    40,    41,    42,
      43,    44,    45,    46,    47,    48,   101,    30,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    41,    42,    43,    44,
      45,    46,    47,    48,    42,    43,    44,    45,    46,    47,
      48,     7,     8,     4,     4,     7,     4,     3,     7,    30,
       7,    30,     7,     7,     3,     3,     7,     7,    11,    11,
       7,    24,    30,    30,     7,     7,     5,    97
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    21,    33,    34,    53,    54,    55,    64,    65,     3,
       4,    32,     0,    55,    53,    53,     7,    35,     4,     4,
      14,    31,    34,    37,    39,    56,    57,    58,    61,    62,
      65,    66,    67,    69,     4,     7,    13,    13,    29,     4,
       4,    29,     4,    28,    57,    63,     7,     3,     4,    29,
      51,    59,    60,    59,     4,     7,     7,    13,     8,    38,
      59,     7,     3,    60,    60,     7,    40,    41,    42,    43,
      44,    45,    46,    47,    48,     7,    70,    59,    30,    11,
      11,     7,    30,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    30,     7,     7,     3,     3,    24,    68,    30,
      30,    56,     7,     7,    25
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
        case 17:

/* Line 1806 of yacc.c  */
#line 421 "analis.y"
    {
			string tmp((yyvsp[(1) - (4)].string));
			string where = mem.getInnerName(tmp);			
			asmr.move((yyvsp[(3) - (4)].string), where.data());
			//cout << "!" << $<string>3 << "!" << endl;
			mem.free_cell((yyvsp[(3) - (4)].string));
		}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 429 "analis.y"
    { (yyval.string) = (yyvsp[(1) - (1)].string); }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 432 "analis.y"
    {
			asmr.add((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyvsp[(1) - (3)].string));
			mem.free_cell((yyvsp[(3) - (3)].string));
			(yyval.string) = (yyvsp[(1) - (3)].string);
		}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 438 "analis.y"
    {			
			asmr.sub((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyvsp[(1) - (3)].string));
			mem.free_cell((yyvsp[(3) - (3)].string));
			(yyval.string) = (yyvsp[(1) - (3)].string);
		}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 444 "analis.y"
    {			
			asmr._or((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyvsp[(1) - (3)].string));
			mem.free_cell((yyvsp[(3) - (3)].string));
			(yyval.string) = (yyvsp[(1) - (3)].string);
		}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 450 "analis.y"
    {			
			asmr._and((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyvsp[(1) - (3)].string));
			mem.free_cell((yyvsp[(3) - (3)].string));
			(yyval.string) = (yyvsp[(1) - (3)].string);
		}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 456 "analis.y"
    {			
			asmr.eq((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyvsp[(1) - (3)].string));
			mem.free_cell((yyvsp[(3) - (3)].string));
			(yyval.string) = (yyvsp[(1) - (3)].string);
		}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 462 "analis.y"
    {			
			asmr.lt((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyvsp[(1) - (3)].string));
			mem.free_cell((yyvsp[(3) - (3)].string));
			(yyval.string) = (yyvsp[(1) - (3)].string);
		}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 468 "analis.y"
    {			
			asmr.le((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyvsp[(1) - (3)].string));
			mem.free_cell((yyvsp[(3) - (3)].string));
			(yyval.string) = (yyvsp[(1) - (3)].string);
		}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 474 "analis.y"
    {			
			asmr.gt((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyvsp[(1) - (3)].string));
			mem.free_cell((yyvsp[(3) - (3)].string));
			(yyval.string) = (yyvsp[(1) - (3)].string);
		}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 480 "analis.y"
    {			
			asmr.ge((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), (yyvsp[(1) - (3)].string));
			mem.free_cell((yyvsp[(3) - (3)].string));
			(yyval.string) = (yyvsp[(1) - (3)].string);
		}
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 486 "analis.y"
    {
			asmr._not((yyvsp[(2) - (2)].string), (yyvsp[(2) - (2)].string));
			(yyval.string) = (yyvsp[(2) - (2)].string);
		}
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 491 "analis.y"
    {
			string *str = new string(mem.numberToInnerName(mem.alloc()));  //memory leak
			asmr.move((yyvsp[(1) - (1)].number), str->data());
			(yyval.string) = str->data();
		}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 497 "analis.y"
    {
			string ext_name((yyvsp[(1) - (1)].string));
			string *int_name;
			try { int_name = new string(mem.getInnerName(ext_name)); }  //memory leak
			catch (int e) { Error::error(e); break; }
			string *new_mem = new string(mem.numberToInnerName(mem.alloc()));  //memory leak
			asmr.move(int_name->data(), new_mem->data());
			(yyval.string) = new_mem->data();
		}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 507 "analis.y"
    {
			(yyval.string) = (yyvsp[(2) - (3)].string);
		}
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 512 "analis.y"
    {
			string *ext_name = new string((yyvsp[(2) - (5)].string));  //memory leak
			string *int_name = new string((yyvsp[(4) - (5)].string));  //memory leak
			mem.define(*ext_name, *int_name);
			/*string *new_mem;
			try { new_mem = new string(mem.alloc(str)); }  //memory leak
			catch (int e) { Error::error(e); break; }
			asmr.move($<string>4, new_mem->data());
			mem.free_cell($<string>4);*/
		}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 523 "analis.y"
    {
			string str((yyvsp[(2) - (3)].string));
			try { mem.alloc(str); }
			catch (int e) { Error::error(e); break; }
		}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 530 "analis.y"
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

  case 35:

/* Line 1806 of yacc.c  */
#line 540 "analis.y"
    {
			print(*((yyvsp[(3) - (7)].string)), (yyvsp[(5) - (7)].number));
		}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 544 "analis.y"
    {
			if ((yyvsp[(5) - (7)].number)>4) { Error::error(9); break; }
			char *tmp = (char *)malloc(33*sizeof(char));
			string where = string("LED") + string(mem.itoa((yyvsp[(5) - (7)].number), tmp));
			free(tmp);
			asmr.cd((yyvsp[(3) - (7)].string), where.data());
			mem.free_cell((yyvsp[(3) - (7)].string));
		}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 561 "analis.y"
    {
			asmr.move((yyvsp[(2) - (3)].number), "NETLIST_SELECT");
		}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 566 "analis.y"
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

  case 39:

/* Line 1806 of yacc.c  */
#line 580 "analis.y"
    {
			string str((yyvsp[(3) - (4)].string));
			string *int_name;
			try { int_name = new string(mem.alloc(str)); }  //memory leak
			catch (int e) { Error::error(e); break; }
			mem.makeGlobal(*int_name);
		}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 589 "analis.y"
    {
			string *ext_name = new string((yyvsp[(2) - (5)].string));  //memory leak
			string *int_name = new string((yyvsp[(4) - (5)].string));  //memory leak
			mem.define(*ext_name, *int_name);
		}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 596 "analis.y"
    {
			string ext_name = string((yyvsp[(2) - (3)].string));	
			try { mem.free_mem(ext_name); }
			catch (int e) { Error::error(e); break; }
		}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 603 "analis.y"
    {
			string ext_name((yyvsp[(2) - (3)].string));
			try { mem.erase(ext_name); }
			catch (int e) { Error::error(e); }
		}
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 614 "analis.y"
    {
			string tmp((yyvsp[(3) - (3)].string));
			string *int_name = new string(mem.getInnerName(tmp));
			string *new_mem = new string(mem.numberToInnerName(mem.alloc()));
			asmr.move(int_name->data(), new_mem->data());
			if (mem.ifs.empty())
			{
				mem.ifs.push(new_mem);
			}
			else
			{
				string *top = mem.ifs.top();
				asmr._and(int_name->data(), top->data(), new_mem->data());
				mem.ifs.push(new_mem);
			}
		}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 631 "analis.y"
    {
			mem.free_cell(mem.ifs.top()->data());
			delete mem.ifs.top();
			mem.ifs.pop();
		}
    break;



/* Line 1806 of yacc.c  */
#line 2208 "analis.tab.c"
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
#line 637 "analis.y"


