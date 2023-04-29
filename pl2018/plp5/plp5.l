/*------------------------------ ejemplo.l -------------------------------*/
D    [0-9]
L    [a-zA-Z]
LD   [0-9a-zA-Z]

%{
#include "comun.h"
#include "plp5.tab.h"

int ncol = 1, nlin = 1;

int findefichero = 0;

int ret(int token); // función que actualiza 'nlin' y 'ncol' y devuelve el token

void msgError(int nerror, int nlin,int ncol,const char *s); // función para producir mensajes de error

%}

%x COMENTARIO

%%
%{
/* codigo local */
%}

" "                  {ncol++;}
[\t]                 {ncol++;}
[\n]                 {nlin++;ncol=1;}
"//"(.)*             {; /* comentarios de una linea, no hacer nada */} 
"/*"                 {ncol += strlen(yytext);BEGIN(COMENTARIO);}
<COMENTARIO>[^/*\n]+ {ncol += strlen(yytext);}
<COMENTARIO>\n       {nlin++;ncol=1;}
<COMENTARIO><<EOF>>  {msgError(ERRLEXICO,-1,-1,"");}
<COMENTARIO>"*/"     {ncol+=strlen(yytext); BEGIN(INITIAL);}
<COMENTARIO>[*/]     {ncol += strlen(yytext);}
"algoritmo"			 {return ret(algoritmo);}
"falgoritmo"		 {return ret(falgoritmo);}
"var"				 {return ret(var);}
"fvar"				 {return ret(fvar);}
"entero"			 {return ret(entero);}
"real"				 {return ret(real);}
"logico"			 {return ret(logico);}
"tabla"				 {return ret(tabla);}
"de"				 {return ret(de);}
"escribe"			 {return ret(escribe);}
"lee"				 {return ret(lee);}
"si"				 {return ret(si);}
"entonces"			 {return ret(entonces);}
"sino"				 {return ret(sino);}
"mientras"			 {return ret(mientras);}
"hacer"				 {return ret(hacer);}
"blq"				 {return ret(blq);}
"fblq"				 {return ret(fblq);}
"cierto"			 {return ret(cierto);}
"falso"				 {return ret(falso);}
{L}({LD})*           {return ret(id);}
{D}+                 {return ret(nentero);}
{D}+(\.){D}+         {return ret(nreal);}
"+"                  {return ret(opas);}
"-"                  {return ret(opas);}
"("                  {return ret(pari);}
")"                  {return ret(pard);}
";"                  {return ret(pyc);}
","                  {return ret(coma);}
":"					 {return ret(dospto);}
"="					 {return ret(oprel);}
"<>"				 {return ret(oprel);}
"<"					 {return ret(oprel);}
"<="				 {return ret(oprel);}
">"					 {return ret(oprel);}
">="				 {return ret(oprel);}
"*"					 {return ret(opmd);}
"/"					 {return ret(opmd);}
":="				 {return ret(assig);}
"["					 {return ret(cori);}
"]"					 {return ret(cord);}
"&&"				 {return ret(ybool);}
"||"				 {return ret(obool);}
"!"					 {return ret(nobool);}
.                    {msgError(ERRLEXICO,nlin,ncol,yytext);}

%%

int yywrap(void) {findefichero = 1; return 1;} /* para no tener que linkar con la libreria del lex */

int ret(int token)
{
   yylval.lex = strdup(yytext);
   yylval.nlin = nlin;
   yylval.ncol = ncol;
   ncol += (strlen(yytext));
   return(token);
}