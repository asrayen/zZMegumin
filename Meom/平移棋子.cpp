// # 黑白棋子的移动

// ## 题目描述

// 有 $2n$ 个棋子排成一行，开始为位置白子全部在左边，黑子全部在右边，如下图为 $n=5$ 的情况：

// ![](https://cdn.luogu.com.cn/upload/image_hosting/dzfwand6.png)

// 移动棋子的规则是：每次必须同时移动相邻的两个棋子，颜色不限，可以左移也可以右移到空位上去，但不能调换两个棋子的左右位置。每次移动必须跳过若干个棋子（不能平移），要求最后能移成黑白相间的一行棋子。如 $n=5$ 时，成为：

// ![](https://cdn.luogu.com.cn/upload/image_hosting/yus9ph6d.png)

// 任务：编程打印出移动过程。

// ## 输入格式

// 一个整数 $n$。

// ## 输出格式

// 若干行，表示初始状态和每次移动的状态，用 $\verb!o!$ 表示白子，$\verb!*!$ 表示黑子，$\verb!-!$ 表示空行。

// ## 样例 #1

// ### 样例输入 #1

// ```
// 7
// ```

// ### 样例输出 #1

// ```
// ooooooo*******--
// oooooo--******o*
// oooooo******--o*
// ooooo--*****o*o*
// ooooo*****--o*o*
// oooo--****o*o*o*
// oooo****--o*o*o*
// ooo--***o*o*o*o*
// ooo*o**--*o*o*o*
// o--*o**oo*o*o*o*
// o*o*o*--o*o*o*o*
// --o*o*o*o*o*o*o*
// ```

// ## 提示

// $ 4\leq n\leq 100$

#include <iostream>
#include <string.h>
#include <algorithm>
using namespace std;
#define ll long long int
#define maxn 5005

void cswap(char *go, int n, int m)
{
    int temp = go[n];
    go[n] = go[m];
    go[m] = temp;
}

void show(char *go, int n)
{
    for (int i = 1; i <= n; i++)
    {
        printf("%c", go[i]);
    }
    printf("\n");
}

int main()
{
    int n;
    scanf("%d", &n);
    char go[maxn];
    for (int i = 1; i <= n; i++)
    {
        go[i] = 'o';
        go[i + n] = '*';
    }
    go[2 * n + 1] = '-';
    go[2 * n + 2] = '-';
    int len = 2 * n + 2;
    show(go, len);
    while (n > 4)
    {
        cswap(go, n, 2 * n + 1);
        cswap(go, n + 1, 2 * n + 2);
        show(go, len);

        cswap(go, n, 2 * n - 1);
        cswap(go, n + 1, 2 * n);
        show(go, len);
        n--;
    }
    go[4] = '-';
    go[5] = '-';
    go[9] = 'o';
    go[10] = '*';
    show(go, len);
    go[4] = '*';
    go[5] = 'o';
    go[8] = '-';
    go[9] = '-';
    show(go, len);
    go[2] = '-';
    go[3] = '-';
    go[8] = 'o';
    go[9] = 'o';
    show(go, len);
    go[2] = '*';
    go[3] = 'o';
    go[7] = '-';
    go[8] = '-';
    show(go, len);
    go[1] = '-';
    go[2] = '-';
    go[7] = 'o';
    go[8] = '*';
    show(go, len);
    return 0;
}