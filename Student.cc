#include "Student.h"

#define PATH	"data.db"

Student::Student(){}
Student::~Student(){}

static int callback_find(void *data, int argc, char **argv, char **cols);
static int callback_all(void *data, int argc, char **argv, char **cols);
// FIND
Student Student::find(int id){
	std::string ssql;
	ssql = "SELECT * FROM students WHERE id='?'";
	boost::replace_first(ssql, "?", std::to_string(id));

	Student t;
	t.id = 0;
	sqlite3 *db;
	if(sqlite3_open(PATH, &db))
		return t;
	sqlite3_exec(db, ssql.c_str(), &callback_find, &t, NULL);
	sqlite3_close(db);
	return t;	
}
// ALL
std::vector<Student> Student::all(){
	std::vector<Student> ts;

	sqlite3 *db;
	if(sqlite3_open(PATH, &db)){
		return ts; 
	}
	sqlite3_exec(db, "SELECT * FROM students", &callback_all, &ts, NULL);	
	sqlite3_close(db);
	return ts;
}
// SAVE
bool Student::save(){
	if(code.size() == 0 || name.size() == 0)
		return false;	
	std::string ssql;
	if(id == 0)
		ssql = "INSERT INTO students (code, name) VALUES('?', '?')";
	else
		ssql = "UPDATE students SET code='?', name='?' WHERE id=" + std::to_string(id);
	boost::replace_first(ssql, "?", code);
	boost::replace_first(ssql, "?", name);
	
	sqlite3 *db;
	if(sqlite3_open(PATH, &db))
		return false;

	if(sqlite3_exec(db, ssql.c_str(), NULL, NULL, NULL) != SQLITE_OK){
		sqlite3_close(db);
		return false;
	}
	sqlite3_close(db);
	return true;
}

int callback_all(void *data, int argc, char **argv, char **cols)
{
	std::vector<Student> *ts = (std::vector<Student> *) data;
	Student t;
	t.id = atoi(argv[0]);
	t.code = argv[1];
	t.name = argv[2];
	ts->push_back(t);
	return 0;
}

int callback_find(void *data, int argc, char **argv, char **cols)
{
	Student *t = (Student *)data;
	t->id = atoi(argv[0]);	
	t->code = argv[1];
	t->name = argv[2];
	return 0;
}

/*
int main(int argc, char **argv)
{
	if(argc != 3)
		return 1;
	Student t;
	t.code = argv[1];
	t.name = argv[2];
	t.save();

	std::vector<Student> ts = Student::all();
	for(int i = 0; i < ts.size(); ++i)
		std::cout << ts[i].id << "\t" << ts[i].code << "\t" << ts[i].name << std::endl;

	return 0;
}
*/ 
