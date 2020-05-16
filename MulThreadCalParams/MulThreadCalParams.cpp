#pragma warning(disable:4996)
#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <windows.h> 
#define P 9967
#define LL long long
using namespace std;


typedef struct MyData
{
    int startA;
    int endA;
}MYDATA;

static int mA = 0;
static int mB = 0;
static int maxn = 0;

HANDLE hMutex = NULL;//互斥量

DWORD WINAPI FIND(LPVOID lpParamter)
{
    MYDATA* pmd = (MYDATA*)lpParamter;
    int startA = pmd->startA, endA = pmd->endA;
    int endB = P;
    for (int a = startA; a < endA; a++)
        for (int b = 1; b < endB; b++)
        {
            int n = 0;
            for (int x = 1; x < P; x++)
            {
                int m = x * x * x + a * x + b;
                if (sqrt(m) - int(sqrt(m)) <= 0)
                {
                    n++;
                }
            }
            //请求一个互斥量锁
            WaitForSingleObject(hMutex, INFINITE);
            if (n > maxn)
            {
                maxn = n;
                mA = a;
                mB = b;
                cout << "找到a = " << a << " , b = " << b << " , n = " << n << endl;
            }
            //释放互斥量锁
            ReleaseMutex(hMutex);
        }
    return 0;
}

DWORD WINAPI Fun(LPVOID lpParamter)
{
    long long up = (long long)lpParamter;
    for (long long i = 0; i < up; ++i)
    {
        cout << i << endl;
    }
    return 0;
}

int main()
{
    freopen("params.txt", "w", stdout);
    //findECC();
    //printBigPrime();
    const int tNum = 8;
    HANDLE handles[tNum];
    MYDATA datas[tNum];
    int each = P / tNum;
    for (int i = 0; i < tNum - 1; ++i)
    {
        datas[i].startA = i * each;
        datas[i].endA = (i + 1) * each;
    }
    datas[tNum - 1].startA = (tNum - 1) * each;
    datas[tNum - 1].endA = P;

    for (int i = 0; i < tNum; ++i)
    {
        handles[i] = CreateThread(NULL, 0, FIND, &datas[i], 0, NULL);
    }

    /*handles[0] = CreateThread(NULL, 0, Fun, (LPVOID)1000, 0, NULL);
    handles[1] = CreateThread(NULL, 0, Fun, (LPVOID)15, 0, NULL);*/
    //WaitForSingleObject(hThread1, INFINITE); // 等待，直到线程被激发
    WaitForMultipleObjects(tNum, handles, true, INFINITE);
    for (int i = 0; i < tNum; ++i)
    {
        CloseHandle(handles[i]);
    }
    cout << "Child thread is over." << endl;
    cout << "解的数量最多时：" << endl;
    cout << "a:" << mA << ",b:" << mB << ",解的最大数量:" << maxn << endl;
}