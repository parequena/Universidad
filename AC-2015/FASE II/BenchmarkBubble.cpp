#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

void bubbleSort(int* vector, int tam0);
const int kTAM = 5000;
const int seed = 0;

void mostrar(int tam, int* vector)
{
	cout << "{";
	for (int j = 0; j < tam; j++)
	{
		cout << vector[j];
		if (j<tam - 1)
			cout << ",";
	}
	cout << "}\n";
}

int main()
{
	long double inicio, final, total = 0;
	int vector1[kTAM];
	int vector2[kTAM * 2];
	int vector3[kTAM * 3];
	int tam0 = 0;
	srand(seed);

	for (int i = 1; i <= 3; i++)
	{
		int tam0 = kTAM*i;
		for (int j = 0; j < tam0; j++)
		{
			if (i == 1)
				vector1[j] = rand();
			if (i == 2)
				vector2[j] = rand();
			if (i == 3)
				vector3[j] = rand();
		}
		if (i == 1)
		{
			cout << "Vector1 desordenado ";
			//mostrar(tam0, vector1);
			cout << endl;
			inicio = clock();
			bubbleSort(vector1, tam0);
			final = clock();
			cout << "Vector1 ordenado ";
			//mostrar(tam0, vector1);
			cout << endl << endl;
		}
		if (i == 2)
		{
			cout << "Vector2 desordenado ";
			//mostrar(tam0, vector2);
			cout << endl;
			inicio = clock();
			bubbleSort(vector2, tam0);
			final = clock();
			cout << "Vector2 ordenado ";
			//mostrar(tam0, vector2);
			cout << endl << endl;
		}
		if (i == 3)
		{
			cout << "Vector3 desordenado ";
			//mostrar(tam0, vector3);
			cout << endl;
			inicio = clock();
			bubbleSort(vector3, tam0);
			final = clock();
			cout << "Vector3 ordenado ";
			//mostrar(tam0, vector3);
			cout << endl << endl;
		}
		total = (final - inicio) / CLK_TCK;
		cout << "Vector de tamagno " << kTAM*i << ", ordenado en: " << total << ".\n";
		getchar();
	}
}



void bubbleSort(int* vector, int tam)
{
	int i = 0;
	int j = 0;
	__asm
	{
	for_ext:
			mov eax, i; // eax=0 (i)-
			mov ebx, tam; // ebx=tam (Tama�o del vector).
			cmp eax, ebx - 1; // Compara 0 y tam-1.
			jnc final; // Si se cumple, saltar� al final de los bucles.
			
			mov j, ebx - 1; // Si no, pone j=tam-1.

		for_int:
			mov ebx, j; // ebx=j.
			cmp i, ebx; // Compara i y ebx (j)
			jnc increase; // Saltar� a incrementar la i.
			
			mov ecx, vector; // Si no, trabajamos con el vector
			mov eax, [ecx + 4 * ebx]; // eax = vector[j] ecx-> vector + 4 -> bits * ebx -> j.
			dec ebx; // Decrementa ebx (j) en 1.
			mov edx, [ecx + 4 * ebx]; // edx = vector[j - 1] ecx-> vector + 4 -> bits * ebx -> j.
			inc ebx; // Incrementa ebx (j) en 1.
			cmp eax, edx; // Compara el contenido de las posiciones vector[j] y vector[j - 1].
			jnc decrease; // Si vector[j] < vector[j - 1] entonces satar� a decrease
			
			mov[ecx + 4 * ebx], edx; // Si no, har� el swap vector[j] = edx = vector[j - 1]
			dec ebx; // Decrementar� j, para seguir trabajando con �l.
			mov[ecx + 4 * ebx], eax; // Otro paso del swap vector[j - 1] = eax = vector[j]
			jmp for_int; // Y volver� al principio

		increase :
			inc i; // Incrementar� el valor de la i.
			jmp for_ext; // Saltar� al for_ext.

		decrease :
			dec j; // Decrementar� el valor de j.
			jmp for_int; // Saltar� al for_int

		final: // Final del algoritmo.

	}
}