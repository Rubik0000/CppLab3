#include <iostream>
#include <cstdlib>
#include <sstream>
#include "Date.h"

//�������� �� ���������� ���
bool LeapYear(int year) {
	return year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
}

//���������� ���-�� ���� � ������ 
int CountDayInMonth(int month, int year) {
	switch (month)
	{
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	case 2:
		return LeapYear(year) ? 29 : 28;
	default:
		return 31;
	}
}

//��������� ����� �� ���������� ������������� ����
int GetNum(std::string str, int &num) {
	int p = str.find('.', 0);
	if (p != -1) {
		const char *ch = str.c_str();
		num = atoi(ch);
		return p;
	}
	return -1;
}

//�������� ���� �� ������
bool DateFromStr(std::string str, Date &date) {
	if (str == "") {
		return false;
	}
	str += '.';
	Date d;
	int p = GetNum(str, d.day);
	if (p != 1 && p != 2) {
		return false;
	}
	str = str.substr(p + 1);
	p = GetNum(str, d.month);
	if (p != 1 && p != 2) {
		return false;
	}
	str = str.substr(p + 1);
	p = GetNum(str, d.year);
	if (p != 4) {
		return false;
	}
	if (d.month >= 1 && d.month <= 12 && d.year > 1900 &&
		d.day >= 1 && d.day <= CountDayInMonth(d.month, d.year)) 
	{
		date = d;
		return true;
	}
	return false;
}

//����� ���� � �������
void DateToConsole(Date date) {
	std::cout << date.day << '.' << date.month << '.' << date.year;
}

std::string DateToStr(Date d) {
	std::string res = "";
	if (d.day < 10) {
		res += '0';
	}
	res += std::to_string(d.day) + '.';
	if (d.month < 10) {
		res += '0';
	}
	res += std::to_string(d.month) + '.';
	res += std::to_string(d.year);
	return res;
}

//���������� ���� �� ��������� �����
bool DateFromBin(Date &d, std::fstream *f) {
	return f->read(reinterpret_cast<char*>(&(d.day)), sizeof(d.day)) &&
		f->read(reinterpret_cast<char*>(&(d.month)), sizeof(d.day)) &&
		f->read(reinterpret_cast<char*>(&(d.year)), sizeof(d.day));
}

//������ ���� � �������� ����
void DateToBin(Date d, std::fstream *f) {
	f->write(reinterpret_cast<char*>(&(d.day)), sizeof(d.day));
	f->write(reinterpret_cast<char*>(&(d.month)), sizeof(d.month));
	f->write(reinterpret_cast<char*>(&(d.year)), sizeof(d.year));
}

//��������� ���
int CompareDates(Date d1, Date d2) {
	if (d1.year > d2.year) 
		return 1;
	if (d1.year < d2.year) 
		return -1;
	if (d1.month > d2.month) 
		return 1;
	if (d1.month < d2.month) 
		return -1;
	if (d1.day > d2.day) 
		return 1;
	if (d1.day < d2.day) 
		return -1;
	return 0;
}
