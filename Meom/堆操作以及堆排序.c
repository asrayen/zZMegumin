#include <stdio.h>
#include <stdlib.h>

void swap(int *arr, int a, int b)
{
    int temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}

void heapInsert(int *arr, int i) // ?? i 向上插入
{
    while (arr[i] > arr[(i - 1) / 2])
    {
        swap(arr, i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

void heapify(int *arr, int i, int size) // ?? i 向下插入
{
    int left = i * 2 + 1;
    while (left < size)
    {
        int max = left + 1 < size && arr[left + 1] > arr[left] ? left + 1 : left;
        max = arr[i] > arr[max] ? i : max;
        if (max == i)
        {
            break;
        }
        swap(arr, i, max);
        i = max;
        left = i * 2 + 1;
    }
}

void heapsortTop(int *arr, int n) // 从顶建堆 O （n * logn??
{
    for (int i = 0; i < n; i++)
    {
        heapInsert(arr, arr[i]);
    }
    int size = n;
    while (size > 1)
    {
        swap(arr, 0, --size);
        heapify(arr, 0, size);
    }
}

void heapsortbelow(int *arr, int n) // 从底建堆 O（n??
{
    for (int i = n - 1; i >= 0; i--)
    {
        heapify(arr, i, n);
    }
    int size = n;
    while (size > 1)
    {
        swap(arr, 0, --size);
        heapify(arr, 0, size);
    }
}

int main()
{
    int n;
    printf("????????????????????????");
    scanf("%d", &n);
    printf("??????????");
    int *number;
    number = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &number[i]);
    }
    heapsortbelow(number, n);
    printf("?????????飺");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", number[i]);
    }
    return 0;
}