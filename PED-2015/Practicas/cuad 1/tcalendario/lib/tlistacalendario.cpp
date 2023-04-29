/*
 * tlistacalendario.cpp
 *
 *  Created on: 17/03/2015
 *      Author: prg26
 */

#include "../include/tlistacalendario.h"

// ---------------------------------------------------------------------------------------------- NODO

// Constructor de copia
TNodoCalendario::TNodoCalendario (const TNodoCalendario &tnodo)
{
	c = tnodo.c;
	*this=tnodo;
}

// Destructor
TNodoCalendario::~TNodoCalendario ()
{
	delete siguiente;
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

// Constructor de copia
TListaPos::TListaPos(const TListaPos &tpos)
{
	*this=tpos;
}

// Destructor
TListaPos::~TListaPos()
{
	delete pos;
	pos=NULL;
}

// Sobrecarga del operador asignación
TListaPos & TListaPos::operator=(const TListaPos &tpos)
{
	if(this!=&tpos)
		(*this).~TListaPos();

	if(&tpos!=NULL)
		pos=tpos.pos;
	else
		pos=NULL;

	return *this;
}

// Sobrecarga del operador igualdad
bool TListaPos::operator==(const TListaPos &tpos)
{
	if(pos==tpos.pos)
		return true;
	else return false;
}

// Sobrecarga del operador desigualdad
bool TListaPos::operator!=(TListaPos &tpos)
{
	return !(pos==tpos.pos);
}

// Devuelve la posición siguiente
TListaPos TListaPos::Siguiente()
{
	TListaPos posList;

	posList.pos = pos->siguiente;

	return posList;
}

// Posición vacía
bool TListaPos::EsVacia()
{
	if(pos==NULL)
		return true;
	else return false;
}

// ---------------------------------------------------------------------------------------------- LISTACALENDARIO

void TListaCalendario::Remove (TListaPos &tpos)
{

}

void TListaCalendario::copiaListaCalendario(const TListaCalendario &tlist)
{
	TNodoCalendario *pivote = tlist.primero;

	while(pivote!=NULL)
	{
		TNodoCalendario *pivote2 = NodoNuevo(pivote->c);
		if(pivote != NULL)
			MeterNodo(pivote2);
	}
}

void TListaCalendario::MeterNodo (TNodoCalendario *tnodo)
{
	if (primero == NULL)
		primero = ultimo = tnodo;
	else
	{
		TNodoCalendario *pivote = primero;
		TNodoCalendario *pivote_next = pivote;
		while (pivote != NULL && (tnodo->c > pivote->c || tnodo->c == pivote->c))
		{
			pivote_next = pivote;
			pivote = pivote->siguiente;
		}

		if (pivote != NULL)
		{
			tnodo->siguiente = pivote;

			if (primero == pivote)
				primero = tnodo;
			else
				pivote_next->siguiente = tnodo;
		}
		else
		{
			ultimo->siguiente = tnodo;
			ultimo = tnodo;
		}
	}
}

TNodoCalendario *TListaCalendario::NodoNuevo (const TCalendario &tcal) const
{
	TNodoCalendario *pivote = new TNodoCalendario ();
	if (pivote != NULL)
		pivote->c = tcal;
	return pivote;
}

// Constructor de copia
TListaCalendario::TListaCalendario(TListaCalendario &tlist)
{
	if(this!=&tlist)
	{
		primero=tlist.primero;
		ultimo=tlist.ultimo;
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
}

// Sobrecarga del operador asignación
TListaCalendario & TListaCalendario::operator=(const TListaCalendario &tlist)
{
	if(this!=&tlist)
		(*this).~TListaCalendario();

	if(&tlist!=NULL)
		primero=tlist.primero;
	else
		primero=NULL;

	return *this;
}

// Sobrecarga del operador igualdad
bool TListaCalendario::operator==(const TListaCalendario &tlist)
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
TListaCalendario TListaCalendario::operator+ (const TListaCalendario &tlist)
{
	TListaCalendario auxList(*this);
	TNodoCalendario *pivote = primero;

	while(pivote!=NULL)
	{

		pivote=pivote->siguiente;
	}

	return auxList;
}

//Sobrecarga del operador resta
TListaCalendario TListaCalendario::operator- (const TListaCalendario &tlist)
{
	TListaCalendario result;
	TNodoCalendario *pivote = primero;

	while(pivote!=NULL)
	{
		if (!tlist.Buscar(pivote->c))
		{
			TNodoCalendario *nodo = NodoNuevo (pivote->c);
			if (nodo != NULL)
				result.MeterNodo (nodo);
		}
		pivote=pivote->siguiente;
	}

	return result;
}


bool TListaCalendario::Insertar (const TCalendario &tcal)
{
	if (!Buscar (tcal))
	{
		TNodoCalendario *pivote = NodoNuevo(tcal);
		if (Buscar (tcal))
		{
			MeterNodo (pivote);
			return true;
		}
		else return false;
	}
	else return false;
}

// Inserta el elemento en la posición que le corresponda dentro de la lista
/*bool TListaCalendario::Insertar(const TCalendario &tcal)
{
	TNodoCalendario *actual = new TNodoCalendario();
	actual->c=tcal;

	if(!Buscar(tcal))
	{
		if(EsVacia())
		{
			actual->siguiente=NULL;
			primero=actual;
		}
		else
		{
			cout << "No es vacia\n";
			TNodoCalendario *pivote = primero;
			TNodoCalendario *next = pivote->siguiente;

			cout << "Estmos dentro\n";
			while(pivote!=NULL && (pivote->c < tcal || pivote->c == tcal))
			{
				cout << tcal << endl;
				cout << pivote << endl;
				cout << next << endl;
				pivote=next;
				next=next->siguiente;
			}
			if(next!=NULL)
			{
				actual->siguiente=next;

				if(primero == next)
					primero = actual;
				else
					pivote->siguiente=actual;
			}
			else
			{
				pivote->siguiente=actual;
				next=actual;
			}
		}
		return true;
	}
	else return false;
}
*/
// Busca y borra el elemento
bool TListaCalendario::Borrar(const TCalendario &tcal)
{
	bool result = false;
	TListaPos pivote = Primera ();

	while (!result && pivote.pos != NULL)
	{
		if (pivote.pos->c == tcal)
			result = true;
		else
			pivote = pivote.Siguiente ();
	}
	if (result)
		Remove(pivote);

	return result;
}

// Borra el elemento que ocupa la posición indicada
/*bool TListaCalendario::Borrar (const TListaPos &tpos)
{
	bool result;

	if(Belong (tpos))
	{
		Remove(tpos);
		result = true;
	}
	else
		result = false;

	return result;
}
*/
//Borra todos los Calendarios con fecha ANTERIOR a la pasada.
bool TListaCalendario::Borrar(int day, int month, int year)
{
	bool result = false;
	TCalendario auxCal (day, month, year, NULL);

	if (auxCal != error)
		for (TNodoCalendario *n = primero; n != NULL; n = primero)
		{
			auxCal.ModMensaje ((n->c).Mensaje ());

			// borrado de nodos anteriores al pasado
			if (n->c < auxCal)
			{
				TListaPos p(n);
				Remove (p);
				result = true;
			}
			// salida de bucle para nodos no anteriores
			else
				break;
		}

	return result;
}

// Devuelve true si la lista está vacía, false en caso contrario
bool TListaCalendario::EsVacia()
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

	while (seguir && pivote.pos!=NULL)
	{
		if(pivote==tpos)
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
int TListaCalendario::Longitud()
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
TListaPos TListaCalendario::Primera()
{
	TListaPos auxPos;

	if(EsVacia())
		return auxPos;
	else
		auxPos.pos=primero;

	return auxPos;
}

// Devuelve la última posición en la lista
TListaPos TListaCalendario::Ultima()
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
TListaCalendario TListaCalendario::SumarSubl (int I_L1, int F_L1, TListaCalendario & L2, int I_L2, int
F_L2)
{

}

// Extraer un rango de nodos de la lista
TListaCalendario TListaCalendario::ExtraerRango (int n1, int n2)
{
	TListaCalendario *auxList= NULL;
	if(n1<=n2)
	{
		// Comprobamos que no este fuera de los limites
		if(n1<=0) n1=1;
		if(n2<=0) n2=1;
		if(n1<=Longitud()) n1=Longitud();
		if(n2<=Longitud()) n2=Longitud();

		TNodoCalendario *inicio;
		TNodoCalendario *pivote;
		TNodoCalendario *fin;

		// Bucles para encontrar los nodos en las posiciones n1 y n2.
		for(int i=0; i<n1; i++)
			inicio=inicio->siguiente;
		for(int i=0; i<n2; i++)
			fin=fin->siguiente;

		pivote=inicio;
		auxList->primero=inicio;

		while(pivote!=fin)
		{
			auxList->primero->siguiente=pivote->siguiente;
			pivote=pivote->siguiente;
		}
	}

	return *auxList;
}

//Sobrecarga del operador salida
ostream & operator<<(ostream &os, TListaCalendario &tlist)
{
	TListaPos pivote (tlist.Primera ());

	os << "<";
	while (!pivote.EsVacia ())
	{
		os << tlist.Obtener (pivote);
		pivote = pivote.Siguiente ();
		if (!pivote.EsVacia ())
			os << ' ';
	}
	os << ">";

	return os;
}
