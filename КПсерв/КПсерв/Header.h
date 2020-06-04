#pragma once
#include "winsock2.h"
#include "process.h"
#include "locale.h"
#include <iostream>
#include <fstream>
#include "time.h"
#include "conio.h"
#include <vector>
#include <list>
#include <iomanip>
#include <cmath>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)



using namespace std;

class Enter {
private:
	SOCKET s2;
	char log[20];
	char pass[20];
public:
	string Login;
	string Password;
	int status;
	Enter() {
		status = 0;
	}
	char* get_log() {
		return log;
	};
	char* get_pass() {
		return pass;
	};
	void set(char* l, char* p) {
		strcpy(log, l);
		strcpy(pass, p);
	};
	int registration(char*, char*, char*);
	int check(char*, char*, char*);
	friend ifstream& operator >> (ifstream& a, Enter& b);
	friend ostream& operator<<(ostream& a, Enter& b);
	~Enter();
};

class Exception : public exception {
protected:
	int numbEr;
public:
	Exception();
	Exception(int);
	int show();
	~Exception();
};
class LogicError : Exception
{
public:LogicError() {}
};
class InvalidArgument : LogicError
{
public:InvalidArgument() { cout << "Передано неверное значение" << endl; }
};
class LengthError : LogicError
{
public:LengthError() { cout << "Выход за пределы массива" << endl; }
};

class GoalWeight {
public:
	double goalWeight[3];
	int index[3];
	int sum;

};

class Matrix
{
public:
	static const int size = 3;

	int** Preference;
	GoalWeight g;
	int goalPrice[size];
	

	void output()
	{
		ofstream fout("matrix.txt");
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				fout << Preference[i][j] << endl;
			}
		}
		fout.close();
	}

	void input(Matrix& matrix)
	{
		ifstream fin;
		fin.open("matrix.txt");
		char buff[size];

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				fin >> matrix.Preference[i][j];

			}

			
		}
		fin.close();
	}
	void calculation();

	Matrix() {
		Preference = new int* [size];
		for (int i = 0; i < size; i++)
			Preference[i] = new int[size];
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++)
			{
				Preference[i][j] = 0;
			}
		}
		g.sum = 0;
		for (int j = 0; j < size; j++)
		{
			goalPrice[j] = 0;
			g.goalWeight[j] = 0;
			g.index[j] = j;
		}

	}
	~Matrix() {
		for (int i = 0; i < size; i++)
		{
			delete[] Preference[i];
		}
		delete[]Preference;
	
	}
};

void Matrix::calculation(){
	for (int i = 0; i < size; i++) {
		goalPrice[i] = 0;
		g.goalWeight[i] = 0;
		g.index[i] = i;
	}
	for (int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++)
		goalPrice[i] = Preference[i][j] + goalPrice[i];
	}
	for (int i = 0; i < size; i++) {
		g.sum = g.sum + goalPrice[i];
	}
	for (int i = 0; i < size; i++) {
		if (g.sum != 0)
			g.goalWeight[i] = (double)goalPrice[i]/ (double)g.sum;
		else g.goalWeight[i] = 0;
	}
}




