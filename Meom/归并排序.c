#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define maxn 505

// 全局数组便于减少参数
int arr[maxn];
int help[maxn];

// 递归写法
void mergeSort1(int l, int r)
{
    if(l == r)
    {
        return;
    }
    int m = (l + r) / 2;
    mergeSort1(l, m);
    mergeSort1(m + 1, r);
    merge(l, m, r);
}

// 非递归
void mergeSort2(int n) // n 是数组长度
{
    for(int l, m, r, step = 1; step < n; step <<= 1)
    {
        l = 0;
        while(l < n)
        {
            m = l + step - 1;
            if(m + 1 >= n)
            {
                break;
            }
            r = (l + (step << 1) - 1) < n - 1 ? (l + (step << 1) - 1) : n - 1;
            merge(l, m, r);
            l = r + 1;
        }
    }
}
void merge(int l, int m, int r)
{
    int i = l;
    int a = l;
    int b = m + 1;
    while(a <= m && b <= r)
    {
        help[i++] = arr[a] <= arr[b] ? arr[a++] : arr[b++];
    }
    while(a <= m)
    {
        help[i++] = arr[a++];
    }
    while(b <= r)
    {
        help[i++] = arr[b++];
    }
    for(int j = l; j <= r; j++)
    {
        arr[j] = help[j];
    }
}

int main()
{
    printf("请输入要排序的数的个数：");
    int n;
    scanf("%d", &n);
    for(int i = 0; i < n; i++)
    {
        printf("第%d个数是: ", i + 1);
        scanf("%d", &arr[i]);
    }
    // mergeSort1(0, n - 1);
    mergeSort2(n);
    printf("排序后的数组如下:\n");
    for(int i = 0; i < n; i++)
    {
        printf("第%d个数是:%d\n", i + 1, arr[i]);
    }
    return 0;
}