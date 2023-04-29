#ifndef COMUN_H
#define COMUN_H

/*----------------------- comun.h -----------------------------*/
// Pablo Requena González, PL 2018.

/* fichero con definciones comunes para los ficheros .l y .y */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>

// ------------------------- DEFINES -------------------- //
#define YYSTYPE MITIPO
#define ERRLEXICO      1
#define ERRSINT        2
#define ERREOF         3
#define ERRYADECL      4
#define ERRNODECL      5
#define ERRDIM         6
#define ERRFALTAN      7
#define ERRSOBRAN      8
#define ERR_EXP_ENT    9
#define ERR_EXP_LOG   10
#define ERR_EXDER_LOG 11
#define ERR_EXDER_ENT 12
#define ERR_EXDER_RE  13
#define ERR_EXIZQ_LOG 14
#define ERR_EXIZQ_RE  15
#define ERR_NOCABE    16
#define ERR_MAXVAR    17
#define ERR_MAXTIPOS  18
#define ERR_MAXTMP    19

void msgError(int nerror, int nlin, int ncol, const char *s);
enum class Types {Integer, Real, Logical};
bool isBasic(int);

typedef struct
{
   std::string lex;
   int nlin, ncol;
   int tipo;
   int baseDir;
   std::string trad;
} MITIPO;

struct TypeTable
{   
    //                   Size, BaseType
    std::vector<std::pair<int, int>> table;

    friend std::ostream &operator<<(std::ostream&, const TypeTable&);

    TypeTable()
    {
        table.push_back(std::make_pair(1, -1)); // Entero
        table.push_back(std::make_pair(1, -1)); // Real
        table.push_back(std::make_pair(1, -1)); // Logico
    }

    // Adds a type.
    bool add(int size, int baseType)
    {
        for(std::pair<int,int> p: table)
            if(p.first == size && p.second == baseType)
                return false;
        
        table.push_back(std::make_pair(size, baseType));
    }

    // Get type
    inline int getType() { return table.size() - 1; };
    // Get size by position.
    inline int getSize(unsigned int pos) { return pos < table.size() ? table[pos].first : -1; };
    // Get baseType by position
    inline int getBaseType(unsigned int pos) { return pos < table.size() ? table[pos].second : -1; };
    // Get real size (if is an array: size * sizeof(type), matrix: size * sizeof(array) * sizeof(type)).
    inline int getRealSize(unsigned int pos)
    {
        if(isBasic(pos))
            return 1;

        int size = table[pos].first; // Set size to array size.
        pos = table[pos].second; // Set pos to look up for a matrix.

        while(!isBasic(pos))
        {
            size *= table[pos].first; // Multiply acutal size by the new one.
            pos = table[pos].second; // Update the pointer.
        }

        return size;
    }
};

// Controles the temporalDirection
//const int globDirecStart = 16000;
const int globDirecStart = 15999;
const int maxDirection = 16383;

struct Symbol
{
    std::string name;
    int direction, type, expType;
    float value = 0.0f;

    friend std::ostream &operator<<(std::ostream&, const Symbol&);

    // Ctors.
    Symbol() { name = ""; direction = type = -1; }
    Symbol(std::string n, int d, int t) { name = n; direction = d; expType = type = t; };
    Symbol(std::string n, int d, int t, int et, float v) { name = n; direction = d; expType = et; type = t; value = v; };

    Symbol operator=(const Symbol& s)
    {
        name = s.name;
        direction = s.direction;
        type = s.type;

        return (*this);
    }

    std::string getValue()
    {
        std::string ret = std::to_string(value);
        ret.erase(ret.end()-5, ret.end());

        if(type == static_cast<int>(Types::Integer))
            ret.erase(ret.end()-2, ret.end());
        else if(type == static_cast<int>(Types::Logical))
            ret = value >= 0 ? "c" : "f";

        return ret;
    }

    bool operator==(const Symbol& s)
    {
        return name == s.name; // Just the name tells if it's the same
    }
};

struct SymbolTable
{
    // TypeTable MUST be global.
    TypeTable tT;

    std::vector<Symbol> symbols;
    int lastUsedDirection = 0;

    friend std::ostream &operator<<(std::ostream&, const SymbolTable&);

    bool add(Symbol sy)
    {
        for(Symbol s: symbols)
            if(s == sy) return false;

        sy.direction = lastUsedDirection; // Get last used direction.
        lastUsedDirection += tT.getRealSize(sy.type); // Add real size (arrays and matrixes).
        symbols.push_back(sy);
        return true;
    }

    inline bool setName(const Symbol& s, float val)
    {
        std::string auxVal = std::to_string(val);
        auxVal.erase(auxVal.end()-5, auxVal.end());

        for(int i = 0; i < symbols.size(); ++i)
            if(symbols[i] == s)
            {
                if(s.type == static_cast<int>(Types::Integer))
                    auxVal.erase(auxVal.end()-2, auxVal.end());

                symbols[i].name = auxVal;
            }
    }

    inline bool setValue(const Symbol& s, float val)
    {
        for(int i = 0; i < symbols.size(); ++i)
            if(symbols[i] == s)
                symbols[i].value = val;
    }

    inline bool setType(const Symbol& s, int t)
    {
        for(int i = 0; i < symbols.size(); ++i)
            if(symbols[i] == s)
                symbols[i].type = t;
    }

    inline bool addValue(const Symbol& s, float val)
    {
        for(int i = 0; i < symbols.size(); ++i)
            if(symbols[i] == s)
                symbols[i].value += val;
    }

    inline bool multValue(const Symbol& s, float val)
    {
        for(int i = 0; i < symbols.size(); ++i)
            if(symbols[i] == s)
                symbols[i].value *= val;
    }

    Symbol search(std::string name)
    {
        for(Symbol s: symbols)
            if(s.name == name) return s;

        return Symbol();
    }

    Symbol search(int direction)
    {
        for(Symbol s: symbols)
            if(s.direction == direction) return s;

        return Symbol();
    }

    inline bool newTemporal(std::string n, int t, int et, int dir)
    {
        if(dir > maxDirection)
            return false;

        symbols.push_back(Symbol(n, dir, t, et, std::stof(n)));

        return true;
    }

    inline bool newTemporal(std::string n, int t, int et, int dir, float v)
    {
        if(dir > maxDirection)
            return false;

        symbols.push_back(Symbol(n, dir, t, et, v));

        return true;
    }

    inline void eraseTemporals(int initDir, int endDir)
    {
        bool copy = true;
        std::vector<Symbol> auxSym;

        for(int i = 0; i < symbols.size(); ++i)
        {
            if(copy) auxSym.push_back(symbols[i]);

            if(symbols[i].direction == initDir) copy = false;
            if(symbols[i].direction == endDir) copy = true;
        }

        symbols.clear();
        symbols = auxSym;
    }

    void eraseNotVariables(int numVar)
    {
        symbols.resize(numVar);
    }

    Symbol getFirstTemporal(int numVar)
    {
        return symbols[numVar];
    }

    inline int getLastUsedDirection()
    {
        return symbols[symbols.size() - 1].direction;
    }
};

#endif