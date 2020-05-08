#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
	HANDLE semaphore = CreateSemaphore(NULL, 0, 1, L"LB3");
	char serverPipeName[100];
	HANDLE namedPipe = CreateFile(L"\\\\.\\pipe\\MyPipe",
		GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, 0, NULL);
	while (true) {
		WaitForSingleObject(semaphore, INFINITE); //ожидание семафора не в нулевом состоянии
		cout << "\nCUSTOMER: Server process is ready for data transfer!\n";
		DWORD  cbRead;
		char   buf[100];
		ReadFile(namedPipe, buf, 100, &cbRead, NULL);
		cout << "CUSTOMER: Transfered string: " << buf <<"!\n";
		ReleaseSemaphore(semaphore, 1, 0); //изменения семафора с 0 на 1
	}
	return 0;
}
