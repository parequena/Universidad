/*
* TCalendario.cpp
*
*  Created on: 03/02/2015
*      Author: pablo
*/

#include "../include/tcalendario.h"
bool TCalendario::comprobarBisiesto(const int year)
{
	if((year%4==0 && year%100!=0) || year%400==0)
		return true;
	return false;
}

bool TCalendario::comprobarAgno(const int agno)
{
	if(agno>=1900)
		return true;
	return false;
}

// Comporobara que la fecha que le entra (dia, mes, agno) es correcta.
bool TCalendario::comprobarFecha(const int day, const int month, const int year)
{
	if((day>=1 && day<=31) && (month>=1 && month<=12) && comprobarAgno(year))
	{
		if(month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12)
			return true;
		else if(day<=30 && (month==4 || month==6 || month==9 || month==11))
			return true;
		else if(month==2)
		{
			if(day<=29 && comprobarBisiesto(year))
				return true;
			else if (day<=28 && !comprobarBisiesto(year))
				return true;
			else return false;
		}
		else return false;
	}
	else return false;
}

// Sumara tantos dias como le indiques.
TCalendario & TCalendario::sumaDias(const int numero)
{
	for(int i=0; i<numero; i++)
	{
		if(comprobarFecha(this->Dia()+1, this->Mes(), this->Anyo()))
			this->dia++;
		else if(comprobarFecha(1, this->Mes()+1, this->Anyo()))
		{
			this->dia=1;
			this->mes++;
		}
		else if(comprobarFecha(1, 1, this->Anyo()+1))
		{
			this->dia=1;
			this->mes=1;
			this->anyo++;
		}
	}
	return *this;
}

// Restara tantos dias como indiques.
TCalendario & TCalendario::restaDias(const int numero)
{
	bool sigue=true;
	for(int i=0; (i<numero && sigue); i++)
	{
		if(this->dia==1 && this->mes==1 && this->anyo==1900)
			sigue=false;
		if(comprobarFecha(this->Dia()-1, this->Mes(), this->Anyo()))
			this->dia--;
		else if(comprobarFecha(31, this->Mes()-1, this->Anyo()))
		{
			this->dia=31;
			this->mes--;
		}
		else if(comprobarFecha(30, this->Mes()-1, this->Anyo()))
		{
			this->dia=30;
			this->mes--;
		}
		else if(comprobarFecha(29, this->Mes()-1, this->Anyo()))
		{
			this->dia=29;
			this->mes--;
		}
		else if(comprobarFecha(28, this->Mes()-1, this->Anyo()))
		{
			this->dia=28;
			this->mes--;
		}
		else if(comprobarFecha(31, 12, this->Anyo()-1))
		{
			this->dia=31;
			this->mes=12;
			this->anyo--;
		}
	}
	if(!sigue && this->mensaje!=NULL)
	{
		delete[] this->mensaje;
		this->mensaje=NULL;
	}
	return *this;
}

TCalendario::TCalendario()
{
	dia = 1;
	mes = 1;
	anyo = 1900;
	mensaje = NULL;
}

TCalendario::TCalendario(const int dia, const int mes, const int anyo, const char* mens)
{

	if(comprobarFecha(dia, mes, anyo))
	{
		this->dia=dia;
		this->mes=mes;
		this->anyo=anyo;
		this->mensaje=NULL;
		this->ModMensaje(mens);
	}
	else
	{
		this->dia=1;
		this->mes=1;
		this->anyo=1900;
		this->mensaje=NULL;
	}
}

TCalendario::TCalendario(const TCalendario &calend)
{
	dia=calend.dia;
	mes=calend.mes;
	anyo=calend.anyo;
	this->mensaje=NULL;
	this->ModMensaje(calend.mensaje);
}

TCalendario::~TCalendario()
{
	dia=1;
	mes=1;
	anyo=1900;
	mensaje=NULL;
}

TCalendario & TCalendario::operator=(const TCalendario &calend)
{
	if(this!=&calend)
	{
		this->~TCalendario();
		dia=calend.dia;
		mes=calend.mes;
		anyo=calend.anyo;
		this->ModMensaje(calend.mensaje);
	}
	return *this;
}


// Sobrecarga del operador SUMA de fecha + un número de dias;
TCalendario TCalendario::operator+(const int numero)
{
	TCalendario auxCalendario((*this));
	auxCalendario.sumaDias(numero);
	return auxCalendario;
}
// Sobrecarga del operador RESTA de fecha - un número de dias;
TCalendario TCalendario::operator-(const int numero)
{
	TCalendario auxCalendario((*this));
	auxCalendario.restaDias(numero);
	return auxCalendario;
}
// Modifica la fecha incrementandola en un dia (con postincremento);
TCalendario TCalendario::operator++(const int numero)
{
	TCalendario auxCalendario ((*this));
	this->sumaDias(1);
	return auxCalendario;
}
// Modifica la fecha incrementandola en un dia (con preincremento);
TCalendario & TCalendario::operator++()
{
	this->sumaDias(1);
	return *this;
}

// Modifica la fecha decrementándola en un dia (con postdecremento);
TCalendario TCalendario::operator--(const int numero)
{
	TCalendario auxCalendario (*this);
	this->restaDias(1);
	return auxCalendario;
}
// Modifica la fecha decrementándola en un día (con predecremento);
TCalendario & TCalendario::operator--()
{
	this->restaDias(1);
	return (*this);
}

// Modifica la fecha
bool TCalendario::ModFecha (int dia, int mes, int anyo)
{
	if(comprobarFecha(dia, mes, anyo))
	{
		this->dia=dia;
		this->mes=mes;
		this->anyo=anyo;

		return true;
	}
	return false;
}
// Modifica el mensaje
bool TCalendario::ModMensaje(const char * mens)
{
	if(mensaje!=NULL)
	{
		delete[] mensaje;
		mensaje=NULL;
	}
	if(mens!=NULL)
	{
		mensaje = new char[strlen(mens) + 1];
		strcpy(mensaje, mens);
		return true;
	}
	return false;
}
// Sobrecarga del operador igualdad;
bool TCalendario::operator==(const TCalendario &calendario) const
{
	if(calendario > *this || calendario < *this)
		return false;
	else return true;
}

// Sobrecarga del operador desigualdad;
bool TCalendario::operator!=(const TCalendario &calendario) const
{
	if(!((*this)==calendario))
		return true;
	else return false;
}
// Sobrecarga del operador >; (ver ACLARACIÓN sobre ORDENACIÓN)
bool TCalendario::operator>(const TCalendario &calendario) const
{
	if(anyo>calendario.anyo)
		return true;
	else if (anyo==calendario.anyo && mes>calendario.mes)
		return true;
	else if (anyo==calendario.anyo && mes==calendario.mes && dia>calendario.dia)
		return true;
	else if (anyo==calendario.anyo && mes==calendario.mes && dia==calendario.dia)
	{
		if(mensaje==NULL && calendario.mensaje==NULL)
			return false;
		else if(mensaje!=NULL && calendario.mensaje==NULL)
			return true;
		else if(mensaje==NULL && calendario.mensaje!=NULL)
			return false;
		else if(strcmp(mensaje,calendario.mensaje)==1)
			return true;
		else return false;
	}
	else return false;
}
// Sobrecarga del operador <; (ver ACLARACIÓN sobre ORDENACIÓN)
bool TCalendario::operator<(const TCalendario &calendario) const
{
	if(anyo<calendario.anyo)
		return true;
	else if (anyo==calendario.anyo && mes<calendario.mes)
		return true;
	else if (anyo==calendario.anyo && mes==calendario.mes && dia<calendario.dia)
		return true;
	else if (anyo==calendario.anyo && mes==calendario.mes && dia==calendario.dia)
	{
		if(mensaje==NULL && calendario.mensaje==NULL)
			return false;
		else if(mensaje!=NULL && calendario.mensaje==NULL)
			return false;
		else if(mensaje==NULL && calendario.mensaje!=NULL)
			return true;
		else if(strcmp(mensaje,calendario.mensaje)==-1)
			return true;
		else return false;
	}
	else return false;
}

//TCalendario vacío
bool TCalendario::EsVacio() const
{
	if(dia==1 && mes==1 && anyo==1900 && mensaje==NULL)
		return true;
	return false;
}

ostream &operator<<(ostream &os, const TCalendario &calendario)
{
	if(calendario.dia<10)
		os<<"0";
	os<< calendario.dia;

	os<<"/";

	if(calendario.mes<10)
		os<<"0";
	os<< calendario.mes;

	os<<"/";

	os<<calendario.anyo << " \"";

	if(calendario.mensaje!=NULL)
		os<< calendario.mensaje;
	os << "\"";

	return os;
}
