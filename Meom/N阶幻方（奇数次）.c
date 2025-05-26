#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
struct p
{
    int x;
    int y;
};
typedef struct p point;
int main()
{
    int n;
    scanf("%d", &n);
    point *pt;
    pt = (point *)malloc((n + 1) * n * sizeof(point));
    int matrix[40][40] = {0};
    pt[1].x = (n + 1) / 2;
    pt[1].y = 1;
    matrix[1][(n + 1) / 2] = 1;
    for (int i = 2; i <= n * n; i++)
    {
        if (pt[i - 1].y == 1 && pt[i - 1].x != n)
        {
            pt[i].y = n;
            pt[i].x = pt[i - 1].x + 1;
        }
        if (pt[i - 1].y != 1 && pt[i - 1].x == n)
        {
            pt[i].y = pt[i - 1].y - 1;
            pt[i].x = 1;
        }
        if (pt[i - 1].y == 1 && pt[i - 1].x == n)
        {
            pt[i].y = 2;
            pt[i].x = n;
        }
        if (pt[i - 1].y != 1 && pt[i - 1].x != n)
        {
            if (matrix[pt[i - 1].y - 1][pt[i - 1].x + 1] == 0)
            {
                pt[i].y = pt[i - 1].y - 1;
                pt[i].x = pt[i - 1].x + 1;
            }
            else
            {
                pt[i].y = pt[i - 1].y + 1;
                pt[i].x = pt[i - 1].x;
            }
        }
        matrix[pt[i].y][pt[i].x] = i;
    }
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    return 0;
}