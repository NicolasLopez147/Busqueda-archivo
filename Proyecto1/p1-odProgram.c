#include "datos.h"

// Limita los posibles valores que puede tomar los id de origen y de llegada
int idLugar(int *id)
{
    while (*id < 1 || *id > 1160)
    {
        printf("El id ingresado no es valido; debe ser un valor entre 1 y 1160. Ingrese nuevamente el valor\n");
        scanf("%d", id);
    }
}

// Limita la hora ingresada
int formatoHora(int *hora)
{
    while (*hora < 0 || *hora > 23)
    {
        printf("La hora ingresada no es valida; debe ser un valor entre 0 y 23. Ingrese nuevamente el valor\n");
        scanf("%d", hora);
    }
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
            idLugar(&origen); // Revision de valores
            printf("\nEl id ingresado fue %d\n", origen);
            datos->idOrigen = origen;
            break;

        case 2:
            printf("Ingrese el ID del destino ");
            scanf("%d", &destino);
            idLugar(&destino); // Revision de valores
            printf("\nEl id ingresado fue %d\n", destino);
            datos->idDestino = destino;
            break;

        case 3:
            printf("Ingrese hora del dia ");
            scanf("%d", &hora);
            formatoHora(&hora); // Revision de valores
            printf("\nLa hora ingresada fue %d\n", hora);
            datos->hora = hora;
            break;

        case 4:
            escribirTuberia(tuberia, *datos, sizeof(*datos));
            leerTuberia(tuberia2, datos, sizeof(*datos));
            if (datos->idOrigen == -1)
            {
                // printf("No hay registros con los parametros indicados\n");
                printf("NA\n");
                break;
            }
            printf("Se encontro el registro %d %d %d ", datos->idOrigen, datos->idDestino, datos->hora);
            printf("con un tiempo de viaje medio de %f\n", datos->mediaViaje);
            break;

        case 5:
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
