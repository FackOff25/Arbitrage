#include "currency.h"
#include <iostream>
#include <string>

Currency::~Currency(){
	while (course->next != NULL) course = course->next;
	while (course->prev != NULL){
		course = course->prev;
		delete course->next;
	};
	delete course;
};

Currency::Currency(){id=-1;};

//Основной конструктор
Currency::Currency(int _id, string _name){
	id=_id;
	name=_name;
	
	course=new course_t;
	course->idTo=id;
	course->courseTo=1;
	course->next=NULL;
	course->prev=NULL;
};

//Конструктор копирования
Currency::Currency(const Currency & op2){
	id = op2.id;
	name = op2.name;
	course_t *courseToCopy = op2.course;
	while(courseToCopy->prev != NULL) courseToCopy = courseToCopy->prev;
	course = new course_t;
	course->idTo = courseToCopy->idTo;
	course->courseTo = courseToCopy->courseTo;
	course->next = NULL;
	course->prev = NULL;
	
	
	while(courseToCopy->next != NULL){
		course_t *toSave = course;
		courseToCopy = courseToCopy->next;
		
		course->next = new course_t;
		course = course->next;
		course->next = NULL;
		course->prev = toSave;
		course->idTo = courseToCopy->idTo;
		course->courseTo = courseToCopy->courseTo;
	};
};

//Конвертирует количество, указанной вторым аргументом, данной валюты в валюту, указанной в первом аргументе. Возвращает количество полученной валюты в у.е.
double Currency::convert(int newCurrencyId, double number){
	while(course->prev!=NULL && course->idTo!=newCurrencyId) course = course->prev
	;
	while(course->idTo!=newCurrencyId && course->next!=NULL){
		course=course->next;
	};
	if (course->idTo == newCurrencyId) return number*(course->courseTo);
	return 0;								//Если нужный курс не был найден, то прямая покупка невозможна и при попытке получаем 0 у.е. новой валюты.
};

//Возвращает название валюты
string Currency::getName(){return name;};

//Возвращает id валюты
int Currency::getId(){return id;};

//Добавляет курс продажи валюты
void Currency::addCourse(int idTo, double numCourse){
	while (course->next != NULL) course=course->next;
	course_t *toSave=course;
	course->next = new course_t;
	course = course->next;
	course->idTo = idTo;
	course->courseTo = numCourse;
	course->prev = toSave;
	course->next = NULL;
};

//Возвращает все сохранённые курсы продажи этой валюты
course_t* Currency::getCourse(){
	while (course->prev != NULL) course=course->prev;
	return course;
};

int Currency::operator==(Currency op2){
	return id == op2.getId();
};

Currency& Currency::operator=(Currency op2){
	id = op2.id;
	name = op2.name;
	course_t *courseToCopy = op2.getCourse();
	course = new course_t;
	course->idTo = courseToCopy->idTo;
	course->courseTo = courseToCopy->courseTo;
	course->next = NULL;
	course->prev = NULL;
	
	while(courseToCopy->next != NULL){
		course_t *toSave = course;
		courseToCopy = courseToCopy->next;
		
		course->next = new course_t;
		course = course->next;
		course->next = NULL;
		course->prev = toSave;
		course->idTo = courseToCopy->idTo;
		course->courseTo = courseToCopy->courseTo;
	};
	
	return *this;
}

//Выводит информацию о валюте (название, id и курсы продажи)
void Currency::print(){
	while (course->prev != NULL) course=course->prev;
	std::cout << name << " (id = " << id << "):" << endl;
	std::cout << "to " << course->idTo << " = " << course->courseTo << endl;
	do{
		course = course->next;
		std::cout << "to " << course->idTo << " = " << course->courseTo << endl;
	}while (course->next != NULL);
}
