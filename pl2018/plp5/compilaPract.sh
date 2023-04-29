#!/bin/bash

rm plp5.tab.c lex.yy.c plp5.tab.h

flex plp5.l
bison -d plp5.y
g++ -o plp5 plp5.tab.c lex.yy.c -std=c++11
