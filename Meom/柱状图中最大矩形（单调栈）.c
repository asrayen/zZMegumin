// 给定 n 个非负整数，用来表示柱状图中各个柱子的高度。每个柱子彼此相邻，且宽度为 1 。

// 求在该柱状图中，能够勾勒出来的矩形的最大面积。

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define maxn 1005

int max(int a, int b)
{
    return a > b ? a : b;
}

int main()
{
    int n;
    printf("please put in the number of rectangles:\n");
    scanf("%d", &n);
    printf("and then put in their heights:\n");
    int *height;
    height = (int*)malloc(n * sizeof(int));
    int stack[maxn];
    int left, r, cur, ans;
    left = 0;
    r = 0;
    ans = 0;
    for(int i = 0; i < n; i++)
    {
        printf("number %d: ", i + 1);
        scanf("%d", &height[i]);
    }
    for(int i = 0; i < n; i++)
    {
        while(r > 0 && height[stack[r - 1]] >= height[i])
        {
            cur = stack[--r];
            left = r == 0 ? -1 : stack[r - 1];
            ans = max(ans, height[cur] * (i - left - 1));
        }
        stack[r++] = i;
    }
    while(r > 0)
    {
        cur = stack[--r];
        left = r == 0 ? -1 : stack[r - 1];
        ans = max(ans, height[cur] * (n - left - 1));
    }
    printf("their max square is %d\n", ans);
    system("pause");
    return 0;
}