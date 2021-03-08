#pragma once

#include <menu.h>
#include <iostream>
#include <vector>

typedef enum{
	DIALOG_QUESTION = 0,
	DIALOG_INFORMATION = 1
} DIALOG_TYPE;

typedef enum{
	DIALOG_ALIGN_CENTER = 0,
	DIALOG_ALIGN_LEFT,
	DIALOG_ALIGN_RIGHT
} DIALOG_BODY_ALIGN;

typedef enum{
	DIALOG_OK = 0,
	DIALOG_CANCEL = 1
} DIALOG_RESPONSE;

class Dialog{
	public:
		int x, y, w, h;
		bool vc, hc;

		Dialog();
		~Dialog();

		int start();
		void addlinebody(std::string line);

		std::string header;
		std::string footer;
		std::vector<std::string> body;

		DIALOG_TYPE type;
		DIALOG_BODY_ALIGN align;

	private:
		WINDOW *win;
		MENU *menu;
		ITEM **items;
};

