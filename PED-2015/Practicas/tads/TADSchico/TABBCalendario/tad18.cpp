#include <iostream>
#include "tabbcalendario.h"

using namespace std;

int main(void)
{
 
 TABBCalendario a;
 TListaCalendario l1;
 int *v;
 
 v=a.BuscarLista(l1);

 if (!v)
   cout<<"correcto"<<endl;
 else
   cout<<"incorrecto"<<endl;
  
 return 0;
}

