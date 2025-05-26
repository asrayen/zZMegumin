#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

struct place
{
	int f_type;
	int if_open;
} place;
int max(int n, int m)
{
	return n > m ? n : m;
}
int isOpen(struct place *p, int month, int n)
{
	if (p[n].f_type == 1)
	{
		if (month >= 1 && month <= 3)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if (p[n].f_type == 2)
	{
		if (month >= 3 && month <= 4)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if (p[n].f_type == 3)
	{
		if (month >= 4 && month <= 5)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if (month >= 9 && month <= 10)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

int main()
{
	long int n;
	int p, q;
	scanf("%ld", &n);
	struct place *point = (struct place *)malloc(n * sizeof(place));
	int m;
	int u, v;
	int month;
	int sort1, sort2, sort3, sort4;
	for (long int i = 1; i <= n; i++)
	{
		scanf("%d", &point[i].f_type);
	}
	scanf("%d %d", &p, &q);
	scanf("%d", &m);
	while (m--)
	{
		int cnt = 0;
		scanf("%d %d %d", &month, &u, &v);
		if (month > 5 && month < 9)
		{
			printf("So Sad.");
		}
		if (u > p && v > q && u < q)
		{
			int all = 0;
			for (int i = 1; i <= n; i++)
			{
				if (isOpen(point, month, i))
				{
					all++;
				}
			}
			for (int i = u; i <= v; i++)
			{
				if (isOpen(point, month, i))
				{
					cnt++;
				}
			}
			sort1 = cnt;
			sort2 = all - sort1 + isOpen(point, month, u) + isOpen(point, month, v);
			cnt = 0;
			for (int i = u; i >= p; i--)
			{
				if (isOpen(point, month, i))
				{
					cnt++;
				}
			}
			for (int i = v; i >= q; i--)
			{
				if (isOpen(point, month, i))
				{
					cnt++;
				}
			}
			sort3 = cnt;
			sort4 = all - sort3 + isOpen(point, month, u) + isOpen(point, month, v) + isOpen(point, month, p) + isOpen(point, month, q);
			cnt = max(max(sort1, sort2), max(sort3, sort4));
		}
		else if (u < p && v < q && v > p)
		{
			int all = 0;
			for (int i = 1; i <= n; i++)
			{
				if (isOpen(point, month, i))
				{
					all++;
				}
			}
			for (int i = u; i <= v; i++)
			{
				if (isOpen(point, month, i))
				{
					cnt++;
				}
			}
			sort1 = cnt;
			sort2 = all - sort1 + isOpen(point, month, u) + isOpen(point, month, v);
			cnt = 0;
			for (int i = u; i <= p; i++)
			{
				if (isOpen(point, month, i))
				{
					cnt++;
				}
			}
			for (int i = v; i <= q; i++)
			{
				if (isOpen(point, month, i))
				{
					cnt++;
				}
			}
			sort3 = cnt;
			sort4 = all - sort3 + isOpen(point, month, u) + isOpen(point, month, v) + isOpen(point, month, p) + isOpen(point, month, q);
			cnt = max(max(sort1, sort2), max(sort3, sort4));
		}
		else
		{
			int all = 0;
			for (int i = 1; i <= n; i++)
			{
				if (isOpen(point, month, i))
				{
					all++;
				}
			}
			for (int i = u; i <= v; i++)
			{
				if (isOpen(point, month, i))
				{
					cnt++;
				}
			}
			sort1 = cnt;
			sort2 = all - sort1 + isOpen(point, month, u) + isOpen(point, month, v);
			cnt = max(sort1, sort2);
		}
		printf("һ���� %d �����ڿ���\n", cnt);
	}
}
