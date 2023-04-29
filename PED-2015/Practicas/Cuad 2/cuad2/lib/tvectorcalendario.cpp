/*
 * tvectorcalendario.cpp
 *
 *  Created on: 12/03/2015
 *      Author: prg26
 */

#include "../include/tcalendario.h"
#include "../include/tvectorcalendario.h"

void TVectorCalendario::copiaVector(const TVectorCalendario &vectCal)
{

}

// Constructor por defecto
TVectorCalendario::TVectorCalendario()
{
	c=NULL;
	tamano=0;
}
// Constructor a partir de un tamaño
TVectorCalendario::TVectorCalendario(const int tam)
{
	if(tam<=0)
	{
		c=NULL;
		tamano=0;
	}
	else
	{
		c = new TCalendario[tam];
		tamano=tam;
	}
}

// Constructor de copia
TVectorCalendario::TVectorCalendario(const TVectorCalendario &vectCal)
{
	if(tamano!=0)
	{
		c=NULL;
		tamano=0;
	}

	if(vectCal.tamano!=0)
	{
		tamano=vectCal.tamano;
		(*this).c=new TCalendario[tamano];
		for(int i=0; i<tamano;i++)
			(*this).c[i]=vectCal.c[i];
	}
	else
	{
		tamano=0;
		(*this).c=NULL;
	}
}

// Destructor
TVectorCalendario::~TVectorCalendario()
{
	delete[] c;
	c=NULL;
	tamano=0;
}

// Sobrecarga del operador asignación
TVectorCalendario & TVectorCalendario::operator=(const TVectorCalendario &vectCal)
{
	if(c!=NULL)
	{
		delete[] c;
		c = NULL;
	}

	if(this!=&vectCal)
	{
		if(vectCal.tamano!=0)
		{
			tamano=vectCal.tamano;
			c = new TCalendario[tamano];

			for(int i=0; i<tamano; i++)
				c[i]=vectCal.c[i];
		}
		else tamano=0;
	}
	return *this;
}

// Sobrecarga del operador igualdad
bool TVectorCalendario::operator==(const TVectorCalendario &vectCal)
{
	bool seguir=true;
	if(tamano==vectCal.tamano)
	{
		for(int i=0; i<tamano && seguir; i++)
			if(c[i]!=vectCal.c[i])
				seguir=false;
	}
	else seguir=false;
	return seguir;
}

// Sobrecarga del operador desigualdad
bool TVectorCalendario::operator!=(const TVectorCalendario &vectCal)
{
	return !((*this)==vectCal);
}

// Sobrecarga del operador corchete (parte IZQUIERDA)
TCalendario & TVectorCalendario::operator[](int pos)
{
	TCalendario *pivote = NULL;

	if(pos>tamano || pos<=0)
		pivote=&error;
	else
		pivote=&c[pos-1];

	return *pivote;
}

// Sobrecarga del operador corchete (parte DERECHA)
TCalendario TVectorCalendario::operator[](int pos) const
{
	if(pos<0 || pos>tamano)
		return error;
	else
		return c[pos-1];
}

// Cantidad de posiciones OCUPADAS (no vacías) en el vector
int TVectorCalendario::Ocupadas() const
{
	int ocupadas=0;

	for(int i=0; i<tamano; i++)
		if(!c[i].EsVacio())
			ocupadas++;
	return ocupadas;
}

// Devuelve true si existe el calendario en el vector
bool TVectorCalendario::ExisteCal(const TCalendario &cal)
{
	bool existe=false;
	for(int i=0; !existe && i<tamano; i++)
	{
		if(c[i]==cal)
			existe=true;
	}
	return existe;
}

// Mostrar por pantalla mensajes de TCalendario en el vector, de fecha IGUAL O POSTERIOR a la pasada
void TVectorCalendario::MostrarMensajes(int day, int month, int year)
{
	TCalendario auxCal(day, month, year, NULL);

	cout << "[";
	if(auxCal!=error)
	{
		for(int i=0; i<tamano; i++)
		{
			if(c[i]>auxCal)
			{
				cout << c[i];
				c[i].Mensaje();

			if(i<tamano-1)
				cout << ", ";
			}
		}
	}
	cout << "]";
}

// REDIMENSIONAR el vector de TCalendario
bool TVectorCalendario::Redimensionar(int tam)
{
	if(tam<=0 || tam==tamano)
		return false;
	else if(tam>tamano)
	{
		TVectorCalendario auxVectCal(tam);
		for(int i=0; i<tamano; i++)
		{
			auxVectCal.c[i]=c[i];
		}
		(*this)=auxVectCal;
		return true;
	}
	else if(tam<tamano)
	{
		TVectorCalendario auxVectCal(tam);
		for(int i=0; i<tam; i++)
		{
			auxVectCal.c[i]=c[i];
		}
		(*this)=auxVectCal;
		return true;
	}
	else return false;
}

// Sobrecarga del operador salida
ostream & operator<<(ostream &os, const TVectorCalendario &vectCal)
{
	os << "[";
	if(vectCal.tamano>0)
	{
		for(int i = 0; i<vectCal.tamano; i++)
		{
			os << "(" << i+1 << ") ";
			os << vectCal.c[i];
			if(i<vectCal.tamano-1)
				os << ", ";
		}
	}
	os << "]";

	return os;
}
