all: ejecutarMenu

#Compila menu
compilarMenu: p1-odProgram.c
	gcc p1-odProgram.c -o menu

#Compila buscador
compilarBuscador: buscador.c
	gcc buscador.c -o buscador

#Crear terminal y compila y ejecuta menu
ejecutarMenu: compilarMenu crearTerminal menu
	./menu

#Compila buscador, crea una nueva terminal y ejecuta buscador en esa terminal
crearTerminal: compilarBuscador
	gnome-terminal -- ./buscador

ejecutarBuscador: compilarBuscador buscador
	./buscador

#Ejecutar una vez para indexar el archivo
compilarIndexador: indexador.c
	gcc indexador.c -o indexador

indexarArchivo: compilarIndexador
	./indexador

# compilarTabla: escribir.c
# 	gcc escribir.c -o escribir

# ejecuta: compilarTabla
# 	./escribir

#Borra los ejecutables de indexador, menu y buscador
clean: indexador menu buscador
	rm -r indexador menu buscador

#Borra los archivos binarios salidaHash y salidaIndex
cleanIndex: salidaHash salidaIndex
	rm -r salidaHash salidaIndex
