#pragma once
#include <string>
#include <fstream>

//��������� "����"
struct Date {
	int32_t day;
	int32_t month;
	int32_t year;
};

//���������� ���� �� ������
bool DateFromStr(std::string str, Date &date);
std::string DateToStr(Date d);
//����� ���� � �������
void DateToConsole(Date date);

//��������� ���
int CompareDates(Date d1, Date d2);

//���������� ���� �� ��������� �����
bool DateFromBin(Date &d, std::fstream *f);

//������ ���� � �������� ����
void DateToBin(Date d, std::fstream *f);

