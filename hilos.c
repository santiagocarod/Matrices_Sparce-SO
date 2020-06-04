#include "hilos.h"

//Realizado por Santiago Caro y Juan Carlos Suárez

//Función que se ejecuta por cada hilo. Recorre un número determinado de filas de la matriz empezando desde una fila determinada, 
//contando la cantidad de números distintos de cero y guardando el resultado en uno de los campos de su posición correspondiente del arreglo
//datosHilos.
void* contar(void* arg)
{
	struct DatosHilo* datos; //Variable que apunta a la posición del arreglo datosHilos que le corresponde al hilo
	int noCeros; //Cantidad de números distintos que va a contar el hilo
	int filaFinal; //Fila consecutiva a la última fila que debe ser contada por el hilo
	int i; //Contador para recorrer las filas
	int j; //Contador para recorrer las columnas
	int n; //Cantidad de columnas
	int** matriz; //Matriz leída del archivo
	datos = (struct DatosHilo*) arg;
	noCeros = 0;
	filaFinal = datos -> filaInicio + datos -> nFilas;
	n = datos -> n;
	matriz = datos -> matriz;
	for(i = datos -> filaInicio; i < filaFinal; i++)
	{	
		for(j = 0; j < n; j++)
		{	
			if(matriz[i][j] != 0)
			{	
				noCeros++;
			}
		}
	}
	//El hilo guarda su resultado en su posición del arreglo
	datos -> noCeros = noCeros;
}

//Función que crea hilos de acuerdo a los datos ingresados por el usuario. Recibe las dimensiones de la matriz, la cantidad de hilos, 
//el arreglo de los datos de los hilos, el arreglo de hilos y la matriz.
void crearHilos(int m, int nHilos, struct DatosHilo* datosHilos, pthread_t* hilos, int** matriz, int n)
{
	int i;	
	//En caso de que el número de filas sea menor al número de hilos
	if(m/nHilos==0)
	{	
		for(i=0; i<nHilos; i++)
		{	
			//argumentos para el hilo
			//matriz
			datosHilos[i].matriz = matriz;
			//n
			datosHilos[i].n = n;	
			//número de filas que debe contar			
			if(i<m)
			{
				datosHilos[i].filaInicio = i;				
				datosHilos[i].nFilas = 1;	
			}
			else
			{
				datosHilos[i].filaInicio = 0;				
				datosHilos[i].nFilas = 0;	
			}
			if(pthread_create(&hilos[i], NULL, contar, (void*)&datosHilos[i])!=0)
			{
				perror("Error en pthread_create: ");
				exit(3);
			}	
		}
	}
	else
	{
		for(i=0;i<nHilos;i++)
		{
			//argumentos para el hilo	
			//matriz
			datosHilos[i].matriz = matriz;
			//n
			datosHilos[i].n = n;			
			//fila de inicio
			datosHilos[i].filaInicio = i*(m/nHilos);
			//número de filas que debe contar
			if(i!=nHilos-1)
			{
				//En caso de que no sea el útimo hilo, deberá contar m/nHilos filas			
				datosHilos[i].nFilas = m/nHilos;	
			}
			else
			{
				//En caso de que sea el último hilo, deber contar también el residuo
				datosHilos[i].nFilas = m/nHilos + m%nHilos;
			}
			if(pthread_create(&hilos[i], NULL, contar, (void*)&datosHilos[i])!=0)
			{
				perror("Error en pthread_create: ");
				exit(3);
			}
		}
	}
}

//Función que espera a todos hilos y suma sus resultados. Recibe la cantidad de hilos, el arreglo de hilos y el arreglo de los datos de 
//los hilos. Retorna el total de números distintos de cero contados por todos los hilos.
int hallarTotal(int nHilos, pthread_t* hilos, struct DatosHilo* datosHilos)	
{
	int totalNoCeros = 0;
	int i;
	//Ciclo de pthread_join donde se suman los resultados de los hilos
	for(i=0; i<nHilos; i++)
	{
		if(pthread_join(hilos[i], NULL)!=0)
		{
			perror("Error en pthread_join: ");
			exit(4);
		}
		totalNoCeros += datosHilos[i].noCeros;
	}
	return totalNoCeros;
}
