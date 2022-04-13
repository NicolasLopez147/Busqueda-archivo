all: ejecutarMenu

#Compila menu
compilarMenu: menu.c
	gcc menu.c -o menu

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
