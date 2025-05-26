#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int max(int a, int b)
{
    return a > b ? a : b;
}
int add(int *fact, int *addend, int len)
{
    int carry = 0;
    for (int k = 0; k < len; k++)
    {
        int sum = fact[k] + addend[k] + carry;
        addend[k] = sum % 10;
        carry = sum / 10;
    }
    if (carry)
    {
        addend[len++] = carry;
    }
    return len;
}

int main()
{
    int num1[10000] = {0};
    int num2[10000] = {0};
    int len1 = 1;
    int len2 = 1;
    num1[0] = 1;
    num2[0] = 1;
    int n;
    scanf("%d", &n);
    if (n == 1 || n == 2)
    {
        printf("1");
    }
    else if (n == 0)
    {
        printf("0");
    }
    else
    {
        for (int i = 0; i < (n - 3) / 2 + 1; i++)
        {
            len1 = add(num2, num1, max(len1, len2));
            len2 = add(num1, num2, max(len1, len2));
        }
        if (n % 2 == 1)
        {
            for (int j = len1 - 1; j >= 0; j--)
            {
                printf("%d", num1[j]);
            }
        }
        else
        {
            for (int j = len2 - 1; j >= 0; j--)
            {
                printf("%d", num2[j]);
            }
        }
    }
    return 0;
}