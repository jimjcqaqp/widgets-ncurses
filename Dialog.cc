#include "Dialog.h"

#define DEFAULT_WIDTH 30

Dialog::Dialog(){
	x = 0;
	y = 0;
	w = DEFAULT_WIDTH;
	h = 0;
	header = "";
	footer = "";
	type = DIALOG_QUESTION;
	align = DIALOG_ALIGN_CENTER;

	vc = false;
	hc = false;

	win = NULL;
	menu = NULL;
	items = NULL;

	parent = stdscr;
}

int Dialog::start(){
	curs_set(0);
	noecho();
	int option = 0;
	int i;
	// Dejamos de la height por el tamaño de body
	int posx = x;
	int posy = y;
	int width = w;

	int height = body.size();
	int offset_header = (header.size() == 0)? 1 : 3;
	int offset_footer = 3;

	int globalx = 0, globaly = 0;
	int globalw = 0, globalh = 0;
	getmaxyx(parent, globalh, globalw);
	getbegyx(parent, globaly, globalx);

	if(hc == true)
		posx = (globalw - w) / 2;
	if(vc == true)
		posy = (globalh - (height + offset_header + offset_footer)) / 2;

	win = newwin(height + offset_header + offset_footer, width, 
							 posy + globaly, posx + globalx);
	keypad(win, TRUE);

	box(win, 0, 0);
	if(header.size() != 0){
		mvwaddch(win, 2, 0, ACS_LTEE);
		mvwhline(win, 2, 1, ACS_HLINE, width - 2);
		mvwaddch(win, 2, width - 1, ACS_RTEE);
		mvwprintw(win, 1, (width - header.size()) / 2, header.c_str());
	}
	mvwaddch(win, height + offset_header, 0, ACS_LTEE);
	mvwhline(win, height + offset_header, 1, ACS_HLINE, width - 2);
	mvwaddch(win, height + offset_header, width - 1, ACS_RTEE);
	mvwprintw(win, height + offset_footer + 1, (width - footer.size()) / 2, footer.c_str());

	// Align
	for(i = 0; i < (int)body.size(); ++i)
	{
		if(align == DIALOG_ALIGN_CENTER)
			mvwprintw(win, offset_header + i, (width - body[i].size())/2, body[i].c_str());
		else if(align == DIALOG_ALIGN_LEFT)
			mvwprintw(win, offset_header + i, 1, body[i].c_str());
		else
			mvwprintw(win, offset_header + i, 1, 
				(boost::format("%" + std::to_string(width - 2) + "s") % body[i]).str().c_str() 
					);
	}
	if(type == DIALOG_QUESTION){
		items = (ITEM **) calloc(3, sizeof(ITEM *));	
		items[0] = new_item("  Aceptar  ", NULL);
		items[1] = new_item("  Cancelar ", NULL);
		items[2] = new_item(NULL, NULL);
	}else{
		items = (ITEM **) calloc(2, sizeof(ITEM *));
		items[0] = new_item("  Aceptar  ", NULL);
		items[1] = new_item(NULL, NULL);
	}
	menu = new_menu(items);
	
	set_menu_win(menu, win);

	if(type == DIALOG_QUESTION){
		set_menu_sub(menu, derwin(win, 1, 23, offset_header + height + 1, (width - 23)/2 ));
		set_menu_format(menu, 1, 2);
	}else{
		set_menu_sub(menu, derwin(win, 1, 11, offset_header + height + 1, (width - 11)/2 ));
		set_menu_format(menu, 1, 1);
	}

	set_menu_mark(menu, "");

	post_menu(menu);
	wrefresh(win);

	int c;
	while((c = wgetch(win)) != 10){
		switch(c){
			case KEY_LEFT:
				menu_driver(menu, REQ_LEFT_ITEM);
				break;
			case KEY_RIGHT:
				menu_driver(menu, REQ_RIGHT_ITEM);
				break;	
		}	
		wrefresh(win);
	}

	ITEM *select_item = current_item(menu);
	option = item_index(select_item);

	unpost_menu(menu);
	free_menu(menu);

	int n = (type == DIALOG_QUESTION)? 3: 2;

	for(int i = 0; i < n; ++i)
		free_item(items[i]);

	wclear(win);
	wrefresh(win);
	delwin(win);
	win = NULL;
	
	curs_set(1);
	echo();
	return option;
}

void Dialog::addlinebody(std::string line){
	body.push_back(line);
}
Dialog::~Dialog(){
	echo();
}

