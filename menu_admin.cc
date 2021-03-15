#include "menu_admin.h"
#include "Dialog.h"
#include "Table.h"
#include "Form.h"

#include "Teacher.h"
#include "Course.h"
#include "Student.h"
#include "Grade.h"

#define ADMIN_USER_DEFAULT	"root"
#define ADMIN_PASSWORD_DEFAULT 	"root"

#define GRADE_MIN	10
#define GRADE_MAX 20

static WINDOW *win_parent;

static void add_teacher();
static void add_course ();
static void add_student();
static void view_teachers();
static void view_courses();
static void view_students();
static void register_student();
static void add_dependency();

void menu_admin(Form &formlogin)
{
	win_parent = formlogin.parent;

	if(formlogin.responses[0] != ADMIN_USER_DEFAULT ||
		 formlogin.responses[1] != ADMIN_PASSWORD_DEFAULT){
		Dialog derr;
		derr.hc = derr.vc = true;
		derr.w = 40;
		derr.type = DIALOG_INFORMATION;
		derr.header = "Login failed";
		derr.addlinebody("User or password incorrect");
		derr.start();
	return;
	}

	formlogin.clear();

	Menu madmin;
	madmin.parent = win_parent;
	madmin.add("Agregar profesor");
	madmin.add("Agregar curso");
	madmin.add("Agregar estudiante");
	madmin.add("Ver profesores");
	madmin.add("Ver cursos");
	madmin.add("Ver estudiantes");
	madmin.add("Matricular alumno a curso");
	madmin.add("Agregar dependencia a curso");
	madmin.addkey('q');
	madmin.addkey('Q');
	madmin.footer = "(q) para salir";
	// madmin.vc = true;
	madmin.x = 2;
	madmin.y = 3;
	madmin.w = 30;
	madmin.header = "Menu administrador";

	while(true){
		int index = -1;
		if(madmin.running() == true)
			index = madmin.resume();
		else
			index = madmin.start();

		if(madmin.keypress == 'q' || madmin.keypress == 'Q'){
			madmin.resetkey();
			break;
		}
		
		switch(index){
			case 0: add_teacher(); break;
			case 1: add_course(); break;
			case 2: add_student(); break;
			case 3: view_teachers(); break;
			case 4: view_courses(); break;
			case 5: view_students(); break;
			case 6: register_student(); break;
			case 7: add_dependency(); break;
		}
	}
}

void add_teacher(){
	Form f;
	f.parent = win_parent;
	f.x = 34;	
	f.y = 3;
	f.w = 50;
	f.header = "Nuevo profesor";
	f.footer = "(q) para cancelar";
	f.addquittext("q");
	f.addInput("Codigo del profesor", INPUT_STRING, 10);
	f.addInput("Nombre del profesor", INPUT_STRING, 25);
	
	if(f.start() == false)
		return;

	Teacher newteacher;
	newteacher.code = f.responses[0];
	newteacher.name = f.responses[1];

	bool response = newteacher.save();

	Dialog dialog;
	dialog.w = 40;
	dialog.hc = true;
	dialog.vc = true;
	dialog.type = DIALOG_INFORMATION;

	if(response == true)
	{
		dialog.header = "Completado";
		dialog.addlinebody("Se ha ingresado correctamente");
		dialog.addlinebody("al profesor");
		dialog.addlinebody(f.responses[1]);
		dialog.start();
	}
	else
	{
		dialog.header = "ERROR";
		dialog.align = DIALOG_ALIGN_LEFT;
		dialog.addlinebody("1. El codigo puede estar en uso");
		dialog.addlinebody("2. Otra aplicacion esta usando la DB");
		dialog.start();
	}
}
void add_course()
{
	Form f;
	f.parent = win_parent;
	f.x = 33;
	f.y = 3;
	f.w = 50;
	f.header = "Nuevo Curso";
	f.footer = "(q) para cancelar";
	f.addquittext("q");
	f.addInput("Codigo del curso", INPUT_STRING, 10);
	f.addInput("Nombre del curso", INPUT_STRING, 25);
	
	std::vector<Teacher> teachers = Teacher::all();
	std::vector<std::string> viewteachers;
	for(int i = 0; i < (int) teachers.size(); ++i)
		viewteachers.push_back(teachers[i].name);
	f.addSelect("Encargado del curso", viewteachers, 30);
	if(f.start() == false){
		return;
	}

	// prepare dialog
	Dialog dialog;
	dialog.w = 40;
	dialog.hc = true;
	dialog.vc = true;
	dialog.type = DIALOG_INFORMATION;

	bool response = false;
	if(teachers.size() != 0)
	{
		Course course;
		course.code = f.responses[0];
		course.name = f.responses[1];
		course.teacher = teachers[ atoi(f.responses[2].c_str()) ];
		response = course.save();
	}
	else{
		dialog.header = "Error";
		dialog.addlinebody("No se pudo continuar puede continuar");
		dialog.addlinebody("por que no hay profesores");
		dialog.start();
		return;
	}

	if(response == true)
	{
		dialog.header = "Completado";
		dialog.addlinebody("Se ha ingresado correctamente");
		dialog.addlinebody("el curso");
		dialog.addlinebody(f.responses[1]);
		dialog.start();
	}
	else
	{
		dialog.header = "ERROR";
		dialog.align = DIALOG_ALIGN_LEFT;
		dialog.addlinebody("1. El codigo puede estar en uso");
		dialog.addlinebody("2. Otra aplicacion esta usando la DB");
		dialog.start();
	}
}
void add_student(){
	Form f;
	f.parent = win_parent;
	f.x = 33;	
	f.y = 3;
	f.w = 50;
	f.header = "Nuevo estudiante";
	f.footer = "(q) para cancelar";
	f.addquittext("q");
	f.addInput("Codigo del estudiante", INPUT_STRING, 10);
	f.addInput("Nombre del estudiante", INPUT_STRING, 25);
	if(f.start() == false)
		return;

	Student student;
	student.code = f.responses[0];
	student.name = f.responses[1];

	bool response = student.save();

	Dialog dialog;
	dialog.w = 40;
	dialog.hc = true;
	dialog.vc = true;
	dialog.type = DIALOG_INFORMATION;

	if(response == true)
	{
		dialog.header = "Completado";
		dialog.addlinebody("Se ha ingresado correctamente");
		dialog.addlinebody("al estudiante");
		dialog.addlinebody(f.responses[1]);
		dialog.start();
	}
	else
	{
		dialog.header = "ERROR";
		dialog.align = DIALOG_ALIGN_LEFT;
		dialog.addlinebody("1. El codigo puede estar en uso");
		dialog.addlinebody("2. Otra aplicacion esta usando la DB");
		dialog.start();
	}
}
void view_teachers(){

	std::vector<Teacher> teachers = Teacher::all();

	Table t;
	t.parent = win_parent;
	t.addrow("Codigo", 15);
	t.addrow("Nombre", 36);
	t.h = 14;
	t.x = 33;
	t.y = 3;
	t.header = "Todos los profesores";
	t.footer = "Total de profesores " + std::to_string(teachers.size()) + " (e) editar";
	t.addkey('e');
	t.addkey('E');

	for(int i = 0; i < (int) teachers.size(); ++i){
		std::vector<std::string> viewt;
		viewt.push_back(teachers[i].code);
		viewt.push_back(teachers[i].name);
		t.addcolumn(viewt);
	}

	int index = t.start();
	if(teachers.size() == 0)
		return;

	if(t.keypress == 'e' ||  t.keypress == 'E'){
		t.clear();	
		//////////// edit /////////////
		Form f;
		f.parent = win_parent;
		f.x = 34;	
		f.y = 3;
		f.w = 50;
		f.header = "Editar " + teachers[index].name;
		f.footer = "(q) para cancelar - (d) default";
		f.addquittext("q");
		f.addInput("Codigo del profesor (e)", INPUT_STRING, 10);
		f.addInput("Nombre del profesor (e)", INPUT_STRING, 25);
		
		if(f.start() == false)
			return;

		Teacher newteacher = Teacher::find(teachers[index].id);
		if(f.responses[0] != "d") newteacher.code = f.responses[0];
		if(f.responses[1] != "d") newteacher.name = f.responses[1];

		bool response = newteacher.save();

		Dialog dialog;
		dialog.w = 40;
		dialog.hc = true;
		dialog.vc = true;
		dialog.type = DIALOG_INFORMATION;

		if(response == true)
		{
			dialog.header = "Completado";
			dialog.addlinebody("Se ha editado correctamente");
			dialog.addlinebody("al profesor");
			dialog.addlinebody(f.responses[1]);
			dialog.start();
		}
		else
		{
			dialog.header = "ERROR";
			dialog.align = DIALOG_ALIGN_LEFT;
			dialog.addlinebody("1. El codigo puede estar en uso");
			dialog.addlinebody("2. Otra aplicacion esta usando la DB");
			dialog.start();
		}
		///////////////////////////////	
	}
}
void view_courses(){
	std::vector<Course> courses = Course::all();

	Table t;
	t.parent = win_parent;
	t.addrow("Codigo", 10);
	t.addrow("Nombre", 20);
	t.addrow("Dependencia", 20);
	t.h = 14;
	t.x = 33;
	t.y = 3;
	t.addkey('e');
	t.addkey('E');
	t.header = "Todos los cursos";
	t.footer = "Total de cursos " + std::to_string(courses.size()) + " (e) editar";
	
	for(int i = 0; i < (int)courses.size(); ++i)
	{
		std::vector<std::string> col;
		col.push_back(courses[i].code);
		col.push_back(courses[i].name);
		Course coursedep = Course::find(courses[i].course_id);
		col.push_back(coursedep.name);
		t.addcolumn(col);
	}
	int index = t.start();
	
	if(t.keypress == 'e' || t.keypress == 'E')
	{
		t.clear();
		/////////////////////////////////
		Form f;
		f.parent = win_parent;
		f.x = 33;	
		f.y = 3;
		f.w = 50;
		f.header = "Editar " + courses[index].name;
		f.footer = "(q) para cancelar";
		f.addquittext("q");
		f.addInput("Codigo del curso", INPUT_STRING, 10);
		f.addInput("Nombre del curso", INPUT_STRING, 25);
		
		std::vector<Teacher> teachers = Teacher::all();
		std::vector<std::string> viewteachers;
		viewteachers.push_back("DEFAULT");
		for(int i = 0; i < (int) teachers.size(); ++i)
			viewteachers.push_back(teachers[i].name);
		f.addSelect("Encargado del curso", viewteachers, 30);
		if(f.start() == false){
			return;
		}

		// prepare dialog
		Dialog dialog;
		dialog.w = 40;
		dialog.hc = true;
		dialog.vc = true;
		dialog.type = DIALOG_INFORMATION;

		bool response = false;
		if(teachers.size() != 0)
		{
			Course course = Course::find(courses[index].id);
		  if(f.responses[0] != "d") course.code = f.responses[0];
			if(f.responses[1] != "d") course.name = f.responses[1];
			if(f.responses[2] != "0") course.teacher = teachers[ atoi(f.responses[2].c_str()) - 1 ];
			response = course.save();
		}
		else
		{
			dialog.header = "Error";
			dialog.addlinebody("No se pudo continuar puede continuar");
			dialog.addlinebody("por que no hay profesores");
			dialog.start();
			return;
		}

		if(response == true)
		{
			dialog.header = "Completado";
			dialog.addlinebody("Se ha editado correctamente");
			dialog.addlinebody("el curso");
			// dialog.addlinebody(f.responses[1]);
			dialog.start();
		}
		else
		{
			dialog.header = "ERROR";
			dialog.align = DIALOG_ALIGN_LEFT;
			dialog.addlinebody("1. El codigo puede estar en uso");
			dialog.addlinebody("2. Otra aplicacion esta usando la DB");
			dialog.start();
		}

		/////////////////////////////////
	}
}
void view_students(){
	std::vector<Student> students = Student::all();
	Table t;
	t.parent = win_parent;
	t.addrow("Codigo", 15);
	t.addrow("Nombre", 36);
	t.h = 14;
	t.x = 33;
	t.y = 3;
	t.header = "Todos los estudiantes";
	t.footer = "Total de estudiantes " + std::to_string(students.size()) + " (e) editar";
	t.addkey('e');
	t.addkey('E');

	for(int i = 0; i < students.size(); ++i)
	{
		std::vector<std::string> col;
		col.push_back(students[i].code);
		col.push_back(students[i].name);
		t.addcolumn(col);
	}
	int index = t.start();
	if(students.size() == 0)
		return ;
	if(t.keypress == 'e' || t.keypress == 'E'){
		t.clear();
		/////////////////////////////////
		Form f;
		f.parent = win_parent;
		f.x = 33;	
		f.y = 3;
		f.w = 50;
		f.header = "Editar " + students[index].name;
		f.footer = "(q) cancelar - (d) default";
		f.addquittext("q");
		f.addInput("Codigo del estudiante (e)", INPUT_STRING, 10);
		f.addInput("Nombre del estudiante (e)", INPUT_STRING, 25);
		if(f.start() == false)
			return;

		Student student = Student::find(students[index].id);;
		if(f.responses[0] != "d") student.code = f.responses[0];
		if(f.responses[1] != "d") student.name = f.responses[1];

		bool response = student.save();

		Dialog dialog;
		dialog.w = 40;
		dialog.hc = true;
		dialog.vc = true;
		dialog.type = DIALOG_INFORMATION;

		if(response == true)
		{
			dialog.header = "Completado";
			dialog.addlinebody("Se ha editado correctamente");
			dialog.addlinebody("al estudiante");
			dialog.addlinebody(f.responses[1]);
			dialog.start();
		}
		else
		{
			dialog.header = "ERROR";
			dialog.align = DIALOG_ALIGN_LEFT;
			dialog.addlinebody("1. El codigo puede estar en uso");
			dialog.addlinebody("2. Otra aplicacion esta usando la DB");
			dialog.start();
		}

		/////////////////////////////////	
	}
}
void register_student(){
	std::vector<Student> students = Student::all();
	std::vector<Course> courses = Course::all();	

	int indexstudent = 0;
	int indexcourse = 0;

	Menu vstudents;
	Menu vcourses;

	vstudents.add("CANCELAR");
	vcourses.add("CANCELAR");
	
	vstudents.parent = win_parent;
	vstudents.w = 50;	
	vstudents.x = 33;
	vstudents.y = 3;
	vstudents.h = 6;
	vstudents.header = "Seleccione al estudiante";

	vcourses.parent = win_parent;
	vcourses.w = 50;
	vcourses.x = 33;
	vcourses.y = 13;
	vcourses.h = 7;
	vcourses.header = "Seleccione el curso";

	for(int i = 0; i < (int)students.size(); ++i)
		vstudents.add(students[i].name);
	for(int i = 0; i < (int)courses.size(); ++i)
		vcourses.add(courses[i].name);
	
	indexstudent =  vstudents.start();
	if(indexstudent == 0)
		return ;
 	indexcourse = vcourses.start();
	if(indexcourse == 0)
		return ;
	
	--indexstudent;
	--indexcourse;
	
	Grade grade = Grade::findByStudentAndCourse( students[indexstudent].id, courses[indexcourse].id);
	if(grade.id != 0){
		Dialog derr;
		derr.parent = win_parent;
		derr.vc = true;
		derr.hc = true;
		derr.w = 40;
		derr.addlinebody("No se pudo registrar al estudiante");
		derr.addlinebody("por que el estudiante ya esta");
		derr.addlinebody("registrado en el curso");
		derr.type = DIALOG_INFORMATION;
		derr.start();
		return ;	
	}

	if(courses[indexcourse].course_id != 0){
		Grade grade = Grade::findByStudentAndCourse(students[indexstudent].id, courses[indexcourse].course_id);
		if(grade.id == 0){
			Dialog derr;
			derr.parent = win_parent;
			derr.hc = derr.vc = true;
			derr.w = 40;
			derr.align = DIALOG_ALIGN_LEFT;
			derr.type = DIALOG_INFORMATION;
			derr.header = "Error";
			derr.addlinebody("El curso tiene el requerimiento de");
			// derr.addlinebody(courses[indexcourse].name);
			derr.addlinebody(Course::find(courses[indexcourse].course_id).name);
			derr.addlinebody("Se tiene que matricular");
			derr.start();
			return;
		}
		if(grade.average < GRADE_MIN){
			Dialog derr;
			derr.parent = win_parent;
			derr.hc = derr.vc = true;
			derr.type = DIALOG_INFORMATION;
			derr.w = 40;
			derr.align = DIALOG_ALIGN_LEFT;
			derr.header = "Error";
			derr.addlinebody("El alumno no tiene nota");
			derr.addlinebody("aprovatoria");
			derr.start();
			return;
		}	
	}

	grade.student_id = students[indexstudent].id;
	grade.course_id = courses[indexcourse].id;

	if(grade.save() == false){
		Dialog derr;
		derr.parent = win_parent;
		derr.vc = true;
		derr.hc = true;
		derr.w = 40;
		derr.addlinebody("Error con la DB");
		derr.type = DIALOG_INFORMATION;
		derr.start();
		return ;
	}

	Dialog dialog;
	dialog.type = DIALOG_INFORMATION;
	dialog.parent = win_parent;
	dialog.hc = true;
	dialog.vc = true;
	dialog.w = 40;
	dialog.addlinebody("Se ha matriculado a");
	dialog.addlinebody(students[indexstudent].name);
	dialog.addlinebody("en " + courses[indexcourse].name);
	dialog.start();
}
void add_dependency(){
	std::vector<Course> courses = Course::all();	
	int indexcourse = 0;
	int indexcoursedep = 0;

	Menu m;
	m.parent = win_parent;
	m.w = 50;
	m.x = 33;
	m.y = 3;
	m.h = 6;
	m.header = "Curso dependiente";
	m.add("CANCELAR");
	for(int i = 0; i < (int) courses.size(); ++i)
		m.add(courses[i].name);
	indexcourse = m.start();
	if(indexcourse == 0)
		return ;

	Menu mdep;
	mdep.parent = win_parent;
	mdep.w = 50;
	mdep.x = 33;
	mdep.y = 13;
	mdep.h = 6;
	mdep.header = "Curso de dependencia";
	mdep.add("CANCELAR");
	for(int i = 0; i < (int) courses.size(); ++i)
		mdep.add(courses[i].name);
	indexcoursedep = mdep.start();
	if(indexcoursedep == 0)
		return;

	--indexcourse;
	--indexcoursedep;

	if(indexcourse == indexcoursedep){
		Dialog derr;
		derr.parent = win_parent;
		derr.type = DIALOG_INFORMATION;
		derr.hc = derr.vc = true;
		derr.w = 40;
		derr.header = "Error";
		derr.addlinebody("El mismo curso no puede");
		derr.addlinebody("ser dependencia");
		derr.start();
		return;	
	}
	Course course = Course::find(courses[indexcourse].id);
	course.course_id = courses[indexcoursedep].id;
	if(course.save() == false){
		Dialog dialog;
		dialog.type = DIALOG_INFORMATION;
		dialog.hc = dialog.vc = true;
		dialog.parent = win_parent;
		dialog.w = 40;
		dialog.header = "ERROR";
		dialog.addlinebody("Otra aplicacion esta usando la DB");
		dialog.start();
		return;	
	}
	Dialog dialog;
	dialog.type = DIALOG_INFORMATION;
	dialog.hc = dialog.vc = true;
	dialog.parent = win_parent;
	dialog.w = 40;
	dialog.header = "Completado";
	dialog.addlinebody("Se ha agregado correctamente");
	dialog.addlinebody("la dependencia");
	dialog.start();
}

