#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

// Los datos que tiene cada registro
struct Datos
{
    int idOrigen;
    int idDestino;
    int hora;
    float mediaViaje;
    float standard_deviation_travel_time;
    float geometric_mean_travel_time;
    float geometric_standard_deviation_travel_time;
    long sig;
};
// Estructura del archivo salidaIndex
struct index
{
    int idOrigen;
    long apuntador;
};

void escribirTuberia(char *tuberia, struct Datos buffer, int tamano)
{
    // Descriptor del archivo
    int descriptor = open(tuberia, O_WRONLY);
    if (descriptor < 0)
    {
        perror("Hubo un error abriendo el archivo de la tuberia");
        exit(-1);
    }
    //Numero de bytes escritos
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
    // Lee el archivo FIFO y guarla la informacion en buffer, retorna el numero de bytes leidos
    int r = read(descriptor, buffer, tamano);
    if (r < 0)
    {
        perror("Hubo un error leyendo el archivo de la tuberia");
        exit(-1);
    }
    close(descriptor);
}