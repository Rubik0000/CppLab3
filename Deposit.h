#pragma once
#include <string>
#include <fstream>
#include "Date.h"


//тип счета
struct TypeDeposit {
	std::string typeName;
	bool incMoney;
	bool decMoney;
	double percent;
};

//счет
struct Deposit {
	int32_t number;
	std::string fio;
	int32_t sum;
	int32_t idTypeDep;
	Date date;
	int32_t period;
};

extern int32_t const countTypes;
//типы счетов
extern TypeDeposit typDep[3];

//изменить тип счета
int ChangeTypeDep(TypeDeposit &tp);

//вывод счета в консоль
void TypeDepToConsole(TypeDeposit tp);

//Открытие счета для клиента
bool OpenDeposit(Deposit &dep);

//Показ информации о счете
void ShowDeposit(Deposit dep);

//изменение в консоли
bool ChangeDepositForUser(Deposit &dep);

//ввод из файла
bool DepositFromFile(Deposit &dep, std::fstream *f);

//вывод в файл
void DepositToFile(Deposit dep, std::fstream *f);

//сохранение типов счетов в файл
void SaveTypesToFile(std::string fileName);

//загрузка типов счетов из файла
void LoadTypesFromFile(std::string fileName);

