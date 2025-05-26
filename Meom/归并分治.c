// 给定一个数组 nums ，如果 i < j 且 nums[i] > 2*nums[j] 我们就将 (i, j) 称作一个重要翻转对。

// 你需要返回给定数组中的重要翻转对的数量。

// 示例 1:
// 输入: [1,3,2,3,1]
// 输出: 2

// 示例 2:
// 输入: [2,4,3,5,1]
// 输出: 3

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define maxn 5005

int help[maxn];  // 辅助数组

int count(int *arr, int l, int r)
{
    if(l == r)
    {
        return 0;
    }
    int m = (l + r) / 2;
    return count(arr, l, m) + count(arr, m + 1, r) + merge(arr, l, m, r);
}
// 分治思想
int merge(int *arr, int l, int m, int r)
{
    // 统计部分
    int ans = 0;
    for(int i = l, j = m + 1; i <= m; i++)
    {
        while(j <= r && arr[i] > arr[j] * 2)
        {
            j++;
        }
        ans += j - m - 1;
    }

    // 归并排序
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
