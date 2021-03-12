#include "Form.h"
#include <curses.h>

#define DEFAULT_WIDTH	20

Form::Form(){
	x = 0;
	y = 0;
	w = DEFAULT_WIDTH;
	h = 0;
	win = NULL;
	hc = false;
	vc = false;
	parent = stdscr;
}

Form::~Form(){
	if(win != NULL)
	{
		wclear(win);
		wrefresh(win);
		delwin(win);
		win = NULL;
	}
}

void Form::clear(){
	if(win != NULL)
	{
		wclear(win);
		wrefresh(win);
	}
}

void Form::addquittext(std::string text){
	quittext.push_back(text);
}

void Form::addInput(std::string question, FORM_INPUT_TYPE type, int len){
	this->questions.push_back(question);
	this->types.push_back(type);
	this->lengths.push_back(len);
	this->responses.push_back("");
}

void Form::addSelect(std::string question, 
										 std::vector<std::string> options, 
										 int width){
	this->listselect[questions.size()] = options;
	this->questions.push_back(question);
	this->types.push_back(INPUT_SELECT);
	this->lengths.push_back(width);
}

bool Form::start(){
	int globalx = 0, globaly = 0;	
	int globalw = 0, globalh = 0;
	getmaxyx(parent, globalh, globalw);	
	getbegyx(parent, globaly, globalx);

	int posx = x, posy = y;
	int width = w, height = 0;	

	int offset_header = (header.size() == 0)? 1 : 3;
	int offset_footer = (footer.size() == 0)? 0 : 2;

	height += (types.size() * 3);
	height += (listselect.size() * 3);

	if(hc == true)	
		posx = (globalw - width) / 2;
	else
		posx = x;
	
	if(vc == true)
		posy = (globalh - (height + offset_footer + offset_header)) / 2;
	else
		posy = y;
	
	win = newwin(height + offset_header + offset_footer, 
											 width, posy + globaly, posx + globalx);	
	box(win, 0, 0);	
	keypad(win, FALSE);

	int line = offset_header - 1;
	if(header.size() != 0){
		mvwaddch(win, 2, 0, ACS_LTEE);	
		mvwhline(win, 2, 1, ACS_HLINE, width - 2);
		mvwaddch(win, 2, width - 1, ACS_RTEE);	
		mvwprintw(win, 1, (width - header.size()) / 2, header.c_str());
	}

	for(int i = 0; i < (int) types.size(); ++i){
		if(header.size() == 0 && i == 0){
			mvwprintw(win, line + 1, 1, questions[i].c_str());
		}else{
			mvwaddch(win, line, 0, ACS_LTEE);	
			mvwhline(win, line, 1, ACS_HLINE, width - 2);
			mvwaddch(win, line, width - 1, ACS_RTEE);	
			mvwprintw(win, line + 1, 1, questions[i].c_str());
		}
		if(types[i] == INPUT_SELECT){
			line += 6;
		}else{
			line += 3;
		}
	}

	if(footer.size() != 0){
		mvwaddch(win, offset_header + height + offset_footer - 3, 0, ACS_LTEE);	
		mvwhline(win, offset_header + height + offset_footer - 3, 1, ACS_HLINE, width - 2);
		mvwaddch(win, offset_header + height + offset_footer - 3, width - 1, ACS_RTEE);	
		mvwprintw(win, offset_header + height, (width - footer.size()) / 2 , footer.c_str());	
	}

	wrefresh(win);
	// Respuestas
	mm.clear();
	line = offset_header - 1;
	for(int i = 0; i < (int) types.size(); ++i){
		char text[1024];

		// Para que los menus se conserven y sean eliminados cuando termine el for

		WINDOW *wini;
		if(types[i] != INPUT_SELECT){

			while(true)
			{
				wini = derwin(win, 1, lengths[i] + 1, line + 2, 1);	
				// keypad(wini, FALSE);
				wattron(wini, A_BOLD);
				wattron(wini, A_ITALIC);
				if(types[i] == INPUT_PASSWORD) noecho();
				mvwgetstr(wini, 0, 0, text);
				if(types[i] == INPUT_PASSWORD) echo();
				wattroff(wini, A_BOLD);
				wattroff(wini, A_ITALIC);

				if(strlen(text) == 0){
					wclear(wini);
					wrefresh(wini);
					delwin(wini);
					continue;
				}

				wattron(wini, A_BOLD);
				wattron(wini, A_ITALIC);
				
				if(types[i] == INPUT_PASSWORD){
					for(int l = 0; l < strlen(text); ++l)
						mvwaddch(wini, 0, l, '*');
					wrefresh(wini);
				}

				if(types[i] == INPUT_NUMBER){
					sprintf(text, "%i", atoi(text));
					werase(wini);
					wrefresh(wini);
					mvwprintw(wini, 0, 0, "%s", text);
					wrefresh(wini);
				}
				wattroff(wini, A_BOLD);
				wattroff(wini, A_ITALIC);
				responses[i] = text;

				break;
			}
			line += 3;
		}else{ 
			mm[i] = Menu();
			for(int k = 0; k < (int)listselect[i].size(); ++k)
				mm[i].add(listselect[i][k]);
			mm[i].w = lengths[i];
			mm[i].h = 2;
			mm[i].x = posx + 1 + globalx;
			mm[i].y = posy + line + 2 + globaly;
			int index = mm[i].start();
			line += 6;
			responses.push_back(std::to_string(index));
		}

		for(int i = 0; i < (int) quittext.size(); ++i){
			if(strcmp(text, quittext[i].c_str()) == 0)
			{
				return false;
			}
		}
	}	
	// wclear(win);
	// wrefresh(win);
	// delwin(win);
	return true;
}

