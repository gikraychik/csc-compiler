%{
#include <stdio.h>
#include "analis.tab.h"

%}

%option noyywrap 
%option yylineno
%option stack

DIGIT [0-9]
NAME [a-zA-Z][a-zA-Z0-9]*
QUOTE [\"]
STRINGCONST [a-zA-Z0-9]*
WS [ \t\n]+
WSN [ \t\n]*

%%
<INITIAL>
{
	{DIGIT}+{WSN}				{ yylval.number = atoi(yytext); return NUMBER; }
	"if"{WS}				{ return IF; }	
	"else"{WS}				{ return ELSE; }
	"{"{WS}					{ return OBLOCK; }
	"}"{WS}					{ return CBLOCK; }
	"goto"{WS}				{ return GOTO; }
	"print"{WSN}/"("			{ return WRITE; }
	"int"{WSN}				{ /*printf("INTEGER");*/ return INTEGER; }
	"label"{WS}				{ return LABEL; }
	"global"{WS}				{ return GLOBAL; }
	"["{WSN}				{ return RECOPENBRACE; }
	"]"{WSN}				{ return RECCLOSEBRACE; }
	"<"{WSN}				{ return LT; }
	"<="{WSN}				{ return LE; }
	">"{WSN}				{ return GT; }
	">="{WSN}				{ return GE; }
	"=="{WSN}				{ return EQ; }
	"!="{WSN}				{ return NE; }
	"&&"{WSN}				{ return AND; }
	"||"{WSN}				{ return OR; }
	"!"{WSN}				{ return NOT; }
	"+"{WSN}				{ return ADD; }
	"-"{WSN}				{ return SUB; }
	"*"{WSN}				{ return MUL; }
	"/"{WSN}				{ return DIV; }		
	"="{WSN}				{ /*printf("ASSIGN");*/ return ASSIGN; }
	","{WSN}				{ /*printf("COMA");*/ return COMA; }
	":"{WSN}				{ /*printf("COLON");*/ return COLON; }
	";"{WSN}				{ /*printf("SEMICOLON");*/ return SEMICOLON; }
	"("{WSN}				{ return OBRACE; }
	")"{WSN}				{ return CBRACE; }
	{NAME}{WSN}				{
							char *s = yytext;
							while (1) {
								if ((*yytext == ' ') || (*yytext == 0)) { break; }
								yytext++;
							}
							*yytext = 0; yytext = s;
							yylval.string = strdup(yytext); /*printf(yytext);*/ return NAME;
						}
	/*<<EOF>>					{ printf("EOF\n"); return EOFF; }*/
}

%%

/*int main()
{
	yylex();
	return 0;
}*/
