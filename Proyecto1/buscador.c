#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "datos.h"
#include <sys/wait.h>

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
void escribirTuberia(char *tuberia, struct Datos buffer , int tamano)
{
    // Descriptor del archivo
    int descriptor = open(tuberia, O_WRONLY);
    if (descriptor < 0)
    {
        perror("Hubo un error abriendo el archivo de la tuberia");
        exit(-1);
    }
    int r = write(descriptor, &buffer, tamano);
    if (r < 0)
    {
        perror("No se pudo escribir el origen en la tuberia");
    }
    close(descriptor);
}

int main()
{

    // Ruta del archivo tuberia
    char *tuberia = "./menuBuscador";
    char *tuberia2 = "./buscadorMenu";

    // Se lee el mensaje de la tuberia
    struct Datos *buffer;

    leerTuberia(tuberia, buffer, sizeof(*buffer));
    printf("El mensaje recibido fue %d %d %d\n", buffer->idOrigen, buffer->idDestino, buffer->hora);

    printf("Ingrese cualquier letra para continuar");
    char a;
    scanf("%s", &a);

    buffer->idOrigen = 5;
    buffer->idDestino = 6;
    buffer->hora = 7;

    mkfifo(tuberia2, 0666);
    escribirTuberia(tuberia2,*buffer, sizeof(*buffer));


    return 0;
}
