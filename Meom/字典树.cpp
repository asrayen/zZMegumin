#include <iostream>
#include <string.h>
#define ll long long int
#define maxn 500005
#define INF 0x7fffffff
using namespace std;

int trie[maxn][26];
bool b[maxn][1005];
int n;
int id = 0;
char s[10005];

inline int read()
{
    int k = 0, f = 1;
    char ch = getchar();
    while (ch < '0' || ch > '9')
    {
        if (ch == '-')
            f = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9')
    {
        k = k * 10 + ch - '0';
        ch = getchar();
    }
    return k * f;
}

void insert(int x)
{
    int p = 0;
    scanf("%s", s + 1);
    int len = strlen(s + 1);
    for (int i = 1; i <= len; i++)
    {
        int x = s[i] - 'a';
        if (trie[p][x] == 0)
            trie[p][x] = ++id;
        p = trie[p][x];
    }
    b[p][x] = 1;
}

void check()
{
    scanf("%s", s + 1);
    int len = strlen(s + 1);
    int p = 0;
    int flag = 1;
    for (int i = 1; i <= len; i++)
    {
        int x = s[i] - 'a';
        if (trie[p][x] == 0)
        {
            flag = 0;
            break;
        }
        p = trie[p][x];
    }
    if (flag)
    {
        for (int i = 1; i <= n; i++)
        {
            if (b[p][i])
            {
                printf("%d ", i);
            }
        }
    }
    puts("");
}

int main()
{
    n = read();
    for (int i = 1; i <= n; i++)
    {
        int x = read();
        for (int j = 1; j <= x; j++) // 一个单词一个单词的插入Trie树里
            insert(i);
    }
    int m = read();
    for (int i = 1; i <= m; i++)
        check();
    return 0;
}