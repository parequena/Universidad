%token algoritmo falgoritmo var fvar entero real
%token logico tabla de escribe lee si entonces sino
%token mientras hacer blq fblq cierto falso id nentero
%token nreal coma pyc dosp pari pard oprel
%token opas opmd assig cori cord ybool
%token obool nobool dospto

%{
#include "comun.h"

// variables y funciones del A. Léxico
std::string conversion(const Symbol&);
extern int ncol, nlin, findefichero;
extern int yylex();
extern char *yytext;
extern FILE *yyin;
int yyerror(char *s);

// TypeTable.
SymbolTable tS;

// Number of variables.
int numVariables = 0;

// Global Direction
int globalDirection = globDirecStart;

// Vector to take the other variables (LIds).
// Vector to take the other variables (LIds).
struct Variable
{
	std::string id;
	int nlin, ncol;
	int tipo;

	Variable(std::string i, int l, int c, int t):
		id(i),
		nlin(l),
		ncol(c),
		tipo(t)
	{}

	friend std::ostream &operator<<(std::ostream&, const Variable&);
};


std::vector<Variable> moreVar, llids;

void insertVariable(const Variable& v)
{
	if(moreVar.empty()) moreVar.push_back(v);
	else
	{
		int i = 0;
		for(; i < moreVar.size(); ++i)
			if(v.nlin <= moreVar[i].nlin && v.ncol < moreVar[i].ncol)
				break;

		moreVar.insert(moreVar.begin() + i, v);
	}
}
// Auxiliar varibale
std::string auxDirLeft, auxDirRight, auxType, auxVal, opasStr;

int expectedType = -1;

%}

%%
S 		: algoritmo dospto id SDec
		{
			for(Variable v: moreVar)
			{
				if(tS.add(Symbol(v.id, numVariables, v.tipo)))
                    ++numVariables;
                else msgError(ERRYADECL, v.nlin, v.ncol, v.id.c_str());
			}
		}
		SInstr falgoritmo
		{
			// std::cout << tS;
			std::cout << $6.trad << "halt\n";
		}
		;

SDec	: Dec {$$.trad = $1.trad; }
		| { $$.trad = "holi?s"; }
		;

Dec     : var { numVariables = 0; } DVar MDVar fvar
        ;
DVar    : Tipo dospto id LId pyc
        {
			for(Variable v: llids)
			{
				v.tipo = $1.tipo;
				insertVariable(v);
			}
			llids.clear();
			insertVariable(Variable($3.lex, $3.nlin, $3.ncol, $1.tipo));
        }
        ;

MDVar	: DVar MDVar
		{
			$$.trad = $1.trad + $2.trad;
		}		
		| {$$.trad ="";}
		;

LId		: coma id LId
		{
			// moreVar.push_back(Variable($2.lex, $2.nlin, $2.ncol)); /* Recursive add. */
			llids.push_back(Variable($2.lex, $2.nlin, $2.ncol, -1));
		}
        | {} // Recursive stop.
        ;

Tipo 	: entero{$$.trad ="entero "; $$.tipo = static_cast<int>(Types::Integer); }
		| real {$$.trad ="real "; $$.tipo = static_cast<int>(Types::Real); }
		| logico {$$.trad = "logico "; $$.tipo = static_cast<int>(Types::Logical); }
		| tabla nentero de Tipo
        {
            tS.tT.add(stoi($2.lex), $4.tipo); // Add a new type, with the base type.
            $$.tipo = tS.tT.getType();        // Return the current type.
        }
		;

SInstr	: SInstr pyc Instr { $$.trad = $1.trad + $3.trad; }
		| Instr {$$.trad = $1.trad;}
		;

Instr	: escribe Expr
		{
			if($2.tipo == static_cast<int>(Types::Logical))
				$$.trad ="wrc " + $2.lex + "\n";
			else
			{
				Symbol s1 = tS.getFirstTemporal(numVariables);
				Symbol s2 = tS.search($2.lex);
				auxType = "wr";
				auxType += s2.type == static_cast<int>(Types::Integer) ? "i " : "r ";

				/*$$.trad += "ESCRmov #0 " + std::to_string(++globalDirection)   + "\t\t; 0 -> " + std::to_string(globalDirection) + "\n";
				$$.trad += "mov " + std::to_string(globalDirection) + " A" + "\t\t; " + std::to_string(globalDirection) + " -> " + "A" + "\n";*/
				$$.trad = $2.trad;
				$$.trad += "addi #" + std::to_string(s2.direction)         + "\t\t\t; " + "\n";
				$$.trad += "mov @A " + std::to_string(globalDirection)     + "\t\t; " + "\n";
				$$.trad += auxType + std::to_string(globalDirection)       + "\t\t; " + "write\n";
				$$.trad += "wrl\n";

				tS.setValue(s1, 0);
				tS.eraseNotVariables(numVariables);
				globalDirection = globDirecStart;
			}
		}
		| lee Ref {$$.trad = "";}
		| si Expr entonces Instr
		{
			if($2.tipo != static_cast<int>(Types::Logical))
				msgError(ERR_EXP_LOG, $1.nlin, $1.ncol, "");
			$$.trad = "";
		}
		| si Expr entonces Instr sino Instr
		{
			if($2.tipo != static_cast<int>(Types::Logical))
				msgError(ERR_EXP_LOG, $1.nlin, $1.ncol, "");
			$$.trad = "";
		}
		| mientras Expr hacer Instr {$$.trad = "";}
		| Ref assig Expr
		{
			if(!isBasic($1.tipo))
				msgError(ERRFALTAN, $1.nlin, $1.ncol, "");

			if($3.tipo == static_cast<int>(Types::Logical))
				msgError(ERR_EXDER_RE, $2.nlin, $2.ncol, $2.lex.c_str());

			Symbol s1 = tS.search($1.lex);
			if(s1 == Symbol())
				msgError(ERRNODECL, $1.nlin, $1.ncol, $1.lex.c_str());
			Symbol s2 = tS.search($3.lex);

			// Set value to the left.
			tS.setValue(s1, s2.value);

			$$.trad = "; " + $1.lex + " " + $2.lex + " " + $3.lex + "\n";
			$$.trad += $3.trad;
			$$.trad += $1.trad;
			/*$$.trad += "ASSIGmov #0 " + std::to_string(++globalDirection)   + "\t\t; 0 -> " + std::to_string(globalDirection) + "\n";
			$$.trad += "mov " + std::to_string(globalDirection) + " A" + "\t\t; " + std::to_string(globalDirection) + " -> " + "A" + "\n";*/

			if(s1.type != s2.type)
			{
				std::string lop = s1.type == static_cast<int>(Types::Integer) ? "i" : "r"; // Left
				std::string rop = s2.type == static_cast<int>(Types::Integer) ? "i" : "r"; // Right
				std::string conv = rop + "to" + lop;
				$$.tipo = rop == "i" ? static_cast<int>(Types::Integer) : static_cast<int>(Types::Real);

				$$.trad += "mov " + std::to_string(globalDirection) + " A" + "\t\t; L" + " -> " + std::to_string(globalDirection) + "\n";
				$$.trad += conv + "\n";
				$$.trad += "mov A " + std::to_string(globalDirection)      + "\t\t; " + std::to_string(globalDirection) + " -> A" + "\n";
			}

			$$.trad += "addi #" + std::to_string(s1.direction)         + "\t\t\t; dir[" + std::to_string(s1.direction) + "]\n";
			$$.trad += "mov " + std::to_string(s2.direction) + " @A"   + "\t\t; L -> @A\n";

			tS.eraseNotVariables(numVariables);
			globalDirection = globDirecStart;
		}
		| blq SInstr fblq {$$.trad = "";}
		;

Expr	: Expr obool Econj
		{
			if($3.tipo != static_cast<int>(Types::Logical))
				msgError(ERR_EXDER_LOG, $2.nlin, $2.ncol, $2.lex.c_str());
			$$.tipo = static_cast<int>(Types::Logical);

			if($1.lex == "c" || $3.lex == "c")
				$$.lex = "c";
			else $$.lex = "f";
		}
		| Econj
		{
			$$.trad = $1.trad;
			$$.tipo = $1.tipo;
		}
		;

Econj	: Econj ybool Ecomp
		{
			if($1.lex == "f" || $3.lex == "f")
				$$.lex = "f";
			else $$.lex = "c";

			if($3.tipo != static_cast<int>(Types::Logical))
				msgError(ERR_EXDER_LOG, $2.nlin, $2.ncol, $2.lex.c_str());
			
			$$.tipo = static_cast<int>(Types::Logical);
		}
		| Ecomp
		{
			$$.trad = $1.trad;
			$$.tipo = $1.tipo;
		}
		;

Ecomp	: Esimple oprel Esimple
		{
			$$.tipo = static_cast<int>(Types::Logical);
			$$.trad = "";
		}
		| Esimple
		{
			$$.tipo = $1.tipo;
			$$.trad = $1.trad;
		}
		;

Esimple	: Esimple opas Term
		{
			if($3.tipo == static_cast<int>(Types::Logical))
				msgError(ERR_EXDER_RE, $2.nlin, $2.ncol, $2.lex.c_str());

			Symbol s1 = tS.search($1.lex); // Get left symbol.
			if(s1 == Symbol())
				msgError(ERRNODECL, $1.nlin, $1.ncol, $1.lex.c_str());
			Symbol s2 = tS.search(globalDirection); // Get right symbol.

			auxDirLeft = " " + std::to_string(s1.direction); auxDirRight = " " + std::to_string(s2.direction);

			opasStr = $2.lex == "+" ? "add" : "sub";
			if(opasStr == "add")
				tS.addValue(s1, std::stof(s2.name));
			else
				tS.addValue(s1, -std::stof(s2.name));

			if(s1.type != s2.type) // If they are diferent (cast the first one).
			{
				$$.trad += conversion(s1);
				tS.setType(s1, s1.expType);
				$$.tipo = s1.expType;
			}

			auxType = (s2.type == static_cast<int>(Types::Integer) ? "#" : "$");
			opasStr += s1.type == static_cast<int>(Types::Integer) ? "i" : "r";


			$$.trad += $3.trad;
			auxVal = " " + std::to_string(s2.value);
			auxVal.erase(auxVal.end()-5, auxVal.end());

			if(s1.type != s2.type) // If they are diferent (cast the second one).
			{
				$$.trad += conversion(s2);
				tS.setType(s2, s2.expType);
				$$.tipo = s2.expType;
			}

			// Do the operations.
			$$.trad += "mov" + auxDirLeft + " A"                + "\t\t; " + s2.name + " ->" + auxDirLeft + "\n";
			$$.trad += opasStr + auxDirRight                    + "\t\t; " + s1.getValue() + " " + $2.lex + " " + s2.name + "\n";
			$$.trad += "mov A" + auxDirLeft                     + "\t\t;" + "\n";

			if(s1.type == s2.type) // If the are the same (cast the result).
			{
				$$.trad += conversion(s1);
				$$.tipo = s1.expType;
			}

			tS.eraseTemporals(s1.direction, s2.direction);
			globalDirection = tS.getLastUsedDirection();
		}
		| Term { $$.lex = $1.lex; }
		| opas Term {$$.trad = "OLOLO";}
		;

Term	: Term opmd Factor
		{
			if($3.tipo == static_cast<int>(Types::Logical))
				msgError(ERR_EXDER_RE, $2.nlin, $2.ncol, $2.lex.c_str());

			Symbol s1 = tS.search($1.lex); // Get left symbol.
			if(s1 == Symbol())
				msgError(ERRNODECL, $1.nlin, $1.ncol, $1.lex.c_str());
			Symbol s2 = tS.search(globalDirection); // Get right symbol.

			auxDirLeft = " " + std::to_string(s1.direction); auxDirRight = " " + std::to_string(s2.direction);

			opasStr = $2.lex == "*" ? "mul" : "div";
			if(opasStr == "mul")
				tS.multValue(s1, std::stof(s2.name));
			else
				tS.multValue(s1, 1/std::stof(s2.name));

			if(s1.type != s2.type) // If they are diferent (cast the first one).
			{
				$$.trad += conversion(s1);
				tS.setType(s1, s1.expType);
				$$.tipo = s1.expType;
			}

			auxType = (s2.type == static_cast<int>(Types::Integer) ? "#" : "$");
			opasStr += s1.type == static_cast<int>(Types::Integer) ? "i" : "r";


			$$.trad += $3.trad;
			auxVal = " " + std::to_string(s2.value);
			auxVal.erase(auxVal.end()-5, auxVal.end());

			if(s1.type != s2.type) // If they are diferent (cast the second one).
			{
				$$.trad += conversion(s2);
				tS.setType(s2, s2.expType);
				$$.tipo = s2.expType;
			}

			// Do the operations.
			$$.trad += "mov" + auxDirLeft + " A"                + "\t\t; " + s2.name + " ->" + auxDirLeft + "\n";
			$$.trad += opasStr + auxDirRight                    + "\t\t; " + s1.getValue() + " " + $2.lex + " " + s2.name + "\n";
			$$.trad += "mov A" + auxDirLeft                     + "\t\t;" + "\n";

			if(s1.type == s2.type) // If the are the same (cast the result).
			{
				$$.trad += conversion(s1);
				$$.tipo = s1.expType;
			}

			tS.eraseTemporals(s1.direction, s2.direction);
			globalDirection = tS.getLastUsedDirection();
		}
		| Factor
		{
			// $$.trad = $1.trad;
			$$.tipo = $1.tipo;
		}
		;

Factor	: Ref
		{
			Symbol s1 = tS.search($1.lex);
			if(s1 == Symbol())
				msgError(ERRNODECL, $1.nlin, $1.ncol, $1.lex.c_str());
			expectedType = s1.type;

			if($0.lex != "escribe" )
			{
				$$.lex = "_" + s1.name;
				tS.newTemporal("_" + s1.name, s1.type, s1.expType, globalDirection, s1.value);
			}
		}
		| nentero
		{
			$$.lex = $1.lex;
			$$.tipo = static_cast<int>(Types::Integer);
			if(!tS.newTemporal($1.lex, static_cast<int>(Types::Integer), expectedType, ++globalDirection))
				msgError(ERR_MAXTMP, nlin, ncol, "");

			$$.trad = "mov #" + $1.lex + " " + std::to_string(globalDirection) + "\t\t; " + $1.lex + " -> " + std::to_string(globalDirection) + "\n";
		}
		| nreal
		{
			$$.tipo = static_cast<int>(Types::Real);
			$$.lex = $1.lex;
			if(!tS.newTemporal($1.lex, static_cast<int>(Types::Real), expectedType, ++globalDirection))
				msgError(ERR_MAXTMP, nlin, ncol, "");

			$$.trad = "mov $" + $1.lex + " " + std::to_string(globalDirection) + "\t\t; " + $1.lex + " -> " + std::to_string(globalDirection) + "\n";
		}
		| pari Expr pard
		{
			$$.tipo = $2.tipo;
			$$.trad = $2.trad;
		}
		| nobool Factor
		{
			$$.lex = $2.lex == "c" ? "f" : "c";
			$$.tipo = static_cast<int>(Types::Logical);
		}
		| cierto
		{
			$$.lex = "c";
			$$.tipo = static_cast<int>(Types::Logical);
		}
		| falso
		{
			$$.lex = "f";
			$$.tipo = static_cast<int>(Types::Logical);
		}
		;

Ref		: id
		{
			$$.trad += "mov #0 " + std::to_string(++globalDirection)   + "\t\t; 0 -> " + std::to_string(globalDirection+1) + "\n";
			$$.trad += "mov " + std::to_string(globalDirection) + " A" + "\t\t; " + std::to_string(globalDirection) + " -> " + "A" + "\n";

			Symbol s1 = tS.search($1.lex);

			if(s1 == Symbol())
				msgError(ERRNODECL, $1.nlin, $1.ncol, $1.lex.c_str());
			expectedType = s1.type;

			$$.tipo = s1.type;
		}
		| Ref cori Esimple cord
		{
			// Si no es básico.
			if(!isBasic($1.tipo))
				$$.tipo = tS.tT.getBaseType($1.tipo);
			else msgError(ERRSOBRAN, $2.nlin, $2.ncol, "");

			if($2.tipo != static_cast<int>(Types::Integer))
				msgError(ERR_EXP_ENT, 0, 0, "");
				
			$$.trad = "";
			$$.ncol = $4.ncol;
		}
		;

%%

void msgError(int nerror, int nlin, int ncol, const char *s)
{
     if (nerror != ERREOF)
     {
        fprintf(stderr,"Error %d (%d:%d) ", nerror, nlin, ncol);
        switch (nerror)
		{
			case ERRLEXICO: fprintf(stderr,"caracter '%s' incorrecto\n",s);
				break;
			case ERRSINT: fprintf(stderr,"en '%s'\n",s);
				break;
			case ERRYADECL: fprintf(stderr,"variable '%s' ya declarada\n",s);
				break;
			case ERRNODECL: fprintf(stderr,"variable '%s' no declarada\n",s);
				break;
			case ERRDIM: fprintf(stderr,"la dimension debe ser mayor que cero\n");
				break;
			case ERRFALTAN: fprintf(stderr,"faltan indices\n");
				break;
			case ERRSOBRAN: fprintf(stderr,"sobran indices\n");
				break;
			case ERR_EXP_ENT: fprintf(stderr,"la expresion entre corchetes debe ser de tipo entero\n");
				break;
			case ERR_EXP_LOG: fprintf(stderr,"la expresion debe ser de tipo logico\n");
				break;
			case ERR_EXDER_LOG: fprintf(stderr,"la expresion a la derecha de '%s' debe ser de tipo logico\n",s);
				break;
			case ERR_EXDER_ENT: fprintf(stderr,"la expresion a la derecha de '%s' debe ser de tipo entero\n",s);
				break;
			case ERR_EXDER_RE: fprintf(stderr,"la expresion a la derecha de '%s' debe ser de tipo real o entero\n",s);
				break;        
			case ERR_EXIZQ_LOG: fprintf(stderr,"la expresion a la izquierda de '%s' debe ser de tipo logico\n",s);
				break;       
			case ERR_EXIZQ_RE: fprintf(stderr,"la expresion a la izquierda de '%s' debe ser de tipo real o entero\n",s);
				break;       
			case ERR_NOCABE: fprintf(stderr,"la variable '%s' ya no cabe en memoria\n",s);
				break;
			case ERR_MAXVAR: fprintf(stderr,"en la variable '%s', hay demasiadas variables declaradas\n",s);
				break;
			case ERR_MAXTIPOS: fprintf(stderr,"hay demasiados tipos definidos\n");
				break;
			case ERR_MAXTMP: fprintf(stderr,"no hay espacio para variables temporales\n");
				break;
        }
     }
     else
        fprintf(stderr,"Error al final del fichero\n");
     exit(1);
}

int yyerror(char *s)
{
    extern int findefichero;   //  variable definida en plp5.l que indica si
                               // se ha acabado el fichero
    if (findefichero)
       msgError(ERREOF,-1,-1,"");
    else
       msgError(ERRSINT,nlin,ncol-strlen(yytext),yytext);
}

std::string conversion(const Symbol& s)
{
	std::string trad = "";

	if(s.expType != s.type)
	{
		std::string lop = s.type == static_cast<int>(Types::Integer) ? "i" : "r";
		std::string rop = s.expType == static_cast<int>(Types::Integer) ? "i" : "r";

		trad += "mov " + std::to_string(s.direction) + " A\t\t;\n";
		trad += lop + "to" + rop + "\t\t\t;\n";
		trad += "mov A " + std::to_string(s.direction) + "\t\t;\n";

		tS.setType(s, s.expType);
	}

	return trad;
}

// Is a basic type?
bool isBasic(int type)
{
    return type <= static_cast<int>(Types::Logical);
}

std::ostream &operator<<(std::ostream& os, const TypeTable& tT)
{
    os << "idx\tsize\tbaseType\n";
    for(unsigned int i = 0; i < tT.table.size(); ++i)
        os << i << "\t" << tT.table[i].first << "\t" << tT.table[i].second << "\n";

    os << "------------------------\n";
    return os;
}

std::ostream &operator<<(std::ostream& os, const Symbol& s)
{
    return os << s.name << "\t: " << s.direction << "\t(" << s.type << " / " << s.expType << ")\t= " << s.value << "\n"; 
}

std::ostream &operator<<(std::ostream& os, const SymbolTable& sT)
{
    os << sT.tT;
    os << "name\t: dir\t(ty/exp)\tval\n"; 
    for(unsigned int i = 0; i < sT.symbols.size(); ++i)
        os << sT.symbols[i];

    os << "------------------------\n";
    return os;
}

std::ostream &operator<<(std::ostream& os, const Variable& v)
{
    return os << "(" << v.nlin << ", " << v.ncol << "): " << v.id << "\n"; 
}

int main(int argc,char *argv[])
{
    FILE *fent;

    if (argc==2)
    {
        fent = fopen(argv[1],"rt");
        if (fent)
        {
            yyin = fent;
            yyparse();
            fclose(fent);
        }
        else
            fprintf(stderr,"No puedo abrir el fichero\n");
    }
    else
        fprintf(stderr,"Uso: ejemplo <nombre de fichero>\n");
}