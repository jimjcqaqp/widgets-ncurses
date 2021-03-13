#pragma once

#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <boost/algorithm/string.hpp>

class Student{
	public:
		Student();
		~Student();
		static Student find(int id);
		static Student findcode(std::string code);
		static std::vector<Student> all(); 
		bool save();
		// void remove();

		// Propiedades
		int id = 0;
		std::string name;
		std::string code;
};

