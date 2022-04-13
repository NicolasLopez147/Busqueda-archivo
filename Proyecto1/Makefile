all: ejecutarMenu
compilarMenu: menu.c
	gcc menu.c -o menu

compilarBuscador: buscador.c
	gcc buscador.c buscador

ejecutarMenu: compilarMenu menu
	./menu

ejecutarBuscador: compilarBuscador buscador
	./buscador