#include <iostream>
#include <string.h>
#include <algorithm>
#include <math.h>
using namespace std;
#define ll long long int
#define maxn 5005

int mode(int x, int y)
{
    if (x == 0 && y == 0)
    {
        return 4;
    }
    else if (x == 0 && y == 1)
    {
        return 2;
    }
    else if (x == 1 && y == 0)
    {
        return 3;
    }
    else
        return 1;
}

void dfs(int x, int y, int lx, int rx, int ly, int ry, int n)
{
    if (n == 2)
    {
        int ansx = rx;
        int ansy = ry;
        if (x == rx)
        {
            ansx = lx;
        }
        if (y == ry)
        {
            ansy = ly;
        }
        printf("%d %d %d", ansy, ansx, mode(x % 2, y % 2));
        printf("\n");
        return;
    }
    int tempx = (lx + rx) / 2;
    int tempy = (ly + ry) / 2;
    int px = (x - lx) / (n / 2);
    int py = (y - ly) / (n / 2);
    if (px == 0 && py == 0)
    {
        printf("%d %d %d\n", tempy + 1, tempx + 1, 1);
        dfs(x, y, lx, tempx, ly, tempy, n / 2);
        dfs(tempx + 1, tempy + 1, tempx + 1, rx, tempy + 1, ry, n / 2);
        dfs(tempx, tempy + 1, lx, tempx, tempy + 1, ry, n / 2);
        dfs(tempx + 1, tempy, tempx + 1, rx, ly, tempy, n / 2);
    }
    else if (px == 1 && py == 0)
    {
        printf("%d %d %d\n", tempy + 1, tempx, 3);
        dfs(x, y, lx, tempx, tempy + 1, ry, n / 2);
        dfs(tempx, tempy, lx, tempx, ly, tempy, n / 2);
        dfs(tempx + 1, tempy + 1, tempx + 1, rx, tempy + 1, ry, n / 2);
        dfs(tempx, tempy + 1, tempx + 1, rx, ly, tempy, n / 2);
    }
    else if (px == 0 && py == 1)
    {
        printf("%d %d %d\n", tempy, tempx, 2);
        dfs(x, y, lx, tempx, tempy + 1, ry, n / 2);
        dfs(tempx, tempy, lx, tempx, ly, tempy, n / 2);
        dfs(tempx + 1, tempy + 1, tempx + 1, rx, tempy + 1, ry, n / 2);
        dfs(tempx + 1, tempy, lx, tempx, tempy + 1, ry, n / 2);
    }
    else
    {
        printf("%d %d %d\n", tempy, tempx, 4);
        dfs(x, y, tempx + 1, rx, tempy + 1, ry, n / 2);
        dfs(tempx, tempy, lx, tempx, ly, tempy, n / 2);
        dfs(tempx + 1, tempy, tempx + 1, rx, ly, tempy, n / 2);
        dfs(tempx, tempy + 1, lx, tempx, tempy + 1, ry, n / 2);
    }
}

int main()
{
    int k;
    scanf("%d", &k);
    int n = 1;
    for (int i = 1; i <= k; i++)
    {
        n *= 2;
    }
    int x, y;
    scanf("%d%d", &x, &y);
    dfs(x, y, 1, n, 1, n, n);
    return 0;
}