#include <iostream>

using namespace std;

#include "tcalendario.h"


int
main()
{
   //TAD numero: 4
   char fi[] = "Fecha incorrecta";
   char fc[] = "Fecha correcta";
      
   TCalendario cal1(12,12,2006, fc);
   TCalendario cal2(05,01,1900, fc);
   TCalendario cal4;
 
   cout<<cal1<<endl; // 12/12/2006 "Fecha correcta"
   cout<<cal2<<endl; // 05/01/1900 "Fecha correcta"
   cout<<cal1+79<<endl; // 01/03/2007 "Fecha correcta"
   cal4=cal2-10;
   cout<<cal2-10<<endl; // 01/01/1900 ""
   cout<<cal2<<endl; // 05/01/1900 "Fecha correcta"
   cout<<cal4<<endl; // 01/01/1900 ""
}
