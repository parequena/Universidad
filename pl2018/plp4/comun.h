/*----------------------- comun.h -----------------------------*/
// Pablo Requena González, PL 2018.

/* fichero con definciones comunes para los ficheros .l y .y */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <string>
#include <iostream>
#include <vector>

typedef struct {
   char *lexema;
   int nlin,ncol;
   int tipo;
   std::string cod;
   std::string trad;
} MITIPO;

struct Symbol
{
    std::string name; // Token.lexema;
    int type; // 1 = Entero. 2 = Real;

    Symbol()
    {
        name = "";
        type = 0;
        std::cout << "N: " << name << "\tT: " << type << "\n";
    }

    Symbol(std::string n, int t)
    {
        name = n;
        type = t;
    }

    Symbol(const Symbol& s)
    {
        name = s.name;
        type = s.type;
    }

    Symbol& operator=(const Symbol& s)
    {
        if(this != &s)
        {
            (*this).~Symbol();
            name = s.name;
            type = s.type;
        }

        return *this;
    }

    ~Symbol()
    {
        name = "";
        type = 0;
    }

    bool operator==(const Symbol& r)
    {
        return name == r.name && type == r.type;
    }

    bool operator!=(const Symbol& r)
    {
        return name != r.name && type != r.type;
    }

    // friend std::ostream& operator<<(std::ostream& os, const Symbol& s);
};

struct SymbolTable
{
    SymbolTable* parent; // Pareten SymbolTable.
    std::vector<Symbol*> symbols; // Array symbols.
    std::vector<std::string> trans; // Symbol translations.
    std::string prefix;

    SymbolTable()
    {
        std::cout << "symboltable\n";
        parent = nullptr;
        prefix = "";
        std::cout << "Ssize: " << symbols.size() << "\n";
        std::cout << "Tsize: " << trans.size() << "\n";
    }

    ~SymbolTable()
    {
        if(parent != nullptr)
            delete[] parent;
    }

    SymbolTable(SymbolTable& s)
    {
        parent = new SymbolTable();
        parent = &s;
        prefix = s.prefix + "_";
    }

    bool add(Symbol s)
    {
        for(Symbol* a: symbols) // Search the simbol.
            if(a->name == s.name)
                return false;

        symbols.push_back(&s); // If doesn't exists, pushit in.
        trans.push_back(prefix + s.name); // Also the translation.
        return true;
    }

    Symbol search(std::string name)
    {
        for(Symbol* s: symbols)
            if(s->name == name)
                return *s;

        if(parent != nullptr)
            return parent->search(name);

        return Symbol();
    }

    std::string getTrans(std::string name)
    {
        for(unsigned i = 0; i < symbols.size(); ++i)
            if(symbols[i]->name == name)
                return trans[i];

        if(parent != nullptr)
            return parent->getTrans(name);

        return NULL;
    }

};

#define YYSTYPE MITIPO


#define ERRLEXICO    1
#define ERRSINT      2
#define ERREOF       3
#define ERRLEXEOF    4

void msgError(int nerror,int nlin,int ncol, const char *s);
void printVariables();
