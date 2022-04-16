#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include "vueloStruct.h"

//#define TABLE_SIZE 5904703
#define TABLE_SIZE 50

typedef struct {
    int idOrigen;
    int idDestino;
    int hora;
    float tiempoMedioVuelo;
    int hash;
    struct vuelo *next;
    
} vuelo;

vuelo *hash_table[TABLE_SIZE]; // Es mejor que sea un puntero porque:

// 1) No necesito todo el espacio de toda la tabla sino solamente hasta que llene la tabla, solamente necesito el espacio de algunas direcciones.
// 2) Es mas facil decir cuando un espacio esta vacio ya que un puntero puedo "setearlo" a NULL.

int contarVuelos(char *nombreArch) {
	FILE *arch = fopen(nombreArch, "r");
	if (arch == NULL) {
 		printf("Problema al abrir el archivo %s. Termina el programa.\n", nombreArch);
 		exit(-1);
 	}	
 	int n = 0; // Cantidad de vuelos.
 	char c = fgetc(arch); // Recorremos caracter por caracter 
 	while (!feof(arch)) {
 		if (c == '\n') n++;
 		c = fgetc(arch);
 	}
 	fclose(arch);
 	return n - 1; // Porque en la primera linea estan el nombre de los campos. (labels)
}

int getNumberLine(char *linea) {
	int idOrigen;
	int i = 0, j = 0;
	char nro[100];
	//printf("%s\n", linea);
	while (linea[i] != ',') {
		//printf("%c\n",linea[i]);
		nro[j++] = linea[i++]; // Copiamos el idOrigen como cadena
	}
	nro[i] = '\0'; // Caracter de terminación de la cadena
	//printf("%s\n", nro);
    	idOrigen = atoi(nro); // Conversión del nro a cadena
    	//printf("%i\n", idOrigen);
    	return idOrigen;
}

void printValues(vuelo values[]) {
	for (int i = 0; i < TABLE_SIZE; i++) {
		printf("ID origen: %i, ID destino: %i, Hora: %i, Tiempo medio: %.2f, Hash: %i\n", values[i].idOrigen, values[i].idDestino, values[i].hora, values[i].tiempoMedioVuelo, values[i].hash);
	}
}

unsigned int hash(int x) {
	int c2=0x27d4eb2d; // a prime or an odd constant
	x = (x ^ 61) ^ (x >> 16);
	x = x + (x << 3);
	x = x ^ (x >> 4);
	x = x * c2;
	x = x ^ (x >> 15);
	 
    	//printf("%i\n", x % 50);
    	return x % TABLE_SIZE;
}

void init_hash_table() {
	for (int i = 0; i < TABLE_SIZE; i++) {
		hash_table[i] = NULL;
	}
	
	// table is empty
}

void print_table() {
	printf("Start\n");
	for (int i = 0; i < TABLE_SIZE; i++) {
		if (hash_table[i] == NULL) {
			printf("\t%i\t---\n", i);
		} else {
			printf("\t%i\t ", i);
			vuelo *tmp = hash_table[i];
			while (tmp != NULL) {
				printf("%i - ", tmp->idOrigen);
				tmp = tmp->next;
			}
			printf("\n");
		}
	}
	printf("End\n");
}

void print_struct (vuelo *p) {
	fputs("Datos del vuelo\n", stdout);
	printf("Id del vuelo: %i\n", p->idOrigen);
	printf("Id de destino del vuelo: %i\n", p->idDestino);
	printf("Hora del vuelo: %i\n", p->hora);
	printf("Tiempo medio de vuelo: %.2f\n", p->tiempoMedioVuelo);
}



bool hash_table_insert(vuelo *p) {
	if (p == NULL) return false;
	int index = hash(p->idOrigen);
	p->next = hash_table[index];
	hash_table[index] = p;
	return true; 
}

vuelo *hash_table_lookup(int idOrigen) {
	int index = hash(idOrigen);
	vuelo * tmp = hash_table[index];
	while (tmp !=  NULL && idOrigen != tmp->idOrigen) {
		tmp = tmp->next;
	}  
	
	return tmp;
}

vuelo *hash_table_delete(int idOrigen) {
	int index = hash(idOrigen);
	vuelo * tmp = hash_table[index];
	vuelo *prev = NULL;
	while (tmp !=  NULL && idOrigen != tmp->idOrigen) {
		prev = tmp;
		tmp = tmp->next;
	}  
	if (tmp == NULL) return NULL;
	if (prev == NULL) {
		// deleting the head
		hash_table[index] = tmp->next;
	} else {
		prev->next = tmp->next;
	}
	return tmp;
}


void leerTuberia(char *tuberia, struct Datos *buffer, int tamano)
{
    // Descriptor del archivo
    int descriptor;
    //printf("%s\n", tuberia);
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
    printf("%d\n", r);
    printf("%d\n", descriptor);
    printf("%d\n", tamano);
    if (r < 0)
    {
        perror("Hola mundo Hubo un error leyendo el archivo de la tuberia");
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



int main() {	
	init_hash_table();
	print_table();
 	char nombreArchivoEnt[] = "practicaExample.csv";
 	char nombreArchivoSal[] = "practica1Sal.csv";
 	
 	// Ruta del archivo tuberia
 	//char *tuberia = "./menuBuscador";
	//char *tuberia2 = "./buscadorMenu";
	
	// Se lee el mensaje de la tuberia
	//struct Datos *buffer;

	//leerTuberia(tuberia, buffer, sizeof(*buffer));
	//printf("El mensaje recibido fue %d %d %d\n", buffer->idOrigen, buffer->idDestino, buffer->hora);

	//printf("Ingrese cualquier letra para continuar");
	//char a;
	//scanf("%s", &a);
    
    
 	//int n = contarVuelos(nombreArchivoEnt); // Guarda la cantidad de vuelos.
 	//printf("La cantidad de vuelos es: %d\n", n); // La cantidad de vuelos es: 5904703
 
 	FILE *arch = fopen(nombreArchivoEnt, "r");
 	if (arch == NULL) {
 		printf("Problema al abrir el archivo %s. Termina el programa.\n", nombreArchivoEnt);
 		exit(-1);
 	}
 	
 	FILE *archSal = fopen(nombreArchivoSal, "w");
 	if (archSal == NULL) {
 		printf("Problema al crear el archivo %s. Termina el programa.\n", nombreArchivoSal);
 		exit(-1);
 	}
 	 	
 	// Primero llegamos a la segunda linea
 	char linea[1000];
	
	vuelo vuelos[TABLE_SIZE]; // array to structs to store values
	
 	fgets(linea, sizeof(linea), arch); // Titulos de campos
 	
 	// Recordemos que el fgets tambien alamacena el salto de linea!!
 	linea[strlen(linea)-1] = '\0'; // Colocamos el terminador de cadena en vez del salto de linea, la razón de esto es porque vamos a 	agregar una nueva "columna" luego no queremos que se lea ese salto de linea.
 	
 	// Agregamos el "Hash" label y guardamos en la salida
 	fprintf(archSal, "%s,hash\n", linea); // Como si estuvieramos haciendo un append
 	
 	unsigned int hashNum;
 	int i, idOrigen;
 	for (i=0; i<TABLE_SIZE; i++) {
 		fgets(linea, 1000, arch); // Leemos la linea correspondiente a un vuelo
 		linea[strlen(linea)-1] = '\0';
 		idOrigen = getNumberLine(linea);
 		hashNum = hash(idOrigen);
 		fprintf(archSal, "%s,%i\n", linea, hashNum);
 	}
 	
 	fclose(arch);
 	fclose(archSal);
 	
 	archSal = fopen(nombreArchivoSal, "r");
 	if (archSal == NULL) {
 		printf("Problema al crear el archivo %s. Termina el programa.\n", nombreArchivoSal);
 		exit(-1);
 	} 	
 	
 	int row_count = 0;
	int field_count = 0;
 	
 	int j = 0;
 	
	while (fgets(linea, sizeof(linea), archSal)) {
		//printf("%s\n", linea); // this will parse all the data (LO TUVE QUE CORRER)
		field_count = 0;
		row_count++;
		if (row_count == 1) {
			continue; // we did not want to take the labels
		}
		
		char *field = strtok(linea, ","); // separate linea with commas
		int fieldCount = 0;
		while (field) {
			printf("FIELD = %s\n", field);
			if (field_count == 0) {
				vuelos[j].idOrigen = atoll(field);
			}	
				
			if (field_count == 1)
				vuelos[j].idDestino = atoll(field);
				
			if (field_count == 2)
				vuelos[j].hora = atoll(field);
				
			if (field_count == 3)
				vuelos[j].tiempoMedioVuelo = atof(field);
				
			if (field_count == 7)
				vuelos[j].hash = atoll(field);

			field = strtok(NULL, ","); // ¿Para salir del while? Update field value
			field_count++;
		}
		hash_table_insert(&vuelos[j]);
		j++;
	}
	
	vuelo *tmp = hash_table_lookup(478);
	
	if (tmp == NULL) {
		printf("Not found!\n");
	} else  {
		printf("Found %i.\n", tmp->idOrigen);
	}
	
	print_table();
	printValues(vuelos);	
 	
 	/*vuelo vuelo1 = {5, 10, 12, 12.30};
 	vuelo vuelo2 = {10, 2, 11, 11.30};
 	vuelo vuelo3 = {2, 8, 5, 5.30};
 	
 	hash_table_insert(&vuelo1);
 	hash_table_insert(&vuelo2);
 	hash_table_insert(&vuelo3);
 	
 	print_table();
	
 	vuelo *tmp = hash_table_lookup(10);
 	
 	if (tmp == NULL) {
		printf("Not found 1!\n");
	} else  {
		printf("Found 1%i.\n", tmp->idOrigen);
	
	}
	tmp = hash_table_lookup(11);
	
	if (tmp == NULL) {
		printf("Not found 2!\n");
	} else  {
		printf("Found 2%i.\n", tmp->idOrigen);
	}
	
	hash_table_delete(10);
	tmp = hash_table_lookup(10);
 	
 	if (tmp == NULL) {
		printf("Not found 3!\n");
	} else  {
		printf("Found 3%i.\n", tmp->idOrigen);
	}
	
	print_table();*/
 	 			
 	return 0;
 }
 
 /*
 1) Memoria dinamica
 2) Pasar a binario.
 3) Corregir procesos.
 4) Makefile. 
 */
