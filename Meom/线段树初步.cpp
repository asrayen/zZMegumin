// # 【深基16.例1】淘汰赛

// ## 题目描述

// 有 $2^n$（$n\le7$）个国家参加世界杯决赛圈且进入淘汰赛环节。已经知道各个国家的能力值，且都不相等。能力值高的国家和能力值低的国家踢比赛时高者获胜。1 号国家和 2 号国家踢一场比赛，胜者晋级。3 号国家和 4 号国家也踢一场，胜者晋级……晋级后的国家用相同的方法继续完成赛程，直到决出冠军。给出各个国家的能力值，请问亚军是哪个国家？

// ## 输入格式

// 第一行一个整数 $n$，表示一共 $2^n$ 个国家参赛。

// 第二行 $2^n$ 个整数，第 $i$ 个整数表示编号为 $i$ 的国家的能力值（$1\leq i \leq 2^n$）。

// 数据保证不存在平局。

// ## 输出格式

// 仅一个整数，表示亚军国家的编号。

// ## 样例 #1

// ### 样例输入 #1

// ```
// 3
// 4 2 3 1 10 5 9 7
// ```

// ### 样例输出 #1

// ```
// 1
// ```

#include <iostream>
#include <string.h>
#include <math.h>
#define ll long long int
#define maxn 105

struct que
{
    int power;
    int id;
};

que power[maxn];
que tree[maxn * 4];

que max(que a, que b)
{
    return a.power > b.power ? a : b;
}

que min(que a, que b)
{
    return a.power < b.power ? a : b;
}

void build_tree(int left, int right, int node)
{
    if (left == right)
    {
        tree[node] = power[left];
        return;
    }
    int leftnode = node * 2;
    int rightnode = node * 2 + 1;
    int mid = (left + right) / 2;
    build_tree(left, mid, leftnode);
    build_tree(mid + 1, right, rightnode);
    tree[node] = max(tree[leftnode], tree[rightnode]);
}

int main()
{
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= (1 << n); i++)
    {
        scanf("%d", &power[i].power);
        power[i].id = i;
    }
    build_tree(1, (1 << n), 1);
    int ans = min(tree[2], tree[3]).id;
    printf("%d", ans);
    return 0;
}