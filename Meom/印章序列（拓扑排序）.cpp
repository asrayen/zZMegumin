#include <iostream>
#include <string.h>
#include <vector>
#define ll long long int
#define maxn 150
#define INF 0x7fffffff

// int n, m; // 点的个数 边的条数

int head[maxn];
int next[maxn];
int to[maxn];
int weight[maxn];
int cnt = 1;
// 入度表
int indegree[maxn];
int quene[maxn];
bool vis[maxn];
int path[maxn];

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

    int str_len, t_len;
    scanf("%d %d", &str_len, &t_len);
    char str[maxn];
    char target[maxn];
    int l = 0;
    int r = 0;
    int size = 0;
    scanf("%s", str + 1);
    scanf("%s", target + 1);
    init(str_len + t_len);
    for (int i = 1; i <= str_len - t_len + 1; i++)
    {
        indegree[i] = t_len;
    }
    memset(path, 0, sizeof(path));
    memset(quene, 0, sizeof(quene));
    memset(vis, 0, sizeof(vis));
    for (int i = 1; i <= str_len - t_len + 1; i++)
    {
        for (int j = 0; j < t_len; j++)
        {
            if (str[i + j] == target[j + 1])
            {
                if (--indegree[i] == 0)
                {
                    quene[r++] = i;
                }
            }
            else
            {
                add_edge(i + j, i, 1);
            }
        }
    }
    while (l < r)
    {
        int cur = quene[l++];
        path[size++] = cur;
        for (int i = 0; i < t_len; i++)
        {
            if (!vis[cur + i])
            {
                vis[cur + i] = true;
                for (int ei = head[cur + i]; ei > 0; ei = next[ei])
                {
                    if (--indegree[to[ei]] == 0)
                    {
                        quene[r++] = to[ei];
                    }
                }
            }
        }
    }
    for (int i = 0, j = size - 1; i < j; i++, j--)
    {
        int temp = path[i];
        path[i] = path[j];
        path[j] = temp;
    }
    // for (int i = 0; i < size; i++)
    // {
    //     printf("path[%d] = %d\n", i, path[i]);
    // }
    char out[str_len + 1];
    for (int i = 1; i <= str_len; i++)
    {
        out[i] = '.';
    }
    for (int i = 0; i < size; i++)
    {
        memcpy(out + path[i], target + 1, t_len);
        for (int j = 1; j <= str_len; j++)
        {
            printf("%c", out[j]);
        }
        printf("\n");
    }
    return 0;
}
