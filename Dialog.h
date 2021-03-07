#pragma once

#include <menu.h>
#include <iostream>

class Dialog{
	public:
		int x, y, w, h;

		Dialog();
		~Dialog();

		bool start();

		std::string header;
		std::string footer;
		std::string body;
	private:
};

