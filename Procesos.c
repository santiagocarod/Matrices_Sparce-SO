#include "Util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include "Procesos.h"

//Realizado por Santiago Caro y Juan Carlos Suárez

//Esta funcion cuanta la cantidad de numeros que no son ceros de la matriz, recibe desde que fila y sabe cuantas filas tiene que
//entonces puede calcular hasta donde tiene que contar.
//Retorna la cantidad de numeros diferentes a cero
int contarCerosProcesos(int **mat, int m, int n, int desde, int cuantas)
{
    float total = m * n;
    float noCeros = 0;
    int hasta = desde + cuantas;
    for (int i = desde; i < hasta; i++) //Solamente calcula las lineas que sean necesarias
    {
        for (int j = 0; j < n; j++)
        {
            if (mat[i][j] != 0) //Cuentas si es diferente de cero
            {
                noCeros++;
            }
        }
    }
    return noCeros; //Retorna la cantidad de numeros diferentes de cero que encontro
}

//Esta funcion se encarga de crear los procesos que son responsables de contar los numeros diferentes de cero
//Crea los procesos y tambien los recoge, si alguno devuelve 255 crea entonces un archivo y cuando recoge los procesos
//lee los archivos para llevar la cuenta final de la cantidad de no ceros.
//Retorna la cantidad de numeros diferentes a ceros total

int contarProcesos(int **mat, int m, int n, int cantidad, int cuantas)
{
    int noCeros = 0;
    int status;
    int total = 0;
    int residuo = m % cantidad;
    int pids[cantidad];
    char numero[10];
    char c;
    int numFinal = 0;
    int con = 0;

    //mkdir("archivos", 0700);

    pid_t pid;
    pid_t end;
    for (int i = 0; i < cantidad; i++)
    {
        pid = fork(); //Creacion de un proceso
        if (pid != 0) //El padre recoge los pid de los hijos
        {
            pids[i] = pid;
        }
        if (pid < 0)
        {
            perror("Problema con el fork: ");
            exit(-1);
        }
        if (pid == 0) //Si es el hijo
        {

            if (i == cantidad - 1)
            { //El proceso recibe las lineas que le tocan mas las que faltan
                noCeros = contarCerosProcesos(mat, m, n, i * cuantas, cuantas + (m % cantidad));
                if (noCeros > 254) //Si la cantidad de ceros es mayor a 254 entonces se tiene que crear un archivo que guarde la informacion.
                {
                    FILE *salida;
                    char nombre[10];
                    sprintf(nombre, "%d", getpid()); //Cambia el numero del proceso a una cadena de caracteres
                    salida = fopen(nombre, "w");     //Abre el archivo con el nombre del proceso en el que se encuentra
                    fprintf(salida, "%d", noCeros);
                    fclose(salida);
                    exit(255); //Retorna 255 como bandera de que tiene que leer el archivo
                }
                else
                {
                    exit(noCeros); //Si es menor entonces lo retorna por medio del exit
                }
            }
            else
            { //Lo mismo que el anterior segmento pero sin el residuo.
                noCeros = contarCerosProcesos(mat, m, n, i * cuantas, cuantas);
                if (noCeros > 254)
                {
                    FILE *salida;
                    char nombre[10];
                    sprintf(nombre, "%d", getpid());
                    salida = fopen(nombre, "w");
                    fprintf(salida, "%d", noCeros);
                    fclose(salida);
                    exit(255);
                }
                else
                {
                    exit(noCeros);
                }
            }
        }
    }

    if (pid != 0 && pid != -1) //Esto solo lo hace el padre
    {

        for (int i = 0; i < cantidad; i++)
        {
            end = waitpid(pids[i], &status, 0);
            if (end != -1)
            {
                if (WIFEXITED(status)) //Si ya termino el proceso
                {
                    int es = WEXITSTATUS(status); //Se lee lo que retorno como exit el proceso
                    if (es == 255)                //Si es 255 tiene que leer el archivo
                    {
                        FILE *leer;
                        char nombre[10];
                        sprintf(nombre, "%d", pids[i]); //El nombre del archivo esta guardado en el arreglo de codigos de procesos
                        leer = fopen(nombre, "r");

                        if (leer) //si existe el archivo
                        {

                            fscanf(leer, "%d", &numFinal); //Lee el numero y lo convierte a entero

                            total += numFinal; //Lo añade a el totla
                        }
                        else
                        {
                            perror("Problema con el archivo: "); //En caso de que no exista el archivo o no lo pueda abrir
                            exit(-1);
                        }
                    }
                    else
                    {
                        total += es; //Si no tiene que leer el archivo entonces solo añade al total
                    }
                }
                else
                {
                    --i; //si todavia no ha acabado el proceso entonces vuelve a revisar
                }
            }
            else
            {
                perror("Problema con el Waitpid :");
                exit(-1);
            }
        }
    }

    return total;
}
