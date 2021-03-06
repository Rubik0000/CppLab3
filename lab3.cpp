#include <iostream>
#include <string>
#include "Date.h"
#include "Deposit.h"
#include "MyContainer.h"

//меню
void Menu(MyContainer<Deposit> &bank) {
	int ans, num;
	Deposit dep, dep2;
	for (;;) {
		std::cout << "Выберите тип учетной записи? " << std::endl;
		do {
			std::cout << "1 - Клиент" << std::endl;
			std::cout << "2 - Сотрудник банка" << std::endl;
			std::cout << "0 - Выход" << std::endl;
		} while (!InputNum(ans) || ans < 0 || ans > 2);
		if (ans == 0) {
			return;
		}
		std::cout << std::endl;
		//клиент
		if (ans == 1) {
			for (;;) {
				do {
					std::cout << "1 - Открыть счет" << std::endl;
					std::cout << "2 - Просмотреть счета" << std::endl;
					std::cout << "0 - Отмена" << std::endl;
				} while (!InputNum(ans) || ans < 0 || ans > 3);
				if (ans == 0) {
					break;
				}
				std::cout << std::endl;
				int ind;
				MyContainer<Deposit> cnt;
				if (ans == 1) {
					//открыть счет
					if (OpenDeposit(dep)) {
						bank.Add(dep);
						std::cout << "Счет открыт" << std::endl;
					}
					else {
						std::cout << "Введены неверные данные" << std::endl;
					}
				}
				else {
					//ввод номера
					std::string str;
					std::cout << "Введите фамилию: ";
					std::cin >> str;
					if (str == "") {
						std::cout << "Введено пустое поле" << std::endl;
						break;
					}
					dep.fio = str;
					for (;;) {
						cnt = bank.LinearSearchSubSet(
							[](Deposit d, Deposit crit) {
								return d.fio == crit.fio;
							},
							dep);
						if (cnt.Count() == 0) {
							std::cout << "На данную фамилию счетов не зарегестрировано" << std::endl;
							break;
						}
						ShowHeader();
						cnt.ToConsole(ShowDeposit);
						do {
							std::cout << "1 - Изменить средства" << std::endl;
							std::cout << "2 - Закрыть счет" << std::endl;
							std::cout << "0 - Назад" << std::endl;
						} while (!InputNum(ans) || ans < 0 || ans > 3);
						if (ans == 0) {
							break;
						}
						//ввод номера
						std::cout << "Введите номер счета: ";
						if (!InputNum(num) || num < 0) {
							break;
						}
						dep.number = num;
						ind = cnt.LinearSearch(
							[](Deposit d, Deposit crit) {
								return d.number == crit.number;
							},
							dep);
						if (ind == -1) {
							std::cout << "У вас нет такого счета" << std::endl;
							break;
						}
						//изменение средств
						if (ans == 1) {
							if (bank.Change(ChangeDepositForUser, ind)) {
								std::cout << "Счет изменен" << std::endl;
							}
							else {
								std::cout << "Не удалось изменить счет" << std::endl;
							}
						}
						//закрытие счета
						else {
							bank.RemoveInd(ind);
							std::cout << "Счет закрыт" << std::endl;
						}
					}
				}
				std::cout << std::endl;
				system("pause");
			}
		}
		//сотрудник
		else {
			for (;;) {
				do {
					std::cout << "1 - Отфильтровать список вкладчиков" << std::endl;
					std::cout << "2 - Изменить тип вклада" << std::endl;
					std::cout << "3 - Просмотреть всех вкладчиков" << std::endl;
					std::cout << "0 - Отмена" << std::endl;
				} while (!InputNum(ans) || ans < 0 || ans > 3);
				if (ans == 0) {
					break;
				}
				std::cout << std::endl;
				Deposit crit;
				MyContainer<Deposit> subs;
				int num;
				std::string str;
				Date date;
				int ind, res;
				switch (ans)
				{
				case 1:
					std::cout << "Отфильтровать по:" << std::endl;
					do {
						std::cout << "1 - Номеру счета" << std::endl;
						std::cout << "2 - ФИО" << std::endl;
						std::cout << "3 - Сумме" << std::endl;
						std::cout << "4 - Названию вклада" << std::endl;
						std::cout << "5 - Возможности пополнять счет" << std::endl;
						std::cout << "6 - Возможности снимать средства" << std::endl;
						std::cout << "7 - Дате открытия" << std::endl;
						std::cout << "8 - Сроку договора" << std::endl;
						std::cout << "9 - Процентной ставке" << std::endl;
						std::cout << "0 - Отмена" << std::endl;
					} while (!InputNum(ans) || ans < 0 || ans > 9);
					if (ans == 0) {
						break;
					}
					
					switch (ans)
					{
					case 1:
						std::cout << "Введите номер счета: ";
						if (!InputNum(num) || num < 0) {
							std::cout << "Введены неверные данные" << std::endl;
							break;
						}
						crit.number = num;
						subs = bank.LinearSearchSubSet(
							[](Deposit dep, Deposit crit) {
							return dep.number == crit.number;
						}, crit);
						break;
					case 2:
						std::cout << "Введите фамилию: ";
						std::cin >> str;
						if (str == "") {
							std::cout << "Введены неверные данные" << std::endl;
							break;
						}
						crit.fio = str;
						subs = bank.LinearSearchSubSet(
							[](Deposit dep, Deposit crit) {
							return dep.fio == crit.fio;
						}, crit);
						break;
					case 3:
						std::cout << "Введите сумму: ";
						if (!InputNum(num) || num < 0) {
							std::cout << "Введены неверные данные" << std::endl;
							break;
						}
						crit.sum = num;
						subs = bank.LinearSearchSubSet(
							[](Deposit dep, Deposit crit) {
							return dep.sum == crit.sum;
						}, crit);
						break;
					case 4:
						std::cout << "Выберите вклад " << std::endl;
						do {
							std::cout << "1 - " << typDep[0].typeName << std::endl;
							std::cout << "2 - " << typDep[1].typeName << std::endl;
							std::cout << "3 - " << typDep[2].typeName << std::endl;
						} while (!InputNum(ans) || ans < 1 || ans > 3);
						crit.idTypeDep = ans;
						subs = bank.LinearSearchSubSet(
							[](Deposit dep, Deposit crit) {
							return dep.idTypeDep == crit.idTypeDep;
						}, crit);

						break;
					case 5:
						std::cout << "По способности пополнять средства: ";
						subs = bank.LinearSearchSubSet(
							[](Deposit d, Deposit crit) {
								return typDep[d.idTypeDep].incMoney;
							},
							crit);
						break;
					case 6:
						std::cout << "По способности снимать средства: ";
						subs = bank.LinearSearchSubSet(
							[](Deposit d, Deposit crit) {
								return typDep[d.idTypeDep].decMoney;
							},
							crit);
						break;
					case 7:
						std::cout << "Введите дату: ";
						std::cin >> str;
						if (!DateFromStr(str, date)) {
							std::cout << "Введены неверные данные" << std::endl;
							break;
						}
						crit.date = date;
						subs = bank.LinearSearchSubSet(
							[](Deposit d, Deposit crit) {
								return CompareDates(d.date, crit.date) == 0;
							},
							crit);
						break;
					case 8:
						std::cout << "Введите срок вклада: ";
						if (!InputNum(num)) {
							std::cout << "Введены неверные данные" << std::endl;
							break;
						}
						crit.period = num;
						subs = bank.LinearSearchSubSet(
							[](Deposit d, Deposit crit) {
								return d.period == crit.period;
							},
							crit);
						break;
					case 9:
						std::cout << "Введите процент: ";
						do {
							std::cout << "1 - " << typDep[0].percent << std::endl;
							std::cout << "2 - " << typDep[1].percent << std::endl;
							std::cout << "3 - " << typDep[2].percent << std::endl;
						} while (!InputNum(ans) || ans < 1 || ans > 3);
						crit.idTypeDep = ans;
						subs = bank.LinearSearchSubSet(
							[](Deposit dep, Deposit crit) {
							return dep.idTypeDep == crit.idTypeDep;
						}, crit);
						break;
					}
					ShowHeader();
					subs.ToConsole(ShowDeposit);
					break;
				//изменить тип вклада
				case 2:
					std::cout << "Выберите вклад" << std::endl;
					do {
						std::cout << "Номер " << 1 << std::endl;
						TypeDepToConsole(typDep[0]);
						std::cout << "Номер " << 2 << std::endl;
						TypeDepToConsole(typDep[1]);
						std::cout << "Номер " << 3 << std::endl;
						TypeDepToConsole(typDep[2]);
					} while (!InputNum(ans) || ans < 1 || ans > 3);
					res = ChangeTypeDep(typDep[--ans]);
					if (res == 0) {
						std::cout << "Вклад успешно изменен" << std::endl;
						SaveTypesToFile("TypesDep.dat");
					}
					else if (res == 1) {
						std::cout << "Не удалось изменить вклад" << std::endl;
					}
					break;
				//показ всех вкладчиков
				case 3:
					std::cout << "Вкладчики:" << std::endl;
					std::cout << std::endl;
					ShowHeader();
					bank.ToConsole(ShowDeposit);
					break;
				}
				std::cout << std::endl;
				system("pause");
				std::cout << std::endl;
			}
		}
	}
}


int main()
{
	setlocale(LC_ALL, "Russian");
	MyContainer<Deposit> bank("Source.dat", DepositFromFile, DepositToFile);
	
	LoadTypesFromFile("TypesDep.dat");
	Menu(bank);
	SaveTypesToFile("TypesDep.dat");
    return 0;
}

