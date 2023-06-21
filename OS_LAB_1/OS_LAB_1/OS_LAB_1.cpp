#include <windows.h>
#include <iostream>
#include <vector>
#include <process.h>
using namespace std;

UINT WINAPI worker(void *avec)
{
    int* vec;
    vec = (int*)avec;
    int n = vec[0];
    int min = 100000000;

    for (int i = 1; i < n + 1; i++)
    {
        if ((vec[i] < min) and (vec[i] > 0))
            min = vec[i];
    }
    cout << min;
    return 0;
}

int main()
{
    HANDLE	hThread;
    UINT	IDThread;

    int n, x, sus;
    cout << "Enter amount of elements:" << endl;
    cin >> n;
    int* a = new int[n + 1];
    cout << "Enter elements:" << endl;
    for (int i = 1; i < n + 1; i++)
    {
        cin >> x;
        a[i] = x;
    }
    cout << "Enter Suspend time (ms):" << endl;
    cin >> sus;
    a[0] = n;
    hThread = (HANDLE) _beginthreadex(NULL, 0, worker, a, 0, &IDThread);
    if (hThread == NULL)
        return GetLastError();
    SuspendThread(hThread);
    Sleep(sus);
    ResumeThread(hThread);
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
}
