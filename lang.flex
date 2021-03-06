%{
#include <stdio.h>
#include "analis.tab.h"

%}

%option noyywrap 
%option yylineno
%option stack

DIGIT [0-9]
NAME [a-zA-Z][a-zA-Z0-9_]*
QUOTE [\"]
AP[']
STRINGCONST [a-zA-Z0-9]*
SYMBOL [a-zA-Z0-9 .]
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
	"print"{WSN}/"("			{ return PRINT; }
	"int"{WSN}				{ /*printf("INTEGER");*/ return INTEGER; }
	"label"{WS}				{ return LABEL; }
	"global"{WS}				{ return GLOBAL; }
	"define"{WS}				{ return DEFINE; }
	"init"{WS}				{ return INIT; }
	"as"{WS}				{ return AS; }
	"delete"{WS}				{ return DELETE; }
	"erase"{WS}				{ return ERASE; }
	{QUOTE}{STRINGCONST}{QUOTE}{WSN}	{ 
							*yytext = 0; yytext++;
							char *s = yytext;
							while (*yytext != '\"') {
								yytext++;
							}
							*yytext = 0; yytext = s;
							yylval.string = strdup(yytext);
							return STRINGCONST;
						}
	"["{WSN}				{ return RECOPENBRACE; }
	"]"{WSN}				{ return RECCLOSEBRACE; }
	"<"{WSN}				{ return LT; }
	"<="{WSN}				{ return LE; }
	">"{WSN}				{ return GT; }
	">="{WSN}				{ return GE; }
	"=="{WSN}				{ return EQ; }
	"!="{WSN}				{ return NEQ; }
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
	"?"{WSN}				{ return QUEST; }
	{NAME}{WSN}				{
							char *s = yytext;
							while (1) {
								if ((*yytext == ' ') || (*yytext == 0)) { break; }
								yytext++;
							}
							*yytext = 0; yytext = s;
							yylval.string = strdup(yytext); return NAME;
						}
	{AP}{SYMBOL}{AP}			{ *yytext = 0; yytext++; yytext[strlen(yytext)-1] = 0; 
						  yylval.string = strdup(yytext); return SYMBOL;
						}
}

%%

/*int main()
{
	yylex();
	return 0;
}*/
