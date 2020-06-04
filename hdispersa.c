#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include "hilos.h"
#include "Util.h"

//Realizado por Santiago Caro y Juan Carlos Suárez

//Programa que, según los argumentos, lee y guarda en memoria matriz leída de un archivo y crea un número determinado de hilos,
//entre los cuales se dividirá el trabajo de revisar toda la matriz contando números distintos de cero, para finalmente determinar si la
//matriz es o no dispersa.
int main(int argc, char** argv)
{
	struct DatosHilo* datosHilos; //Arreglo que contiene los datos de los hilos
	int** matriz; //Matriz donde se copia la matriz del archivo	
	int nHilos; //Número de hilos que se crearán
	int m; //Número de filas de la matriz
	int n; //Número de columnas de la matriz
	int i; //Contador para los ciclos
	int j; //Contador para los ciclos
	int totalNoCeros; //Total de números distintos de cero en la matriz
	int cerosMatriz; //Total de ceros en la matriz
	int cerosNecesarios; //Cantidad de ceros en la matriz para que sea una matriz dispersa
	double porcentaje; //Porcentaje de ceros en la matriz para que sea una matriz dispersa
	char* archivo; //Nombre del archivo que contiene la matriz
	pthread_t *hilos; //Arreglo de los hilos que usará el proceso
	if(argc!=6) {
		/*Comprobacion de los argumentos del programa*/
		printf("./hdispersa M N matrix.txt nprocesos porcentaje\n");
	}
	nHilos = atoi(argv[4]);
	hilos = (pthread_t*) malloc(nHilos*sizeof(pthread_t));
	if(hilos==NULL)
	{
		perror("Error al asignar memoria:");
		exit(2);//Distintos códigos de salida para las distintas llamadas al sistema fallidas
	}
	porcentaje = (double) atoi(argv[5]);	
	//Arreglo con los datos de cada hilo
	datosHilos = (struct DatosHilo*) malloc(nHilos*sizeof(struct DatosHilo));	
	if(datosHilos==NULL)
	{
		perror("Error al asignar memoria:");
		exit(2);//Distintos códigos de salida para las distintas llamadas al sistema fallidas
	}	
	m=atoi(argv[1]);
	n=atoi(argv[2]);
	archivo = argv[3];	
	/*Devuelve la matriz llena leida del archivo*/
	matriz = leer(matriz,m,n,archivo);
	//Se crean los hilos
	crearHilos(m, nHilos, datosHilos, hilos, matriz, n);
	//Se espera a cada uno de los hilos y se suma el total de número distintos de cero
	totalNoCeros = hallarTotal(nHilos, hilos, datosHilos);
	cerosMatriz = m*n - totalNoCeros;
	//Se calcula el total de ceros necesrios para que la matriz sea dispersa
	cerosNecesarios = hallarCerosNecesarios(porcentaje, n, m);
	//Se imprime el resultado
	if(cerosMatriz >= cerosNecesarios)
	{
		printf("La matriz en el archivo %s tiene un total de %d ceros, por lo tanto, se considera dispersa. Se necesitan %d ceros o mas para considerarla dispersa.\n", archivo, cerosMatriz, cerosNecesarios);
	}
	else
	{
		printf("La matriz en el archivo %s tiene un total de %d ceros, por lo tanto, no se considera dispersa. Se necesitan %d ceros o mas para considerarla dispersa.\n", archivo, cerosMatriz, cerosNecesarios);
	}

	
}

