/*
 * tadvectorcalen.cpp
 *
 *  Created on: 24/2/2015
 *      Author: Diego
 */

//#define _TADVECTOR_
#ifdef _TADVECTOR_
#include <iostream>
#include "../include/tcalendario.h"
#include "../include/tvectorcalendario.h"
#include <string.h>
using namespace std;

int main(void){
	TCalendario c1 = TCalendario(1,2,1901,"C1");
	TCalendario c2 = TCalendario(2,2,1902,"C2");
	TCalendario c3 = TCalendario(3,2,1903,"C3");
	TCalendario c4 = TCalendario(4,2,1904,"C4");
	cout<<"Pruebas unitarias: [Expected - Obtained]"<<endl<<endl;

	/* CONSTRUCTOR VACIO */
	cout<<endl<<"Constructor vacio :"<<endl;
	TVectorCalendario v0;
	cout<<"[] - "<<v0<<endl;

	/* CONSTRUCTOR (INT) */
	cout<<endl<<"Constructor (int) :"<<endl;
	TVectorCalendario v1(3);
	cout<<"[(1) 01/01/1900 \"\", (2) 01/01/1900 \"\", (3) 01/01/1900 \"\"] - "<<v1<<endl;
	TVectorCalendario error(-1);
	cout<<"[] - "<<error<<endl;

	/* CONSTRUCTOR COPIA */
	cout<<endl<<"Constructor copia :"<<endl;
	TVectorCalendario v3(v1);
	cout<<"[(1) 01/01/1900 \"\", (2) 01/01/1900 \"\", (3) 01/01/1900 \"\"] - "<<v3<<endl;
	TVectorCalendario v4 = TVectorCalendario(v0);
	cout<<"[] - "<<v4<<endl;
	/* SOBRECARGA = */
	cout<<endl<<"Constructor copia :\n[Se sobreentiende que funciona, se usa en la segunda del constructor de copia]"<<endl;
	/* SOBRECARGA == */
	cout<<endl<<"Sobrecarga == :"<<endl;
	cout<<"1 - "<<(v0==v4)<<endl;
	cout<<"0 - "<<(v0==v1)<<endl;
	/* SOBRECARGA != */
	cout<<endl<<"Sobrecarga != :"<<endl;
	cout<<"1 - "<<(v0!=v1)<<endl;
	cout<<"0 - "<<(v0!=v4)<<endl;
	/* SOBRECARGA [](INT) */
	cout<<endl<<"Sobrecarga [](int) //Modificadora:"<<endl;
	TVectorCalendario v2 = TVectorCalendario(4);
	v2[1]=c1;v2[2]=c2;v2[3]=c3;v2[4]=c4;
	cout<<"[(1) 01/02/1901 \"C1\", (2) 02/02/1902 \"C2\", (3) 03/02/1903 \"C3\", (4) 04/02/1904 \"C4\"] - "<<v2<<endl;
	cout<<"01/01/1900 \"\" - "<<v2[5]<<endl;
	cout<<"01/01/1900 \"\" - "<<v2[0]<<endl;

	/* SOBRECARGA [](INT) CONST */
	cout<<endl<<"Sobrecarga [](int) const //Consultora:"<<endl;
	TCalendario c5 = v2[1];
	TCalendario c6 = v2[0];
	TCalendario c7 = v2[5];
	cout<<"01/02/1901 \"C1\" - "<<c5<<endl;
	cout<<"01/01/1900 \"\" - "<<c6<<endl;
	cout<<"01/01/1900 \"\" - "<<c7<<endl;

	/* TAMANO */
	cout<<endl<<"Tamano() :"<<endl<<"4 - "<<v2.Tamano()<<endl;

	/* OCUPADAS */
	cout<<endl<<"Ocupadas() :"<<endl;
	cout<<"4 - "<<v2.Ocupadas()<<endl;
	v2[1]=TCalendario();
	cout<<"3 - "<<v2.Ocupadas()<<endl;
	v2[1]=c5;

	/* EXISTECAL */
	cout<<endl<<"ExisteCal() :"<<endl;
	cout<<"1 - "<<v2.ExisteCal(c5)<<endl;
	cout<<"0 - "<<v2.ExisteCal(TCalendario(3,2,1985,"Algo"))<<endl;

	/* MOSTRARMENSAJES */
	cout<<endl<<"MostrarMensajes() :"<<endl;
	cout<<"[03/02/1903 \"C3\", 04/02/1904 \"C4\"] - ";v2.MostrarMensajes(3,2,1903);cout<<endl;
	cout<<"[] - ";v2.MostrarMensajes(3,2,1985);cout<<endl;
	/* REDIMENSIONAR */
	cout<<endl<<"Redimensionar() :"<<endl;
	v2.Redimensionar(2);
	cout<<"[(1) 01/02/1901 \"C1\", (2) 02/02/1902 \"C2\"] - "<<v2<<endl;
	v2.Redimensionar(3);
	cout<<"[(1) 01/02/1901 \"C1\", (2) 02/02/1902 \"C2\", (3) 01/01/1900 \"\"] - "<<v2<<endl;

	return 0;
}
#endif
