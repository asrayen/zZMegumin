// 埃及分数问题

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#define maxn 10005
#define inf 0x3fffffff
#define ll long long
ll lower, upper, deep, commonDivider;
ll ans[maxn];
ll denominator[maxn]; 
int falg = 0;
clock_t start, stop;

ll max(ll a, ll b)
{
    return a > b ? a : b;
}

ll gcd(ll a, ll b)
{
    if(b == 0)
    {
        return a;
    }
    return gcd(b, a % b);
}

void dfs(ll upper, ll lower, ll d)
{
    if(d == deep)
    {
        if(lower % upper)
        {
            return;
        }
        if(lower / upper == denominator[d - 1])
        {
            return;
        }
        falg = 1;
        denominator[d] = lower / upper;

        // for(int i = 1; i <= d; i++)
        // {
        //     printf("%lld ", ans[i]);
        // }
        // printf("\n");
        // system("pause");
        // 分步输出解

        int temp = 0;
        for(int i = d; i > 0; i--)
        {
            if(ans[i] == denominator[i])
            {
                continue;
            }
            if(ans[i] < denominator[i])
            {
                break;
            }
            else
            {
                temp = 1;
                break;
            }
        }
        if(temp)
        {
            memcpy(ans, denominator, (d + 1) * sizeof(ll));
        }
        return;
    }
    for(int i = max(lower / upper + 1, denominator[d - 1] + 1);; i++)
    {
        if(lower * (deep - d + 1) < upper * i)
        {
            break;
        }
        denominator[d] = i;
        ll lower2 = lower * i;
        ll upper2 = upper * i - lower;
        commonDivider = gcd(upper2, lower2);
        lower2 /= commonDivider;
        upper2 /= commonDivider;
        dfs(upper2, lower2, d + 1);
    }
}

int main()
{
    start = clock();
    scanf("%lld%lld", &upper, &lower);
    if(upper % lower == 0)
    {
        printf("%lld", upper / lower);
        return 0;
    }
    commonDivider = gcd(lower, upper);
    lower /= commonDivider;
    upper /= commonDivider;
    for(deep = 2;; deep++)
    {
        ans[1] = 0;
        ans[deep] = inf;
        dfs(upper, lower, 1);
        if(falg)
        {
            break;
        }
    }
    printf("1/%lld", ans[1]);
    for(int i = 2; i <= deep; i++)
    {
        printf(" + ");
        printf("1/%lld", ans[i]);
    }
    // stop = clock();
    // double duration = ((double)(stop - start)) / CLK_TCK;
    // printf("\n运行时间:%lfs", duration);
    return 0;
}   