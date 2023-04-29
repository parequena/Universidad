#include <omp.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <sys/resource.h>


// g++ pix.cpp -o pixopencv `pkg-config --cflags --libs opencv`
// g++ `Magick++-config --cxxflags --cppflags` -o pix pix16.cpp `Magick++-config --ldflags --libs`
/*
./pixopencv 5 fea.jpg f1.jpg pajaro4k.jpeg p1.jpg car8k.jpeg c1.jpg earth8k.jpeg e1.jpg
./pixopencv 25 fea.jpg f2.jpg pajaro4k.jpeg p2.jpg car8k.jpeg c2.jpg earth8k.jpeg e2.jpg
./pixopencv 50 fea.jpg f3.jpg pajaro4k.jpeg p3.jpg car8k.jpeg c3.jpg earth8k.jpeg e3.jpg
./pixopencv 100 fea.jpg f4.jpg pajaro4k.jpeg p4.jpg car8k.jpeg c4.jpg earth8k.jpeg e4.jpg

Fotos:
Pruebas: 
*/

using namespace cv;
using namespace std;

Vec3b to16Palette(double r, double g, double b);
void subSector(Mat &ima, int x, int y, int rango, int ancho, int alto);
long double tiempo();

int main(int argc, char ** argv) {
	long double tiempoTotal_1, tiempoTotal_2, tiempoTotal, tiempo1, tiempo1fin;

	if(argc<4) // Si nos pasan menos de los parametros mínimos, devolveremos un Error.
	{
		cerr << "----------------------------------------------------\n";
		cerr << "Error: prueba con " << argv[0] << " <Num_Pixels> <Nombre_Entrada> <Nombre_Salida> ... \n";
		cerr << "----------------------------------------------------\n";
	}
	else if(argc>=4) // Caso: X fotos.
	{
		int tamBigPix = atoi(argv[1]); // Guardamos el número de Pixels pequeños por Pixel Grande
		tiempoTotal_1 = tiempo();

		for(int i = 2; i < argc; i+= 2) // Iteramos en el numero de fotos que queremos pixelizar.
		{
			tiempo1 = tiempo();
			if(argc%2 != 0 && i == argc-1)
				cerr << "ERROR: Falta archivo de destino para " << argv[argc-1] << endl; // Controlamos que todos los ficheros de entrada tengan salida.
			else
			{
				try
				{
					Mat imagen = imread(argv[i]); // Abrimos la imagen de entrada cada vez que itera el bucle.

					int ancho = imagen.rows, alto = imagen.cols; // Guarademos la altura y anchura del archivo con el que trabajamos ahora.
					int k = 0, j = 0;
					
					for(j = 0; j < ancho; j+= tamBigPix) {// Iteramos en el ancho.
						
						for(k = 0; k < alto; k+= tamBigPix) {// Iteramos en el alto.
							subSector(imagen, j, k, tamBigPix, ancho, alto); // Trabajamos con la imagen actual.
						}
					}

					imwrite(argv[i+1], imagen); // Escribimos la imagen que queremos guardar.
					cout << "Procesada la imagen: " << argv[i] << endl; // Informamos al usuario.
				}
				catch (Exception &error_) 
	  			{
	      			cout << "Ha habido algún problema: " << error_.what() << endl; // Capturará una excepcion al no encontrar el archivo de entrada.
	    		} 
	    	}
	    	tiempo1fin = tiempo();
			cout << "Tiempo imagen " << i/2 << ": " << tiempo1fin - tiempo1 << " segs.\n";
		}
			tiempoTotal_2 = tiempo();
			tiempoTotal = tiempoTotal_2 - tiempoTotal_1;
			cerr << "----------------------------------------------------\n";
			cout << "Tiempo Total: " << tiempoTotal << " segs." << endl;
			cerr << "----------------------------------------------------\n";
			cout << "Procesamiento finalizado" << endl; // Avisamos de que se ha finalizado la ejecución.
			return 0;
	}
}

void subSector(Mat &ima, int x, int y, int rango, int ancho, int alto)
{
	double r = 0, g = 0, b = 0;
	Vec3b auxColor;

	for (int i = x; (i < rango+x) && (i < ancho); i++)
	{
		for (int j = y; (j < rango+y) && (j < alto); j++)
		{
			auxColor = ima.at<cv::Vec3b>(i, j);
			//cout << "prueba `" << (int)auxColor.val[2] << "´" << endl;
			//cout << "pruebablue `" << (ima->imageData + ima->widthStep*i)[j*3] << "´" << endl;
			r = r + ((int)auxColor.val[2]);
			g = g + ((int)auxColor.val[1]);
			b = b + ((int)auxColor.val[0]);
		}
	}
	//cout << "HOLARojo: " << r << ". Verde: " << g << ". Azul: " << b << "." << endl;
	r = r/(rango*rango);
	g = g/(rango*rango);
	b = b/(rango*rango);
	//cout << "ADIOSRojo: " << r << ". Verde: " << g << ". Azul: " << b << "." << endl;

	Vec3b color = to16Palette(r, g, b);

	for (int i = x; (i < rango+x) && (i < ancho); i++) {
		for (int j = y; (j < rango+y) && (j < alto); j++) {
			ima.at<cv::Vec3b>(i, j) = color;
		}
	}
}

Vec3b to16Palette(double r, double g, double b) {
	Vec3b aux;
	if(0 <= r && r <= 255 && 0 <= g && g <= 255 && 0 <= b && b <= 255) {
		if(r <= 66) {
			if(g <= 66) {
				if(b <= 66) {
					aux.val[2] = 0;
					aux.val[1] = 0;
					aux.val[0] = 0;
					//Vec3b color16b(0, 0, 0);	// 0 - Black
				  	return aux;
				}
				else if(b <= 198) {
					aux.val[2] = 0;
					aux.val[1] = 0;
					aux.val[0] = 132;
					//Vec3b color16b(0, 0, 132);	// 1 - RedBrown
				  	return aux;
				}
				else {// 199 - 255
					aux.val[2] = 0;
					aux.val[1] = 0;
					aux.val[0] = 255;
					//Vec3b color16b(0, 0, 255);	// 9 - OrangeRed
				  	return aux;
				}
		      }
		      else if(g <= 198) {
				if(b <= 66) {
					aux.val[2] = 0;
					aux.val[1] = 132;
					aux.val[0] = 0;
					//Vec3b color16b(0, 132, 0);	// 2 - Green
				  	return aux;
				}
				else {// 67 - 255
					aux.val[2] = 0;
					aux.val[1] = 132;
					aux.val[0] = 132;
					//Vec3b color16b(0, 132, 132);	// 3 - BrownGreen
				  	return aux;
				}
		      }
		      else // 199 - 255
		      {
				if(b <= 198)
				{
					aux.val[2] = 0;
					aux.val[1] = 255;
					aux.val[0] = 0;
					//Vec3b color16b(0, 255, 0);	// 10 - VividGreen
				  	return aux;
				}
				else // 199 - 255
				{
					aux.val[2] = 0;
					aux.val[1] = 255;
					aux.val[0] = 255;
					//Vec3b color16b(0, 255, 255);	// 11 - Yellow
				  	return aux;
				}
		      }
	    }
	    else if(r <= 175)
		    {
		      if(g <= 66)
		      {
				if(b <= 66)
				{
					aux.val[2] = 132;
					aux.val[1] = 0;
					aux.val[0] = 0;
					//Vec3b color16b(132, 0, 0);	// 4 - DarkBlue
				  	return aux;
				}
				else // 67 - 255
				{
					aux.val[2] = 132;
					aux.val[1] = 0;
					aux.val[0] = 132;
					//Vec3b color16b(132, 0, 132);	// 5 - Purple
				  	return aux;
				}
		      }
		      else // 67 - 255
		      {
			if(b <= 66)
				{
					aux.val[2] = 132;
					aux.val[1] = 132;
					aux.val[0] = 0;
					//Vec3b color16b(132, 132, 0);	// 6 - BlueGreen
				  	return aux;
				}
				else // 67 - 255
				{
					aux.val[2] = 132;
					aux.val[1] = 132;
					aux.val[0] = 132;
					//Vec3b color16b(132, 132, 132);	// 7 - DarkGrey
				  	return aux;
				}
		      }
	    }
	    else if(r <= 215)
	    {
		      if(g <= 66)
		      {
				if(b <= 66)
				{
					aux.val[2] = 132;
					aux.val[1] = 0;
					aux.val[0] = 0;
					//Vec3b color16b(132, 0, 0);	// 4 - DarkBlue
				  	return aux;
				}
				else // 67 - 255
				{
					aux.val[2] = 132;
					aux.val[1] = 0;
					aux.val[0] = 132;
					//Vec3b color16b(132, 0, 132);	// 5 - Purple
				  	return aux;
				}
		      }
		      else if(g <= 165)
		      {
				if(b <= 66)
				{
					aux.val[2] = 132;
					aux.val[1] = 132;
					aux.val[0] = 0;
					//Vec3b color16b(132, 132, 0);	// 6 - BlueGreen
				  	return aux;
				}
				else // 67 - 255
				{
					aux.val[2] = 132;
					aux.val[1] = 132;
					aux.val[0] = 132;
					//Vec3b color16b(132, 132, 132);	// 7 - DarkGrey
				  	return aux;
				}
		      }
		      else if(g <= 225)
		      {
				if(b <= 66)
				{
					aux.val[2] = 132;
					aux.val[1] = 132;
					aux.val[0] = 0;
					//Vec3b color16b(132, 132, 0);	// 6 - BlueGreen
				  	return aux;
				}
				else if(b <= 165)
				{
					aux.val[2] = 132;
					aux.val[1] = 132;
					aux.val[0] = 132;
					//Vec3b color16b(132, 132, 132);	// 7 - DarkGrey
				  	return aux;
				}
				else if(b <= 225)
				{
					aux.val[2] = 198;
					aux.val[1] = 198;
					aux.val[0] = 198;
					//Vec3b color16b(198, 198, 198);	// 8 - LightGrey
				  	return aux;
				}
				else // 226 - 255
				{
					aux.val[2] = 255;
					aux.val[1] = 255;
					aux.val[0] = 255;
					//Vec3b color16b(255, 255, 255);	// 15 - White
				  	return aux;
				}
		      }
		      else // 226 - 255
		      {
				if(b <= 127)
				{
					aux.val[2] = 255;
					aux.val[1] = 255;
					aux.val[0] = 0;
					//Vec3b color16b(255, 255, 0);	// 14 - LightBlue
				  	return aux;
				}
				else // 128 - 255
				{
					aux.val[2] = 255;
					aux.val[1] = 255;
					aux.val[0] = 255;
					//Vec3b color16b(255, 255, 255);	// 15 - White
				  	return aux;
				}
		      }
		    }
	    else // 216 - 255
	    {
		      if(g <= 127)
		      {
				if(b <= 127)
				{
					aux.val[2] = 255;
					aux.val[1] = 0;
					aux.val[0] = 0;
					//Vec3b color16b(255, 0, 0);	// 12 - Blue
				  	return aux;
				}
				else // 128 - 255
				{
					aux.val[2] = 255;
					aux.val[1] = 0;
					aux.val[0] = 255;
					//Vec3b color16b(255, 0, 255);	// 13 - PinkPurple
				  	return aux;
				}	
		      }
		      else // 128 - 255
		      {
		      	if(b <= 127)
				{
					aux.val[2] = 255;
					aux.val[1] = 255;
					aux.val[0] = 0;
					//Vec3b color16b(255, 255, 0);		// 14 - LightBlue
				  	return aux;
				}
				else // 128 - 255
				{
					aux.val[2] = 255;
					aux.val[1] = 255;
					aux.val[0] = 255;
					//Vec3b color16b(255, 255, 255);	// 15 - White
				  	return aux;
				}
		      }
	    }
  }

  cout << "to16Pallete::ERROR: Parámetros recibidos inválidos" << endl;

  aux.val[2] = 0;
  aux.val[1] = 255;
  aux.val[0] = 255;
  //Vec3b color16b(0, 255, 255);
  return aux;
}

long double tiempo() {
	struct rusage usage;
 	getrusage(RUSAGE_SELF,&usage);
 	return( (long double) usage.ru_utime.tv_sec + (long double) usage.ru_utime.tv_usec/1e6 );
}