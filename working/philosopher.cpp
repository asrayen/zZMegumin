#include <Windows.h>
#include <iostream>
#include <cstdio>
#include <time.h>
#define maxn 10005
using namespace std;

int PHILOSOPHER_NUM;

HANDLE chopstics[maxn];
HANDLE mutex;
HANDLE pthreads[maxn];
int cnt = 0;

DWORD WINAPI Phi(LPVOID param)
{
    int id = (cnt++) % PHILOSOPHER_NUM;
    // int time = 0;
    int left = id;
    int right = (left + 1) % PHILOSOPHER_NUM;
    int flag1, flag2;
    while (1)
    {
        // if (time >= 2)
        // {
        //     break;
        // }
        printf("哲学家【%d】正在思考\n", id + 1);
        Sleep(rand() % 1000);

        printf("哲学家【%d】开始饥饿\n", id + 1);

        if (id % 2 == 0)
        {
            flag1 = WaitForSingleObject(chopstics[left], 0);

            if (flag1 == WAIT_OBJECT_0)
            {
                flag2 = WaitForSingleObject(chopstics[right], 0);

                if (flag2 == WAIT_OBJECT_0)
                {
                    WaitForSingleObject(mutex, INFINITE);
                    printf("哲学家【%d】拿起了%d, %d号筷子, 开始进餐\n", id + 1, left + 1, right + 1);
                    // time++;
                    Sleep(rand() % 1000);
                    ReleaseMutex(mutex);
                    ReleaseSemaphore(chopstics[right], 1, NULL);
                    WaitForSingleObject(mutex, INFINITE);
                    printf("\t\t\t\t\t哲学家【%d】放下了%d, %d号筷子, 开始思考\n", id + 1, left + 1, right + 1);
                    ReleaseMutex(mutex);
                }
                ReleaseSemaphore(chopstics[left], 1, NULL);
            }
        }
        else
        {
            flag1 = WaitForSingleObject(chopstics[right], 0);

            if (flag1 == WAIT_OBJECT_0)
            {
                flag2 = WaitForSingleObject(chopstics[left], 0);

                if (flag2 == WAIT_OBJECT_0)
                {
                    WaitForSingleObject(mutex, INFINITE);
                    printf("哲学家【%d】拿起了%d, %d号筷子, 开始进餐\n", id + 1, left + 1, right + 1);
                    // time++;
                    Sleep(rand() % 1000);
                    ReleaseMutex(mutex);
                    ReleaseSemaphore(chopstics[left], 1, NULL);
                    WaitForSingleObject(mutex, INFINITE);
                    printf("\t\t\t\t\t哲学家【%d】放下了%d, %d号筷子, 开始思考\n", id + 1, left + 1, right + 1);
                    ReleaseMutex(mutex);
                }
                ReleaseSemaphore(chopstics[right], 1, NULL);
            }
        }
    }
    // printf("--------哲学家【%d】号吃饱了--------\n", id + 1);
    return 0;
}

int main()
{

    mutex = CreateMutex(NULL, FALSE, NULL);
    cout << "请输入哲学家的个数: ";
    cin >> PHILOSOPHER_NUM;
    for (int i = 0; i < PHILOSOPHER_NUM; i++)
    {
        chopstics[i] = CreateSemaphore(NULL, 1, 1, NULL);
    }
    for (int i = 0; i < PHILOSOPHER_NUM; i++)
    {

        pthreads[i] = CreateThread(NULL, 0, Phi, NULL, 0, NULL);
    }

    WaitForMultipleObjects(PHILOSOPHER_NUM, pthreads, TRUE, INFINITE);

    return 0;
}