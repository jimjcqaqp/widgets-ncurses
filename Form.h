#pragma once

#include "Menu.h"
#include <menu.h>
#include <map>
#include <string.h>


typedef enum{
	INPUT_NUMBER = 0,
	INPUT_STRING,
	INPUT_PASSWORD,
	INPUT_SELECT
} FORM_INPUT_TYPE;

class Form{
	public:
		Form();
		~Form();

		bool start();
		void addquittext(std::string);
		void addInput(std::string question, FORM_INPUT_TYPE type, int len);
		void addSelect(std::string question, std::vector<std::string> options, int width);

		int x, y, w, h;
		bool vc, hc;

		std::string header;
		std::string footer;

		std::vector<std::string> responses;
		
		void clear();
		WINDOW *parent;

	private:
		std::vector<std::string> questions;
		std::vector<FORM_INPUT_TYPE> types;
		std::vector<int> lengths;
		WINDOW *win;	
		std::map<int, std::vector<std::string> > listselect;
		std::map<int, Menu> mm;

		std::vector<std::string> quittext;
};

