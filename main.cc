#include "Menu.h"
#include "Dialog.h"
#include <unistd.h>

/*
 *	He visto que hay un problema al usar la libreria "menu.h" y las clases
 *	si no se libera el "menu" al finalizar con endwin, la terminal cambiara la 
 *	configuración y no servira. por eso se debe iniciar en una funcion y no en la
 *	funcion principal
 * */ 

void init();

int main(){
	initscr();
	init(); 		// obligatorio
	return endwin();
}

void init(){
	Menu menu;

	menu.header = "Menu principal";
	menu.add("Administrador");
	menu.add("Profesor");
	menu.add("Docente");
	menu.w = 30;
	menu.vc = true;
	menu.hc = true;
	menu.footer = "(q) exit";
	menu.addkey('q');
	menu.addkey('Q');

	while(true){
		int index = 0;
		if(menu.running() == false)
			index = menu.start();
		else 
			index = menu.resume();

		if(menu.keypress == 'q' || menu.keypress == 'Q')
		{
			Dialog dconfig;
			dconfig.hc = true;
			dconfig.vc = true;
			dconfig.w = 40;
			dconfig.addlinebody("Esta seguro que desea salir?");
			int response = dconfig.start();
			if(response == DIALOG_OK)
				break;
		}
		menu.resetkey();

		switch(index){
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
		}
	}	
	clear();
	refresh();
}
