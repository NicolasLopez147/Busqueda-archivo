#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "datos.h"

int idLugar(int *a)
{
    while (*a < 1 || *a > 1160)
    {
<<<<<<< HEAD
        printf("El id ingresado no es valido; debe ser un valor entre 1 y 1160. Ingrese nuevamente el valor\n");
=======
        printf("EL id ingreado no es valido, debe ser un valor entre 1 y 1160. Ingrese nuevamente el valor\n");
>>>>>>> c0395cb18bce0fe93d51cfccb601a82fdf75b4d3
        scanf("%d", a);
    }
}
int formatoHora(int *a)
{
    while (*a < 0 || *a > 23)
    {
<<<<<<< HEAD
        printf("La hora ingresada no es valida; debe ser un valor entre 0 y 23. Ingrese nuevamente el valor\n");
=======
        printf("EL hora ingreada no es valido, debe ser un valor entre 0 y 23. Ingrese nuevamente el valor\n");
>>>>>>> c0395cb18bce0fe93d51cfccb601a82fdf75b4d3
        scanf("%d", a);
    }
}

void escribirTuberia(char *tuberia, struct Datos buffer, int tamano)
{
    // Descriptor del archivo
    int descriptor = open(tuberia, O_WRONLY);
    if (descriptor < 0)
    {
        perror("Hubo un error abriendo el archivo de la tuberia");
        exit(-1);
    }
    int r = write(descriptor, &buffer, sizeof(buffer));
    if (r < 0)
    {
        perror("No se pudo escribir el origen en la tuberia");
    }
    close(descriptor);
}

void leerTuberia(char *tuberia, struct Datos *buffer, int tamano)
{
    // Descriptor del archivo
    int descriptor;
    // Se abre la tuberia
    do
    {
        // Espera a que se cree el archivo tipo FIFO memuBuscador
        descriptor = open(tuberia, O_RDONLY);
    } while (descriptor == -1);

    if (descriptor < 0)
    {
        perror("Hubo un error abriendo el archivo de la tuberia");
        exit(-1);
    }
    // Lee el archivo FIFO y guarla la informacion en buffer
    int r = read(descriptor, buffer, tamano);
    if (r < 0)
    {
        perror("Hubo un error leyendo el archivo de la tuberia");
        exit(-1);
    }
    close(descriptor);
}

int main()
{
    int opc = 0;
    // Ruta del archivo FIFO (tuberia)
    char *tuberia = "./menuBuscador";
    char *tuberia2 = "./buscadorMenu";
    // Crear archivo de tipo FIFO con sus permisos en octal
    mkfifo(tuberia, 0666);

    // Crea una estructura que guardara los datos
    struct Datos *datos;

    do
    {
        printf("Bienvenido\n\n");
        printf("1. Ingresar origen\n");
        printf("2. Ingresar destino\n");
        printf("3. Ingresar hora\n");
        printf("4. Buscar tiempo de viaje medio\n");
        printf("5. Salir\n");
        scanf("%d", &opc);
        int origen;
        int destino;
        int hora;
        switch (opc)
        {
        case 1:
            printf("Ingrese el ID del origen ");
            scanf("%d", &origen);
            idLugar(&origen);       // Revision de valores
            printf("\nEl id ingresado fue %d\n", origen);
            datos->idOrigen = origen;
            system("cls||clear");
            break;

        case 2:
            printf("Ingrese el ID del destino ");
            scanf("%d", &destino);
            idLugar(&destino);      // Revision de valores
            printf("\nEl id ingresado fue %d\n", destino);
            datos->idDestino = destino;
            system("cls||clear");
            break;

        case 3:
            printf("Ingrese hora del dia ");
            scanf("%d", &hora);
            formatoHora(&hora);     // Revision de valores
            printf("\nLa hora ingresada fue %d\n", hora);
            datos->hora = hora;
            system("cls||clear");
            break;

        case 4:
            system("cls||clear");
            escribirTuberia(tuberia, *datos, sizeof(*datos));
            leerTuberia(tuberia2, datos, sizeof(*datos));
            if(datos->idOrigen == -1) {
                // printf("No hay registros con los parametros indicados\n");
                printf("NA\n");
                break;
            }
            printf("Se encontro el registro %d %d %d ", datos->idOrigen, datos->idDestino, datos->hora);
            printf("con un tiempo de viaje medio de %f\n", datos->mediaViaje);
            break;

        case 5:
            system("cls||clear");
            printf("Adios\n");
            // ELimina el archivo FIFO
            unlink(tuberia);
            unlink(tuberia2);
            break;

        default:
            printf("Opcion incorrecta\n");
            break;
        }

    } while (opc != 5);
}