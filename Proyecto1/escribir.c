#include <stdio.h>
#include <stdlib.h>
#include "datos.h"

int main(){
    FILE *archivo;
    archivo = fopen("prueba.txt","wb+");
    if (!archivo){
        perror("Hubo un error abriendo el archivo");
        exit(-1);
    }
    struct index indices[1060];

    for (int i = 1 ; i <= 1060;i++){
        indices[i].idOrigen = i;
        indices[i].apuntador = 100*i;
        fwrite(&indices[i],1,sizeof(indices[i]),archivo);
    }

    fclose(archivo);

    FILE* lectura = fopen("prueba.txt","rb");
    if (!lectura){
        perror("Hubo un error leyendo el archivo");
        exit(-1);
    }
    struct index indice;
    
    int dato = 526;
    fseek(lectura,(dato-1)*(sizeof(indice)),SEEK_SET);
    fread(&indice,sizeof(indice),1,lectura);
    printf("El valor de origen es %d y apunta dentro del archivo a %d\n",indice.idOrigen,indice.apuntador);
    fclose(lectura);

    FILE *escribir;
    escribir = fopen("pruebaTablaHash.txt","wb+");
    if (!escribir){
        perror("Hubo un error abriendo el archivo");
        exit(-1);
    }
    //struct Datos datos[1060];

    for (int i = 1 ; i <= 100000;i++){
        struct Datos dato;
        fwrite(&dato,1,sizeof(dato),escribir);
    }

    fclose(escribir);
}