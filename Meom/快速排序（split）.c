#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
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
    int a[9];
    for (int i = 0; i < 9; i++)
    {
        scanf("%d", &a[i]);
    }
    quicksort(a, 0, 8);
    for (int j = 0; j < 9; j++)
    {
        printf("%d ", a[j]);
    }
    return 0;
}