/*
 * tavlcalendario.h
 *
 *  Created on: 13/05/2015
 *      Author: pablo
 */

#include "tcalendario.h"
#include "tvectorcalendario.h"

#ifndef TAVLCALENDARIO_H_
#define TAVLCALENDARIO_H_

class TNodoAVL;

// ---------------------------------------------------------------------------------------------- AVLCALENDARIO
// ---------------------------------------------------------------------------------------------- AVLCALENDARIO
// ---------------------------------------------------------------------------------------------- AVLCALENDARIO
// ---------------------------------------------------------------------------------------------- AVLCALENDARIO

class TAVLCalendario
{
	// Sobrecarga del operador salida
	friend ostream & operator<< (ostream &, const TAVLCalendario &);
private:
	// Puntero al nodo raíz
	TNodoAVL *raiz;
	// Devuelve el recorrido en inorden
	void InordenAux(TVectorCalendario &, int &) const;
	// Devuelve el recorrido en preorden
	void PreordenAux(TVectorCalendario &, int &) const;
	// Devuelve el recorrido en postorden
	void PostordenAux(TVectorCalendario &, int &) const;
	int calcularFE();
	void propagarFE();
	void equilibrarArbol();

public:
	// Constructor por defecto
	TAVLCalendario ();
	// Constructor de copia
	TAVLCalendario (const TAVLCalendario &);
	// Destructor
	~ TAVLCalendario ();
	// Sobrecarga del operador asignación
	TAVLCalendario & operator=(const TAVLCalendario &);
	// Sobrecarga del operador igualdad
	bool operator==(const TAVLCalendario &);
	// Sobrecarga del operador desigualdad
	bool operator!=(const TAVLCalendario &);
	// Devuelve TRUE si el árbol está vacío, FALSE en caso contrario
	bool EsVacio() const;
	// Inserta el elemento en el árbol
	bool Insertar(const TCalendario &);
	// Devuelve true si el elemento está en el árbol, false en caso contrario
	bool Buscar(const TCalendario &);
	// Devuelve la altura del árbol (la altura de un árbol vacío es 0)
	int Altura() const;
	// Devuelve el número de nodos del árbol (un árbol vacío posee 0 nodos)
	int Nodos() const;
	// Devuelve el número de nodos hoja en el árbol (la raíz puede ser nodo hoja)
	int NodosHoja() const;
	// Devuelve el recorrido en inorden sobre un vector
	TVectorCalendario Inorden() const;
	// Devuelve el recorrido en preorden sobre un vector
	TVectorCalendario Preorden() const;
	// Devuelve el recorrido en postorden sobre un vector
	TVectorCalendario Postorden() const;
	// Borra un TCalendario del árbol AVL
	bool Borrar(const TCalendario &);
	// Devuelve el elemento TCalendario raíz del árbol AVL
	TCalendario Raiz() const;
};

// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO

class TNodoAVL
{
	friend class TAVLCalendario;
private:
	// El elemento (etiqueta) del nodo
	TCalendario item;
	// Subárbol izquierdo y derecho
	TAVLCalendario iz, de;
	// Factor de equilibrio
	int fe;

public:
	// Constructor por defecto
	TNodoAVL ();
	// Constructor copia
	TNodoAVL (const TNodoAVL &);
	// Destructor
	~TNodoAVL ();
	// Sobrecarga del operador asignación
	TNodoAVL & operator=(const TNodoAVL &);
};


#endif /* TAVLCALENDARIO_H_ */
