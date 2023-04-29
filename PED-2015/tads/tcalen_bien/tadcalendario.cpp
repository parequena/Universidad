/*
 * main.cpp
 *
 *  Created on: 16/2/2015
 *      Author: Diego
 */

#define _TADCALENDARIO_
#ifdef _TADCALENDARIO_
#include <iostream>
#include "../include/tcalendario.h"
#include <string.h>
using namespace std;

int main(void){
	cout<<"Pruebas unitarias: [Expected - Obtained]"<<endl<<endl;
	/* PRUEBA 1*/
	cout<<endl<<"Constructor por defecto:"<<endl;
	TCalendario prueba1;
	cout<<"01/01/1900 \"\" - "<<prueba1<<endl;

	/* PRUEBA 2 */
	cout<<endl<<"Constructor con parametros:"<<endl;
	TCalendario prueba2(13,3,1986,"Fecha Random");
	cout<<"13/03/1986 \"Fecha Random\" - "<<prueba2<<endl;
	prueba2 = TCalendario(1,2,1899,"Algo");
	cout<<"01/01/1900 \"\" - "<<prueba2<<endl;
	prueba2 =TCalendario(13,3,1986,"Fecha Random");
	cout<<"13/03/1986 \"Fecha Random\" - "<<prueba2<<endl;

	/* PRUEBA 3 */
	cout<<endl<<"Constructor de copia:"<<endl;
	TCalendario prueba3(prueba2);
	cout<<"13/03/1986 \"Fecha Random\" - "<<prueba3<<endl;
	prueba3 = TCalendario(1,2,1893,"Algo");
	cout<<"01/01/1900 \"\" - "<<prueba3<<endl;
	prueba3 = TCalendario(prueba2);
	TCalendario prueba4;
	prueba4.ModFecha(13,03,1986);
	TCalendario prueba5 = TCalendario(prueba4);
	cout<<"13/03/1986 \"\" - "<<prueba5<<endl;

	/* PRUEBA 4 */
	cout<<endl<<"operator= :"<<endl;
	prueba1 = prueba2;
	cout<<"13/03/1986 \"Fecha Random\" - "<<prueba1<<endl;

	/* PRUEBA 5 */
	cout<<endl<<"operator+ :"<<endl;
	prueba2 = prueba1+1;
	cout<<"14/03/1986 \"Fecha Random\" - "<<prueba2<<endl;

	/* PRUEBA 6 */
	cout<<endl<<"operator- :"<<endl;
	prueba2 = prueba1-2;
	cout<<"11/03/1986 \"Fecha Random\" - "<<prueba2<<endl;

	/* PRUEBA 7 */
	cout<<endl<<"operator++(int) //post:"<<endl;
	prueba2 = prueba1++;
	cout<<"13/03/1986 \"Fecha Random\" - "<<prueba2<<endl;
	cout<<"14/03/1986 \"Fecha Random\" - "<<prueba1<<endl;

	/* PRUEBA 8 */
	cout<<endl<<"operator++() //pre:"<<endl;
	prueba2 = ++prueba1;
	cout<<"15/03/1986 \"Fecha Random\" - "<<prueba2<<endl;
	cout<<"15/03/1986 \"Fecha Random\" - "<<prueba1<<endl;

	/* PRUEBA 9 */
	cout<<endl<<"operator--(int) //post:"<<endl;
	prueba2 = prueba1--;
	cout<<"15/03/1986 \"Fecha Random\" - "<<prueba2<<endl;
	cout<<"14/03/1986 \"Fecha Random\" - "<<prueba1<<endl;

	/* PRUEBA 10 */
	cout<<endl<<"operator--() //pre:"<<endl;
	prueba2 = --prueba1;
	cout<<"13/03/1986 \"Fecha Random\" - "<<prueba2<<endl;
	cout<<"13/03/1986 \"Fecha Random\" - "<<prueba1<<endl;

	/* PRUEBA 11 */
	cout<<endl<<"ModFecha(int,int,int):"<<endl;
	prueba2.ModFecha(27,2,1987);
	cout<<"27/02/1987 \"Fecha Random\" - "<<prueba2<<endl;

	/* PRUEBA 12 */
	cout<<endl<<"ModMensaje(char*):"<<endl;
	prueba2.ModMensaje("Mensaje Cambiado");
	cout<<"27/02/1987 \"Mensaje Cambiado\" - "<<prueba2<<endl;
	prueba4 = TCalendario();
	prueba4.ModMensaje("Probando a quitar null");
	cout<<"01/01/1900 \"Probando a quitar null\" - "<<prueba4<<endl;
	prueba4.ModMensaje(NULL);
		cout<<"01/01/1900 \"\" - "<<prueba4<<endl;

	/* PRUEBA 13 */
	cout<<endl<<"operator== :"<<endl;
	prueba1 = prueba2;
	cout<<"1 - "<<(prueba2==prueba1)<<endl;
	prueba1 = prueba3;
	cout<<"0 - "<<(prueba2==prueba1)<<endl;
	prueba4 = TCalendario();
	cout<<"0 - "<<(prueba4==prueba1)<<endl;
	cout<<"0 - "<<(prueba2==prueba4)<<endl;
	cout<<"1 - "<<(prueba4==prueba4)<<endl;

	/* PRUEBA 14 */
	cout<<endl<<"operator!= :"<<endl;
	prueba1 = prueba2;
	cout<<"0 - "<<(prueba2!=prueba1)<<endl;
	prueba1 = prueba3;
	cout<<"1 - "<<(prueba2!=prueba1)<<endl;

	/* PRUEBA 15 */
	cout<<endl<<"operator> :"<<endl;
	prueba1 = TCalendario(1,1,2000,"Fecha1");
	prueba2 = TCalendario(3,4,2003,"Fecha2");
	cout<<"1 - "<<(prueba2>prueba1)<<endl;
	cout<<"0 - "<<(prueba1>prueba2)<<endl;
	cout<<"0 - "<<(prueba1>prueba1)<<endl;
	prueba4 = TCalendario();
	prueba5 = TCalendario();
	prueba5.ModMensaje("");
	cout<<"1 - "<<(prueba2>prueba4)<<endl;
	cout<<"0 - "<<(prueba4>prueba5)<<endl;
	cout<<"1 - "<<(prueba5>prueba4)<<endl;
	cout<<"0 - "<<(prueba4>prueba4)<<endl;
	prueba4.ModFecha(3,1,2000);
	prueba5.ModFecha(2,2,1901);
	cout<<"1 - "<<(prueba4>prueba5)<<endl;
	cout<<"0 - "<<(prueba5>prueba4)<<endl;
	prueba4.ModFecha(3,3,2000);
	prueba5.ModFecha(2,2,2000);
	cout<<"1 - "<<(prueba4>prueba5)<<endl;
	cout<<"0 - "<<(prueba5>prueba4)<<endl;
	prueba4.ModFecha(3,3,2000);
	prueba5.ModFecha(2,3,2000);
	cout<<"1 - "<<(prueba4>prueba5)<<endl;
	cout<<"0 - "<<(prueba5>prueba4)<<endl;
	prueba4.ModFecha(3,3,2000);
	prueba5.ModFecha(3,3,2000);
	prueba4.ModMensaje("holz");
	prueba5.ModMensaje("holo");
	cout<<"1 - "<<(prueba4>prueba5)<<endl;
	cout<<"0 - "<<(prueba5>prueba4)<<endl;

	/* PRUEBA 16 */
	cout<<endl<<"operator< :"<<endl;
	prueba1 = TCalendario(1,1,2000,"Fecha1");
	prueba2 = TCalendario(3,4,2003,"Fecha2");
	cout<<"0 - "<<(prueba2<prueba1)<<endl;
	cout<<"1 - "<<(prueba1<prueba2)<<endl;
	cout<<"0 - "<<(prueba1<prueba1)<<endl;
	prueba4 = TCalendario();
	prueba5 = TCalendario();
	prueba5.ModMensaje("");
	cout<<"0 - "<<(prueba2<prueba4)<<endl;
	cout<<"1 - "<<(prueba4<prueba5)<<endl;
	cout<<"0 - "<<(prueba5<prueba4)<<endl;
	cout<<"0 - "<<(prueba4<prueba4)<<endl;
	prueba4.ModFecha(3,1,2000);
	prueba5.ModFecha(2,2,1901);
	cout<<"0 - "<<(prueba4<prueba5)<<endl;
	cout<<"1 - "<<(prueba5<prueba4)<<endl;
	prueba4.ModFecha(3,3,2000);
	prueba5.ModFecha(2,2,2000);
	cout<<"0 - "<<(prueba4<prueba5)<<endl;
	cout<<"1 - "<<(prueba5<prueba4)<<endl;
	prueba4.ModFecha(3,3,2000);
	prueba5.ModFecha(2,3,2000);
	cout<<"0 - "<<(prueba4<prueba5)<<endl;
	cout<<"1 - "<<(prueba5<prueba4)<<endl;
	prueba4.ModFecha(3,3,2000);
	prueba5.ModFecha(3,3,2000);
	prueba4.ModMensaje("holz");
	prueba5.ModMensaje("holo");
	cout<<"0 - "<<(prueba4<prueba5)<<endl;
	cout<<"1 - "<<(prueba5<prueba4)<<endl;

	/* PRUEBA 17 */
	cout<<endl<<"EsVacio():"<<endl;
	prueba1 = TCalendario();
	cout<<"1 - "<<(prueba1.EsVacio())<<endl;
	cout<<"0 - "<<(prueba2.EsVacio())<<endl;

	/* PRUEBA 18 */
	cout<<endl<<"Dia():"<<endl;
	prueba1 = TCalendario(1,2,1903,"Msg");
	cout<<"1 - "<<(prueba1.Dia())<<endl;

	/* PRUEBA 19 */
	cout<<endl<<"Mes():"<<endl;
	cout<<"2 - "<<(prueba1.Mes())<<endl;

	/* PRUEBA 20 */
	cout<<endl<<"Anyo():"<<endl;
	cout<<"1903 - "<<(prueba1.Anyo())<<endl;

	/* PRUEBA 21 */
	cout<<endl<<"Mensaje():"<<endl;
	cout<<"Msg - "<<(prueba1.Mensaje())<<endl;

	/* PRUEBA 22 */
	cout<<endl<<"Fechas incomodas:"<<endl;
	prueba4 = TCalendario(31,3,1986,"Una fecha");
	cout<<"31/03/1986 \"Una fecha\" - "<<prueba4<<endl;
	prueba4 = prueba4+2;
	cout<<"02/04/1986 \"Una fecha\" - "<<prueba4<<endl;
	prueba4 = TCalendario(1,4,1986,"Otra fecha");
	cout<<"01/04/1986 \"Otra fecha\" - "<<prueba4<<endl;
	prueba4 = prueba4-2;
	cout<<"30/03/1986 \"Otra fecha\" - "<<prueba4<<endl;
	prueba4 = TCalendario(31,12,1986,"Otra mas");
	cout<<"31/12/1986 \"Otra mas\" - "<<prueba4<<endl;
	prueba4 = prueba4+2;
	cout<<"02/01/1987 \"Otra mas\" - "<<prueba4<<endl;
	prueba4 = TCalendario(1,1,1986,"Y otra");
	cout<<"01/01/1986 \"Y otra\" - "<<prueba4<<endl;
	prueba4 = prueba4-2;
	cout<<"30/12/1985 \"Y otra\" - "<<prueba4<<endl;
	prueba4 = TCalendario(2,1,1900,"No debe restar");
	cout<<"02/01/1900 \"No debe restar\" - "<<prueba4<<endl;
	prueba4 = prueba4-2;
	cout<<"01/01/1900 \"\" - "<<prueba4<<endl;

	prueba4 = TCalendario(2,1,1900,"preincremento");
	cout<<"02/01/1900 \"preincremento\" - "<<prueba4<<endl;
	prueba3 = ++prueba4;
	cout<<"03/01/1900 \"preincremento\" - "<<prueba3<<endl;
	cout<<"03/01/1900 \"preincremento\" - "<<prueba4<<endl<<endl;
	prueba4 = TCalendario(2,1,1900,"postincremento");
	cout<<"02/01/1900 \"postincremento\" - "<<prueba4<<endl;
	prueba3 = prueba4++;
	cout<<"02/01/1900 \"postincremento\" - "<<prueba3<<endl;
	cout<<"03/01/1900 \"postincremento\" - "<<prueba4<<endl<<endl;
	prueba4 = TCalendario(2,1,1900,"predecremento");
	cout<<"02/01/1900 \"predecremento\" - "<<prueba4<<endl;
	prueba3 = --prueba4;
	cout<<"01/01/1900 \"predecremento\" - "<<prueba3<<endl;
	cout<<"01/01/1900 \"predecremento\" - "<<prueba4<<endl<<endl;
	prueba4 = TCalendario(2,1,1900,"postdecremento");
	cout<<"02/01/1900 \"postdecremento\" - "<<prueba4<<endl;
	prueba3 = prueba4--;
	cout<<"02/01/1900 \"postdecremento\" - "<<prueba3<<endl;
	cout<<"01/01/1900 \"postdecremento\" - "<<prueba4<<endl<<endl;

	prueba4--;
	cout<<"01/01/1900 \"\" - "<<prueba4<<endl<<endl;

	return 0;
}
#endif
