#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define max 20005

void Swap(int *num1, int *num2)
{
    int temp;
    temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}
int Split(int *arr, int low, int high)
{
    int pivot = arr[low];
    int i = low;
    int j;
    for (j = low + 1; j <= high; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            if (i != j)
                Swap(&arr[i], &arr[j]);
        }
    }
    Swap(&arr[low], &arr[i]);
    return i;
}
void quicksort(int *arr, int low, int high)
{
    if (low < high)
    {
        int pivotIndex = Split(arr, low, high);
        quicksort(arr, low, pivotIndex - 1);
        quicksort(arr, pivotIndex + 1, high);
    }
}

int main()
{
    int dragonHead[max] = {0};
    int knignt[max] = {0};
    int price[max] = {0};
    int cnt = 0;
    while (1)
    {
        scanf("%d%d", &dragonHead[cnt], &knignt[cnt]);
        if (dragonHead[cnt] == 0 && knignt[cnt] == 0)
        {
            break;
        }

        int *head;
        int *capacity;
        head = (int *)malloc(dragonHead[cnt] * sizeof(int));
        capacity = (int *)malloc(knignt[cnt] * sizeof(int));
        for (int j = 0; j < dragonHead[cnt]; j++)
        {
            scanf("%d", &head[j]);
        }
        for (int j = 0; j < knignt[cnt]; j++)
        {
            scanf("%d", &capacity[j]);
        }
        quicksort(capacity, 0, knignt[cnt] - 1);
        for (int j = 0; j < dragonHead[cnt]; j++)
        {
            int temp = 0;
            while (temp < knignt[cnt])
            {
                if (head[j] <= capacity[temp])
                {
                    price[cnt] += capacity[temp];
                    break;
                }
                temp++;
            }
            if (temp == knignt[cnt])
            {
                price[cnt] = 0;
                break;
            }
        }

        free(head);
        free(capacity);
        cnt++;
    }
    for (int i = 0; i < cnt; i++)
    {
        if (dragonHead[i] > knignt[i])
        {
            printf("Loowater is doomed!\n");
            continue;
        }
        if (!price[i])
        {
            printf("Loowater is doomed!\n");
        }
        else
        {
            printf("%d\n", price[i]);
        }
    }
    return 0;
}