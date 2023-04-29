#include <iostream>
#include "tabbcalendario.h"

using namespace std;

int main(void)
{
 
 TABBCalendario a;
 TListaCalendario l1;
 int *v;
 
 TCalendario c1(1,1,2000,(char *)"fecha");
 
 for (int i=0; i<100; i++)
 {
    c1.ModFecha(1,1,2000+i);
    a.Insertar(c1);
    l1.Insertar(c1);
 }   
 v=a.BuscarLista(l1);
 for (int i=0; i<100; i++)
    cout<<v[i]<<" ";
 cout<<endl;
 
 return 0;
}

