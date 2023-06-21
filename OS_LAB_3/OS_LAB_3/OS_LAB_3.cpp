#include <windows.h>
#include <iostream>
#include <algorithm>

using namespace std;

float ans = 1;

CRITICAL_SECTION cs;
HANDLE hOutEvent;

struct list {
	int n;
	float* arr;
};

void work(LPVOID l)
{
	EnterCriticalSection(&cs);
	int x, c = 0, right;
	cout << "\nEnter time to sleep: ";
	cin >> x;
	
	list* work = (list*)l;
	sort(work->arr, work->arr + work->n);
	float dummy;
	right = work->n - 1;
	while (c < right) {
		if (work->arr[c] == work->arr[c + 1]) {
			while (work->arr[c] == work->arr[c + 1])
				c++;
			c++;
		}
		else {
			dummy = work->arr[c];
			for (int i = c; i < right; i++) {
				work->arr[i] = work->arr[i + 1];
			}
			work->arr[right] = dummy;
			--right;
		}
	}
	for (int i = 0; i < work->n; i++) {
		cout << work->arr[i] << " ";
		Sleep(x);
	}
	LeaveCriticalSection(&cs);
}

void MultElement(LPVOID l)
{
	EnterCriticalSection(&cs);
	list* mult = (list*)l;
	for (int i = 0; i < mult->n; i++) {
		ans *= mult->arr[i];
	}
	LeaveCriticalSection(&cs);
	SetEvent(hOutEvent);
}

int main()
{
	HANDLE 	hThread[2];
	DWORD	dwThread[2];

	InitializeCriticalSection(&cs);
	hOutEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hOutEvent == NULL)
		return GetLastError();

	cout << "Enter size of the array: ";
	list* temp = new list;
	cin >> temp->n;
	temp->arr = new float[temp->n];
	cout << "Enter the elements of the array: ";
	for (int i = 0; i < temp->n; i++) {
		cin >> temp->arr[i];
	}
	cout << "Size of the array: ";
	cout << temp->n;
	cout << "\nElements of the array: ";
	for (int i = 0; i < temp->n; i++) {
		cout << temp->arr[i] << ' ';
	}
	hThread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)work,
		(void*)temp, 0, &dwThread[0]);
	if (hThread[0] == NULL)
		return GetLastError();
	hThread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MultElement,
		(void*)temp, 0, &dwThread[1]);
	if (hThread[1] == NULL)
		return GetLastError();
	WaitForSingleObject(hOutEvent, INFINITE);
	cout << "\nThe answer is " << ans << endl;
	if (WaitForMultipleObjects(2, hThread, TRUE, INFINITE) == WAIT_FAILED)
	{
		cout << "Wait for multiple objects failed." << endl;
		cout << "Press any key to exit." << endl;
	}
	
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	return 0;
}