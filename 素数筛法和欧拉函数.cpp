#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>

#define maxn 1000000005
using namespace std;

vector<int> Eratosthenes_prime;
vector<int> Euler_prime;
vector<bool> vis(maxn);
vector<int> phi(maxn);
vector<int>::iterator it;
// ����ɸ
int Eratosthenes_sieve(int n)
{
    if (n < 2)
    {
        return 0;
    }
    int i, j;
    vis[0] = vis[1] = 1;
    Eratosthenes_prime.push_back(2);
    for (i = 4; i <= n; i += 2)
    {
        vis[i] = 1;
    }
    for (i = 3; i * i <= n; i += 2)
    {
        if (vis[i] == 0)
        {
            for (j = i; i * j <= n; j++)
            {
                vis[i * j] = 1;
            }
        }
    }
    for (i = 3; i <= n; i += 2)
    {
        if (!vis[i])
        {
            Eratosthenes_prime.push_back(i);
        }
    }
    return Eratosthenes_prime.size();
}
// ŷ��ɸ
int Euler_sieve(int n)
{

    int i;
    vis[0] = vis[1] = 1;
    for (i = 2; i <= n; i++)
    {
        if (!vis[i])
        {
            Euler_prime.push_back(i);
            phi[i] = i - 1;
        }
        for (auto it = Euler_prime.begin(); it != Euler_prime.end(); it++)
        {
            int temp = *it;
            vis[i * temp] = 1;
            if (i * temp > n)
            {
                break;
            }
            if (i % temp == 0)
            {
                phi[i * temp] = phi[i] * temp;
                break;
            }
            phi[i * temp] = phi[i] * (temp - 1);
        }
    }
    return Euler_prime.size();
}
int main()
{
    int n, k, m;
    cout << "�����������ѯ�ķ�΄1�7: 1 ~ ";
    while (cin >> n)
    {
        // k = Eratosthenes_sieve(n);
        // cout << "һ����" << k << "������" << endl;
        // for (auto it = Eratosthenes_prime.begin(); it != Eratosthenes_prime.end(); it++)
        // {
        //     cout << *it << endl;
        // }

        m = Euler_sieve(n);
        cout << "һ����" << m << "������" << endl;
        // for (auto it = Euler_prime.begin(); it != Euler_prime.end(); it++)
        // {
        //     cout << *it << endl;
        // }
        cout << n << "��ŷ������ֵ��" << phi[n] << endl;
    }
    return 0;
}