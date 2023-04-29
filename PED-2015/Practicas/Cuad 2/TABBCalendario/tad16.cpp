#include <iostream> 
#include "tabbcalendario.h"

using namespace std;

int
main(void)
{
  TABBCalendario a;
  TListaCalendario l1;
  int *v;
  int i;

  TCalendario c1(1,1,2006,(char *)"fecha"), c2(1,1,2003,(char *)"fecha"), c3(1,1,2001,(char *)"fecha");
  TCalendario c4(1,1,2005,(char *)"fecha"), c5(1,1,2008,(char *)"fecha"), c6(1,1,2007,(char *)"fecha"), c7(1,1,2010,(char *)"fecha"),c8(1,1,2009,(char *)"fecha");
    
  a.Insertar(c1);
  a.Insertar(c2);
  a.Insertar(c3);
  a.Insertar(c4);
  a.Insertar(c5);
  a.Insertar(c6);
  a.Insertar(c7);
  
  l1.Insertar(c4);
  l1.Insertar(c1);
  l1.Insertar(c8);
  l1.Insertar(c7);
  
  v=a.BuscarLista(l1);

  for (i=0;i<4;i++) cout<<v[i]<<" ";
  cout <<endl;

  
  return 0;
}
