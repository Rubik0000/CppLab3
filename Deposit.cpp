#include <iostream>
#include <iomanip>
#include <ctime>
#include "Utils.h"
#include "Deposit.h"
int32_t const countTypes = 3;
TypeDeposit typDep[countTypes];
/*��������� ���� �����
0 - ���� ������� �������
1 - ��������� �������� ����
2 - ������������ ���� "������"*/
int ChangeTypeDep(TypeDeposit & tp) {
	int ans;
	do {
		std::cout << "1 - �������� ������" << std::endl;
		std::cout << "2 - ����������� ��������� ��������" << std::endl;
		std::cout << "3 - ����������� ������� ��������" << std::endl;
		std::cout << "4 - ���������� ������" << std::endl;
		std::cout << "0 - ������" << std::endl;
	} while (!InputNum(ans) || ans < 0 || ans > 4);
	if (ans == 0) {
		return 2;
	}
	std::string str;
	double per;
	switch (ans) {
	case 1:
		std::cout << "������� �������� ������" << std::endl;
		std::cin >> str;
		if (str == "") {
			return 1;
		}
		tp.typeName = str;
		return 0;
	case 2:
		std::cout << "��������� ������������ ��������� ��������? " << std::endl;
		tp.incMoney = InputQuery();
		return 0;
	case 3:
		std::cout << "��������� ������������ ������� ��������? " << std::endl;
		tp.decMoney = InputQuery();
		return 0;
	case 4:
		std::cout << "������� ���������� ������" << std::endl;
		if (!InputNum(per)) {
			return 1;
		}
		tp.percent = per;
		return 0;
	}
}

//����� ����� � �������
void TypeDepToConsole(TypeDeposit tp) {
	std::cout << std::setw(20) << std::left << tp.typeName
		<< std::setw(5) << std::left << (tp.incMoney ? "��" : "���")
		<< std::setw(5) << std::left << (tp.decMoney ? "��" : "���")
		<< std::setw(5) << std::left << tp.percent * 100;
}

//���� � �������
bool OpenDeposit(Deposit &dep) {
	std::cout << "������� ���: ";
	std::string str;
	std::cin >> str;
	if (str == "") {
		return false;
	}
	dep.fio = str;
	int len = sizeof(typDep) / sizeof(typDep[0]);
	std::cout << "�������� ��� �����: " << std::endl;
	for (int i = 0; i < len; ++i) {
		std::cout << "����� " << i + 1 << std::endl;
		TypeDepToConsole(typDep[i]);
		std::cout << std::endl;
	}
	std::cout << "-> ";
	while (!InputNum(dep.idTypeDep) || dep.idTypeDep < 1 || dep.idTypeDep > len) {
		std::cout << "������� �������� ��������, ��������� �������" << std::endl;
	}
	--dep.idTypeDep;

	std::cout << "������� �����: ";
	if (!InputNum(dep.sum) || dep.sum < 0) {
		return false;
	}
	std::cout << "������� ���� �������� ����� (��.��.����): ";
	std::cin >> str;
	if (!DateFromStr(str, dep.date)) {
		return false;
	}
	std::cout << "������� ���� ������ � �����: ";
	if (!InputNum(dep.period) || dep.period < 1) {
		return false;
	}
	std::cout << "�� �������, ��� ������ ������� �����? ";
	if (!InputQuery()) {
		return false;
	}
	std::srand(std::time(NULL));
	std::cout << "��� ����� �����: " << 
		(dep.number = std::rand() % 100000) << std::endl;
	return true;
}

//����� �� �������
void ShowDeposit(Deposit dep) {
	std::cout << std::setw(10) << std::left << dep.number
		<< std::setw(10) << std::left << dep.fio
		<< std::setw(10) << std::left << dep.sum
		<< std::setw(15) << std::left << DateToStr( dep.date)
		<< std::setw(5) << std::left << dep.period;
	TypeDepToConsole(typDep[dep.idTypeDep]);
	std::cout << std::endl;
}

//������ � ���������� ������� ��� �������
bool ChangeDepositForUser(Deposit &dep) {
	int ans;
	std::cout << "�������� ��������" << std::endl;
	do {
		std::cout << "1 - ����� ��������" << std::endl;
		std::cout << "2 - ��������� ����" << std::endl;
		std::cout << "0 - ������" << std::endl;
	} while (!InputNum(ans) || ans < 0 || ans > 2);
	if (ans == 0) {
		return false;
	}
	int quant;
	//������ �������
	if (ans == 1) {
		if (!typDep[dep.idTypeDep].decMoney) {
			std::cout
				<< "�� �� ������ ������� �������� ��� ������ ���� ������" <<
				std::endl;
			return false;
		}
		std::cout << "������� �����: ";
		if (!InputNum(quant) || quant >= dep.sum) {
			std::cout << "������� �������� �����" << std::endl;
			return false;
		}
		dep.sum -= quant;
		return true;
	}
	//���������� �������
	if (!typDep[dep.idTypeDep].incMoney) {
		std::cout
			<< "�� �� ������ ��������� �������� ��� ������ ���� ������" <<
			std::endl;
		return false;
	}
	std::cout << "������� �����: ";
	if (!InputNum(quant)) {
		std::cout << "������� �������� �����" << std::endl;
		return false;
	}
	dep.sum += quant;
	return true;
}

//���������� ����� �� �����
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
		throw "�� ������� ������� �� �����";
	}
}

//����� � ����
void DepositToFile(Deposit dep, std::fstream *f) {
	f->write(reinterpret_cast<char*>(&(dep.number)), sizeof(dep.number));
	StrToBin(f, dep.fio);
	f->write(reinterpret_cast<char*>(&(dep.sum)), sizeof(dep.sum));
	f->write(reinterpret_cast<char*>(&(dep.idTypeDep)), sizeof(dep.idTypeDep));
	DateToBin(dep.date, f);
	f->write(reinterpret_cast<char*>(&(dep.period)), sizeof(dep.period));
}

//���������� ����� ������ � ����
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

//�������� ����� ������ �� �����
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
		typDep[0].typeName = "��������������";
		typDep[0].decMoney = typDep[0].incMoney = false;
		typDep[0].percent = 0.03;

		typDep[1].typeName = "�������������";
		typDep[1].decMoney = false;
		typDep[1].incMoney = true;
		typDep[1].percent = 0.02;

		typDep[2].typeName = "���������";
		typDep[2].decMoney = typDep[2].incMoney = true;
		typDep[2].percent = 0.01;
	}
}





