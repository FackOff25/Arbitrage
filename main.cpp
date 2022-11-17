#include <iostream>
#include <fstream>
#include <string>
#include "currency.h"

typedef struct currencyList{											//Список валют
	Currency currency;
	struct currencyList *next;
	struct currencyList *prev;
}  CurrencyList;

void readLine(CurrencyList, int*, string, string, double);
CurrencyList* getElNumber(int, CurrencyList*);
CurrencyList* getElNumber(int, CurrencyList*, Currency[], int);
void fillAndCheckChain(Currency[], CurrencyList*, int, int, int);

//Разбирает достанные из файла названия валют и курс
void readLine(CurrencyList *list, int* id, string firstCurrency, string secondCurrency, double course){
	Currency currency1, currency2, *pCurrency1, *pCurrency2;
	course_t *currencyCourse;

	while (list->prev != NULL && list->currency.getName() != firstCurrency) list = list->prev;
	while (list->next != NULL && list->currency.getName() != firstCurrency) list = list->next;
	if(list->currency.getName() != firstCurrency) {							//Если валюты нет в списке, добаляет её
		CurrencyList *toSave = list;
		list->next = new currencyList;
		list = list->next;
		list->next = NULL;
		list->prev = toSave;
		list->currency = Currency(*id, firstCurrency);
		*id = *id + 1;
	};
	pCurrency1 = &(list->currency);									//Сохранение адреса валюты, для дальнейшего сохранения после обновления курсов
	currency1 = list->currency;
	
	while (list->prev!=NULL && list->currency.getName()!=secondCurrency) list = list->prev;
	while (list->next!=NULL && list->currency.getName()!=secondCurrency) list = list->next;
	if(list->currency.getName() != secondCurrency) {							//Если валюты нет в списке, добаляет её
		CurrencyList *toSave = list;
		list->next = new currencyList;
		list = list->next;
		list->next = NULL;
		list->prev = toSave;
		list->currency = Currency(*id, secondCurrency);
		*id = *id + 1;
	};
	pCurrency2 = &(list->currency);									//Сохранение адреса валюты, для дальнейшего сохранения после обновления курсов
	currency2 = list->currency;
	
	currency1.addCourse(currency2.getId(), course);
	
	*pCurrency1 = currency1;
	*pCurrency2 = currency2;
}

//Проверяет все "цепочки" валют (обмены, происходящие по цепочке)
void checkChains(CurrencyList *list, int numberOfCurrencies){
	Currency *chain = new Currency[numberOfCurrencies];
	
	for (int count=0; count<numberOfCurrencies; count++) {
		fillAndCheckChain(chain, list, count, numberOfCurrencies, 0);
	}
	
	delete [] chain;
}

//Рекрсивная функция для проверки цепочки
void fillAndCheckChain(Currency chain[], CurrencyList *list, int numOfNext, int len, int ready){
	if(len>ready) {
		double cu = 100;
		chain[ready] = getElNumber(numOfNext, list, chain, ready)->currency;
		
		for(int count=0; count<ready+1; count++) cu=chain[count].convert(chain[(count+1)%(ready+1)].getId(), cu);	//Проверка наличия выгоды при этой цепочке валют
		if (cu>101){
			for(int count=0; count<ready+1; count++) std::cout << chain[count].getName()  << "->";
			std::cout << chain[0].getName() << ": profit is " << cu-100 << "%" << std::endl;
			return;										//При нахождении выгоды проверка цепочек, содержащих эту, не имеет смысла, поэтому они далее не проверяются
		}
	}
	
	for(int count2=0; count2<len-ready-1 || (count2==0 && count2<len-ready); count2++) {		//Перебор валют для следующего места в цепочке
		
		fillAndCheckChain(chain, list, count2, len, ready+1);
	}
	
	
}

//Возвращает num по счёту элемент списка валют
CurrencyList* getElNumber(int num, CurrencyList* list){
	while (list->prev != NULL) list=list->prev;
	for(int count=0; count<num; count++) {
		list = list->next;
	}
	return list;
}

//Возвращает num по счёту элемент списка валют, пропуская те, что находятся в переданном массиве
CurrencyList* getElNumber(int num, CurrencyList* list, Currency toSkip[], int len){
	while (list->prev != NULL) list=list->prev;
	for(int count=0; count<len; count++) {
		for(int i=0; i<len; i++){
			if(toSkip[i] == list->currency) {
				list = list->next;
				break;
			}
		}
	}
	for(int count=0; count<num; count++) {
		list = list->next;
		for(int i=0; i<len; i++){
			if(toSkip[i] == list->currency) {
				list = list->next;
				break;
			}
		}
	}
	for(int count=0; count<len; count++) {
		for(int i=0; i<len; i++){
			if(toSkip[i] == list->currency) {
				list = list->next;
				break;
			}
		}
	}
	return list;
}

int main(int argc, char** argv){
	std::ifstream file;
	//file.open("course2.txt");
	file.open(argv[1]);
	if ( !file.is_open() ) {							//Проверка возможности открыть файл
		std::cout << "File can't be opened" << std::endl;
		return 0;
	}
	
	int id = 1;									//id для простого хранения курсов
	string firstCurrency, secondCurrency;
	double course;
	
	CurrencyList *list = new CurrencyList;					//Создание списка
	list->next = NULL;
	list->prev = NULL;
	
	file >> firstCurrency >> secondCurrency >> course;
	list->currency = Currency(id, firstCurrency);
	id++;
	readLine(list, &id, firstCurrency, secondCurrency, course);
		
	while ( file.good() ) {
		file >> firstCurrency >> secondCurrency >> course;
		readLine(list, &id, firstCurrency, secondCurrency, course);	
	};
	
	
	/*while (list->prev != NULL) list = list->prev;				//Вывод курсов
	list->currency.print();
	do{
		list = list->next;
		list->currency.print();
	}while (list->next != NULL);*/
	
	checkChains(list, id-1);
	
	while (list->next != NULL) list=list->next;					//Очистка списка
	do{
		list=list->prev;
		delete list->next;
	}while (list->prev != NULL);
	delete list;
	
	return 0;  
}

