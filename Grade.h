#pragma once

#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <string>

class Grade{
	public:
		Grade();
		~Grade();
		static Grade find(int id);
		// static Grade findByStudentId(int id);
		// static Grade findByCourseId(int id);

		static std::vector<Grade> all(); 
		// static std::vector<Grade> allByTeacherId(int id);
		static std::vector<Grade> allByStudentId(int id);
		static std::vector<Grade> allByCourseId(int id);
		static Grade findByStudentAndCourse(int student_id, int course_id);
		bool save();
		// void remove();

		// Propiedades
		int id = 0;
		int grade1;
		int grade2;
		int grade3;
		int average;
		int student_id;
		int course_id;
};

