#pragma comment(lib, "ws2_32.lib")
#include "locale.h"
#include "iostream"
#include "Header.h"
#include <string.h>
#include <fstream>
#include "conio.h"
#include <algorithm>
#include <winsock2.h>
#include <iomanip>
#include <string>

using namespace std;


Enter::~Enter() {
}

Exception::Exception() {
	numbEr = 0;
}

Exception::Exception(int k) {
	numbEr = k;
}

int Exception::show() {
	if (numbEr == 1)
		return 2;
}

Exception::~Exception() {
}

ifstream& operator >> (ifstream& a, Enter& b)
{
	a >> b.log;
	a >> b.pass;
	return a;
}

ostream& operator<<(ostream& a, Enter& b)
{
	a << b.log << endl << b.pass << endl;
	return a;
}

int Enter::registration(char* s, char* login, char* password) {
	int tm = 0;
	int r = 0;
	char t[20];
	ofstream fout(s, ios_base::app);
	ifstream fin(s, ios_base::in);
	if (!fout.is_open()) {
		r = -1;
	}
	else {
		try {
			flushall();
			for (int i = 0; login[i]; i++) {
				if ((login[i] < 'a' || login[i] > 'z') && (login[i] < 'A' || login[i] > 'Z'))
					throw Exception(1);
			}
		}
		catch (Exception ob) {
			cin.clear();
			r = ob.show();
		}
	}
	for (unsigned i = 0; login[i]; i++)
		strcpy(log, login);
	while (!fin.eof()) {
		fin >> login;
		if (strcmp(login, log) == 0) {
			r = 1;
			fin.close();
		}
		fin >> login;
	}
	if (r == 0) {
		strcpy(pass, password);
		fout << log << endl;
		fout << pass << endl;
		fout.close();
	}
	return r;
}

int Enter::check(char* s, char* login, char* password) throw(Exception)
{
	Enter v;
	int tmp = 0;
	int result = 0;
	char m;
	char res[20];
	ifstream fin(s, ios_base::in);
	if (!fin) {
		result = -1;
	}
	else {
		try {
			flushall();
			for (int i = 0; login[i]; i++) {
				if ((login[i] < 'a' || login[i] > 'z') && (login[i] < 'а' || login[i] > 'я'))
					throw Exception(1);
			}
		}
		catch (Exception ob) {
			cin.clear();
			result = ob.show();
		}
	}
	if (fin) {
		vector<Enter> mas;
		mas.clear();
		Enter buf;
		while (!fin.eof()) {
			fin >> buf;
			mas.push_back(buf);
		}
		fin.close();
		vector<Enter>::iterator t = mas.begin();
		while (t != mas.end())
		{
			if (strcmp(login, t->log) == 0 && strcmp(password, t->pass) == 0)
			{
				result = 1; break;
			}
			t++;
		}
	}
	return result;
}

int cl = 0;

void print()
{
	if (cl)
		cout << cl << " клиент подключен." << endl;
	else
		cout << "Нет подключенных клиентов." << endl;
}


DWORD WINAPI ThreadFunc(LPVOID client_socket) {
	SOCKET s2 = ((SOCKET*)client_socket)[0];
	char string[20];
	char login[20];
	char admin[20] = "admins.txt";
	char user[20] = "users.txt";
	char password[20];
	int status = 0;
	char buf[20];
	char res[20];
	char filename[20];
	int result = 0;
	FILE* f;
	Enter ob;
	Matrix matrix;
	matrix.input(matrix);
	while (true)
	{
		recv(s2, string, sizeof(string), 0);
		int flag = atoi(string);
		switch (flag)
		{
		case 1:
		{
			int n = 0;
			while (n == 0)
			{
				recv(s2, buf, sizeof(buf), 0);
				int choice = atoi(buf);
				switch (choice)
				{
				case 1:
				{
					recv(s2, login, sizeof(login), 0);
					recv(s2, password, sizeof(password), 0);
					result = ob.check(admin, login, password);
					itoa(result, res, 10);
					send(s2, res, 20, 0);
					if (result == 1) {
						for (int i = 0; i < matrix.size; i++) {
							for (int j = 0; j < matrix.size; j++) {
								result = matrix.Preference[i][j];
								itoa(result, res, 10);
								send(s2, res, 20, 0);
							}
						}

						int u = 1;
						while (u)
						{
							recv(s2, buf, sizeof(buf), 0);
							u = atoi(buf);
							switch (u)
							{
							case 1:
							{
								for (int i = 0; i < matrix.size; i++) {
									for (int j = 0; j < matrix.size; j++) {
										if (i == j) continue;
										int t;
										recv(s2, buf, sizeof(buf), 0);
										t = atoi(buf);
										matrix.Preference[i][j] = t;
									}
								}
							}; break;
							case 2:
							{
								while (true) {
									recv(s2, buf, sizeof(buf), 0);
									choice = atoi(buf);
									if (choice == 2) {
										for (int i = 0; i < matrix.size; i++) {
											for (int j = 0; j < matrix.size; j++) {
												if (i == j) continue;
												int t;
												recv(s2, buf, sizeof(buf), 0);
												t = atoi(buf);
												matrix.Preference[i][j] = t;
											}
										}
										break;
									}
								}
							}; break;
							case 3:
							{

							}break;
							case 4:
							{
								matrix.calculation();
								for (int i = 0; i < matrix.size; i++) {
									itoa(matrix.goalPrice[i], res, 10);
									send(s2, res, 20, 0);
									sprintf(res, "%f", matrix.g.goalWeight[i]);
									send(s2, res, 20, 0);
								}
								itoa(matrix.g.sum, res, 10);
								send(s2, res, 20, 0);
							}break;
							case 5:
							{
								recv(s2, login, sizeof(login), 0);
								recv(s2, password, sizeof(password), 0);
								result = ob.registration(user, login, password);
								itoa(result, res, 10);
								send(s2, res, 20, 0);

							}; break;
							case 6:
							{
								ifstream read("users.txt", ios_base::in);
								if (!read) {
									result = -1;
								}
								else {
									vector<Enter> mas;
									Enter buf2;
									int i = 0;
									mas.clear();
									while (!read.eof())
									{
										read >> buf2;
										mas.push_back(buf2);
										i++;
									}
									mas.pop_back();
									read.close();
									i--;
									itoa(i, res, 10);
									send(s2, res, 20, 0);
									int choice = 0;
									while (choice != -1) {
										vector<Enter>::iterator t = mas.begin();
										char c[20];
										while (t != mas.end())
										{

											send(s2, t->get_log(), 20, 0);
											send(s2, t->get_pass(), 20, 0);
											t++;
										}
										recv(s2, buf, sizeof(buf), 0);
										choice = atoi(buf);
										if (choice == 0)
										{
											choice = -1;
											break;
										}
										else
										{
											choice--;
											mas.erase(mas.begin() + choice);
										}
									}
									ofstream fout("users.txt", ios_base::out);
									vector<Enter>::iterator t = mas.begin();
									while (t != mas.end())
									{
										fout << t->get_log() << endl;
										fout << t->get_pass() << endl;
										t++;
									}
									fout.close();
								}

							}; break;
							case 7:
							{
								int k = 0;
								char l[20], p[20];
								ifstream read("users.txt", ios_base::in);
								if (!read) {
									result = -1;
								}
								else {
									vector<Enter> mas;
									Enter buf2;
									int i = 0;
									mas.clear();
									while (!read.eof())
									{
										read >> buf2;
										mas.push_back(buf2);
										i++;
									}
									mas.pop_back();
									read.close();
									i--;
									itoa(i, res, 10);
									send(s2, res, 20, 0);
									int choice = 0;
									while (choice != -1) {
										vector<Enter>::iterator t = mas.begin();
										char c[20];
										while (t != mas.end())
										{

											send(s2, t->get_log(), 20, 0);
											send(s2, t->get_pass(), 20, 0);
											t++;
										}
										recv(s2, buf, sizeof(buf), 0);
										choice = atoi(buf);
										if (choice == 0)
										{
											choice = -1;
											break;
										}
										else
										{
											choice--;
											strcpy(l, mas[choice].get_log());
											strcpy(p, mas[choice].get_pass());
											k = 1;
											mas.erase(mas.begin() + choice);
											choice = -1;
											break;
										}
									}
									if (k) {
										ofstream fout("users.txt", ios_base::out);
										vector<Enter>::iterator t = mas.begin();
										while (t != mas.end())
										{
											fout << t->get_log() << endl;
											fout << t->get_pass() << endl;
											t++;
										}
										fout.close();
										recv(s2, login, sizeof(login), 0);
										recv(s2, password, sizeof(password), 0);
										result = ob.registration(user, login, password);
										itoa(result, res, 10);
										send(s2, res, 20, 0);
										if (result)
										{
											ob.registration(user, l, p);
										}
									}
								}

							}; break;
							case 8:
							{
								ifstream read("users.txt", ios_base::in);
								if (!read) {
									result = -1;
								}
								else {
									vector<Enter> mas;
									Enter buf2;
									int i = 0;
									mas.clear();
									while (!read.eof())
									{
										read >> buf2;
										mas.push_back(buf2);
										i++;
									}
									mas.pop_back();
									read.close();
									i--;
									itoa(i, res, 10);
									send(s2, res, 20, 0);
									vector<Enter>::iterator t = mas.begin();
									char c[20];
									while (t != mas.end())
									{

										send(s2, t->get_log(), 20, 0);
										send(s2, t->get_pass(), 20, 0);
										t++;
									}
								}
							}; break;
							case 0: {

								n = 1; }; break;
							}

						}
					}
					if (result == 0)
					{
						n = 1;
						break;
					}
					else if (result == 2)
					{
						n = 1;
						break;
					}
					else if (result == -1)
					{
						n = 1;
						break;
					}
					///////////////////////////////////////////////
					n = 1;
					break;
					//////////////////////////////////////////////
				}
				case 2: {
					n = 1;
					break;
				}
				}
			}
			break;
		}
		case 2: {
			int n = 0;
			int lol;
			while (n == 0)
			{
				recv(s2, buf, sizeof(buf), 0);
				int choice = atoi(buf);
				switch (choice)
				{
				case 1:
				{
					recv(s2, login, sizeof(login), 0);
					recv(s2, password, sizeof(password), 0);
					result = ob.check(user, login, password);
					itoa(result, res, 10);
					send(s2, res, 20, 0);
					if (result == 1) {
						for (int i = 0; i < matrix.size; i++) {
							for (int j = 0; j < matrix.size; j++) {
								lol = matrix.Preference[i][j];
								itoa(lol, res, 10);
								send(s2, res, 20, 0);
							}
						}

						int u = 0;
						while (u == 0)
						{
							recv(s2, buf, sizeof(buf), 0);
							u = atoi(buf);
							switch (u)
							{
							case 1:
							{
								for (int i = 0; i < matrix.size; i++) {
									for (int j = 0; j < matrix.size; j++) {
										if (i == j) continue;
										int t;
										recv(s2, buf, sizeof(buf), 0);
										t = atoi(buf);
										matrix.Preference[i][j] = t;
									}
								}
							}; break;
							case 2:
							{
								while (true) {
									recv(s2, buf, sizeof(buf), 0);
									choice = atoi(buf);
									if (choice == 2) {
										for (int i = 0; i < matrix.size; i++) {
											for (int j = 0; j < matrix.size; j++) {
												if (i == j) continue;
												int t;
												recv(s2, buf, sizeof(buf), 0);
												t = atoi(buf);
												matrix.Preference[i][j] = t;
											}
										}
										break;
									}
								}
							}; break;
							case 3:
							{
								break;
							}
							case 4:
							{
								matrix.calculation();
								for (int i = 0; i < matrix.size; i++) {
									itoa(matrix.goalPrice[i], res, 10);
									send(s2, res, 20, 0);
									sprintf(res, "%f", matrix.g.goalWeight[i]);
									send(s2, res, 20, 0);
								}
								itoa(matrix.g.sum, res, 10);
								send(s2, res, 20, 0);
							}break;
							case 0: {

								u = 1; }; break;
							}

						}


					}
					if (result == 0)
					{
						n = 1;
						break;
					}
					else if (result == 2)
					{
						n = 1;
						break;
					}
					else if (result == -1)
					{
						n = 1;
						break;
					}
					///////////////////////////////////////////////
	
					break;
					//////////////////////////////////////////////
				}
				case 2: {
					recv(s2, login, sizeof(login), 0);
					recv(s2, password, sizeof(password), 0);
					result = ob.registration(user, login, password);
					itoa(result, res, 10);
					send(s2, res, 20, 0);
					break;
				}
				case 3:
				{
					n = 1;
					break;
				}
				}

			}
			break;
		}
		case 3: {
			matrix.output();
			return 0;
		}
		}
	}
}

int main()
{
	system("color F0");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0) { return 0; }

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(1280);
	local_addr.sin_addr.s_addr = 0;
	bind(s, (sockaddr*)&local_addr, sizeof(local_addr));
	int c = listen(s, 7);

	cout << "Сервер запущен" << endl;
	cout << endl;

	SOCKET client_socket;

	sockaddr_in client_addr;
	int client_addr_size = sizeof(client_addr);

	while ((client_socket = accept(s, (sockaddr*)&client_addr, &client_addr_size))) {
		cl++;
		print();

		DWORD thID;
		CreateThread(NULL, NULL, ThreadFunc, &client_socket, NULL, &thID);
	}
}
