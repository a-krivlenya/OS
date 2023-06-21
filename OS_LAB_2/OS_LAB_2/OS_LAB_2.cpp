#include <windows.h>
#include <iostream>
#include <iomanip>
using namespace std;

struct list {
	int n;
	int x;
	char* arr;
};

DWORD WINAPI Child(LPVOID l) {
	list* work = (list*)l;
	cout << setw(work->x);
	cout << "The result\n";
	cout << setw(work->x);
	for (int i = 0; i < work->n; i++) {
		cout << work->arr[i] << " ";
	}
	return 0;
}

DWORD WINAPI Parent(LPVOID) {
	cout << "Enter size of the array: ";
	list* temp = new list;
	cin >> temp->n;
	cout << "Enter width: ";
	cin >> temp->x;
	temp->arr = new char[temp->n];
	cout << "Enter the elements of the array: ";
	for (int i = 0; i < temp->n; i++) {
		cin >> temp->arr[i];
	}

	HANDLE childThread;
	DWORD IDchildThread;

	childThread = CreateThread(NULL, 0, Child, (void*)temp, 0, &IDchildThread);

	if (childThread == NULL)
		return GetLastError();

	WaitForSingleObject(childThread, INFINITE);
	CloseHandle(childThread);
}

int main()
{
	HANDLE hThread;
	DWORD IDThread;
	hThread = CreateThread(NULL, 0, Parent, 0, 0, &IDThread);
	if (hThread == NULL)
		return GetLastError();
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	return 0;
}