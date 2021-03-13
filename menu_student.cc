#include "menu_student.h"
#include "Dialog.h"
#include "Table.h"
#include "Student.h"
#include "Course.h"
#include "Grade.h"

static WINDOW *win_parent;

static void view_registration(Student &student);

void menu_student(Form &formlogin)
{
	win_parent = formlogin.parent;
	Student student = Student::findcode(formlogin.responses[0]);
	if(formlogin.responses[0] == student.code && 
		 formlogin.responses[1] == student.code)
	{
		formlogin.clear();	
		//////////////////////////////////////
		Menu mstudent;
		mstudent.parent = win_parent;
		mstudent.add("Ver registro de matriculas");
		mstudent.addkey('q');
		mstudent.addkey('Q');
		mstudent.footer = "(q) para salir";
		// mstudent.vc = true;
		mstudent.x = 2;
		mstudent.y = 3;
		mstudent.w = 30;
		mstudent.header = "Menu del estudiante";

		while(true){
			int index = -1;
			if(mstudent.running() == true)
				index = mstudent.resume();
			else
				index = mstudent.start();

			if(mstudent.keypress == 'q' || mstudent.keypress == 'Q'){
				mstudent.resetkey();
				break;
			}
			switch(index){
				case 0: view_registration(student); break;
			}
		}
		//////////////////////////////////////		
	}
	else
	{
		Dialog dialog;
		dialog.hc = dialog.vc = true;
		dialog.w = 40;
		dialog.header = "Login failed";
		dialog.type = DIALOG_INFORMATION;
		dialog.addlinebody("User or password incorrect");
		dialog.start();
	}
}

void view_registration(Student &student)
{
	std::vector<Grade> gs = Grade::allByStudentId(student.id);
	Table t;
	t.parent = win_parent;
	t.addrow("Curso", 20);
	t.addrow("Nota1", 6);
	t.addrow("Nota2", 6);
	t.addrow("Nota3", 6);
	t.addrow("Promedio", 10);
	t.h = 16;
	t.x = 33;
	t.y = 3;
	t.header = "Todos los cursos";
	t.addkey('q');
	t.addkey('Q');

	for(int i = 0; i < (int) gs.size(); ++i)
	{
		std::vector<std::string> col;
		Course course = Course::find(gs[i].course_id);
		col.push_back(course.name);
		col.push_back( std::to_string(gs[i].grade1) );
		col.push_back( std::to_string(gs[i].grade2) );
		col.push_back( std::to_string(gs[i].grade3) );
		col.push_back( std::to_string(gs[i].average) );
		t.addcolumn(col);
	}
	t.start();
}

