// # [NOI2015] 程序自动分析

// ## 题目描述

// 在实现程序自动分析的过程中，常常需要判定一些约束条件是否能被同时满足。

// 考虑一个约束满足问题的简化版本：假设 $x_1,x_2,x_3,\cdots$ 代表程序中出现的变量，给定 $n$ 个形如 $x_i=x_j$ 或 $x_i\neq x_j$ 的变量相等/不等的约束条件，请判定是否可以分别为每一个变量赋予恰当的值，使得上述所有约束条件同时被满足。例如，一个问题中的约束条件为：$x_1=x_2,x_2=x_3,x_3=x_4,x_4\neq x_1$，这些约束条件显然是不可能同时被满足的，因此这个问题应判定为不可被满足。

// 现在给出一些约束满足问题，请分别对它们进行判定。

// ## 输入格式

// 输入的第一行包含一个正整数 $t$，表示需要判定的问题个数。注意这些问题之间是相互独立的。

// 对于每个问题，包含若干行：

// 第一行包含一个正整数 $n$，表示该问题中需要被满足的约束条件个数。接下来 $n$ 行，每行包括三个整数 $i,j,e$，描述一个相等/不等的约束条件，相邻整数之间用单个空格隔开。若 $e=1$，则该约束条件为 $x_i=x_j$。若$e=0$，则该约束条件为 $x_i\neq x_j$。

// ## 输出格式

// 输出包括 $t$ 行。

// 输出文件的第 $k$ 行输出一个字符串 `YES` 或者 `NO`（字母全部大写），`YES` 表示输入中的第 $k$ 个问题判定为可以被满足，`NO` 表示不可被满足。

// ## 样例 #1

// ### 样例输入 #1

// ```
// 2
// 2
// 1 2 1
// 1 2 0
// 2
// 1 2 1
// 2 1 1
// ```

// ### 样例输出 #1

// ```
// NO
// YES
// ```

// ## 样例 #2

// ### 样例输入 #2

// ```
// 2
// 3
// 1 2 1
// 2 3 1
// 3 1 1
// 4
// 1 2 1
// 2 3 1
// 3 4 1
// 1 4 0
// ```

// ### 样例输出 #2

// ```
// YES
// NO
// ```

// ## 提示

// 【样例解释1】

// 在第一个问题中，约束条件为：$x_1=x_2,x_1\neq x_2$。这两个约束条件互相矛盾，因此不可被同时满足。

// 在第二个问题中，约束条件为：$x_1=x_2,x_1 = x_2$。这两个约束条件是等价的，可以被同时满足。

// 【样例说明2】

// 在第一个问题中，约束条件有三个：$x_1=x_2,x_2= x_3,x_3=x_1$。只需赋值使得 $x_1=x_2=x_3$，即可同时满足所有的约束条件。

// 在第二个问题中，约束条件有四个：$x_1=x_2,x_2= x_3,x_3=x_4,x_4\neq x_1$。由前三个约束条件可以推出 $x_1=x_2=x_3=x_4$，然而最后一个约束条件却要求 $x_1\neq x_4$，因此不可被满足。

// 【数据范围】

// ![](https://cdn.luogu.com.cn/upload/pic/1503.png)

// 注：实际上 $n\le 10^6$ 。

#include<iostream>
#include<string.h>
#include<algorithm>
using namespace std;
#define ll long long int
#define maxn 200005

struct Node
{
    int x, y, e;
}a[maxn];

int fa[maxn], re[maxn];
int n, t;

bool cmp(Node a, Node b)
{
    return a.e > b.e;
}

void init(int n)
{
    for(int i = 1; i <= n; i++)
    {
        fa[i] = i;
    }
}

int find(int x)
{
    if(fa[x] == x)
    {
        return x;
    }
    else
    {
        fa[x] = find(fa[x]);
        return fa[x];
    }
}

void merge(int x, int y)
{
    fa[find(x)] = find(y);
}

int main()
{
    scanf("%d", &t);
    while(t--)
    {
        int flag = 1;
        memset(a, 0, sizeof(a));
        memset(fa, 0, sizeof(fa));
        memset(re, 0, sizeof(re));
        scanf("%d", &n);
        int cnt = 0;
        for(int i = 1; i <= n; i++)
        {
            scanf("%d%d%d", &a[i].x, &a[i].y, &a[i].e);
            re[++cnt] = a[i].x;
            re[++cnt] = a[i].y;
        }
        sort(re + 1, re + cnt + 1);
        int num = unique(re + 1, re + 1 + cnt) - re;
        init(num);
        for(int i = 1; i <= n; i++)
        {
            a[i].x = lower_bound(re + 1, re + 1 + num, a[i].x) - re;
            a[i].y = lower_bound(re + 1, re + 1 + num, a[i].y) - re;
        }
        sort(a + 1, a + 1 + n, cmp);
        for(int i = 1; i <= n; i++)
        {
            int n1 = find(a[i].x);
            int n2 = find(a[i].y);
            if(a[i].e)
            {
                merge(n1, n2);
            }
            else if(n1 == n2 && a[i].e == 0)
            {
                printf("NO\n");
                flag = 0;
                break;
            }
        }
        if(flag)
        {
            printf("YES\n");
        }
    }
    return 0;
}