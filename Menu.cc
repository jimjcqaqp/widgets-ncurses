#include "Menu.h"
#include <curses.h>

#define DEFAULT_WIDTH 20

Menu::Menu(){
	x = 0;
	y = 0;
	w = DEFAULT_WIDTH;
	h = 0;
	header = "";
	footer = "";
	vc = false;
	hc = false;

	win = NULL;
	menu = NULL;
	items = NULL;
}

int Menu::start(){
	/***************/
	if(menu != NULL){
		unpost_menu(menu);
		free_menu(menu);
	}
	if(items != NULL){
		for(int i = 0; i < (int)options.size(); ++i)
			free_item(items[i]);
	}
	if(win != NULL){
		wclear(win);
		wrefresh(win);
		delwin(win);
		win = NULL;
	}
	/***************/
	int index = 0;
	int i = 0;
// 	if(options.size() == 0)
//		return -1;

	int globalw, globalh;
	getmaxyx(stdscr, globalh, globalw);

	int posx = 0;
	int posy = 0;
	int height = h;
	int width = w;

	// Verificamos si se ha puedo en medio
	if(hc == true)
		posx = (globalw - w) / 2;
	else
		posx = x;

	int offset_header = (header.size() == 0)? 1 : 3;
	int offset_footer = (footer.size() == 0)? 1 : 3;
	curs_set(0);
	noecho();

	if(height == 0)
		height = options.size();

	// Center vertical
	if(vc == true)
		posy = (globalh - (height + offset_footer + offset_header)) / 2;
	else
		posy = y;


	// Iniciando WINDOW
	win = newwin(height + offset_footer + offset_header, width, posy, posx);
	keypad(win, TRUE);
	
	// Iniciando ITEM
	items = (ITEM **) calloc(options.size() + 1, sizeof(ITEM *));

	// Tenemos que mantener en memoria la cadena de string de las opciones del menu
	// si no lo hacemos no funcionana correctamente new_item()	
	options_front.clear();

	for(i = 0; i < (int)options.size(); ++i)
	{
		std::string format_complete = "%-" + std::to_string(width - 2) + "s";
		options_front.push_back( (boost::format(format_complete) % options[i]).str() );
		// items[i] = new_item( (boost::format("%30s") % options[i].c_str()).str().c_str(), NULL);	
		items[i] = new_item(options_front[i].c_str(), NULL);
	}
	items[i] = new_item(NULL, NULL);

	// Iniciando MENU
	menu = new_menu(items);	

	// Configurando MENU
	set_menu_win(menu, win);	
	set_menu_sub(menu, derwin(win, height, width - 2, offset_header , 1));
	set_menu_format(menu, height, 1);
	set_menu_mark(menu, "");	

	box(win, 0, 0);

	if(header.size() != 0){
		mvwaddch(win, 2, 0, ACS_LTEE);
 		mvwhline(win, 2, 1, ACS_HLINE, width - 2);
 		mvwaddch(win, 2, width - 1, ACS_RTEE);
		mvwprintw(win, 1, (width - header.size()) / 2, header.c_str());
	}
	if(footer.size() != 0){
		mvwaddch(win, height + offset_header, 0, ACS_LTEE);
		mvwhline(win, height + offset_header, 1, ACS_HLINE, width - 2);
		mvwaddch(win, height + offset_header, width - 1, ACS_RTEE);
		mvwprintw(win, height + offset_header + 1, (width - footer.size()) / 2, footer.c_str());
	}

	post_menu(menu);
	wrefresh(win);

	active_iter_actions();
	int c;
	while((c = wgetch(win)) != 10){
		bool keyactive = false;
		for(i = 0; i < (int)keys.size(); ++i){
			if(keys[i] == c){
				keypress = c;
				keyactive = true;
				break;
			}
			keypress = 10;
		}

		if(keyactive == true)
			break;

		switch(c){
			case KEY_DOWN: 
				menu_driver(menu, REQ_DOWN_ITEM); 
				active_iter_actions();
				break;
			case KEY_UP:
				menu_driver(menu, REQ_UP_ITEM);
				active_iter_actions();
				break;
			case KEY_NPAGE:
				menu_driver(menu, REQ_SCR_DPAGE);
				active_iter_actions();
				break;
			case KEY_PPAGE:
				menu_driver(menu, REQ_SCR_UPAGE);
				active_iter_actions();
				break;
		}
		wrefresh(win);
	}

	ITEM *select_item = current_item(menu);
	if(select_item == NULL)
		index = -1;
	else 
		index = item_index(select_item);

	curs_set(1);	
	echo();
	return index;
}

int Menu::resume(){
	if(win == NULL)
		return -1;
	int index = -1;
	int i;
	// if(win == NULL || options.size() == 0)
	//	return -1;
	curs_set(0);
	noecho();
	// box(win, 0, 0);
	wrefresh(win);
	int c;

	active_iter_actions();
	while((c = wgetch(win)) != 10){
		bool keyactive = false;
		for(i = 0; i < (int)keys.size(); ++i){
			if(keys[i] == c){
				keypress = c;
				keyactive = true;
				break;
			}
			keypress = 10;
		}

		if(keyactive == true)
			break;

		switch(c){
			case KEY_DOWN: 
				menu_driver(menu, REQ_DOWN_ITEM); 
				active_iter_actions();
				break;
			case KEY_UP:
				menu_driver(menu, REQ_UP_ITEM);
				active_iter_actions();
				break;
			case KEY_NPAGE:
				menu_driver(menu, REQ_SCR_DPAGE);
				active_iter_actions();
				break;
			case KEY_PPAGE:
				menu_driver(menu, REQ_SCR_UPAGE);
				active_iter_actions();
				break;
		}
		wrefresh(win);
	}

	ITEM *select_item = current_item(menu);
	index = item_index(select_item);

	curs_set(1);
	echo();
	return index;
}

void Menu::active_iter_actions(){
	for(int i = 0; i < (int)iter_actions.size(); ++i)
	{
		ITEM *select_item = current_item(menu);
		if(select_item == NULL)
			break;
		int index = item_index(select_item);
		iter_actions[i](index, ptr_data[i]);
	}
	// box(win, 0, 0);	
	redrawwin(win);
	wrefresh(win);
}
void Menu::addaction(void (*newaction)(int, void *), void *data){
	iter_actions.push_back(newaction);
	ptr_data.push_back(data);
}
void Menu::add(std::string option){
	options.push_back(option);
}

Menu::~Menu(){

	if(items != NULL){
		for(int i = 0; i < (int)options.size(); ++i)
			free_item(items[i]);
	}
	if(menu != NULL){
		unpost_menu(menu);
		free_menu(menu);
	}
	if(win != NULL){
		wclear(win);
		wrefresh(win);
		delwin(win);
		win = NULL;
	}
}

void Menu::clear(){
	if(win != NULL){
		wclear(win);
		wrefresh(win);
		// delwin(win);
		// win = NULL;
	}
}

void Menu::addkey(int key){
	keys.push_back(key);
}
void Menu::resetkey(){
	keypress = 10;
}
bool Menu::running(){
	if(win == NULL)
		return false;
	return true;
}

///////////////////////////////////////////////////////////// 
/*
void function(int indice, void *data){
	const char *subdata = (const char *)data;
	mvwprintw(stdscr, 1, 1, std::to_string(indice).c_str());
	mvwprintw(stdscr, 2, 1, subdata);
	refresh();
}
void function1(int indice, void *data){
	const char *subdata = (const char *)data;
	mvwprintw(stdscr, 3, 1, std::to_string(indice).c_str());
	mvwprintw(stdscr, 4, 1, subdata);
	refresh();
}
int main(){
	initscr();
	cbreak();
	
	Menu menu;
	
	for(int i = 0; i < 10; ++i){
		menu.add("Option " + std::to_string(i + 1));
	} 

	menu.addaction(&function, (void *)"FUNCION 01");
	menu.addaction(&function1, (void *)"FUNCION 02");

	menu.vc = true;
	menu.hc = true;
	menu.header = "Selecione una opcion";
	menu.w = 40;
	menu.h = 6;
	menu.footer = "10 opciones";
	// menu.addkey('q');
	// menu.h = 5;

	menu.start();

	endwin();
	return 0;
}*/
