// ����һ������ nums ����� i < j �� nums[i] > 2*nums[j] ���Ǿͽ� (i, j) ����һ����Ҫ��ת�ԡ�

// ����Ҫ���ظ��������е���Ҫ��ת�Ե�������

// ʾ�� 1:
// ����: [1,3,2,3,1]
// ���: 2

// ʾ�� 2:
// ����: [2,4,3,5,1]
// ���: 3

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define maxn 5005

int help[maxn];  // ��������

int count(int *arr, int l, int r)
{
    if(l == r)
    {
        return 0;
    }
    int m = (l + r) / 2;
    return count(arr, l, m) + count(arr, m + 1, r) + merge(arr, l, m, r);
}
// ����˼��
int merge(int *arr, int l, int m, int r)
{
    // ͳ�Ʋ���
    int ans = 0;
    for(int i = l, j = m + 1; i <= m; i++)
    {
        while(j <= r && arr[i] > arr[j] * 2)
        {
            j++;
        }
        ans += j - m - 1;
    }

    // �鲢����
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
    for(int i = l; i <= r; i++)
    {
        arr[i] = help[i];
    }
    return ans;
}
