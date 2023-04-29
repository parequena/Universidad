/*
* TAABCalendario.cpp
*
*  Created on: 27/04/2015
*      Author: pablo
*/

#include "../include/tabbcalendario.h"
#include <queue>

// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO

//Constructor por defecto
TNodoABB::TNodoABB()
{

}

//Constructor copia
TNodoABB::TNodoABB (const TNodoABB &nodo)
{
	if(this != &nodo)
	{
		item=nodo.item;
		iz=nodo.iz;
		de=nodo.de;
	}
}

//Destructor
TNodoABB::~TNodoABB ()
{

}

// Sobrecarga del operador asignación
TNodoABB & TNodoABB::operator=(const TNodoABB &nodo)
{
	if(this != &nodo)
	{
		item=nodo.item;
		iz=nodo.iz;
		de=nodo.de;
	}

	return *this;
}



// ---------------------------------------------------------------------------------------------- ABBCALENDARIO
// ---------------------------------------------------------------------------------------------- ABBCALENDARIO
// ---------------------------------------------------------------------------------------------- ABBCALENDARIO
// ---------------------------------------------------------------------------------------------- ABBCALENDARIO

// Devuelve el recorrido en inorden
void TABBCalendario::InordenAux(TVectorCalendario &vector, int &pos) const
{
	if(raiz != NULL)
	{
		raiz->iz.InordenAux(vector, pos);				// Primero por la izquierda.
		vector[pos]=raiz->item;							// Depues el item central.
		pos++;
		raiz->de.InordenAux(vector, pos);				// Por ultimo, por la derecha.
	}
}

// Devuelve el recorrido en preorden
void TABBCalendario::PreordenAux(TVectorCalendario &vector, int &pos) const
{
	if(raiz != NULL)
	{
		vector[pos]=raiz->item;
		pos++;// Primero el item central.
		raiz->iz.PreordenAux(vector, pos);				// Despues por la izquierda.
		raiz->de.PreordenAux(vector, pos);				// Por ultimo, la derecha.
	}
}

// Devuelve el recorrido en postorden
void TABBCalendario::PostordenAux(TVectorCalendario &vector, int &pos) const
{
	if(raiz != NULL)
	{
		raiz->iz.PostordenAux(vector, pos);				// Primero por la izquierda.
		raiz->de.PostordenAux(vector, pos);				// Despues por la derecha.
		vector[pos]=raiz->item;						// Por ultimo, el item central.
		pos++;
	}
}

// Constructor por defecto
TABBCalendario::TABBCalendario ()
{
	raiz = NULL;
}

// Constructor de copia
TABBCalendario::TABBCalendario(const TABBCalendario &abbCal)
{
	if(abbCal.raiz!=NULL)
	{
		TNodoABB *nodo = new TNodoABB();
		nodo->item = abbCal.raiz->item;
		raiz = nodo;
		raiz->iz= TABBCalendario(abbCal.raiz->iz);
		raiz->de= TABBCalendario(abbCal.raiz->de);
	}
	else raiz = NULL;
}

// Destructor
TABBCalendario::~TABBCalendario ()
{
	if(raiz!=NULL)
	{
		delete raiz;
		raiz=NULL;
	}
}

// Sobrecarga del operador asignación
TABBCalendario & TABBCalendario::operator=(const TABBCalendario &abbCal)
{
	this->~TABBCalendario();

	if(abbCal.raiz!=NULL)
	{
		TNodoABB *aux = new TNodoABB();
		aux->item = abbCal.raiz->item;
		raiz = aux;
		raiz->iz=abbCal.raiz->iz;
		raiz->de=abbCal.raiz->de;
	}
	else raiz = NULL;

	return *this;
}

// Sobrecarga del operador igualdad
bool TABBCalendario::operator==(const TABBCalendario &abbCal) const
{
	// Si tienen los mismos nodos, y cualquier recorrido es igual (en este caso Inorden), son iguales.
	if(Nodos() == abbCal.Nodos() && Inorden()==abbCal.Inorden())
		return true;
	else return false;
}

// Devuelve TRUE si el árbol está vacío, FALSE en caso contrario
bool TABBCalendario::EsVacio() const
{
	if(raiz==NULL)
		return true;
	else return false;
}

// Inserta el elemento en el árbol
bool TABBCalendario::Insertar(const TCalendario &cal)
{
	bool insertado = false;
	TABBCalendario *abbCal = NULL;

	if(raiz == NULL) // Si insertamos en un árbol vacio.
	{
		TNodoABB *nodo = new TNodoABB();
		nodo->item = cal;
		raiz = nodo;
		if(raiz != NULL)
			insertado = true;
	}
	else if(raiz->item > cal) // Si se tiene que insertar por la izquierda.
	{
		abbCal = &raiz->iz;
		insertado = abbCal->Insertar(cal); // Llamaremos a la recursividad para seguir buscando donde insertar.
	}
	else if(raiz->item < cal) // Si se tiene que insertar por la derecha.
	{
		abbCal = &raiz->de;
		insertado = abbCal->Insertar(cal); // Llamaremos a la recursividad para seguir buscando donde insertar.
	}
	else
		return insertado=false;

	return insertado;
}


bool TABBCalendario::Borrar(const TCalendario &cal)
{
	if(EsVacio())
		return false;
	else if(cal < raiz->item)
		return raiz->iz.Borrar(cal);
	else if(cal > raiz->item)
		return raiz->de.Borrar(cal);

	if(!raiz->iz.EsVacio() && !raiz->de.EsVacio()) // Si estoy en un nodo 2-padre.
	{
		TNodoABB *nodo = new TNodoABB();

		nodo->iz=raiz->iz;
		nodo->de=raiz->de;

		TVectorCalendario vector = raiz->iz.Inorden();
		nodo->item=vector[vector.Tamano()];

		nodo->iz.Borrar(vector[vector.Tamano()]);
		nodo->item=vector[vector.Tamano()];

		delete raiz;
		raiz=nodo;
	}
	else if(!raiz->iz.EsVacio() && raiz->de.EsVacio()) // Si estoy en un nodo iz-padre.
	{
		TNodoABB *nodo = new TNodoABB();
		nodo->de = raiz->iz.raiz->de;
		nodo->iz = raiz->iz.raiz->iz;
		nodo->iz.Borrar(raiz->iz.raiz->item);
		nodo->item = raiz->iz.raiz->item;

		delete raiz;
		raiz = nodo;
	}
	else if(raiz->iz.EsVacio() && !raiz->de.EsVacio()) // Si estoy en un nodo de-padre.
	{
		TNodoABB *nodo = new TNodoABB();
		nodo->de = raiz->de.raiz->de;
		nodo->iz = raiz->de.raiz->iz;
		nodo->de.Borrar(raiz->de.raiz->item);
		nodo->item = raiz->de.raiz->item;

		delete raiz;
		raiz = nodo;
	}
	else
	{
		TNodoABB *nodo = new TNodoABB();
		nodo=raiz->iz.raiz;
		delete raiz;
		raiz=nodo;
	}

	return true;
}

// Devuelve TRUE si el elemento está en el árbol, FALSE en caso contrario
bool TABBCalendario::Buscar(const TCalendario &cal) const
{
	bool encontrado=false;
	TNodoABB *nodo = raiz;

	while (!encontrado && nodo!=NULL)
	{
		if(nodo->item==cal)
			encontrado=true;
		else if(nodo->item < cal)
			nodo=nodo->de.raiz;
		else if(nodo->item > cal)
			nodo=nodo->iz.raiz;
	}

	return encontrado;
}

// Devuelve el elemento en la raíz del árbol
TCalendario TABBCalendario::Raiz() const
{
	if(EsVacio())
		return TCalendario();
	else return raiz->item;
}

// Devuelve la altura del árbol (la altura de un árbol vacío es 0)
int TABBCalendario::Altura() const
{
	int altura;

	if(EsVacio())
		altura=0;
	else altura = 1+ max(raiz->iz.Altura(), raiz->de.Altura());

	return altura;
}

// Devuelve el número de nodos del árbol (un árbol vacío posee 0 nodos)
int TABBCalendario::Nodos() const
{
	int nodos;

	if(EsVacio())
		nodos=0;
	else
		nodos = 1 + raiz->iz.Nodos() + raiz->de.Nodos();

	return nodos;
}

// Devuelve el número de nodos hoja en el árbol (la raíz puede ser nodo hoja)
int TABBCalendario::NodosHoja() const
{
	int nodos;

	//Si esta vacio.
	if(EsVacio())
		nodos=0;
	else if(raiz->iz.raiz == NULL && raiz->de.raiz == NULL)
		nodos=1;
	else
		nodos = raiz->iz.NodosHoja() + raiz->de.NodosHoja();

	return nodos;
}

// Devuelve el recorrido en inorden sobre un vector
TVectorCalendario TABBCalendario::Inorden() const
{
	int inicio=1;
	TVectorCalendario vector(Nodos());
	InordenAux(vector, inicio);

	return vector;
}

// Devuelve el recorrido en preorden sobre un vector
TVectorCalendario TABBCalendario::Preorden() const
{
	int inicio=1;
	TVectorCalendario vector(Nodos());
	PreordenAux(vector, inicio);

	return vector;
}

// Devuelve el recorrido en postorden sobre un vector
TVectorCalendario TABBCalendario::Postorden() const
{
	int inicio=1;
	TVectorCalendario vector(Nodos());
	PostordenAux(vector, inicio);

	return vector;
}

// Devuelve el recorrido en niveles
TVectorCalendario TABBCalendario::Niveles() const
{
	queue<TABBCalendario> cola;

	if(Nodos() == 0)
		return TVectorCalendario();

	TVectorCalendario vector(Nodos());
	int posicion = 1;
	TABBCalendario abbCal;

	cola.push((*this));
	while(!cola.empty())
	{
		abbCal = cola.front();
		vector[posicion] = abbCal.Raiz();
		posicion++;
		cola.pop();

		if(!abbCal.raiz->iz.EsVacio())
			cola.push(abbCal.raiz->iz);
		if(!abbCal.raiz->de.EsVacio())
			cola.push(abbCal.raiz->de);
	}

	return vector;
}

// Sobrecarga del operador salida
ostream & operator<< (ostream &os, const TABBCalendario &abbCal)
{
	os << abbCal.Niveles();
	return os;
}

// Suma de dos ABB
TABBCalendario TABBCalendario::operator+(const TABBCalendario &abbCal) const
{
	TABBCalendario arbBin(*this); // Creamos una copia del this.
	TVectorCalendario vector = abbCal.Inorden(); // Creamos un vector con el arbol que nos pasan.

	for(int i=1; i<=vector.Tamano(); i++)
		arbBin.Insertar(vector[i]);

	return arbBin;
}

// Resta de dos ABB
TABBCalendario TABBCalendario::operator-(const TABBCalendario &abbCal) const
{
	TABBCalendario arbolBin; // Creamos un arbol binario vacio
	TVectorCalendario vector = this->Inorden(); // Creamos un vector para recorer el arbol que tenemos.

	for(int i=1; i<=vector.Tamano(); i++) // Empezamos en 1 porque nuestros vectores empiezan así.
		if(!abbCal.Buscar(vector[i])) // Si no esta en el arbol que pasamos por paramstro.
			arbolBin.Insertar(vector[i]); // Lo insertaremos en el arbol creado.

	return arbolBin; // Devolvemos el arbol creado.
}
