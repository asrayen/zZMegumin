#include <iostream>
#include <string.h>
#include <vector>
#define ll long long int
#define maxn 1501
#define INF 0x7fffffff

int n, m; // 点的个数 边的条数

int head[maxn];
int next[maxn];
int to[maxn];
int weight[maxn];
int cnt = 1;
// 入度表
int indegree[maxn];

void init(int n)
{
    for (int i = 1; i <= n; i++)
    {
        head[i] = 0;
    }
}

void add_edge(int u, int v, int w)
{
    next[cnt] = head[u];
    to[cnt] = v;
    weight[cnt] = w;
    head[u] = cnt++;
}

void out_put(int i)
{
    for (int ei = head[i]; ei > 0; ei = next[ei])
    {
        printf("from %d to %d weight is %d\n", i, to[ei], weight[ei]);
    }
}

int main()
{
    std::cin >> n >> m;
    int u, v;
    memset(indegree, 0, sizeof(indegree));
    int quene[maxn];
    memset(quene, 0, sizeof(quene));
    int l = 1;
    int r = 1;
    int cnt = 0;
    for (int i = 1; i <= m; i++)
    {
        std::cin >> u >> v;
        add_edge(u, v, 1);
        indegree[v]++;
    }
    for (int i = 1; i <= n; i++)
    {
        if (indegree[i] == 0)
            quene[r++] = i;
    }
    while (l < r)
    {
        int cur = quene[l++];
        cnt++;
        for (int i = head[cur]; i > 0; i = next[i])
        {
            if (--indegree[to[i]] == 0)
                quene[r++] = to[i];
        }
    }
    for (int i = 1; i <= n; i++)
    {
        printf("%d ", quene[i]);
    }
    return 0;
}
