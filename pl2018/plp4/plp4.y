/*------------------------------ plp4.y ------------------------------*/
%token ID INT FLOAT PARI PARD COMA AMP LLAVEI
%token LLAVED ASIG PYC IF OPAS NUMENTERO NUMREAL

%{
#include "comun.h"

// Variables y funciones del A. lexico.
extern int ncol, nlin, findefichero;

extern int yylex();
extern char* yytext;
extern FILE *yyin;

int yyerror(char* s);

const int ENTERO = 1;
const int REAL   = 2;

SymbolTable tS;
std::vector<std::string> variables;

std::string s1, s2;

%}

%%
Fun     : Tipo ID Args Bloque
        {
            tS.prefix = $2.lexema;
            std::cout << "function " << $2.lexema << $3.trad << $1.trad << "\n"; /* function id(args):tipo; */
            printVariables();
            std::cout << $4.trad << "\n"; /* Bloque */
        }
        ;

Tipo    : INT { $$.tipo = ENTERO; $$.trad = ":integer;"; }
        | FLOAT { $$.tipo = REAL; $$.trad = ":real;"; }
        ;

Args    : PARI LArgs PARD { $$.trad = $1.lexema + $2.trad + $3.lexema; } /* "(" + ... + ")"*/
        ;

LArgs   : MasArgs { $$.trad = $1.trad; }
        | { $$.trad = ""; }
        ;

MasArgs : MasArgs COMA A { $$.trad = $1.trad + ";" + $3.trad; }
        | A { $$.trad = $1.trad; }
        ;

A       : Tipo Var
        {
            variables.push_back($2.trad + $1.trad);
            Symbol s($2.trad, $2.tipo);

            tS.add(s);
            $$.trad = $2.trad + $1.trad;
        }
        ;

Var     : AMP ID { s1 = "var "; s2 = $2.lexema; $$.trad = s1 + s2; }
        | ID { $$.trad = $1.lexema; }
        ;

Bloque  : LLAVEI SInstr LLAVED
        {
            tS = SymbolTable(tS);
            if ($2.trad == "") $2.trad = "doNothing";
            $$.trad = "begin\n" + $2.trad + "end\n";
        }
        ;

SInstr  : SInstr Instr
        {
            if($1.trad != "" && $2.trad != "")
                s1 = ";";
            $$.trad = $1.trad + s1 + $2.trad;
        }
        | Instr
        {
            $$.trad = $1.trad;
        }
        ;

Instr   : Bloque
        {
            $$.trad = $1.trad;
        }
        | ID ASIG E PYC
        {
            Symbol s(tS.search($1.lexema));

            $$.trad = s.name + ":= " + $3.trad;
        }
        | IF PARI E PARD Instr
        {
            if($5.trad == "") $5.trad = "doNothing";
            $$.trad = "if " + $3.trad + "then\n" + $5.trad;
        }
        | Tipo ID PYC
        {
            $$.trad = $1.trad;
        }
        ;

E       : E OPAS T
        {
            $1.tipo = $$.tipo; /* PROPAGAMOS EL TIPO DEL ID */
            s1 = $3.tipo == 1 ? "i" : "r";
            $$.trad = $1.trad + $2.lexema + s1 + " " + $3.trad;
        }
        | T
        {
            if($$.tipo == 1 && $1.tipo == 2) /* REAL TO INT */
                msgError(-1, -1, -1, ""); /*TODO: ERROR INCOMPATIBLE!*/
            else if ($$.tipo == 2 && $1.tipo == 2) /* INT TO REAL */
                $$.trad = "itor(" + $1.trad + ")";
            else $$.trad = $1.trad; /* REAL TO REAL OR INT TO INT*/
        }
        ;

T       : NUMENTERO { $$.tipo = ENTERO; $$.trad = $1.lexema; }
        | NUMREAL { $$.tipo = REAL; $$.trad = $1.lexema; } 
        | ID
        {
            Symbol s(tS.search($1.lexema));
            if(s != Symbol()) /* Si ha devuelto algo. */
            {
                $$.trad = s.name;
                $$.tipo = s.type;
            }
            else /* TODO, REVISAR */
                msgError(-1, -1, -1, "");
        }
        ;

%%

void printVariables()
{
    std::cout << "a : " << variables.size();
    std::cout << "var " << variables[0] << "\n";
    for(unsigned i = 1; i < variables.size(); ++i)
        std::cout << "    " << variables[i] << "\n";
}

void msgError(int nerror,int nlin,int ncol,const char *s)
{
     switch (nerror) {
         case ERRLEXICO: fprintf(stderr,"Error lexico (%d,%d): caracter '%s' incorrecto\n",nlin,ncol,s);
            break;
         case ERRSINT: fprintf(stderr,"Error sintactico (%d,%d): en '%s'\n",nlin,ncol,s);
            break;
         case ERREOF: fprintf(stderr,"Error sintactico: fin de fichero inesperado\n");
            break;
         case ERRLEXEOF: fprintf(stderr,"Error lexico: fin de fichero inesperado\n");
            break;
     }
        
     exit(1);
}


int yyerror(char *s)
{
    if (findefichero) 
    {
       msgError(ERREOF,0,0,"");
    }
    else
    {  
       msgError(ERRSINT,nlin,ncol-strlen(yytext),yytext);
    }
}

int main(int argc,char *argv[])
{
    FILE *fent;

    /*if (argc==2)
    {*/
        fent = fopen("file.txt","rt");
        if (fent)
        {
            yyin = fent;
            yyparse();
            fclose(fent);
        }
        else
            fprintf(stderr,"No puedo abrir el fichero\n");
    /*}
    else
        fprintf(stderr,"Uso: ejemplo <nombre de fichero>\n");*/
}
