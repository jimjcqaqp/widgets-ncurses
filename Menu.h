#pragma once

#include <menu.h>
#include <iostream>
#include <boost/format.hpp>
#include <vector>

class Menu{
	public:
		Menu();
		~Menu();

		int start();
		int resume();

		void clear();
		void add(std::string);
		void addkey(int key);
		void addaction(void (*)(int, void *), void *);
		int x, y;
		int w, h;
		bool vc;
		bool hc;

		std::vector<std::string> options;
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

