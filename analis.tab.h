/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
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

/* Line 2068 of yacc.c  */
#line 461 "analis.y"

	int number;
	const char *string;



/* Line 2068 of yacc.c  */
#line 109 "analis.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


