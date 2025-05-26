#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char Number[10][5][3] =
    {
        "XXX",
        "X.X",
        "X.X",
        "X.X",
        "XXX", // 0
        "..X",
        "..X",
        "..X",
        "..X",
        "..X", // 1
        "XXX",
        "..X",
        "XXX",
        "X..",
        "XXX", // 2
        "XXX",
        "..X",
        "XXX",
        "..X",
        "XXX", // 3
        "X.X",
        "X.X",
        "XXX",
        "..X",
        "..X", // 4
        "XXX",
        "X..",
        "XXX",
        "..X",
        "XXX", // 5
        "XXX",
        "X..",
        "XXX",
        "X.X",
        "XXX", // 6
        "XXX",
        "..X",
        "..X",
        "..X",
        "..X", // 7
        "XXX",
        "X.X",
        "XXX",
        "X.X",
        "XXX", // 8
        "XXX",
        "X.X",
        "XXX",
        "..X",
        "XXX" // 9
};

int main()
{
    int n;
    scanf("%d", &n);
    int *num;
    num = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        scanf("%1d", &num[i]);
    }
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int m = 0; m < 3; m++)
            {
                printf("%c", Number[num[j]][i][m]);
            }
            printf("  ");
        }
        printf("\n");
    }
    return 0;
}