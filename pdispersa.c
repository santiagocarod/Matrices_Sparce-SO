#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include "Util.h"
#include "Procesos.h"

//Realizado por Santiago Caro y Juan Carlos Suárez

//Programa que, según los argumentos, lee y guarda en memoria matriz leída de un archivo y crea un número determinado de procesos hijos,
//entre los cuales se dividirá el trabajo de revisar toda la matriz contando números distintos de cero, para finalmente determinar si la
//matriz es o no dispersa.
int main(int argc, char *argv[])
{
	if (argc != 6)
	{
		/*Comprobacion de los argumentos del programa*/
		printf("./pdispersa M N matrix.txt #Procesos Porcentaje\n");
		printf("Ej. ./Contador 10 10 Mat.txt 5 80\n");
	}
	else
	{
		if (atoi(argv[5]) > 100)
		{
			printf("POR FAVOR INGRESE UN PORCENTAJE ENTRE 0-100\n");
		}
		else
		{
			int m = atoi(argv[1]);
			int cerosNecesarios;
			int n = atoi(argv[2]);
			double total = m * n;
			double porcentaje = (double)atoi(argv[5]);
			int cantidad = atoi(argv[4]);
			int **mat;
			int noCerosP = 0;
			int cuantas = m / cantidad;
			int resultado;
			char *archivo;
			int cerosMatriz;

			archivo = argv[3];

			/*Devuelve la matriz llena leida del archivo*/
			mat = leer(mat, m, n, archivo);

			if (m < cantidad) //En caso de que el usuario ingrese mas procesos de los que filas hay entonces se le asigna a cada proceso una fila.
			{
				cantidad = m;
				cuantas = 1;
			}

			//Llama a una funcion que crea procesos segun los parametros y devuelve la cantidad de numeros que son diferentes de cero.
			noCerosP = contarProcesos(mat, m, n, cantidad, cuantas);

			cerosMatriz = m * n - noCerosP;
			cerosNecesarios = hallarCerosNecesarios(porcentaje, n, m);
			//Se imprime el resultado
			if (cerosMatriz >= cerosNecesarios)
			{
				printf("La matriz en el archivo %s tiene un total de %d ceros, por lo tanto, se considera dispersa. Se necesitan %d ceros o mas para considerarla dispersa.\n", archivo, cerosMatriz, cerosNecesarios);
			}
			else
			{
				printf("La matriz en el archivo %s tiene un total de %d ceros, por lo tanto, no se considera dispersa. Se necesitan %d ceros o mas para considerarla dispersa.\n", archivo, cerosMatriz, cerosNecesarios);
			}
		}
	}
}
