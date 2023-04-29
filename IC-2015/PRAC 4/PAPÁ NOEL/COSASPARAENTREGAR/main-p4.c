// Pr�ctica 1-2
// Ingenier�a de Computadores
//
// Esteban Platero Horcajadas
// Manuel Platero Horcajadas
// Mar�a Luisa Risue�o Gonz�lez
// Gacel Ivorra Rodr�guez

#include <dirent.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <jpeglib.h>

#include <mpi.h>
#include <unistd.h>

#include <string.h>

#define NUM_PRUEBAS 5

#define min(a, b) (((a) < (b)) ? (a) : (b)) 
#define max(a, b) (((a) > (b)) ? (a) : (b)) 

/* Puntero para guardar las imagenes */
unsigned char *imagenOriginal = NULL;
unsigned char *imagenModificada = NULL;

/* Dimensiones de la imagen que queremos escribir */
int ancho = 0;
int alto = 0;
int numeroPixeles = 0;
int bytesPorPixel = 3;   /*RGB 3 canales*/
int espacioDeColores = JCS_RGB; 

// Representación de las imágenes
struct ImagenHandler {
	unsigned char* rojo;				// Canal rojo de la imágen original
	unsigned char* verde;				// Canal verde de la imágen original
	unsigned char* azul;				// Canal azul de la imágen original
	unsigned char* rojoDesenfocado;		// Canal rojo de la imágen modificada (desenfocada)
	unsigned char* verdeDesenfocado;	// Canal verde de la imágen modificada (desenfocada)
	unsigned char* azulDesenfocado;		// Canal azul de la imágen modificada (desenfocada)
	float* filtro;						// Filtro gaussiano
};

unsigned char* canal;
unsigned char* canalDesenfocado;

// Coeficientes de peso para la función del desenfoque gaussiano
struct FiltroGaussiano {
	float* peso;						// Array de pesos
	int ancho;							// Ancho del filtro
};

struct ImagenHandler m_handler;
struct FiltroGaussiano m_filtro;


struct DatosCliente {
	int numeroPixeles;
	int anchoFiltro;
	int filas;
	int columnas;
};

struct DatosCliente m_cliente;

int leerImagen( char *fichero )
{
	/* Estructura estandar para lectura y descompresion */
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	/* Estructura para guardar una fila */
	JSAMPROW row_pointer[1];
	
	FILE *ficheroEntrada = fopen( fichero, "rb" );
	unsigned long location = 0;
	int i = 0;
	
	if ( !ficheroEntrada )
	{
		printf("Error opening jpeg file %s\n!", fichero );
		return -1;
	}

	cinfo.err = jpeg_std_error( &jerr );
	/* Creamos la descompresion */
	jpeg_create_decompress( &cinfo );
	/* Lectura desde el archivo */
	jpeg_stdio_src( &cinfo, ficheroEntrada );
	/* Leer la cabecera de la imagen */
	jpeg_read_header( &cinfo, TRUE );
	/* Extraemos alto, ancho y calculamos numero de pixeles */
	ancho = cinfo.image_width;
	alto = cinfo.image_height;
	numeroPixeles = ancho * alto;

	jpeg_start_decompress( &cinfo );

	/* Reservamos memoria para la imagen original y modificada */
	imagenOriginal = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );
	imagenModificada = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );
	/* Leemos la imagen */
	row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );
	/* Leemos linea a linea la imagen */
	while( cinfo.output_scanline < cinfo.image_height )
	{
		jpeg_read_scanlines( &cinfo, row_pointer, 1 );
		for( i=0; i<cinfo.image_width*cinfo.num_components;i++) 
			imagenOriginal[location++] = row_pointer[0][i];
	}
	/* Comprimimos la imagen y limpiamos memoria */
	jpeg_finish_decompress( &cinfo );
	jpeg_destroy_decompress( &cinfo );
	free( row_pointer[0] );
	fclose( ficheroEntrada );

	return 1;
}

void reservarMemoriaCanalesServidor()
{
	// Reservar memoria para los canales
	m_handler.rojo = (unsigned char *) malloc(numeroPixeles * sizeof(unsigned char));
	m_handler.verde = (unsigned char *) malloc(numeroPixeles * sizeof(unsigned char));
	m_handler.azul = (unsigned char *) malloc(numeroPixeles * sizeof(unsigned char));

	// Reservar memoria para los canales desenfocados
	m_handler.rojoDesenfocado = (unsigned char *) malloc(numeroPixeles * sizeof(unsigned char));
	m_handler.verdeDesenfocado = (unsigned char *) malloc(numeroPixeles * sizeof(unsigned char));
	m_handler.azulDesenfocado = (unsigned char *) malloc(numeroPixeles * sizeof(unsigned char));
}

void reservarMemoriaCanalCliente()
{
	// Reservar memoria para el canales
	canal = (unsigned char *) malloc(numeroPixeles * sizeof(unsigned char));

	// Reservar memoria para el canales desenfocado
	canalDesenfocado = (unsigned char *) malloc(numeroPixeles * sizeof(unsigned char));
}

void crearFiltroGaussiano()
{
	int r = 0;
	int c = 0;

	const int ancho = 9;
	const float sigma = 2.f;

	const int mitad = ancho / 2;
	float sum = 0.f;

	m_filtro.ancho = ancho;

	// Para la convolution matrix
	m_filtro.peso = (float *) malloc(ancho * ancho * sizeof(float));

	// Calcular filter sum.
	for (r = -mitad; r <= mitad; ++r)
	{
		for (c = -mitad; c <= mitad; ++c)
		{
			float peso = expf( (float) (c * c + r * r) / (2.f * sigma * sigma));
			int idx = (r + mitad) * ancho + c + mitad;

			m_filtro.peso[idx] = peso;
			sum += peso;
		}
	}

	// Normalizar pesos: la suma de los pesos debe ser igual a 1
	float normal = 1.f / sum;

	for (r = -mitad; r <= mitad; ++r)
	{
		for (c = -mitad; c <= mitad; ++c)
		{
			int idx = (r + mitad) * ancho + c + mitad;

			m_filtro.peso[idx] *= normal;
		}
	}
}

unsigned char calcularDesenfoquePixel(unsigned char* canalEntrada, int r, int c, int filas, int columnas) {
	int i = 0;
	int j = 0;

	int mitad = m_filtro.ancho / 2;
	int ancho = columnas - 1;
	int alto = filas - 1;

	float desenfoque = 0.f;

	for (i = -mitad; i <= mitad; ++i)
	{
		for (j = -mitad; j <= mitad; ++j)
		{	
			desenfoque += (float)canalEntrada[columnas * min(max(r + i, 0), alto) + min(max(c + j, 0), ancho)] * m_filtro.peso[(i + mitad) * m_filtro.ancho + j + mitad];
		}
	}

	return (unsigned char) desenfoque;
}

void calcularDesenfoqueCanal(unsigned char* canalEntrada, unsigned char* canalModificado, int filas, int columnas, int inicio)
{
	int r = 0;
	int c = 0;

	for (r = inicio; r < filas; ++r)
	{
		for (c = 0; c < columnas; ++c)
		{
			canalModificado[columnas * r + c] = calcularDesenfoquePixel(canalEntrada, r, c, filas, columnas);
		}
	}
}

void desenfoqueGaussianoCanal(int rank)
{
	int p = 0;
	int i = 0;
	
	// Calcular desenfoque por cada canal individual
	if (rank == 0) {
		calcularDesenfoqueCanal(m_handler.rojo, m_handler.rojoDesenfocado, alto/2, ancho, 0);
	}
	/*else if (rank == 3) {
		int inicio = alto/2;
		calcularDesenfoqueCanal(m_handler.rojo, m_handler.rojoDesenfocado, alto, ancho, inicio);
	}*/
	else {
		int inicio = 0;
		if (rank >= 3) {
			inicio = alto/2;
			calcularDesenfoqueCanal(canal, canalDesenfocado, alto, ancho, inicio);
		} else {
			calcularDesenfoqueCanal(canal, canalDesenfocado, alto/2, ancho, inicio);
		}
	}
}

int guardarImagen( char *fichero )
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	JSAMPROW row_pointer[1];
	FILE *ficheroSalida = fopen( fichero, "wb" );

	if ( !ficheroSalida )
	{
		printf("Error opening output jpeg file %s\n!", fichero );
		return -1;
	}
	cinfo.err = jpeg_std_error( &jerr );
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, ficheroSalida);

	/* Parametros de la imagen de salida */
	cinfo.image_width = ancho;	
	cinfo.image_height = alto;
	cinfo.input_components = bytesPorPixel;
	cinfo.in_color_space = espacioDeColores;

	jpeg_set_defaults( &cinfo );
	jpeg_start_compress( &cinfo, TRUE );
	/* Escribimos la imagen linea a linea */
	while( cinfo.next_scanline < cinfo.image_height )
	{
		row_pointer[0] = &imagenModificada[ cinfo.next_scanline * cinfo.image_width *  cinfo.input_components];
		jpeg_write_scanlines( &cinfo, row_pointer, 1 );
	}
	/* Y limpiamos la memoria */
	jpeg_finish_compress( &cinfo );
	jpeg_destroy_compress( &cinfo );
	fclose( ficheroSalida );
	
	return 1;
}

void liberarMemoriaServidor()
{
	// Liberar datos del PC
	free(m_handler.rojo);
	free(m_handler.verde);
	free(m_handler.azul);
	free(m_handler.rojoDesenfocado);
	free(m_handler.verdeDesenfocado);
	free(m_handler.azulDesenfocado);
}

void liberarMemoriaCliente()
{
	// Liberar datos del PC
	free(canal);
	free(canalDesenfocado);
}

int numprocs, rank, namelen;
char processor_name[MPI_MAX_PROCESSOR_NAME];

void aplicarDesenfoqueCliente(int rank) {
	crearFiltroGaussiano();

	clock_t desenfoque_ini = clock();
	desenfoqueGaussianoCanal(rank);
	clock_t desenfoque_fin = clock();

	//double desenfoque_secs = (double)(desenfoque_fin - desenfoque_ini) / CLOCKS_PER_SEC;
	//printf( "[%02d/%02d %s]: %f\n", rank, numprocs, processor_name, desenfoque_secs );
	//printf( "%f;\n", (desenfoque_secs * 1000.0) );
}

int main( int argc, char** argv )
{
	int i = 0;
	int p = 0;
	
	double tiempo_ini;
	//clock_t tiempo_ini = clock();
	
	MPI_Status stat;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);
	
	tiempo_ini = MPI_Wtime();
	
	printf( "[%02d/%02d %s]\n", rank, numprocs, argv[1] );
	//printf( "[%02d/%02d %s]\n", rank, numprocs, processor_name );
	
	if (rank == 0) {
		if(leerImagen(argv[1]) > 0) {
			printf("%s\n", argv[1]);
			reservarMemoriaCanalesServidor();
			
			// Separar los canales
			for (p = 0; p < numeroPixeles*3; p=p+3) {
				m_handler.rojo[i] = imagenOriginal[p + 0];
				m_handler.verde[i] = imagenOriginal[p + 1];
				m_handler.azul[i] = imagenOriginal[p + 2];
				i++;
			}
			
			MPI_Send(&numeroPixeles, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			MPI_Send(&alto, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			MPI_Send(&ancho, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			MPI_Send(&m_handler.verde[0], numeroPixeles, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
			
			MPI_Send(&numeroPixeles, 1, MPI_INT, 4, 0, MPI_COMM_WORLD);
			MPI_Send(&alto, 1, MPI_INT, 4, 0, MPI_COMM_WORLD);
			MPI_Send(&ancho, 1, MPI_INT, 4, 0, MPI_COMM_WORLD);
			MPI_Send(&m_handler.verde[0], numeroPixeles, MPI_CHAR, 4, 0, MPI_COMM_WORLD);
			
			MPI_Send(&numeroPixeles, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
			MPI_Send(&alto, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
			MPI_Send(&ancho, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
			MPI_Send(&m_handler.azul[0], numeroPixeles, MPI_CHAR, 2, 0, MPI_COMM_WORLD);

			MPI_Send(&numeroPixeles, 1, MPI_INT, 5, 0, MPI_COMM_WORLD);
			MPI_Send(&alto, 1, MPI_INT, 5, 0, MPI_COMM_WORLD);
			MPI_Send(&ancho, 1, MPI_INT, 5, 0, MPI_COMM_WORLD);
			MPI_Send(&m_handler.azul[0], numeroPixeles, MPI_CHAR, 5, 0, MPI_COMM_WORLD);

			MPI_Send(&numeroPixeles, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
			MPI_Send(&alto, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
			MPI_Send(&ancho, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
			MPI_Send(&m_handler.rojo[0], numeroPixeles, MPI_CHAR, 3, 0, MPI_COMM_WORLD);
			
			clock_t desenfoque_ini = clock();
			aplicarDesenfoqueCliente(rank);
			MPI_Recv(&m_handler.verdeDesenfocado[0], numeroPixeles / 2, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &stat);
			clock_t desenfoque_fin = clock();
			double desenfoque_secs = (double)(desenfoque_fin - desenfoque_ini) / CLOCKS_PER_SEC;
			printf( "[%02d/%02d Tiempo desenfoque %f]\n", rank, numprocs, desenfoque_secs );
			
			MPI_Recv(&m_handler.azulDesenfocado[0], numeroPixeles / 2, MPI_CHAR, 2, 0, MPI_COMM_WORLD, &stat);
			MPI_Recv(&m_handler.rojoDesenfocado[numeroPixeles / 2], numeroPixeles / 2, MPI_CHAR, 3, 0, MPI_COMM_WORLD, &stat);
			MPI_Recv(&m_handler.verdeDesenfocado[numeroPixeles / 2], numeroPixeles / 2, MPI_CHAR, 4, 0, MPI_COMM_WORLD, &stat);
			MPI_Recv(&m_handler.azulDesenfocado[numeroPixeles / 2], numeroPixeles / 2, MPI_CHAR, 5, 0, MPI_COMM_WORLD, &stat);
			
			i = 0;
		
			for (p = 0; p < numeroPixeles*3; p=p+3) {
				imagenModificada[p + 0] = m_handler.rojoDesenfocado[i];
				imagenModificada[p + 1] = m_handler.verdeDesenfocado[i];
				imagenModificada[p + 2] = m_handler.azulDesenfocado[i];
				i++;
			}
			
			//char salida[255];
			char salida[5] = { '2', '.', 'j', 'p', 'g' };
			//char* carpeta = "salida/";

			//strcpy(salida, carpeta);
			//strcat(salida, entrada);
		
			guardarImagen(salida);
			liberarMemoriaServidor();
			
			printf("\n");
		}
	}
	else {
		MPI_Recv(&numeroPixeles, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
		MPI_Recv(&alto, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
		MPI_Recv(&ancho, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
		reservarMemoriaCanalCliente();
		MPI_Recv(canal, numeroPixeles, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &stat);
		
		printf( "[%02d/%02d] aplicarDesenfoqueCliente(%02d)\n", rank, numprocs, rank );
		aplicarDesenfoqueCliente(rank);
		
		if (rank < 3)
			MPI_Send(&canalDesenfocado[0], numeroPixeles/2, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
		else
			MPI_Send(&canalDesenfocado[numeroPixeles/2], numeroPixeles/2, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
		
		liberarMemoriaCliente();
	}
	
	double tiempo_fin;
	//clock_t tiempo_fin = clock();
	tiempo_fin = MPI_Wtime();
	
	double tiempo_secs = (double)(tiempo_fin - tiempo_ini);
	printf( "[%02d/%02d %s]: %f\n", rank, numprocs, processor_name, (tiempo_secs * 1000.0) );

		
	MPI_Finalize();
	
	return 0;
}
