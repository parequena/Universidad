/*
 * tlistacalendario.h
 *
 *  Created on: 17/03/2015
 *      Author: prg26
 */

#ifndef TLISTACALENDARIO_H_
#define TLISTACALENDARIO_H_

#include "tcalendario.h"

// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO

class TListaCalendario;

class TNodoCalendario
{
	friend class TListaPos;
	friend class TListaCalendario;
	friend ostream & operator<<(ostream &, const TListaCalendario &);

private:
	TNodoCalendario *siguiente;
	TCalendario c;
public:
	// Constructor por defecto
	TNodoCalendario () { TCalendario c(); siguiente = NULL; }
	// Constructor de copia
	TNodoCalendario (const TNodoCalendario &);
	// Destructor
	~ TNodoCalendario ();
	// Sobrecarga del operador asignación
	TNodoCalendario & operator=(const TNodoCalendario &);
};

// ---------------------------------------------------------------------------------------------- LISTAPOS
// ---------------------------------------------------------------------------------------------- LISTAPOS
// ---------------------------------------------------------------------------------------------- LISTAPOS
// ---------------------------------------------------------------------------------------------- LISTAPOS
// ---------------------------------------------------------------------------------------------- LISTAPOS

class TListaPos
{
	friend class TListaCalendario;
	friend ostream & operator<<(ostream &, const TListaCalendario &);
private:
	// Para implementar la POSICIÓN a NODO de la LISTA de TCalendario
	TNodoCalendario *pos;
public:
	// Constructor por defecto
	TListaPos() { pos=NULL; }
	// Constructor de copia
	TListaPos(const TListaPos &);
	// Destructor
	~TListaPos();
	// Sobrecarga del operador asignación
	TListaPos & operator=(const TListaPos &);
	// Sobrecarga del operador igualdad
	bool operator==(const TListaPos &) const;
	// Sobrecarga del operador desigualdad
	bool operator!=(const TListaPos &) const;
	// Devuelve la posición siguiente
	TListaPos Siguiente() const;
	// Posición vacía
	bool EsVacia() const;

};

// ---------------------------------------------------------------------------------------------- LISTACALENDARIO
// ---------------------------------------------------------------------------------------------- LISTACALENDARIO
// ---------------------------------------------------------------------------------------------- LISTACALENDARIO
// ---------------------------------------------------------------------------------------------- LISTACALENDARIO
// ---------------------------------------------------------------------------------------------- LISTACALENDARIO

class TListaCalendario {
	friend ostream & operator<<(ostream &, const TListaCalendario &);
private:
	// Primer item de la lista
	TNodoCalendario *primero;
	TListaCalendario SubRangos(int n1, int n2);
	void copiaLista(const TListaCalendario &);

public:
	// Constructor por defecto
	TListaCalendario() { primero=NULL; }
	// Constructor de copia
	TListaCalendario(const TListaCalendario &);
	//Destructor
	~TListaCalendario();
	// Sobrecarga del operador asignación
	TListaCalendario & operator=(const TListaCalendario &);
	// Sobrecarga del operador igualdad
	bool operator==(const TListaCalendario &) const;
	//Sobrecarga del operador suma
	TListaCalendario operator+ (const TListaCalendario &) const;
	//Sobrecarga del operador resta
	TListaCalendario operator- (const TListaCalendario &) const;
	// Inserta el elemento en la posición que le corresponda dentro de la lista
	bool Insertar(const TCalendario &);
	// Busca y borra el elemento
	bool Borrar(const TCalendario &);
	// Borra el elemento que ocupa la posición indicada
	bool Borrar (const TListaPos &);
	//Borra todos los Calendarios con fecha ANTERIOR a la pasada.
	bool Borrar(int,int,int);
	// Devuelve true si la lista está vacía, false en caso contrario
	bool EsVacia() const;
	// Obtiene el elemento que ocupa la posición indicada
	TCalendario Obtener(const TListaPos &) const;
	// Devuelve true si el Calendario está en la lista.
	bool Buscar(const TCalendario &) const;
	// Devuelve la longitud de la lista
	int Longitud() const;
	// Devuelve la primera posición en la lista
	TListaPos Primera() const;
	// Devuelve la última posición en la lista
	TListaPos Ultima() const;
	// Suma de dos sublistas en una nueva lista
	TListaCalendario SumarSubl (const int, const int, const TListaCalendario &, const int, const int);
	// Extraer un rango de nodos de la lista
	TListaCalendario ExtraerRango (int, int);

};

#endif /* TLISTACALENDARIO_H_ */
