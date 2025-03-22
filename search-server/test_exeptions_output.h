/*Для тестировании системы работы исключений мной было принято решение создать специальные "тестирующие функции".*/
/*Работа функций основана на ожидании выдачи исключения без использования сравнения, т.к. сравнивать нечего.*/
/*В большинсвте случаев тестирования используется переменная "number_of_unsuccessful_exceptions_thrown" выступающая в 
качестве счетчика некорректных обработок исключения, в случае, если хотя бы один из кейсов увеличит значение счетчика, 
то сообщение о прохождении теста выведено не будет, вместо этого будет использован макрос, дающий подсказку в какой
функции и какой строке произошла ошибка для каждого из некорректно исполненных кейсов.*/
#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "search_server.h"

#define INCORRECT_EXEPTION_HANDLING std::cerr << "Incorrect exception handling for function: "s << __FUNCTION__ << ": " << __LINE__ << std::endl;

void TestExeptionForConstructorWithStopWords();
void TestExeptionForDocumentsWithSameIndex();
void TestExeptionForDocumentsWithNegativeIndex();
void TestExeptionForDocumentsWithSpecialCharacters();
void TestExeptionForQueryWithSpecialCharacters();
void TestExeptionForQueryWithMinusWords();
void TestExeptionForGetDocumentId();
