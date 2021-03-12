#include "menu_admin.h"
#include "Table.h"
#include "Form.h"

#define ADMIN_USER_DEFAULT	"root"
#define ADMIN_PASSWORD_DEFAULT 	"root"

WINDOW *win_parent;

static void add_teacher();
static void add_course ();
static void add_student();
static void view_teachers();
static void view_courses();
static void view_students();

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
	madmin.add("Matricular alumno a curso");
	madmin.add("Ver profesores");
	madmin.add("Ver cursos");
	madmin.add("Ver estudiantes");
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
			case 3: break;
			case 4: view_teachers(); break;
			case 5: view_courses(); break;
			case 6: view_students(); break;
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
	f.addInput("Codigo del profesor", INPUT_STRING, 10);
	f.addInput("Nombre del profesor", INPUT_STRING, 25);
	f.start();

	Dialog dialog;
	dialog.w = 40;
	dialog.hc = true;
	dialog.vc = true;
	dialog.header = "Completado";
	dialog.type = DIALOG_INFORMATION;
	dialog.addlinebody("Se ha ingresado correctamente");
	dialog.addlinebody("al profesor");
	dialog.addlinebody(f.responses[1]);
	dialog.start();
}
void add_course(){
	Form f;
	f.parent = win_parent;
	f.x = 33;	
	f.y = 3;
	f.w = 50;
	f.header = "Nuevo Curso";
	f.addInput("Codigo del curso", INPUT_STRING, 10);
	f.addInput("Nombre del curso", INPUT_STRING, 25);
	
	std::vector<std::string> teachers;
	teachers.push_back("Johana Chullunquia Quispe");
	teachers.push_back("Anny Chullunquia Quispe");
	f.addSelect("Encargado del curso", teachers, 30);

	f.start();

	Dialog dialog;
	dialog.w = 40;
	dialog.hc = true;
	dialog.vc = true;
	dialog.header = "Completado";
	dialog.type = DIALOG_INFORMATION;
	dialog.addlinebody("Se ha ingresado correctamente");
	dialog.addlinebody("el curso de");
	dialog.addlinebody(f.responses[1]);
	dialog.start();
}
void add_student(){
	Form f;
	f.parent = win_parent;
	f.x = 33;	
	f.y = 3;
	f.w = 50;
	f.header = "Nuevo estudiante";
	f.addInput("Codigo del estudiante", INPUT_STRING, 10);
	f.addInput("Nombre del estudiante", INPUT_STRING, 25);
	f.start();

	Dialog dialog;
	dialog.w = 40;
	dialog.hc = true;
	dialog.vc = true;
	dialog.header = "Completado";
	dialog.type = DIALOG_INFORMATION;
	dialog.addlinebody("Se ha ingresado correctamente");
	dialog.addlinebody("al estudiante");
	dialog.addlinebody(f.responses[1]);
	dialog.start();
}
void view_teachers(){
	Table t;
	t.parent = win_parent;
	t.addrow("Codigo", 15);
	t.addrow("Nombre", 36);
	t.h = 13;
	t.x = 33;
	t.y = 3;
	t.header = "Todos los profesores";
	t.footer = "Total de profesores 30";

	std::vector<std::string> col;
	col.push_back("1234567890");
	col.push_back("Jimmy Josue Chullunquia Quispe");
	t.addcolumn(col);
	col[0] = "0987654321";
	col[1] = "Anny Mabel Chullunquia Quispe";
	t.addcolumn(col);

	t.start();
}
void view_courses(){
	Table t;
	t.parent = win_parent;
	t.addrow("Codigo", 10);
	t.addrow("Nombre", 20);
	t.addrow("Encargado", 20);
	t.h = 13;
	t.x = 33;
	t.y = 3;
	t.header = "Todos los cursos";
	t.footer = "Total de cursos 30";

	std::vector<std::string> col;
	col.push_back("1234567890");
	col.push_back("Ingles I");
	col.push_back("Jimmy Chullunquia Quispe");
	t.addcolumn(col);
	col[0] = "0987654321";
	col[1] = "Ingles II";
	col[2] = "Anny Chullunquia Quispe";
	t.addcolumn(col);

	t.start();
}
void view_students(){
	Table t;
	t.parent = win_parent;
	t.addrow("Codigo", 15);
	t.addrow("Nombre", 36);
	t.h = 13;
	t.x = 33;
	t.y = 3;
	t.header = "Todos los estudiantes";
	t.footer = "Total de estudiantes 30";

	std::vector<std::string> col;
	col.push_back("1234567890");
	col.push_back("Jimmy Josue Chullunquia Quispe");
	t.addcolumn(col);
	col[0] = "0987654321";
	col[1] = "Anny Mabel Chullunquia Quispe";
	t.addcolumn(col);

	t.start();
}

