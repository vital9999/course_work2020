#pragma once
#include "iostream"
#include <vector>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

class Enter {
public:
	char login[20];
	char password[20];
public:
	Enter()
	{
		login[0] = NULL;
		password[0] = NULL;
	}
	friend ofstream& operator <<(ofstream& a, Enter& b);
	friend ifstream& operator >>(ifstream& a, Enter& b);
	~Enter() {}
};

class GoalWeight {
public:
	double goalWeight[3];
	int index[3];
	int sum;
	void printZ(int ID) {
		if (ID == 0)cout << "Z1";
		else if (ID == 1)cout << "Z2";
		else if (ID == 2)cout << "Z3";
	};
	void sort() {
		for (int i = 1; i < 3; ++i)
		{
			for (int r = 0; r < 3 - i; r++)
			{
				if (goalWeight[r] < goalWeight[r + 1])
				{
					int temp = goalWeight[r];
					int tempID = index[r];
					goalWeight[r] = goalWeight[r + 1];
					index[r] = index[r + 1];
					goalWeight[r + 1] = temp;
					index[r + 1] = tempID;
				}
			}
		}
	};
};

class Matrix
{
public:
	static const int size = 3;

	int** Preference;
	GoalWeight g;
	int goalPrice[size];
	
	

	void show();
	void showCalcul();
	void edit(int a, int b);
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

void Matrix::show() {
	cout << "Матрица бинарных предпочтений" << endl;
	cout << setfill('_') << setw(37) << "_" << endl;
	cout << "| Zi/Zj" << setfill(' ') << setw(3) << "|" << "  Z1" << setw(5) << "|" << "  Z2" << setw(5) << "|" << "  Z3" << setw(5) << "|" << endl;
	cout << setfill('_') << setw(37) << "_";
	for (int i = 0; i < size; i++) {
		cout << endl;
		cout << "|  Z" << i+1 << setfill(' ') << setw(5) << "|";
		for (int j = 0; j < size; j++) {
			cout << "   ";
			if (i == j) cout << "-";
			else cout <<Preference[i][j];
			cout << "    " << "|";
		}
	}
	cout << endl;
	cout << setfill('_') << setw(37) << "_"<<endl;

}
void Matrix::showCalcul() {
	cout << "Цена каждой цели: " << endl;
	for (int i = 0; i < size; i++) {
		cout << "C" << i + 1 << " = " << goalPrice[i] << endl;
	}
	cout << "Сумма целей: " << g.sum << endl;
	cout << "Вес каждой цели: " << endl;
	for (int i = 0; i < size; i++) {
		cout << "W" << i + 1 << " = " << g.goalWeight[i] << endl;
	}
	g.sort();
	cout << endl << "Приоритеты целей: ";
	for (int i = 0; i < size; i++) {
		g.printZ(g.index[i]);
		if(i != 2)
		cout << " => ";
	}
	cout << endl;
}
void Matrix::edit(int a, int b) {
	cout << "Введите новое значение для эл-та: ";
	cin >> Preference[b - 1][a - 1];
	while (Preference[b - 1][a - 1] < 0 || Preference[b - 1][a - 1] > 1) {
		cout << "Введите бинарное значение(1 или 0)";
		cin >> Preference[b - 1][a - 1];
	}
}
