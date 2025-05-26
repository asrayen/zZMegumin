#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#define maxn 1005
#define mod 100007

vector<char> mat[maxn];
vector<char>::iterator it;
int vis[maxn][maxn];

int dfsl(int i)
{
    int len = 0;
    int llen = 1;
    int ans = 0;
    for (auto it = mat[i].begin(); it != mat[i].end(); it++)
    {
        if (*it == '.')
        {
            len++;
            vis[i][llen++] = 1;
        }
        if (*it == '*')
        {
            ans += (1 + len) * len / 2;
            len = 0;
        }
    }
    return ans;
}

int dfsr(int j)
{
    int len = 0;
    int llen = 1;
    int ans = 0;
    int fact = 0;
}

int main()
{
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            char ch;
            cin >> ch;
            mat[i].push_back(ch);
        }
    }
}