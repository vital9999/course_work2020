#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <fstream>
#include <iomanip>
#include "Header.h"
#include <cstdlib>

using namespace std;

template <class T>
T Check()
{
	T i;
	while (!(cin >> i) || (cin.peek() != '\n'))
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "Ошибка ввода. Повторите попытку\n";
	}
	return i;
}

ofstream& operator<<(ofstream& a, Enter& b)
{
	a << b.login << endl << b.password << endl; return a;
}

ifstream& operator >> (ifstream& a, Enter& b)
{
	a >> b.login >> b.password; return a;
}

ostream& operator<<(ostream& a, Enter& b)
{
	a << "Логин " << b.login << endl << "Пароль " << b.password << endl << endl; return a;
}

istream& operator >> (istream& a, Enter& b)
{
	cout << "Логин: ";
	a >> b.login;
	cout << "Пароль: ";
	a >> b.password;
	return a;
}

int main()
{
	system("color F0");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	WORD wstock_matersionRequested;
	WSADATA wsaData;
	int err;
	wstock_matersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wstock_matersionRequested, &wsaData);
	if (err != 0) { return 0; }

	while (true)
	{
		SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
		sockaddr_in dest_addr;
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(1280);
		dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		connect(s, (sockaddr*)&dest_addr, sizeof(dest_addr));
		char string[20];
		char buf[20];
		char buf1[20];
		char name[20];
		Enter ob;
		vector<Enter> mas;
		Enter buf2;
		Matrix matrix;
		char res[20];
		int result;
		int choice = 0;
		while (true)
		{
			cout << "Авторизация!" << endl;
			cout << "1) Войти под администратором" << endl;
			cout << "2) Войти под пользователем  " << endl;
			cout << "3) Завершение работы        " << endl;
			cout << endl;
			int flag;
			flag = Check<int>();
			while (flag < 0 || flag > 3) {
				cout << "Неправильный ввод. Попробуйте снова\n";
				flag = Check<int>();
			};
			itoa(flag, string, 10);
			send(s, string, 20, 0);
			switch (flag)
			{
			case 1:
			{
				system("cls");
				int t = 0;
				int f = 0;
				while (t == 0)
				{
					if (f != 0) {
						cout << "1) Продолжить работу\n";
						cout << "2) Вернуться назад \n";
						int flag;
						flag = Check<int>();
						while (flag < 0 || flag > 2) {
							cout << "Ошибка ввода. Повторите попытку\n";
							flag = Check<int>();
						};
					}
					else flag = 1;
					f = 1;
					itoa(flag, buf, 10);
					send(s, buf, 20, 0);
					switch (flag)
					{
					case 1:
					{
						system("cls");
						cout << "Введите логин администратора: " << endl;
						cin >> ob.login;
						send(s, ob.login, 20, 0);
						cout << "Введите пароль администратора: " << endl;
						cin >> ob.password;
						send(s, ob.password, 20, 0);
						if (recv(s, res, sizeof(res), 0) != 0)
						{
							result = atoi(res);
							if (result == 1)
							{
								for (int i = 0; i < matrix.size; i++) {
									for (int j = 0; j < matrix.size; j++) {
										recv(s, res, sizeof(res), 0);
										result = atoi(res);
										matrix.Preference[i][j] = result;
									}
								}
								system("cls");
								int u = 0;
								while (u == 0)
								{
									cout << "Меню администратора:\n" << endl;
									cout << "1) Заполнить матриц" << endl;
									cout << "2) Редактировать бинарные предпочтения" << endl;
									cout << "3) Вывод матриц оценок" << endl;
									cout << "4) Проведение вычислений и вывод результата" << endl;
									cout << "5) Добавление нового пользователя" << endl;
									cout << "6) Удаление пользователей" << endl;
									cout << "7) Редактирование пользователей" << endl;
									cout << "8) Просмотр списка пользователей" << endl;
									cout << "0. Выход\n\n" << endl;

									int choice = Check<int>();
									while (choice < 0 || choice > 8) {
										cout << "Неправильный ввод. Попробуйте снова\n";
										choice = Check<int>();
									};
									itoa(choice, buf, 10);
									send(s, buf, 20, 0);
									system("cls");
									switch (choice)
									{
									case 1:
									{
										system("cls");
										cout << "Заполнените матрицу бинарных предпочтений конкурирующих вариантов системы" << endl;
										cout << "Альтернативы" << endl;
										cout << "Z1 - Больше самолетов и пилотов" << endl;
										cout << "Z2 - Увеличить плотность графика работы" << endl;
										cout << "Z3 - Расширения сферы полетов" << endl;
										cout << endl;
										for (int i = 0; i < matrix.size; i++) {
											for (int j = 0; j < matrix.size; j++) {
												if (i == j) {
													continue;
												}
												cout << "Preference" << i + 1 << j + 1 << " = ";
												matrix.Preference[i][j] = Check<int>();
												while (matrix.Preference[i][j] < 0 || matrix.Preference[i][j] > 1) {
													cout << "Ошибка ввода. Повторите попытку. Оценка должна быть больше бинарной(0 или 1)\n";
													cout << "Preference" << i + 1 << j + 1 << " = ";
													matrix.Preference[i][j] = Check<int>();
													cout << endl;
												};
												itoa(matrix.Preference[i][j], buf, 10);
												send(s, buf, 20, 0);
											}
										}
										system("pause");
									}; break;
									case 2:
									{
										while (true) {
											system("cls");
											cout << "1)Изменить значение\n2)Выйти\n";
											choice = Check<int>();
											while (choice < 1 || choice > 2) {
												choice = Check<int>();
											}
											itoa(choice, buf, 10);
											send(s, buf, 20, 0);
											if (choice == 1) {

												matrix.show();
												int a, b;
												cout << "Введите номер эл-та по строке: ";
												a = Check<int>();
												cout << "Введите номер эл-та по столбцу: ";
												b = Check<int>();
												matrix.edit(a, b);
											}
											if (choice == 2) {
												for (int i = 0; i < matrix.size; i++) {
													for (int j = 0; j < matrix.size; j++) {
														if (i == j) continue;
														itoa(matrix.Preference[i][j], buf, 10);
														send(s, buf, 20, 0);
													}
												}
												break;
											}
										}
									}; break;
									case 3:
									{
										matrix.show();
										system("pause");
									}break;
									case 4:
									{
										for (int i = 0; i < matrix.size; i++) {
											recv(s, res, sizeof(res), 0);
											matrix.goalPrice[i] = atoi(res);
											recv(s, res, sizeof(res), 0);
											matrix.g.goalWeight[i] = atof(res);
										}
										recv(s, res, sizeof(res), 0);
										matrix.g.sum = atoi(res);
										matrix.show();
										cout << endl;
										matrix.showCalcul();
										system("pause");
									}break;
									case 5:
									{
										system("cls");
										cout << "Введите логин: " << endl;
										cin >> ob.login;
										send(s, ob.login, 20, 0);
										cout << "Введите пароль: " << endl;
										cin >> ob.password;
										send(s, ob.password, 20, 0);
										if (recv(s, res, sizeof(res), 0) != 0) {
											result = atoi(res);
											if (result == 0) {
												cout << "Регистрация прошла успешно." << endl;
											}
											else {
												if (result == 1)
													cout << "Такой логин уже используется." << endl;
												else if (result == 2)
													cout << "Логин должен состоять только из строчных букв." << endl;
												getch();
											}
										}

									}; break;
									case 6:
									{
										recv(s, res, sizeof(res), 0);
										result = atoi(res);
										if (result > 0)
										{
											while (result > 0)
											{
												system("cls");
												cout << "Список пользователей:" << endl << endl;
												for (int i = 0; i < result; i++)
												{
													recv(s, ob.login, sizeof(ob.login), 0);
													recv(s, ob.password, sizeof(ob.password), 0);
													cout << i + 1 << ") логин: " << ob.login << " пароль: " << ob.password << endl;
												}
												cout << "0 для выхода\nВведите номер удаляемого пользователя:" << endl;
												int choice = Check<int>();
												while (choice < 0 || choice > result) {
													cout << "Ошибка ввода. Повторите попытку\n";
													choice = Check<int>();
												};
												itoa(choice, buf, 10);
												send(s, buf, 20, 0);
												result--;
												if (choice == 0)
												{
													result = -1;
													break;
												}
											}
										}
										else { cout << "Пользователей нет" << endl << endl; }
										system("pause");
									}; break;
									case 7:
									{
										int k = 0;
										recv(s, res, sizeof(res), 0);
										result = atoi(res);
										if (result > 0)
										{
											while (result)
											{
												system("cls");
												cout << "Список пользователей:" << endl << endl;
												for (int i = 0; i < result; i++)
												{
													recv(s, ob.login, sizeof(ob.login), 0);
													recv(s, ob.password, sizeof(ob.password), 0);
													cout << i + 1 << ") логин: " << ob.login << " пароль: " << ob.password << endl;
												}
												cout << "0 для выхода\nВведите номер изменяемого пользователя:" << endl;
												int choice = Check<int>();
												while (choice < 0 || choice > result) {
													cout << "Ошибка ввода. Повторите попытку\n";
													choice = Check<int>();
												};
												itoa(choice, buf, 10);
												send(s, buf, 20, 0);
												if (choice == 0)
												{
													result = 0;
													break;
												}
												else {
													cout << "Введите логин: " << endl;
													cin >> ob.login;
													k = 1;
													cout << "Введите пароль: " << endl;
													cin >> ob.password;
													result = 0;
													break;
												}
											}
											if (k) {
												send(s, ob.login, 20, 0);
												send(s, ob.password, 20, 0);
												if (recv(s, res, sizeof(res), 0) != 0) {
													result = atoi(res);
													if (result == 0) {
														break;
													}
													else {
														if (result == 1)
															cout << "Такой логин уже используется." << endl;
														else if (result == 2)
															cout << "Логин должен состоять только из строчных букв." << endl;
														getch();
														break;
													}
												}
											}
										}
										else { cout << "Пользователей нет" << endl << endl; }
										system("pause");

									}; break;
									case 8:
									{

										recv(s, res, sizeof(res), 0);
										result = atoi(res);
										if (result > 0)
										{
											system("cls");
											cout << "Список пользователей:" << endl << endl;
											for (int i = 0; i < result; i++)
											{
												recv(s, ob.login, sizeof(ob.login), 0);
												recv(s, ob.password, sizeof(ob.password), 0);
												cout << i + 1 << ") логин: " << ob.login << " пароль: " << ob.password << endl;
											}
										}
										else { cout << "Пользователей нет" << endl << endl; }
										system("pause");
									}; break;
									case 0: {
										u = 1;
										t = 1; }; break;
									default: cout << "Неверный ввод"; break;
									}
									system("cls");
								}
							}
							else {
								t = 1;
								if (result == 2)
									cout << "Неверный логин или пароль!" << endl;
								else
									cout << "Неверный ввод!" << endl;
								getch();
							}
						}
						system("cls");
						break;
					}
					case 2:
					{
						t = 1;
						break;
					}
					default:
					{
						system("cls");
						cout << "Повторите ввод\n";
						system("pause");
						break; }
					}
				}
				system("cls");
				break;
			}
			case 2:
			{
				system("cls");
				int t = 0;
				while (t == 0) {
					cout << "1) Зайти в существующий аккаунт\n";
					cout << "2) Зарегистрировать нового пользователя\n";
					cout << "3) Выход \n";
					int flag;
					flag = Check<int>();
					while (flag < 0 || flag > 3) {
						cout << "Ошибка ввода. Повторите попытку\n";
						flag = Check<int>();
					};
					itoa(flag, buf, 10);
					send(s, buf, 20, 0);
					switch (flag)
					{
					case 1:
					{
						system("cls");
						cout << "Введите логин: " << endl;
						cin >> ob.login;
						send(s, ob.login, 20, 0);
						cout << "Введите пароль: " << endl;
						cin >> ob.password;
						send(s, ob.password, 20, 0);
						if (recv(s, res, sizeof(res), 0) != 0) {
							result = atoi(res);
							if (result == 1)
							{	
								for (int i = 0; i < matrix.size; i++) {
									for (int j = 0; j < matrix.size; j++) {
										recv(s, res, sizeof(res), 0);
										result = atoi(res);
										matrix.Preference[i][j] = result;
									}
								}
								int u = 0;
								while (u == 0) {
									system("cls");
									cout << "Меню пользователя:" << endl;
									cout << "1) Заполнение матриц" << endl;
									cout << "2) Редактирование оценок" << endl;
									cout << "3) Вывод матриц оценок" << endl;
									cout << "4) Проведение вычислений и вывод результата" << endl;
									cout << "0. Выход\n\n" << endl;
									int choice = Check<int>();
									while (choice < 0 || choice > 4) {
										cout << "Ошибка ввода. Повторите попытку\n";
										choice = Check<int>();
									};
									itoa(choice, buf, 10);
									send(s, buf, 20, 0);
									system("cls");
									switch (choice)
									{
									case 1:
									{
										system("cls");
										cout << "Заполнените матрицу бинарных предпочтений конкурирующих вариантов системы" << endl;
										cout << "Альтернативы" << endl;
										cout << "Z1 - Больше самолетов и пилотов" << endl;
										cout << "Z2 - Увеличить плотность графика работы" << endl;
										cout << "Z3 - Расширения сферы полетов" << endl;
										cout << endl;
										for (int i = 0; i < matrix.size; i++) {
											for (int j = 0; j < matrix.size; j++) {
												if (i == j) {
													continue;
												}
												cout << "Preference" << i + 1 << j + 1 << " = ";
												matrix.Preference[i][j] = Check<int>();
												while (matrix.Preference[i][j] < 0 || matrix.Preference[i][j] > 1) {
													cout << "Ошибка ввода. Повторите попытку. Оценка должна быть больше бинарной(0 или 1)\n";
													cout << "Preference" << i + 1 << j + 1 << " = ";
													matrix.Preference[i][j] = Check<int>();
													cout << endl;
												};
												itoa(matrix.Preference[i][j], buf, 10);
												send(s, buf, 20, 0);
											}
										}
										system("pause");
									}; break;
									case 2:
									{
										while (true) {
											system("cls");
											cout << "1)Изменить значение\n2)Выйти\n";
											choice = Check<int>();
											while (choice < 1 || choice > 2) {
												choice = Check<int>();
											}
											itoa(choice, buf, 10);
											send(s, buf, 20, 0);
											if (choice == 1) {

												matrix.show();
												int a, b;
												cout << "Введите номер эл-та по строке: ";
												a = Check<int>();
												cout << "Введите номер эл-та по столбцу: ";
												b = Check<int>();
												matrix.edit(a, b);
											}
											if (choice == 2) {
												for (int i = 0; i < matrix.size; i++) {
													for (int j = 0; j < matrix.size; j++) {
														if (i == j) continue;
														itoa(matrix.Preference[i][j], buf, 10);
														send(s, buf, 20, 0);
													}
												}
												break;
											}
										}
									}; break;
									case 3:
									{
										matrix.show();
										system("pause");
										break;
									}
									case 4: {
										for (int i = 0; i < matrix.size; i++) {
											recv(s, res, sizeof(res), 0);
											matrix.goalPrice[i] = atoi(res);
											recv(s, res, sizeof(res), 0);
											matrix.g.goalWeight[i] = atof(res);
										}
										recv(s, res, sizeof(res), 0);
										matrix.g.sum = atoi(res);
										matrix.show();
										cout << endl;
										matrix.showCalcul();
										system("pause");
										break;
									}
									case 0:
									{
										u = 1;
										break;
									}
									}
								}
							}
							else {
								t = 1;
								if (result == 0) {
									cout << "Неверно! Повторите ввод!." << endl;
								}
								else if (result == 2)
									cout << "Логин должен состоять только из строчных букв." << endl;
								else if (result == -1)
									cout << "Нет зарегистрированных пользователей." << endl;
								getch();
							}
						}
						//
						system("cls");
						break;
					}
					case 2: {
						system("cls");
						cout << "Введите логин: " << endl;
						cin >> ob.login;
						send(s, ob.login, 20, 0);
						cout << "Введите пароль: " << endl;
						cin >> ob.password;
						send(s, ob.password, 20, 0);
						if (recv(s, res, sizeof(res), 0) != 0) {
							result = atoi(res);
							if (result == 0) {
								cout << "Регистрация прошла успешно." << endl;
							}
							else {
								if (result == 1)
									cout << "Такой логин уже используется." << endl;
								else if (result == 2)
									cout << "Логин должен состоять только из строчных букв." << endl;
								getch();
							}
						}
						//
						system("cls");
						break;
					}
					case 3:
					{
						t = 1;
						break;
					}
					default:
					{
						system("cls");
						cout << "Неверный ввод. Введите другое число\n";
						system("pause");
						break; }
					}
				}
				system("cls");
				break;
			}
			case 3:
				return 0;
			default:
			{
				cout << "Неверный выбор" << endl;
				break; }
			}
		}
		closesocket(s);
	}
	WSACleanup();

}
