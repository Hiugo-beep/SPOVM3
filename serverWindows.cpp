#include<windows.h>//for processes/events
#include <conio.h>//_getch()
#include <iostream>
#include <string>


using namespace std;

int main() {
	HANDLE semaphore = CreateSemaphore(NULL, 0, 1, L"LB3");
	HANDLE customerProcess;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	ZeroMemory(&pi, sizeof(pi));
	if (!CreateProcess(L"D:\\Проекты\\СПОВМ лабы\\LB3Customer\\Debug\\LB3Customer.exe",
		NULL,//командная строка
		NULL,//атрибуты защиты для процесса
		NULL,//атрибуты защиты для первого потока
		FALSE,//флаг наследования дескрипторов
		0,//DWORD,  флаг способа создания процесса
		NULL,//блок переменных окружения, NULL-родительский
		NULL,//текущий диск и каталог
		&si,
		&pi
	))
	{
		printf("SERVER: Error! Probably problems with the path\n");
		rewind(stdin);
		return -1;
	}
	customerProcess = pi.hProcess;
	//семафор без атрибутов защиты, нач значение семафора 0, макс = 1 + уникальное имя
	string transmittedWord;  //передаваемое слово
	HANDLE namedPipe = CreateNamedPipe(L"\\\\.\\pipe\\MyPipe",
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		100, 100, 5000, NULL);
	while (transmittedWord!="e") { //при 'е' выход
		cout << "SERVER: Enter string to transfer, please...\nFor exit enter \"e\":\n";  // сообщение о вводе
		cin >> transmittedWord; //ввод
		if (transmittedWord == "e") continue;
		DWORD  cbWritten;
		char   szBuf[100];
		strcpy(szBuf, transmittedWord.c_str());
		bool index = ConnectNamedPipe(namedPipe, NULL);
		//ReleaseSemaphore(semaphore, 1, 0); //изменения семафора с 0 на 1 (для сообщения)
		//WaitForSingleObject(semaphore, INFINITE); //ожидание семафора не в нулевом состоянии
		WriteFile(namedPipe, szBuf, strlen(szBuf) + 1,
			&cbWritten, NULL);
		ReleaseSemaphore(semaphore, 1, 0); //изменения семафора с 0 на 1 (для сообщения)
		WaitForSingleObject(semaphore, INFINITE); //ожидание семафора не в нулевом состоянии
	}
	cout << "\nExit...\n";
	TerminateProcess(customerProcess, 0);//interrupts process and all his threads work,descriptor,exit code
	CloseHandle(customerProcess);//close the descr of open object
	CloseHandle(namedPipe);
	system("pause");
	return 0;
}
