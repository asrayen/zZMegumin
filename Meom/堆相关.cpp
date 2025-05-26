#include <iostream>
#include <string.h>
#include <algorithm>
#include <math.h>
using namespace std;
#define ll long long int
#define maxn 50005

ll heap[maxn];
ll len;

void swap(ll *arr, ll a, ll b)
{
    ll temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}

void heap_ins(ll n)
{
    len++;
    heap[len] = n;
    ll i = len;
    while (i != 1 && heap[i] < heap[i / 2])
    {
        swap(heap, i, i / 2);
        i = i / 2;
    }
}

ll heap_get()
{
    ll ans = heap[1];
    heap[1] = heap[len];
    len--;
    ll i = 1;
    while (i * 2 <= len)
    {
        ll min;
        if (i * 2 + 1 > len || heap[i * 2 + 1] > heap[i * 2])
        {
            min = i * 2;
        }
        else
        {
            min = i * 2 + 1;
        }
        if (heap[i] > heap[min])
        {
            swap(heap, i, min);
            i = min;
        }
        else
        {
            return ans;
        }
    }
    return ans;
}