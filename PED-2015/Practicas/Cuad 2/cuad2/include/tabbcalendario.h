/*
 * TABBCalendario.h
 *
 *  Created on: 27/04/2015
 *      Author: pablo
 */

#ifndef TABBCALENDARIO_H_
#define TABBCALENDARIO_H_

#include "tcalendario.h"
#include "tvectorcalendario.h"

// ---------------------------------------------------------------------------------------------- ABBCALENDARIO
// ---------------------------------------------------------------------------------------------- ABBCALENDARIO
// ---------------------------------------------------------------------------------------------- ABBCALENDARIO
// ---------------------------------------------------------------------------------------------- ABBCALENDARIO
class TNodoABB;

class TABBCalendario
{
	// Sobrecarga del operador salida
	friend ostream & operator<< (ostream &, const TABBCalendario &);
private:
	// Puntero al nodo raíz
	TNodoABB *raiz;
	// Devuelve el recorrido en inorden
	void InordenAux(TVectorCalendario &, int &) const;
	// Devuelve el recorrido en preorden
	void PreordenAux(TVectorCalendario &, int &) const;
	// Devuelve el recorrido en postorden
	void PostordenAux(TVectorCalendario &, int &) const;

public:
	// Constructor por defecto
	TABBCalendario ();
	// Constructor de copia
	TABBCalendario (const TABBCalendario &);
	// Destructor
	~TABBCalendario ();
	// Sobrecarga del operador asignación
	TABBCalendario & operator=(const TABBCalendario &);
	// Sobrecarga del operador igualdad
	bool operator==(const TABBCalendario &) const;
	// Devuelve TRUE si el árbol está vacío, FALSE en caso contrario
	bool EsVacio() const;
	// Inserta el elemento en el árbol
	bool Insertar(const TCalendario &);
	// Borra el elemento en el árbol
	bool Borrar(const TCalendario &);
	// Devuelve TRUE si el elemento está en el árbol, FALSE en caso contrario
	bool Buscar(const TCalendario &) const;
	// Devuelve el elemento en la raíz del árbol
	TCalendario Raiz() const;
	// Devuelve la altura del árbol (la altura de un árbol vacío es 0)
	int Altura() const ;
	// Devuelve el número de nodos del árbol (un árbol vacío posee 0 nodos)
	int Nodos() const;
	// Devuelve el número de nodos hoja en el árbol (la raíz puede ser nodo hoja)
	int NodosHoja() const;
	// Devuelve el recorrido en inorden sobre un vector
	TVectorCalendario Inorden() const;
	// Devuelve el recorrido en preorden sobre un vector
	TVectorCalendario Preorden() const;
	// Devuelve el recorrido en postorden sobre un vector
	TVectorCalendario Postorden() const ;
	// Devuelve el recorrido en niveles
	TVectorCalendario Niveles() const;
	// Suma de dos ABB
	TABBCalendario operator+(const TABBCalendario &) const;
	// Resta de dos ABB
	TABBCalendario operator-(const TABBCalendario &) const;
};

// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO

class TNodoABB
{
	friend class TABBCalendario;
private:
	// Elemento del nodo
	TCalendario item;
	// Subárbol izquierdo y derecho
	TABBCalendario iz, de;

public:
	//Constructor por defecto
	TNodoABB ();
	//Constructor copia
	TNodoABB (const TNodoABB &);
	//Destructor
	~TNodoABB ();
	// Sobrecarga del operador asignación
	TNodoABB & operator=(const TNodoABB &);
};


#endif /* TABBCALENDARIO_H_ */
