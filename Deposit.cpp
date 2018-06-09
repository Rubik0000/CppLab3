#include <iostream>
#include <iomanip>
#include <ctime>
#include "Utils.h"
#include "Deposit.h"
int32_t const countTypes = 3;
TypeDeposit typDep[countTypes];
/*Изменение типа счета
0 - счет успешно изменен
1 - неудалось изменить счет
2 - пользователь ввел "отмена"*/
int ChangeTypeDep(TypeDeposit & tp) {
	int ans;
	do {
		std::cout << "1 - Название вклада" << std::endl;
		std::cout << "2 - Возможность пополнять средства" << std::endl;
		std::cout << "3 - Возможность снимать средства" << std::endl;
		std::cout << "4 - Процентная ставка" << std::endl;
		std::cout << "0 - Отмена" << std::endl;
	} while (!InputNum(ans) || ans < 0 || ans > 4);
	if (ans == 0) {
		return 2;
	}
	std::string str;
	double per;
	switch (ans) {
	case 1:
		std::cout << "Введите название вклада" << std::endl;
		std::cin >> str;
		if (str == "") {
			return 1;
		}
		tp.typeName = str;
		return 0;
	case 2:
		std::cout << "Разрешить пользователю пополнять средства? " << std::endl;
		tp.incMoney = InputQuery();
		return 0;
	case 3:
		std::cout << "Разрешить пользователю снимать средства? " << std::endl;
		tp.decMoney = InputQuery();
		return 0;
	case 4:
		std::cout << "Введите процениную ставку" << std::endl;
		if (!InputNum(per)) {
			return 1;
		}
		tp.percent = per;
		return 0;
	}
}

//вывод типов в консоль
void TypeDepToConsole(TypeDeposit tp) {
	std::cout << std::setw(20) << std::left << tp.typeName
		<< std::setw(5) << std::left << (tp.incMoney ? "да" : "нет")
		<< std::setw(5) << std::left << (tp.decMoney ? "да" : "нет")
		<< std::setw(5) << std::left << tp.percent * 100;
}

//ввод с консоли
bool OpenDeposit(Deposit &dep) {
	std::cout << "Введите ФИО: ";
	std::string str;
	std::cin >> str;
	if (str == "") {
		return false;
	}
	dep.fio = str;
	int len = sizeof(typDep) / sizeof(typDep[0]);
	std::cout << "Выберите тип счета: " << std::endl;
	for (int i = 0; i < len; ++i) {
		std::cout << "Номер " << i + 1 << std::endl;
		TypeDepToConsole(typDep[i]);
		std::cout << std::endl;
	}
	std::cout << "-> ";
	while (!InputNum(dep.idTypeDep) || dep.idTypeDep < 1 || dep.idTypeDep > len) {
		std::cout << "Введено неверное значение, повторите попытку" << std::endl;
	}
	--dep.idTypeDep;

	std::cout << "Введите сумму: ";
	if (!InputNum(dep.sum) || dep.sum < 0) {
		return false;
	}
	std::cout << "Введите дату открытия счета (дд.мм.гггг): ";
	std::cin >> str;
	if (!DateFromStr(str, dep.date)) {
		return false;
	}
	std::cout << "Введите срок вклада в годах: ";
	if (!InputNum(dep.period) || dep.period < 1) {
		return false;
	}
	std::cout << "Вы уверены, что хотите открыть вклад? ";
	if (!InputQuery()) {
		return false;
	}
	std::srand(std::time(NULL));
	std::cout << "Ваш номер счета: " << 
		(dep.number = std::rand() % 100000) << std::endl;
	return true;
}

//вывод на консоль
void ShowDeposit(Deposit dep) {
	std::cout << std::setw(10) << std::left << dep.number
		<< std::setw(10) << std::left << dep.fio
		<< std::setw(10) << std::left << dep.sum
		<< std::setw(15) << std::left << DateToStr( dep.date)
		<< std::setw(5) << std::left << dep.period;
	TypeDepToConsole(typDep[dep.idTypeDep]);
	std::cout << std::endl;
}

//снятие и зачисление средств для клиента
bool ChangeDepositForUser(Deposit &dep) {
	int ans;
	std::cout << "Выберите действие" << std::endl;
	do {
		std::cout << "1 - Снять средства" << std::endl;
		std::cout << "2 - Пополнить счет" << std::endl;
		std::cout << "0 - Отмена" << std::endl;
	} while (!InputNum(ans) || ans < 0 || ans > 2);
	if (ans == 0) {
		return false;
	}
	int quant;
	//снятие средств
	if (ans == 1) {
		if (!typDep[dep.idTypeDep].decMoney) {
			std::cout
				<< "Вы не можете снимать средства при данном типе вклада" <<
				std::endl;
			return false;
		}
		std::cout << "Введите сумму: ";
		if (!InputNum(quant) || quant >= dep.sum) {
			std::cout << "Введена неверная сумма" << std::endl;
			return false;
		}
		dep.sum -= quant;
		return true;
	}
	//пополнение средств
	if (!typDep[dep.idTypeDep].incMoney) {
		std::cout
			<< "Вы не можете пополнять средства при данном типе вклада" <<
			std::endl;
		return false;
	}
	std::cout << "Введите сумму: ";
	if (!InputNum(quant)) {
		std::cout << "Введена неверная сумма" << std::endl;
		return false;
	}
	dep.sum += quant;
	return true;
}

//считывание счета из файла
bool DepositFromFile(Deposit &dep, std::fstream *f) {
	Deposit d;
	if (!f->is_open()) {
		return false;
	}
	std::string str;
	try {
		f->read(reinterpret_cast<char*>(&(d.number)), sizeof(d.number));
		str = StrFromBin(f);
		d.fio = str;
		f->read(reinterpret_cast<char*>(&(d.sum)), sizeof(d.sum));
		f->read(reinterpret_cast<char*>(&(d.idTypeDep)), sizeof(d.idTypeDep));
		if (!DateFromBin(d.date, f)) {
			return false;
		}
		f->read(reinterpret_cast<char*>(&(d.period)), sizeof(d.period));
		dep = d;
		return true;
	}
	catch (...) {
		throw "Не удалось считать из файла";
	}
}

//вывод в файл
void DepositToFile(Deposit dep, std::fstream *f) {
	f->write(reinterpret_cast<char*>(&(dep.number)), sizeof(dep.number));
	StrToBin(f, dep.fio);
	f->write(reinterpret_cast<char*>(&(dep.sum)), sizeof(dep.sum));
	f->write(reinterpret_cast<char*>(&(dep.idTypeDep)), sizeof(dep.idTypeDep));
	DateToBin(dep.date, f);
	f->write(reinterpret_cast<char*>(&(dep.period)), sizeof(dep.period));
}

//сохранение типов счетов в файл
void SaveTypesToFile(std::string fileName) {
	std::fstream file;
	file.open(fileName, std::ios::out | std::ios::binary);
	if (!file.is_open())
		throw "file is not open";
	int i;
	for (i = 0; i < countTypes; ++i) {
		StrToBin(&file, typDep[i].typeName);
		file.write(reinterpret_cast<char*>( &(typDep[i].incMoney) ), sizeof(typDep[i].incMoney) );
		file.write(reinterpret_cast<char*>( &(typDep[i].decMoney) ), sizeof(typDep[i].decMoney));
		file.write(reinterpret_cast<char*>( &(typDep[i].percent) ), sizeof(typDep[i].percent));
	}
	file.close();
}

//загрузка типов счетов из файла
void LoadTypesFromFile(std::string fileName)
{
	std::fstream file;
	file.open(fileName, std::ios::binary | std::ios::in);
	if (file.is_open()) {
		int i = 0;
		bool OK = true;
		std::string str;
		while (OK) {
			str = StrFromBin(&file);
			if (str == "") {
				break;
			}
			typDep[i].typeName = str;
			OK = 
				file.read(reinterpret_cast<char*>(&(typDep[i].incMoney)), sizeof(typDep[i].incMoney)) &&
				file.read(reinterpret_cast<char*>(&(typDep[i].decMoney)), sizeof(typDep[i].decMoney)) &&
				file.read(reinterpret_cast<char*>(&(typDep[i].percent)), sizeof(typDep[i].percent));
			++i;
		}
		file.close();
	}
	else {
		typDep[0].typeName = "Сберегательный";
		typDep[0].decMoney = typDep[0].incMoney = false;
		typDep[0].percent = 0.03;

		typDep[1].typeName = "Накопительный";
		typDep[1].decMoney = false;
		typDep[1].incMoney = true;
		typDep[1].percent = 0.02;

		typDep[2].typeName = "Расчетный";
		typDep[2].decMoney = typDep[2].incMoney = true;
		typDep[2].percent = 0.01;
	}
}





