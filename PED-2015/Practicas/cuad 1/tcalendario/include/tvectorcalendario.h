/*
 * tvectorcalendario.h
 *
 *  Created on: 12/03/2015
 *      Author: prg26
 */

#ifndef TVECTORCALENDARIO_H_
#define TVECTORCALENDARIO_H_

#include "tcalendario.h"

class TVectorCalendario {
private:
	TCalendario *c;
	int tamano;
	TCalendario error;
	void copiaVector(const TVectorCalendario &);

public:
	// Constructor por defecto
	TVectorCalendario();
	// Constructor a partir de un tamaño
	TVectorCalendario(const int);
	// Constructor de copia
	TVectorCalendario(const TVectorCalendario &);
	// Destructor
	~TVectorCalendario();
	// Sobrecarga del operador asignación
	TVectorCalendario & operator=(const TVectorCalendario &);

	// Sobrecarga del operador igualdad
	bool operator==(const TVectorCalendario &);
	// Sobrecarga del operador desigualdad
	bool operator!=(const TVectorCalendario &);
	// Sobrecarga del operador corchete (parte IZQUIERDA)
	TCalendario & operator[](int);
	// Sobrecarga del operador corchete (parte DERECHA)
	TCalendario operator[](int) const;
	// Tamaño del vector (posiciones TOTALES)
	int Tamano() const { return tamano;}
	// Cantidad de posiciones OCUPADAS (no vacías) en el vector
	int Ocupadas() const;
	// Devuelve true si existe el calendario en el vector
	bool ExisteCal(const TCalendario &);
	// Mostrar por pantalla mensajes de TCalendario en el vector, de fecha IGUAL O POSTERIOR a la pasada
	void MostrarMensajes(const int, const int, const int);
	// REDIMENSIONAR el vectorde TCalendario
	bool Redimensionar(int);

	// Sobrecarga del operador salida
	friend ostream & operator<<(ostream &, const TVectorCalendario &);
};

#endif /* TVECTORCALENDARIO_H_ */
