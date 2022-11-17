#ifndef CURRENCY
#define CURRENCY
#include <string>
using namespace std;

typedef struct course{								//Список курсов продажи валюты
	int idTo;								//id валюты, за которую покупается
	double courseTo;							//Количество у.е. новой валюты при продаже у.е. этой
	struct course *next;
	struct course *prev;
} course_t;

class Currency{
protected:
	int id;
	string name;
	course_t *course;
public:
	~Currency();
	Currency();								//Конструктор по умолчанию(только для компилятора)
	Currency(int, string);							//Основной конструктор
	Currency(const Currency&);						//Конструктор копирования
	double convert(int, double);						//Конвертирует количество, указанной вторым аргументом, данной валюты в валюту, указанной в первом аргументе. Возвращает количество полученной валюты в у.е.
	string getName();							//Возвращает название валюты
	int getId();								//Возвращает id валюты
	void addCourse(int, double);						//Добавляет курс продажи валюты
	course_t* getCourse();							//Возвращает все сохранённые курсы продажи этой валюты
	int operator==(Currency);
	Currency& operator=(Currency);
	void print();								//Выводит информацию о валюте (название, id и курсы продажи)
};

#endif
