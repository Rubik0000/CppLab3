#pragma once
#include <string>
#include <fstream>
#include "Date.h"


//��� �����
struct TypeDeposit {
	std::string typeName;
	bool incMoney;
	bool decMoney;
	double percent;
};

//����
struct Deposit {
	int32_t number;
	std::string fio;
	int32_t sum;
	int32_t idTypeDep;
	Date date;
	int32_t period;
};

extern int32_t const countTypes;
//���� ������
extern TypeDeposit typDep[3];

//�������� ��� �����
int ChangeTypeDep(TypeDeposit &tp);

//����� ����� � �������
void TypeDepToConsole(TypeDeposit tp);

//�������� ����� ��� �������
bool OpenDeposit(Deposit &dep);

//����� ���������� � �����
void ShowDeposit(Deposit dep);

//��������� � �������
bool ChangeDepositForUser(Deposit &dep);

//���� �� �����
bool DepositFromFile(Deposit &dep, std::fstream *f);

//����� � ����
void DepositToFile(Deposit dep, std::fstream *f);

//���������� ����� ������ � ����
void SaveTypesToFile(std::string fileName);

//�������� ����� ������ �� �����
void LoadTypesFromFile(std::string fileName);

