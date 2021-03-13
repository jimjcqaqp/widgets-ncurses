#include "Teacher.h"

#define PATH	"data.db"

Teacher::Teacher(){}
Teacher::~Teacher(){}

static int callback_find(void *data, int argc, char **argv, char **cols);
static int callback_all(void *data, int argc, char **argv, char **cols);
// FIND
Teacher Teacher::find(int id){
	std::string ssql;
	ssql = "SELECT * FROM teachers WHERE id='?'";
	boost::replace_first(ssql, "?", std::to_string(id));

	Teacher t;
	t.id = 0;
	sqlite3 *db;
	if(sqlite3_open(PATH, &db))
		return t;
	sqlite3_exec(db, ssql.c_str(), &callback_find, &t, NULL);
	sqlite3_close(db);
	return t;	
}
// FIND CODE
Teacher Teacher::findcode(std::string code){
	std::string ssql;
	ssql = "SELECT * FROM teachers WHERE code='?'";
	boost::replace_first(ssql, "?", code);

	Teacher t;
	t.id = 0;
	sqlite3 *db;
	if(sqlite3_open(PATH, &db))
		return t;
	sqlite3_exec(db, ssql.c_str(), &callback_find, &t, NULL);
	sqlite3_close(db);
	return t;	
}
// ALL
std::vector<Teacher> Teacher::all(){
	std::vector<Teacher> ts;

	sqlite3 *db;
	if(sqlite3_open(PATH, &db)){
		return ts; 
	}
	sqlite3_exec(db, "SELECT * FROM teachers", &callback_all, &ts, NULL);	
	sqlite3_close(db);
	return ts;
}
// SAVE
bool Teacher::save(){
	if(code.size() == 0 || name.size() == 0)
		return false;	
	std::string ssql;
	if(id == 0)
		ssql = "INSERT INTO teachers (code, name) VALUES('?', '?')";
	else
		ssql = "UPDATE teachers SET code='?', name='?' WHERE id=" + std::to_string(id);
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
	std::vector<Teacher> *ts = (std::vector<Teacher> *) data;
	Teacher t;
	t.id = atoi(argv[0]);
	t.code = argv[1];
	t.name = argv[2];
	ts->push_back(t);
	return 0;
}

int callback_find(void *data, int argc, char **argv, char **cols)
{
	Teacher *t = (Teacher *)data;
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
	Teacher t;
	t.code = argv[1];
	t.name = argv[2];
	t.save();

	std::vector<Teacher> ts = Teacher::all();
	for(int i = 0; i < ts.size(); ++i)
		std::cout << ts[i].id << "\t" << ts[i].code << "\t" << ts[i].name << std::endl;

	return 0;
}
*/ 
