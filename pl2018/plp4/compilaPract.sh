#!/bin/bash

flex plp4.l
bison -d plp4.y
g++ -o plp4 plp4.tab.c lex.yy.c -std=c++11 -Wall -Wextra -g
