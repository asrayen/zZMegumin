#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define maxn 505

// ȫ��������ڼ��ٲ���
int arr[maxn];
int help[maxn];

// �ݹ�д��
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

// �ǵݹ�
void mergeSort2(int n) // n �����鳤��
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
    printf("������Ҫ��������ĸ�����");
    int n;
    scanf("%d", &n);
    for(int i = 0; i < n; i++)
    {
        printf("��%d������: ", i + 1);
        scanf("%d", &arr[i]);
    }
    // mergeSort1(0, n - 1);
    mergeSort2(n);
    printf("��������������:\n");
    for(int i = 0; i < n; i++)
    {
        printf("��%d������:%d\n", i + 1, arr[i]);
    }
    return 0;
}