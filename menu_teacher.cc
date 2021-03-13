#include "menu_teacher.h"
#include "Dialog.h"
#include "Teacher.h"
#include "Course.h"
#include "Student.h"
#include "Grade.h"
#include "Table.h"

static WINDOW *win_parent;

static void view_courses(Teacher &teacher);
static void add_grade(Teacher &teacher);

void menu_teacher(Form &formlogin)
{
	win_parent = formlogin.parent;
	Teacher teacher = Teacher::findcode(formlogin.responses[0]);
	if(formlogin.responses[0] == teacher.code && 
		 formlogin.responses[1] == teacher.code)
	{
		formlogin.clear();	
		//////////////////////////////////////
		Menu mteacher;
		mteacher.parent = win_parent;
		mteacher.add("Ver Carga Lectiva");
		mteacher.add("Agregar nota");
		mteacher.addkey('q');
		mteacher.addkey('Q');
		mteacher.footer = "(q) para salir";
		// mteacher.vc = true;
		mteacher.x = 2;
		mteacher.y = 3;
		mteacher.w = 30;
		mteacher.header = "Menu del profesor";

		while(true){
			int index = -1;
			if(mteacher.running() == true)
				index = mteacher.resume();
			else
				index = mteacher.start();

			if(mteacher.keypress == 'q' || mteacher.keypress == 'Q'){
				mteacher.resetkey();
				break;
			}
			switch(index){
				case 0: view_courses(teacher); break;
				case 1: add_grade(teacher); break;
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

void add_grade(Teacher &teacher){
	std::vector<Course> courses = Course::allByTeacher( teacher.id );
	Table t;
	t.parent = win_parent;
	t.addrow("Curso", 30);
	t.addrow("Matriculados", 21);
	t.h = 5;
	t.x = 33;
	t.y = 3;
	t.header = "Todos los cursos (q) cancelar";
	t.addkey('q');
	t.addkey('Q');

	for(int i = 0; i < (int) courses.size(); ++i)
	{
		std::vector<Grade> matriculas_curso = Grade::allByCourseId(courses[i].id);
		std::vector<std::string> col;
		col.push_back(courses[i].name);
		col.push_back( std::to_string(matriculas_curso.size()) + " Estudiantes");
		t.addcolumn(col);
	}
	int indexcourse = t.start();
	
	if(t.keypress == 'q' || t.keypress == 'Q' || 
		 courses.size() == 0)
		return;
	
	Table ts;
	ts.parent = win_parent;
	ts.addrow("Alumno", 30);
	ts.addrow("Nota Final", 21);
	ts.h = 5;
	ts.x = 33;
	ts.y = 14;
	ts.header = "Todos los alumnos (q) cancelar";
	ts.addkey('q');
	ts.addkey('Q');

	std::vector<Grade> matriculas = Grade::allByCourseId(courses[indexcourse].id);
	
	for(int i = 0; i < (int) matriculas.size(); ++i)
	{
		std::vector<std::string> col;

		Student student = Student::find(matriculas[i].student_id);
		std::string nota = (matriculas[i].average == -1)? "Sin nota" : std::to_string(matriculas[i].average);

		col.push_back(student.name);
		col.push_back( nota );
		ts.addcolumn(col);
	}

	int indexstudent = 0;	

		indexstudent = ts.start();

		if(matriculas.size() == 0)
			return;
		if(ts.keypress == 'q' || ts.keypress == 'Q')
			return;

		Form fgrade;
		fgrade.parent = win_parent;	
		fgrade.x = 2;
		fgrade.y = 11;
		fgrade.w = 30;
		fgrade.header = "Ingrese la nota";
		fgrade.footer = "(q) cancelar";
		fgrade.addquittext("q");
		fgrade.addInput("Nota 1", INPUT_STRING, 2);
		fgrade.addInput("Nota 2", INPUT_STRING, 2);
		fgrade.addInput("Nota 3", INPUT_STRING, 2);

		if(fgrade.start() == false)
			return;

		Grade g = Grade::find(matriculas[indexstudent].id);	
		g.grade1 = atoi(fgrade.responses[0].c_str());
		g.grade2 = atoi(fgrade.responses[1].c_str());
		g.grade3 = atoi(fgrade.responses[2].c_str());
		g.average = (g.grade1 + g.grade2 + g.grade3) / 3;

		g.save();

		Dialog dialog;
		dialog.parent = win_parent;
		dialog.vc = dialog.hc = true;
		dialog.w = 40;
		dialog.type = DIALOG_INFORMATION;
		dialog.addlinebody("Se ha actualizado la nota");
		dialog.addlinebody("Promedio : " + std::to_string(g.average));
		dialog.start();
}

void view_courses(Teacher &teacher){
	std::vector<Course> courses = Course::allByTeacher( teacher.id );
	Table t;
	t.parent = win_parent;
	t.addrow("Curso", 30);
	t.addrow("Matriculados", 21);
	t.h = 13;
	t.x = 33;
	t.y = 3;
	t.header = "Todos los cursos";
	t.footer = std::to_string(courses.size()) + " cursos";

	for(int i = 0; i < (int) courses.size(); ++i)
	{
		std::vector<Grade> matriculas_curso = Grade::allByCourseId(courses[i].id);
		std::vector<std::string> col;
		col.push_back(courses[i].name);
		col.push_back( std::to_string(matriculas_curso.size()) + " Estudiantes");
		t.addcolumn(col);
	}

	t.start();
}

