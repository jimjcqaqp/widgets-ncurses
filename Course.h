#pragma once

#include "Teacher.h"
#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <boost/algorithm/string.hpp>

class Course{
	public:
		Course();
		~Course();
		static Course find(int id);
		static std::vector<Course> all(); 
		static std::vector<Course> allByTeacher(int id);
		bool save();
		// void remove();

		// Propiedades
		int id = 0;
		std::string name;
		std::string code;
		Teacher teacher;
		int course_id;
};

