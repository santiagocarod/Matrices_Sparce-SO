#include "Util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

//Realizado por Santiago Caro y Juan Carlos Suárez

//Esta funcion lee la matriz que esta en un archivo y la devuelve.
//Tiene que identificar si el numero es de mas de dos digitos y guardarlo y despues convertirlo a entero
//Retorna la referencia a una matriz que ya esta llena
int **leer(int **mat, int m, int n, char *archivo)
{
	FILE *f;										 //Apuntador al archibo
	int con;										 //Contador de caracteres
	char c;											 //Caracter donde se guardan los caracteres del archivo conforme son leídos
	char *numero = (char *)malloc(5 * sizeof(char)); //Cadena de caracteres donde se guardan los números del archivo conforme son leídos
	if (numero == NULL)
	{
		perror("Error al asignar memoria:");
		exit(2); //¿Distintos códigos de salida para las distintas llamadas al sistema fallidas?
	}

	/*Asignacion de memoria para cada fila de la matriz*/
	mat = (int **)malloc(m * sizeof(int *));
	if (mat == NULL)
	{
		perror("Error al asignar memoria:");
		exit(2); //¿Distintos códigos de salida para las distintas llamadas al sistema fallidas?
	}
	/*Asignacion de memoria para cada columna de la matriz*/
	for (int i = 0; i < m; i++)
	{
		mat[i] = (int *)malloc(n * sizeof(int));
		if (mat[i] == NULL)
		{
			perror("Error al asignar memoria:");
			exit(2); //¿Distintos códigos de salida para las distintas llamadas al sistema fallidas?
		}
	}

	/*Abre el archivo*/
	f = fopen(archivo, "r");
	/*Si no encuentra el archivo manda un mensaje de error y sale del programa*/
	if (f == NULL)
	{
		perror("Error en el archivo: ");
		exit(1);
	}
	/*Llenar la matriz*/
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			con = 0;
			/*Lee los caracteres uno por uno hasta que uno sea espacio y los va concatenando en un vector de caracteres*/
			c = (char)fgetc(f);
			while (c != 32)
			{
				if (c == EOF)
				{
					perror("Dimensiones incorrectas de la matriz: ");
					exit(5);
				}
				if (con == 0)
				{ //Si es el primer caracter que lee lo copia
					strcpy(numero, &c);
				}
				else
				{
					strcat(numero, &c); //si es mas que el primero lo concatena
				}
				c = (char)fgetc(f); //Lee el siquiente caracter
				con++;
			}
			/*Cuando encuentra el espacio asigna el valor encontrado a la posicion de la matriz y reinicia el vector*/
			mat[i][j] = atoi(numero);
			numero[0] = '\0';
		}
	}
	return mat;
}

//Función que calcula la cantidad de ceros necesarios para que la matriz sea dispersa, de acuerdo a la información ingresada por el usuario. Recibe el porcentaje requrido de ceros y las dimensiones de la matriz. Retorna la cantidad de ceros necesaria para que la matriz sea dispersa.
int hallarCerosNecesarios(double porcentaje, int n, int m)
{
	if (((n * m) * (porcentaje / 100) - (int)((n * m) * (porcentaje / 100))) < 0.5)
	{
		return (int)((n * m) * (porcentaje / 100));
	}
	else
	{
		return (int)((n * m) * (porcentaje / 100)) + 1;
	}
}
