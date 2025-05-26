// # 马的遍历

// ## 题目描述

// 有一个 $n \times m$ 的棋盘，在某个点 $(x, y)$ 上有一个马，要求你计算出马到达棋盘上任意一个点最少要走几步。

// ## 输入格式

// 输入只有一行四个整数，分别为 $n, m, x, y$。

// ## 输出格式

// 一个 $n \times m$ 的矩阵，代表马到达某个点最少要走几步（不能到达则输出 $-1$）。

// ## 样例 #1

// ### 样例输入 #1

// ```
// 3 3 1 1
// ```

// ### 样例输出 #1

// ```
// 0    3    2
// 3    -1   1
// 2    1    4
// ```

// ## 提示

// ### 数据规模与约定

// 对于全部的测试点，保证 $1 \leq x \leq n \leq 400$，$1 \leq y \leq m \leq 400$。

#include <iostream>
#include <string.h>

// #define ll long long int
#define maxn 505

int matrix[maxn][maxn];
int vis[maxn][maxn];
int n, m;
int dir[8][2] = {{-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}};

struct quene
{
    int x;
    int y;
} q[maxn * maxn];

bool is_ok(int x, int y)
{
    return x <= n && x >= 1 && y <= m && y >= 1;
}

void bfs(int x, int y)
{
    int head = 0;
    int tail = 1;
    int cnt = 0;
    vis[x][y] = 1;
    matrix[x][y] = 0;
    q[1].x = x;
    q[1].y = y;
    while (head < tail)
    {
        head++;
        cnt = matrix[q[head].x][q[head].y] + 1;
        for (int i = 0; i <= 7; i++)
        {
            int px = q[head].x + dir[i][0];
            int py = q[head].y + dir[i][1];
            if (is_ok(px, py) && !vis[px][py])
            {
                tail++;
                q[tail].x = px;
                q[tail].y = py;
                vis[px][py] = 1;
                matrix[q[tail].x][q[tail].y] = cnt;
            }
        }
    }
}

int main()
{
    int x, y;
    scanf("%d%d%d%d", &n, &m, &x, &y);
    memset(matrix, -1, sizeof(matrix));
    bfs(x, y);
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    return 0;
}