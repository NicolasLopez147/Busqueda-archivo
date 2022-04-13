#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "datos.h"

int main()
{
    // Descriptor del archivo
    int descriptor;
    // Ruta del archivo tuberia
    char *tuberia = "./menuBuscador";

    // Se abre la tuberia
    descriptor = open(tuberia, O_RDONLY);
    if (descriptor < 0)
    {
        perror("Hubo un error abriendo el archivo de la tuberia");
        exit(-1);
    }

    // Se lee el mensaje de la tuberia
    struct Datos buffer;

    int r = read(descriptor, &buffer, 12);
    if (r < 0)
    {
        perror("Hubo un error leyendo el archivo de la tuberia");
        exit(-1);
    }
    printf("El mensaje recibido fue %d %d %d\n", buffer.idOrigen,buffer.idDestino,buffer.hora);
    close(descriptor);

    return 0;
}