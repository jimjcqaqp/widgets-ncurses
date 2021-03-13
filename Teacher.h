#pragma once

#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <boost/algorithm/string.hpp>

class Teacher{
	public:
		Teacher();
		~Teacher();
		static Teacher find(int id);
		static Teacher findcode(std::string code);
		static std::vector<Teacher> all(); 
		bool save();
		// void remove();

		// Propiedades
		int id = 0;
		std::string name;
		std::string code;
};

