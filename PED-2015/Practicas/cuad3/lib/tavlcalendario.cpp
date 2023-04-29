/*
* tavlcalendario.cpp
*
*  Created on: 13/05/2015
*      Author: pablo
*/

#include "../include/tavlcalendario.h"

// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO

// Constructor por defecto
TNodoAVL::TNodoAVL()
{
	item=TCalendario();
	fe=0;
}

// Constructor copia
TNodoAVL::TNodoAVL (const TNodoAVL &nodo)
{
	if(this!=&nodo)
	{
		item=nodo.item;
		iz=nodo.iz;
		de=nodo.de;
		fe=nodo.fe;
	}
}

// Destructor
TNodoAVL::~TNodoAVL ()
{

}

// Sobrecarga del operador asignación
TNodoAVL & TNodoAVL::operator=(const TNodoAVL &nodo)
{
	if(this!=&nodo)
	{
		item=nodo.item;
		iz=nodo.iz;
		de=nodo.de;
		fe=nodo.fe;
	}

	return *this;
}

// ---------------------------------------------------------------------------------------------- AVLCALENDARIO
// ---------------------------------------------------------------------------------------------- AVLCALENDARIO
// ---------------------------------------------------------------------------------------------- AVLCALENDARIO
// ---------------------------------------------------------------------------------------------- AVLCALENDARIO

void TAVLCalendario::InordenAux(TVectorCalendario &vector, int &pos) const
{
	if(raiz!=NULL)
	{
		raiz->iz.InordenAux(vector, pos);				// Primero por la izquierda.
		vector[pos]=raiz->item;							// Depues el item central.
		pos++;
		raiz->de.InordenAux(vector, pos);				// Por ultimo, por la derecha.
	}
}

// Devuelve el recorrido en preorden
void TAVLCalendario::PreordenAux(TVectorCalendario &vector, int &pos) const
{
	if(raiz!=NULL)
	{
		vector[pos]=raiz->item;
		pos++;// Primero el item central.
		raiz->iz.PreordenAux(vector, pos);				// Despues por la izquierda.
		raiz->de.PreordenAux(vector, pos);				// Por ultimo, la derecha.
	}
}

// Devuelve el recorrido en postorden
void TAVLCalendario::PostordenAux(TVectorCalendario &vector, int &pos) const
{
	if(raiz!=NULL)
	{
		raiz->iz.PostordenAux(vector, pos);				// Primero por la izquierda.
		raiz->de.PostordenAux(vector, pos);				// Despues por la derecha.
		vector[pos]=raiz->item;						// Por ultimo, el item central.
		pos++;
	}
}

int TAVLCalendario::calcularFE()
{
	return (raiz->de.Altura() - raiz->iz.Altura());
}

void TAVLCalendario::propagarFE()
{
	if(raiz->iz.raiz!=NULL)
		raiz->iz.raiz->fe = calcularFE();
	if(raiz->de.raiz!=NULL)
		raiz->de.raiz->fe = calcularFE();
}

void TAVLCalendario::equilibrarArbol()
{
	TNodoAVL *nodo;

		raiz->fe=calcularFE();
		if (raiz->fe>1)
		{
			if (raiz->de.raiz->fe >= 0)
			{
				nodo = raiz->de.raiz;
				raiz->de.raiz = raiz->de.raiz->iz.raiz;
				nodo->iz.raiz = raiz;
				raiz = nodo;
			}
			else
			{
				nodo = raiz->de.raiz;
				raiz->de.raiz = nodo->iz.raiz->iz.raiz;
				nodo->iz.raiz->iz.raiz = raiz;
				raiz = nodo->iz.raiz;
				nodo->iz.raiz = raiz->de.raiz;
				raiz->de.raiz = nodo;
			}

			raiz->fe=calcularFE();
			if (raiz->iz.raiz!=NULL)
				raiz->iz.raiz->fe=calcularFE();

			if (raiz->de.raiz!=NULL)
				raiz->de.raiz->fe=calcularFE();
		}
		else if (raiz->fe<-1)
		{
			if (raiz->iz.raiz->fe <= 0)
			{
				nodo = raiz->iz.raiz;
				raiz->iz.raiz = raiz->iz.raiz->de.raiz;
				nodo->de.raiz = raiz;
				raiz = nodo;
			}
			else
			{
				nodo = raiz->iz.raiz;
				raiz->iz.raiz = nodo->de.raiz->de.raiz;
				nodo->de.raiz->de.raiz = raiz;
				raiz = nodo->de.raiz;
				nodo->de.raiz = raiz->iz.raiz;
				raiz->iz.raiz = nodo;
			}

			raiz->fe=calcularFE();
			propagarFE();
		}
}

// Constructor por defecto
TAVLCalendario::TAVLCalendario ()
{
	raiz = NULL;
}

// Constructor de copia
TAVLCalendario::TAVLCalendario (const TAVLCalendario &avlCal)
{
	if(avlCal.raiz!=NULL)
	{
		TNodoAVL *nodo = new TNodoAVL();
		nodo->item = avlCal.raiz->item;
		raiz = nodo;
		raiz->fe=avlCal.raiz->fe;
		raiz->iz= TAVLCalendario(avlCal.raiz->iz);
		raiz->de= TAVLCalendario(avlCal.raiz->de);
	}
	else raiz = NULL;
}

// Destructor
TAVLCalendario::~TAVLCalendario ()
{
	if(raiz!=NULL)
	{
		delete raiz;
		raiz=NULL;
	}
}

// Sobrecarga del operador asignación
TAVLCalendario & TAVLCalendario::operator=(const TAVLCalendario &avlCal)
{
	this->~TAVLCalendario();

	if(avlCal.raiz!=NULL)
	{
		TNodoAVL *nodo = new TNodoAVL();
		nodo->item = avlCal.raiz->item;
		raiz = nodo;
		raiz->fe=avlCal.raiz->fe;
		raiz->iz=avlCal.raiz->iz;
		raiz->de=avlCal.raiz->de;
	}
	else raiz = NULL;

	return *this;
}

// Sobrecarga del operador igualdad
bool TAVLCalendario::operator==(const TAVLCalendario &avlCal)
{
	// Si tienen los mismos nodos, y cualquier recorrido es igual (en este caso Inorden), son iguales.
	if(Nodos()==avlCal.Nodos() && Inorden()==avlCal.Inorden())
		return true;
	else return false;
}

// Sobrecarga del operador desigualdad
bool TAVLCalendario::operator!=(const TAVLCalendario &avlCal)
{
	if((*this)==avlCal) return false;
	else return true;
}

// Devuelve TRUE si el árbol está vacío, FALSE en caso contrario
bool TAVLCalendario::EsVacio() const
{
	if(raiz==NULL)
		return true;
	else return false;
}

// Inserta el elemento en el árbol
bool TAVLCalendario::Insertar(const TCalendario &cal)
{
	bool insertado=false;
	if(EsVacio())
	{
		raiz = new TNodoAVL();
		raiz->item=cal;
		insertado=true;
	}
	else
	{
		if(raiz->item==cal)
			insertado=false;
		else if(raiz->item>cal)
			insertado=raiz->iz.Insertar(cal);
		else
			insertado=raiz->de.Insertar(cal);
	}
	if(insertado)
		equilibrarArbol();

	return insertado;
}

// Devuelve true si el elemento está en el árbol, false en caso contrario
bool TAVLCalendario::Buscar(const TCalendario &cal)
{
	bool encontrado=false;
	TNodoAVL *nodo = raiz;

	while (!encontrado && nodo!=NULL)
	{
		if(nodo->item==cal)
			encontrado=true;
		else if(nodo->item<cal)
			nodo=nodo->de.raiz;
		else if(nodo->item>cal)
			nodo=nodo->iz.raiz;
	}

	return encontrado;
}

// Devuelve la altura del árbol (la altura de un árbol vacío es 0)
int TAVLCalendario::Altura() const
{
	int altura;

	if(EsVacio())
		altura=0;
	else altura = 1 + max(raiz->iz.Altura(), raiz->de.Altura());

	return altura;
}

// Devuelve el número de nodos del árbol (un árbol vacío posee 0 nodos)
int TAVLCalendario::Nodos() const
{
	int nodos;

	if(EsVacio())
		nodos=0;
	else
		nodos = 1 + raiz->iz.Nodos() + raiz->de.Nodos();

	return nodos;
}

// Devuelve el número de nodos hoja en el árbol (la raíz puede ser nodo hoja)
int TAVLCalendario::NodosHoja() const
{
	//Si esta vacio.
	if(EsVacio())
		return 0;
	else if(raiz->iz.raiz==NULL && raiz->de.raiz==NULL)
		return 1;
	else
		return (raiz->iz.NodosHoja() + raiz->de.NodosHoja());
}

// Devuelve el recorrido en inorden sobre un vector
TVectorCalendario TAVLCalendario::Inorden() const
{
	int inicio=1;
	TVectorCalendario vector(Nodos());
	InordenAux(vector, inicio);

	return vector;
}

// Devuelve el recorrido en preorden sobre un vector
TVectorCalendario TAVLCalendario::Preorden() const
{
	int inicio=1;
	TVectorCalendario vector(Nodos());
	PreordenAux(vector, inicio);

	return vector;
}

// Devuelve el recorrido en postorden sobre un vector
TVectorCalendario TAVLCalendario::Postorden() const
{
	int inicio=1;
	TVectorCalendario vector(Nodos());
	PostordenAux(vector, inicio);

	return vector;
}

// Borra un TCalendario del árbol AVL
bool TAVLCalendario::Borrar(const TCalendario &cal)
{
	bool result = false;

		if (raiz!=NULL)
		{
			if (cal<raiz->item)
				raiz->iz.Borrar(cal);
			else if (cal>raiz->item)
				raiz->de.Borrar(cal);
			else if (cal==raiz->item)
			{
				if (raiz->iz.EsVacio() && raiz->de.EsVacio())
				{
					delete raiz;
					raiz = NULL;
				}
				else if (raiz->iz.raiz!=NULL)
				{
					TNodoAVL *nodo = new TNodoAVL();

					nodo->iz=raiz->iz;
					nodo->de=raiz->de;

					TVectorCalendario vector = raiz->iz.Inorden();
					nodo->item=vector[vector.Tamano()];

					nodo->iz.Borrar(vector[vector.Tamano()]);
					nodo->item=vector[vector.Tamano()];

					delete raiz;
					raiz=nodo;
				}
				else if (raiz->de.raiz!=NULL)
				{
					TNodoAVL *nodo = new TNodoAVL();

					nodo->iz=raiz->iz;

					TVectorCalendario vector = raiz->de.Inorden();
					nodo->item=vector[0];

					nodo->iz.Borrar(vector[0]);
					nodo->item=vector[vector.Tamano()];

					delete raiz;
					raiz=nodo;
				}

				result = true;
			}
		}
		if (raiz!=NULL)
			equilibrarArbol();

		return result;
}

// Devuelve el elemento TCalendario raíz del árbol AVL
TCalendario TAVLCalendario::Raiz() const
{
	return raiz->item;
}

// Sobrecarga del operador salida
ostream & operator<< (ostream &os, const TAVLCalendario &avlCal)
{
	os << avlCal.Inorden();
	return os;
}
