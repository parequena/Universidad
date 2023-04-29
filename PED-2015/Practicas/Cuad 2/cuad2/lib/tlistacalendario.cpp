/*
 * tlistacalendario.cpp
 *
 *  Created on: 17/03/2015
 *      Author: prg26
 */

#include "../include/tlistacalendario.h"

// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO
// ---------------------------------------------------------------------------------------------- NODO

// Constructor de copia
TNodoCalendario::TNodoCalendario (const TNodoCalendario &tnodo)
{
	c=tnodo.c;
	siguiente=tnodo.siguiente;
}

// Destructor
TNodoCalendario::~TNodoCalendario ()
{
	c=TCalendario();
	siguiente=NULL;
}

// Sobrecarga del operador asignación
TNodoCalendario & TNodoCalendario::operator=(const TNodoCalendario &tnodo)
{
	if(this!=&tnodo)
	{
		(*this).~TNodoCalendario();
		c = tnodo.c;
		siguiente = tnodo.siguiente;
	}
	return (*this);
}

// ---------------------------------------------------------------------------------------------- LISTAPOS
// ---------------------------------------------------------------------------------------------- LISTAPOS
// ---------------------------------------------------------------------------------------------- LISTAPOS
// ---------------------------------------------------------------------------------------------- LISTAPOS
// ---------------------------------------------------------------------------------------------- LISTAPOS

// Constructor de copia
TListaPos::TListaPos(const TListaPos &tpos)
{
	if (this!=&tpos)
		pos=tpos.pos;
}

// Destructor
TListaPos::~TListaPos()
{
	pos=NULL;
}

// Sobrecarga del operador asignación
TListaPos & TListaPos::operator=(const TListaPos &tpos)
{
	if(&tpos!=NULL)
		pos=tpos.pos;

	return *this;
}

// Sobrecarga del operador igualdad
bool TListaPos::operator==(const TListaPos &tpos) const
{
	if(pos==tpos.pos)
		return true;
	else return false;
}

// Sobrecarga del operador desigualdad
bool TListaPos::operator!=(const TListaPos &tpos) const
{
	return !(pos==tpos.pos);
}

// Devuelve la posición siguiente
TListaPos TListaPos::Siguiente() const
{
	TListaPos posList;
	if(this!=NULL)
		posList.pos = pos->siguiente;

	return posList;
}

// Posición vacía
bool TListaPos::EsVacia() const
{
	if(pos==NULL)
		return true;
	else return false;
}

// ---------------------------------------------------------------------------------------------- LISTACALENDARIO
// ---------------------------------------------------------------------------------------------- LISTACALENDARIO
// ---------------------------------------------------------------------------------------------- LISTACALENDARIO
// ---------------------------------------------------------------------------------------------- LISTACALENDARIO
// ---------------------------------------------------------------------------------------------- LISTACALENDARIO

TListaCalendario TListaCalendario::SubRangos(int n1, int n2)
{
	TListaCalendario auxList;
	if(n1<=n2)
	{
		if(n1<=0) n1=1;
		if(n2<=0) n2=1;
		if(n1>=Longitud()) n1=Longitud();
		if(n2>=Longitud()) n2=Longitud();

		int posicion=1;

		TListaPos pivote;
		pivote.pos=primero;

		while(pivote.pos!=NULL && posicion<=n2)
		{
			if(posicion>=n1 && posicion<=n2)
				auxList.Insertar(pivote.pos->c);
			posicion++;
			pivote.pos=pivote.pos->siguiente;
		}
	}

	return auxList;
}

// Constructor de copia
TListaCalendario::TListaCalendario(const TListaCalendario &tlist)
{
	TListaPos pivote, anterior;

	if( this != &tlist)
	{
		primero = new TNodoCalendario(*tlist.primero);
		anterior = tlist.Primera();
		pivote = tlist.Primera();
		pivote = pivote.Siguiente();

		//if( p.primero->siguiente != NULL )
		if( pivote.pos != NULL)
		{
			while(pivote.pos != NULL)
			{
				anterior.Siguiente();
				anterior.pos= new TNodoCalendario(*pivote.pos);
				anterior = pivote;
				pivote = pivote.Siguiente();
			}
		}
	}
}

//Destructor
TListaCalendario::~TListaCalendario()
{
	TNodoCalendario *pivote;
	while (primero != NULL)
	{
		pivote = primero->siguiente;
		delete primero;
		primero = pivote;
	}

	delete pivote;
}

// Sobrecarga del operador asignación
TListaCalendario & TListaCalendario::operator=(const TListaCalendario &tlist)
{
	TListaPos pivote;
	if(this!=&tlist)

		pivote.pos=tlist.primero;

		while(!pivote.EsVacia())
		{
			Insertar(pivote.pos->c);
			pivote.pos=pivote.pos->siguiente;
		}
	return (*this);
}

// Sobrecarga del operador igualdad
bool TListaCalendario::operator==(const TListaCalendario &tlist) const
{
	bool seguir=true;
	while(tlist.primero!=NULL && seguir)
	{
		if(primero!=tlist.primero)
			seguir = false;
	}
	return seguir;
}

//Sobrecarga del operador suma
TListaCalendario TListaCalendario::operator+ (const TListaCalendario &tlist) const
{
	TListaPos borrarNodo;
	borrarNodo.pos=tlist.primero;

	TListaCalendario auxCal(*this);

	while(borrarNodo.pos!=NULL)
	{
		auxCal.Insertar(borrarNodo.pos->c);
		borrarNodo.pos=borrarNodo.pos->siguiente;
	}

	return auxCal;
}

//Sobrecarga del operador resta
TListaCalendario TListaCalendario::operator- (const TListaCalendario &tlist) const
{
	TListaCalendario auxCal(*this);

	TListaPos borrarNodo;
	borrarNodo.pos=tlist.primero;

	while(borrarNodo.pos!=NULL)
	{
		auxCal.Borrar(borrarNodo.pos->c);
		borrarNodo.pos=borrarNodo.pos->siguiente;
	}

	return auxCal;
}

// Inserta el elemento en la posición que le corresponda dentro de la lista
bool TListaCalendario::Insertar (const TCalendario &tcal)
{
	if(!Buscar(tcal))
	{
		TNodoCalendario *nodo= new TNodoCalendario();
		nodo->c=tcal;
		if(primero==NULL)
		{
			primero = nodo;
			return true;
		}
		else
		{
			TListaPos actual;
			actual.pos=primero;
			TListaPos anterior;
			bool insertado = false;
			while(!actual.EsVacia())
			{
				if(actual.pos->c == tcal)
					return false;
				else if(actual.pos->c > tcal)
				{
					if(actual.pos==primero)
					{
						//inserto en cabeza
						primero=nodo;
						nodo->siguiente=actual.pos;
						return true;
					}
					else
					{
						//inserto intermedio
						anterior.pos->siguiente = nodo;
						nodo->siguiente = actual.pos;
						return true;
					}
				}
				anterior.pos=actual.pos;
				actual.pos=actual.pos->siguiente;
			}
			if(!insertado)
			{ //inserto cola
				anterior.pos->siguiente=nodo;
				return true;
			}
		}
	}
	return false;
}

// Busca y borra el elemento
bool TListaCalendario::Borrar(const TCalendario &tcal)
{
	if(Buscar(tcal))
	{
		TNodoCalendario *nodo= new TNodoCalendario();
		nodo->c=tcal;
		if(nodo==primero)
		{
			primero=primero->siguiente; //Si queremos borrar el primer elemento
			return true;
		}
		else
		{
			TListaPos actual, anterior;
			actual.pos=nodo;
			anterior.pos=primero;

			while(anterior.pos->siguiente!=actual.pos)
				anterior.pos=anterior.pos->siguiente;
			while(!actual.EsVacia())
			{
				if(actual.pos->c == tcal)
				{
					if(actual.pos->siguiente==NULL)
					{
						anterior.pos->siguiente=NULL; // Si quieremos borrar el ultimo elemento
						return true;
					}
					else
						anterior.pos->siguiente=actual.pos->siguiente; // Si queremos borrar un elemento medio
				}

				anterior.pos=actual.pos;
				actual.pos=actual.pos->siguiente;
			}
		}
	}
	return false;
}

// Borra el elemento que ocupa la posición indicada
bool TListaCalendario::Borrar (const TListaPos &tpos)
{
	if(tpos.pos!=NULL)
	{
		if(tpos.pos==primero)
		{
			primero=primero->siguiente; // Si quieremos borrar el elemento de la cabeza
			return true;
		}
		else
		{
			TListaPos next, actual;
			next.pos=tpos.pos->siguiente;
			actual.pos=primero;
			TListaPos anterior;

			while(!actual.EsVacia())
			{
				if(actual.pos==tpos.pos)
				{
					if(actual.pos->siguiente == NULL)
					{
						// Si queremos borrar el ultimo elemento
						anterior.pos->siguiente = NULL;
						return true;
					}
					else
					{
						anterior.pos->siguiente = next.pos;
						return true;
					}
				}

				next.pos=next.pos->siguiente;
				anterior.pos=actual.pos;
				actual.pos=actual.pos->siguiente;
			}
		}
	}
	return false;
}

//Borra todos los Calendarios con fecha ANTERIOR a la pasada.
bool TListaCalendario::Borrar(int day, int month, int year)
{
	TCalendario tcal(day, month, year, NULL);

	if(Buscar(tcal))
	{
		TNodoCalendario *nodo= new TNodoCalendario();
		nodo->c=tcal;
		if(nodo==primero)
		{
			primero=primero->siguiente; //Si queremos borrar el primer elemento
			return true;
		}
		else
		{
			TListaPos actual;
			actual.pos=primero;
			TListaPos anterior;

			while(!actual.EsVacia())
			{
				if(actual.pos->c == tcal)
				{
					if(actual.pos->siguiente==NULL)
					{
						anterior.pos->siguiente=NULL; // Si quieremos borrar el ultimo elemento
						return true;
					}
					else
						anterior.pos->siguiente=actual.pos->siguiente; // Si queremos borrar un elemento medio
				}

				anterior.pos=actual.pos;
				actual.pos=actual.pos->siguiente;
			}
		}
	}
	return false;
}

// Devuelve true si la lista está vacía, false en caso contrario
bool TListaCalendario::EsVacia() const
{
	if(primero==NULL)
		return true;
	else return false;
}

TCalendario TListaCalendario::Obtener (const TListaPos &tpos) const
{
	TCalendario auxCal;
	bool seguir=true;
	TListaPos pivote;
	pivote.pos=primero;
	while (seguir && pivote.pos!=NULL)
	{
		if(pivote.pos->c == tpos.pos->c)
		{
			seguir = false;
			auxCal = tpos.pos->c;
		}
		else
			pivote = pivote.Siguiente();
	}

return auxCal;
}

// Devuelve true si el Calendario está en la lista.
bool TListaCalendario::Buscar(const TCalendario &tcal) const
{
	bool encontrado = false;

	TNodoCalendario *pivote = primero;

	while(!encontrado && pivote != NULL)
	{
		if(pivote->c==tcal)
			encontrado=true;
		pivote=pivote->siguiente;
	}
	return encontrado;
}

// Devuelve la longitud de la lista
int TListaCalendario::Longitud() const
{
	int tam=0;

	TNodoCalendario *pivote = primero;

	while(pivote!=NULL)
	{
		tam+=1;
		pivote=pivote->siguiente;
	}

	return tam;
}

// Devuelve la primera posición en la lista
TListaPos TListaCalendario::Primera() const
{
	TListaPos auxPos;

	if(EsVacia())
		return auxPos;
	else
		auxPos.pos=primero;

	return auxPos;
}

// Devuelve la última posición en la lista
TListaPos TListaCalendario::Ultima() const
{
	TListaPos auxPos;

	if(EsVacia())
		return auxPos;
	else
		while(auxPos.pos->siguiente != NULL)
			auxPos.pos=auxPos.pos->siguiente;

	return auxPos;
}

// Suma de dos sublistas en una nueva lista
TListaCalendario TListaCalendario::SumarSubl (const int I_L1, const int F_L1, const TListaCalendario & L2, const int I_L2, const int
F_L2)
{
	TListaCalendario auxList1, auxList2;
	auxList1.SubRangos(I_L1,F_L1);
	auxList2.SubRangos(I_L2,F_L2);

	return (auxList1+auxList2);
}

// Extraer un rango de nodos de la lista
TListaCalendario TListaCalendario::ExtraerRango (int n1, int n2)
{
	TListaCalendario auxList, finalList;
	if(n1<=n2)
	{
		if(n1<=0) n1=1;
		if(n2<=0) n2=1;
		if(n1>=Longitud()) n1=Longitud();
		if(n2>=Longitud()) n2=Longitud();

		int posicion=1;

		TListaPos pivote, pivote2;
		pivote.pos=primero;

		while(pivote.pos!=NULL && posicion<=n2)
		{
			if(posicion>=n1 && posicion<=n2)
				auxList.Insertar(pivote.pos->c);
			posicion++;
			pivote.pos=pivote.pos->siguiente;
		}
		pivote2.pos=auxList.primero;

		while(pivote2.pos!=NULL)
		{
			Borrar(pivote2.pos->c);
			pivote2.pos=pivote2.pos->siguiente;
		}
	}
	return (*this);
}

//Sobrecarga del operador salida
ostream & operator<<(ostream &os, const TListaCalendario &tlist)
{
	TListaPos pivote;
	pivote.pos=tlist.primero;

	os << "<";
	while (!pivote.EsVacia ())
	{
		os << tlist.Obtener (pivote);
		pivote.pos = pivote.pos->siguiente;
		if (!pivote.EsVacia ())
			os << ' ';
	}
	os << ">";

	return os;
}
