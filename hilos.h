#ifndef HILOS_H_ 
#define HILOS_H_
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

struct DatosHilo
{
	int filaInicio; //Fila desde donde empieza a contar el hilo
	int nFilas; //Número de filas que debe contar el hilo
	int noCeros; //Variable donde el hilo guarda la cantidad de números distintos de cero contados en sus filas
	int** matriz; //Matriz leída del archivo
	int n; ////Cantidad de columnas de la matriz. Se incluye para que los hilos puedan usarla al momento de recorrer sus filas.
};

//Función que se ejecuta por cada hilo. Recorre un número determinado de filas de la matriz empezando desde una fila determinada, contando la cantidad de números distintos de cero y guardando el resultado en uno de los campos del arreglo global datosHilos.
void* contar(void* arg);

void crearHilos(int m, int nHilos, struct DatosHilo* datosHilos, pthread_t* hilos, int** matriz, int n);

int hallarTotal(int nHilos, pthread_t* hilos, struct DatosHilo* datoshilos);


#endif
