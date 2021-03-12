#include "Course.h"

#define PATH	"data.db"

Course::Course(){
	id = 0;
	course_id = 0;
	code = "";
	name = "";
}
Course::~Course(){}

static int callback_find(void *data, int argc, char **argv, char **cols);
static int callback_all(void *data, int argc, char **argv, char **cols);
// FIND
Course Course::find(int id){
	std::string ssql;
	ssql = "SELECT * FROM courses WHERE id='?'";
	boost::replace_first(ssql, "?", std::to_string(id));

	Course t;
	t.id = 0;
	sqlite3 *db;
	if(sqlite3_open(PATH, &db))
		return t;
	sqlite3_exec(db, ssql.c_str(), &callback_find, &t, NULL);
	sqlite3_close(db);
	return t;	
}
// ALL
std::vector<Course> Course::all(){
	std::vector<Course> ts;

	sqlite3 *db;
	if(sqlite3_open(PATH, &db)){
		return ts; 
	}
	sqlite3_exec(db, "SELECT * FROM courses", &callback_all, &ts, NULL);	
	sqlite3_close(db);
	return ts;
}

// ALL COURSES
std::vector<Course> Course::allByTeacher(int id){
	std::vector<Course> ts;

	sqlite3 *db;
	if(sqlite3_open(PATH, &db)){
		return ts; 
	}
	std::string ssql = "SELECT * FROM courses WHERE teacher_id=" + std::to_string(id);
	sqlite3_exec(db, ssql.c_str(), &callback_all, &ts, NULL);	
	sqlite3_close(db);
	return ts;
}


// SAVE
bool Course::save(){
	if(code.size() == 0 || name.size() == 0 || teacher.id == 0)
		return false;	
	std::string ssql;
	if(id == 0)
		ssql = "INSERT INTO courses (code, name, teacher_id, course_id) VALUES('?', '?', '?', ?)";
	else
		ssql = "UPDATE courses SET code='?', name='?', teacher_id='?', course_id=? WHERE id=" + std::to_string(id);
	boost::replace_first(ssql, "?", code);
	boost::replace_first(ssql, "?", name);
	boost::replace_first(ssql, "?", std::to_string(teacher.id));
	if(course_id == 0) 
		boost::replace_first(ssql, "?", "null");
	else
		boost::replace_first(ssql, "?", std::to_string(course_id));

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
	std::vector<Course> *ts = (std::vector<Course> *) data;
	Course t;
	t.id = atoi(argv[0]);
	t.code = argv[1];
	t.name = argv[2];
	t.teacher = Teacher::find(atoi(argv[3]));
	if(argv[4] != NULL)
		t.course_id = atoi(argv[4]);
	else
		t.course_id = 0;
	ts->push_back(t);
	return 0;
}

int callback_find(void *data, int argc, char **argv, char **cols)
{
	Course *t = (Course *)data;
	t->id = atoi(argv[0]);	
	t->code = argv[1];
	t->name = argv[2];
	t->teacher = Teacher::find(atoi(argv[3]));
	if(argv[4] != NULL)
		t->course_id = atoi(argv[4]);
	else
		t->course_id = 0;
	return 0;
}

/*
int main(int argc, char **argv)
{
	if(argc != 3)
		return 1;
	Course t;
	t.code = argv[1];
	t.name = argv[2];
	t.save();

	std::vector<Course> ts = Course::all();
	for(int i = 0; i < ts.size(); ++i)
		std::cout << ts[i].id << "\t" << ts[i].code << "\t" << ts[i].name << std::endl;

	return 0;
}
*/ 
