#include "Grade.h"

#define PATH	"data.db"

Grade::Grade(){
	id = 0;
	grade1 = -1;
	grade2 = -1;
	grade3 = -1;
	average = -1;
	student_id = 0;
	course_id = 0;
}
Grade::~Grade(){}

static int callback_find(void *data, int argc, char **argv, char **cols);
static int callback_all(void *data, int argc, char **argv, char **cols);
// FIND
Grade Grade::find(int id){
	std::string ssql;
	ssql = "SELECT * FROM grades WHERE id='?'";
	boost::replace_first(ssql, "?", std::to_string(id));

	Grade t;
	t.id = 0;
	sqlite3 *db;
	if(sqlite3_open(PATH, &db))
		return t;
	sqlite3_exec(db, ssql.c_str(), &callback_find, &t, NULL);
	sqlite3_close(db);
	return t;
}
// FIND STUDENT AND COURSE
Grade Grade::findByStudentAndCourse(int student_id, int course_id){
	std::string ssql;
	ssql = "SELECT * FROM grades WHERE student_id='?' AND course_id='?'";
	boost::replace_first(ssql, "?", std::to_string(student_id));
	boost::replace_first(ssql, "?", std::to_string(course_id));

	Grade t;
	t.id = 0;
	sqlite3 *db;
	if(sqlite3_open(PATH, &db))
		return t;
	sqlite3_exec(db, ssql.c_str(), &callback_find, &t, NULL);
	sqlite3_close(db);
	return t;
}


// ALL
std::vector<Grade> Grade::all(){
	std::vector<Grade> ts;

	sqlite3 *db;
	if(sqlite3_open(PATH, &db)){
		return ts; 
	}
	sqlite3_exec(db, "SELECT * FROM grades", &callback_all, &ts, NULL);	
	sqlite3_close(db);
	return ts;
}
// ALL BY STUDENT
std::vector<Grade> Grade::allByStudentId(int id){
	std::vector<Grade> ts;

	sqlite3 *db;
	if(sqlite3_open(PATH, &db)){
		return ts; 
	}
	std::string ssql = "SELECT * FROM grades WHERE student_id=" + std::to_string(id);
	sqlite3_exec(db, ssql.c_str(), &callback_all, &ts, NULL);	
	sqlite3_close(db);
	return ts;
}
// ALL BY COURSE
std::vector<Grade> Grade::allByCourseId(int id){
	std::vector<Grade> ts;

	sqlite3 *db;
	if(sqlite3_open(PATH, &db)){
		return ts; 
	}
	std::string ssql = "SELECT * FROM grades WHERE course_id=" + std::to_string(id);
	sqlite3_exec(db, ssql.c_str(), &callback_all, &ts, NULL);	
	sqlite3_close(db);
	return ts;
}

// SAVE
bool Grade::save(){
	if(course_id == 0 || student_id == 0)
		return false;	
	std::string ssql;
	if(id == 0)
		ssql = "INSERT INTO grades (grade1, grade2, grade3, average, student_id, course_id) VALUES(?,?,?,?,?,?)";
	else
		ssql = "UPDATE grades SET grade1=?, grade2=?, grade3=?, average=?, student_id=?, course_id=? WHERE id=" + std::to_string(id);
	boost::replace_first(ssql, "?", (grade1 == -1)? "null" : std::to_string(grade1));
	boost::replace_first(ssql, "?", (grade2 == -1)? "null" : std::to_string(grade2));
	boost::replace_first(ssql, "?", (grade3 == -1)? "null" : std::to_string(grade3));
	boost::replace_first(ssql, "?", (average == -1)? "null" : std::to_string(average));
	boost::replace_first(ssql, "?", std::to_string(student_id));
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
	std::vector<Grade> *ts = (std::vector<Grade> *) data;
	Grade t;
	t.id = atoi(argv[0]);
	t.grade1 = argv[1] != NULL? atoi(argv[1]): -1;
	t.grade2 = argv[2] != NULL? atoi(argv[2]): -1;
	t.grade3 = argv[3] != NULL? atoi(argv[3]): -1;
	t.average = argv[4] != NULL? atoi(argv[4]): -1;
	t.student_id = argv[5] != NULL? atoi(argv[5]): 0;
	t.course_id = argv[6] != NULL? atoi(argv[6]): 0;
	ts->push_back(t);
	return 0;
}

int callback_find(void *data, int argc, char **argv, char **cols)
{
	Grade *t = (Grade *)data;
	t->id = atoi(argv[0]);	
	t->grade1 = argv[1] != NULL? atoi(argv[1]): -1;
	t->grade2 = argv[2] != NULL? atoi(argv[2]): -1;
	t->grade3 = argv[3] != NULL? atoi(argv[3]): -1;
	t->average = argv[4] != NULL? atoi(argv[4]): -1;
	t->student_id = argv[5] != NULL? atoi(argv[5]): 0;
	t->course_id = argv[6] != NULL? atoi(argv[6]): 0;
	return 0;
}

