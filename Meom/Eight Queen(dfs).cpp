// # [USACO1.5] 八皇后 Checker Challenge

// ## 题目描述

// 一个如下的 $6 \times 6$ 的跳棋棋盘，有六个棋子被放置在棋盘上，使得每行、每列有且只有一个，每条对角线（包括两条主对角线的所有平行线）上至多有一个棋子。

// ![](https://cdn.luogu.com.cn/upload/image_hosting/3h71x0yf.png)

// 上面的布局可以用序列 $2\ 4\ 6\ 1\ 3\ 5$ 来描述，第 $i$ 个数字表示在第 $i$ 行的相应位置有一个棋子，如下：

// 行号 $1\ 2\ 3\ 4\ 5\ 6$

// 列号 $2\ 4\ 6\ 1\ 3\ 5$

// 这只是棋子放置的一个解。请编一个程序找出所有棋子放置的解。
// 并把它们以上面的序列方法输出，解按字典顺序排列。
// 请输出前 $3$ 个解。最后一行是解的总个数。

// ## 输入格式

// 一行一个正整数 $n$，表示棋盘是 $n \times n$ 大小的。

// ## 输出格式

// 前三行为前三个解，每个解的两个数字之间用一个空格隔开。第四行只有一个数字，表示解的总数。

// ## 样例 #1

// ### 样例输入 #1

// ```
// 6
// ```

// ### 样例输出 #1

// ```
// 2 4 6 1 3 5
// 3 6 2 5 1 4
// 4 1 5 2 6 3
// 4
// ```

// ## 提示

// 【数据范围】
// 对于 $100\%$ 的数据，$6 \le n \le 13$。

// 题目翻译来自NOCOW。

// USACO Training Section 1.5

#include <iostream>
#include <string.h>

// #define ll long long int
#define maxn 50

int n, cnt = 0;
int matrix[maxn][maxn] = {0};
int mx[maxn] = {0};
int my[maxn] = {0};
int mxy[maxn] = {0};
int myx[maxn] = {0};

bool is_occ(int x, int y)
{
    if (mx[x] == 1 || my[y] == 1 || mxy[x + y] == 1 || myx[y - x + n] == 1 || matrix[x][y] == 1)
        return 1;
    else
        return 0;
}

void occ(int x, int y)
{
    matrix[x][y] = 1;
    mx[x] = 1;
    my[y] = 1;
    mxy[x + y] = 1;
    myx[y - x + n] = 1;
}

void rel(int x, int y)
{
    matrix[x][y] = 0;
    mx[x] = 0;
    my[y] = 0;
    mxy[x + y] = 0;
    myx[y - x + n] = 0;
}

void prin()
{
    if (cnt >= 3)
    {
        cnt++;
        return;
    }
    else
    {
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (matrix[i][j])
                {
                    printf("%d ", j);
                }
            }
        }
        printf("\n");
        cnt++;
        return;
    }
}

void dfs(int m)
{
    if (m == n + 1)
        prin();
    else
    {
        for (int i = 1; i <= n; i++)
        {
            if (!is_occ(m, i))
            {
                occ(m, i);
                dfs(m + 1);
                rel(m, i);
            }
        }
    }
}

int main()
{

    scanf("%d", &n);
    dfs(1);
    printf("%d", cnt);
    return 0;
}