/*
 * TCalendario.h
 *
 *  Created on: 03/02/2015
 *      Author: pablo
 */

#ifndef TCALENDARIO_H_
#define TCALENDARIO_H_

#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

class TCalendario {
private:
	int dia, mes, anyo;
	char* mensaje;
	bool comprobarBisiesto(int year);
	bool comprobarFecha(int day, int month, int year);
	bool comprobarAgno(int agno);
	TCalendario& sumaDias(int numero);
	TCalendario& restaDias(int numero);

public:
	TCalendario(){dia=1; mes=1; anyo=1900; mensaje=NULL;}; //Done
	TCalendario(int dia, int mes, int anyo, char* mens); //Done
	TCalendario(const TCalendario &calendario); //Done
	virtual ~TCalendario(); //Done
	TCalendario &operator=( TCalendario &calend);

	// Sobrecarga del operador SUMA de fecha + un número de dias;
	TCalendario operator+(int dias);
	// Sobrecarga del operador RESTA de fecha - un número de dias;
	TCalendario operator-(int dias);
	// Modifica la fecha incrementandola en un dia (con postincremento);
	TCalendario operator++(int numero);
	// Modifica la fecha incrementandola en un dia (con preincremento);
	TCalendario &operator++();
	// Modifica la fecha decrementándola en un dia (con postdecremento);
	TCalendario operator--(int numero);
	// Modifica la fecha decrementándola en un día (con predecremento);
	TCalendario &operator--();

	// Modifica la fecha
	bool ModFecha (int dia, int mes, int anyo);
	// Modifica el mensaje
	bool ModMensaje(char * mensaje);
	// Sobrecarga del operador igualdad;
	bool operator==(const TCalendario &calendario);
	// Sobrecarga del operador desigualdad;
	bool operator!=(const TCalendario &calendario);
	// Sobrecarga del operador >; (ver ACLARACIÓN sobre ORDENACIÓN)
	bool operator>(const TCalendario &calendario);
	// Sobrecarga del operador <; (ver ACLARACIÓN sobre ORDENACIÓN)
	bool operator<(const TCalendario &calendario);

	//TCalendario vacío
	bool EsVacio(); // Done
	// Devuelve el día del calendario;
	int Dia(){return dia;} //Done
	// Devuelve el mes del calendario;
	int Mes(){return mes;} //Done
	// Devuelve el año del calendario;
	int Anyo(){return anyo;} //Done
	// Devuelve el mensaje del calendario;
	char *Mensaje(){return mensaje;}

	friend ostream &operator<<(ostream &os, const TCalendario &calendario);
};

#endif /* TCALENDARIO_H_ */
