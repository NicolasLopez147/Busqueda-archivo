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
        printf("EL id ingreado no es valido, debe ser un valor entre 1 y 1160. Ingrese nuevamente el valor\n");
        scanf("%d", a);
    }
}
int formatoHora(int *a)
{
    while (*a < 0 || *a > 23)
    {
        printf("EL hora ingreada no es valido, debe ser un valor entre 0 y 23. Ingrese nuevamente el valor\n");
        scanf("%d", a);
    }
}

void escribirTuberia(int *descriptor, char *tuberia, struct Datos buffer)
{

    *descriptor = open(tuberia, O_WRONLY);
    if (*descriptor < 0)
    {
        perror("Hubo un error abriendo el archivo de la tuberia");
        exit(-1);
    }
    int r = write(*descriptor, &buffer, sizeof(buffer));
    if (r < 0)
    {
        perror("No se pudo escribir el origen en la tuberia");
    }
    close(*descriptor);
}

int main()
{
    int opc = 0;
    // Descriptor del archivo
    int descriptor;
    // Ruta del archivo FIFO (tuberia)
    char *tuberia = "./menuBuscador";
    // Crear archivo de tipo FIFO con sus permisos en octal
    mkfifo(tuberia, 0666);

    // Crea una estructura que guardara los datos
    struct Datos datos;

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
        int viajeMedio = 0;
        switch (opc)
        {
        case 1:

            printf("Ingrese el ID del origen ");
            scanf("%d", &origen);
            idLugar(&origen);
            printf("El id ingresado fue %d\n", origen);
            datos.idOrigen = origen;
            break;

        case 2:

            printf("Ingrese el ID del destino ");
            scanf("%d", &destino);
            idLugar(&destino);
            printf("El id ingresado fue %d\n", destino);
            datos.idDestino = destino;
            break;

        case 3:
            printf("Ingrese hora del dia ");
            scanf("%d", &hora);
            formatoHora(&hora);
            printf("\nLa hora ingrasado fue %d\n", hora);
            datos.hora = hora;
            break;

        case 4:
            escribirTuberia(&descriptor, tuberia, datos);
            break;
        case 5:
            printf("Adios\n");
            // ELimina el archivo FIFO
            unlink(tuberia);
            break;

        default:
            printf("Opcion incorrecta\n");
            break;
        }

    } while (opc != 5);
}