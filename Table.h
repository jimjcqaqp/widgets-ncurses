#pragma once

#include <menu.h>
#include <iostream>
#include <boost/format.hpp>
#include <vector>
#include <string>

class Table{
	public:
		Table();
		~Table();

		int start();
		int resume();
		void resetkey();

		void addrow(std::string col, int width);
		void addcolumn(std::vector<std::string>);

		void clear();
		void addkey(int key);
		void addaction(void (*)(int, void *), void *);
		bool running();
		int x, y;
		int w, h;
		bool vc;
		bool hc;
		
		// std::vector<std::string> options;
		std::vector<std::string> rownames;
		std::vector<int> rowwidth;
		std::vector< std::vector<std::string> > cols;

		///////////////////////////////////
		std::vector<int> keys;
		std::vector<void (*)(int, void *)> iter_actions;
		std::vector<void *> ptr_data;

		int keypress;
		std::string header;
		std::string footer;

	private:
		WINDOW *win;
		ITEM **items;
		MENU *menu;
		
		std::vector<std::string> options_front;

		void active_iter_actions();
};

