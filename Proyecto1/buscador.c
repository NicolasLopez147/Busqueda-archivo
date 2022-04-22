#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "datos.h"
// #include <sys/wait.h>
    
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

int hash(int x) {
    return x;
}

int main()
{
    FILE *lectura;
    
    // Ruta del archivo tuberia
    char *tuberia = "./menuBuscador";
    char *tuberia2 = "./buscadorMenu";
    char a;

    // Leer datos de busqueda de la tuberia
    struct Datos buffer, *bufferP;
    bufferP = &buffer;
    while (1){
    
    leerTuberia(tuberia, bufferP, sizeof(*bufferP));
    // printf("El mensaje recibido fue %d %d %d\n", bufferP->idOrigen, bufferP->idDestino, bufferP->hora);
    
    int origen = bufferP->idOrigen;
    int destino = bufferP->idDestino;
    int hora = bufferP->hora;

    if ((lectura = fopen("salidaHash", "rb")) == NULL) {
        perror("Hubo un error leyendo el archivo hash\n");
        exit( EXIT_FAILURE );
    }

    struct index indice;
    int hashOrigen = hash(origen);

    fseek(lectura, hashOrigen*sizeof(struct index), SEEK_SET);
    fread(&indice, sizeof(struct index), 1, lectura);
    
    if(indice.apuntador == -1) {
    //     printf("No hay registros con idOrigen %d\n", origen);
        bufferP->idOrigen = -1;      // Indica que no se encontraron registros
    } //else {
    //     printf("El primer registro con idOrigen %d se encuentra en la posicion %ld del archivo indexado\n", indice.idOrigen, indice.apuntador);
    // }

    fclose(lectura);

    // Busqueda del registro adecuado en el archivo indexado
    if ((lectura = fopen("salidaIndex", "rb")) == NULL) {
        perror("Hubo un error leyendo el archivo index\n");

        exit( EXIT_FAILURE );
    }

    if(bufferP->idOrigen != -1) {
        fseek(lectura, (indice.apuntador-1) * sizeof(struct Datos), SEEK_SET);
        fread(&buffer, sizeof(struct Datos), 1, lectura);

        // printf("Se encontro el registro %d %d %d\n", bufferP->idOrigen, bufferP->idDestino, bufferP->hora);

        while( (bufferP->idOrigen != origen) || (bufferP->idDestino != destino) || (bufferP->hora != hora) ) {
            if(bufferP->sig == -1) {
                printf("No hay registros con los parametros indicados\n");
                bufferP->idOrigen = -1;      // Indica que no se encontraron registros
                break;
            }

            // Leer registro siguiente
            fseek(lectura, (bufferP->sig-1) * sizeof(struct Datos), SEEK_SET);
            fread(&buffer, sizeof(struct Datos), 1, lectura);

            // printf("Se encontro el registro %d %d %d\n", bufferP->idOrigen, bufferP->idDestino, bufferP->hora);
        }
    }

    fclose(lectura);

    // printf("Ingrese cualquier letra para continuar\n");
    // scanf("%s", &a);

    mkfifo(tuberia2, 0666);
    escribirTuberia(tuberia2,*bufferP, sizeof(*bufferP));
    }
    return 0;
}
