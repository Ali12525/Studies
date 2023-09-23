#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void shell(int* items, int count);
void qs(int* items, int left, int right);
void fillArray(int* Arr, int size, int direction);
void fillArrayHalf(int* Arr, int size);
int compare(const void* x1, const void* x2);
void exercise1(int* Arr, int size);
void exercise2(int* Arr, int size);
void exercise3(int* Arr, int size);
void exercise4(int* Arr, int size);

int main()
{
    srand(time(NULL));

    for (int i = 0; i < 120000; i += 20000)
    {
        printf("Size: %d", i);
        int *Arr = (int*)malloc(i * sizeof(int));
        exercise1(Arr, i);
        exercise2(Arr, i);
        exercise3(Arr, i);
        exercise4(Arr, i);
        free(Arr);
        printf("\n\n\n");
    }

    return 0;
}

void exercise1(int* Arr, int size)
{
    printf("\n\nRandom numbers");
    for (int i = 0; i < size; i++)
    {
        Arr[i] = rand() % 100;
    }

    clock_t start = clock();
    shell(Arr, size);
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\ntime shell: %f s.", time_spent);

    for (int i = 0; i < size; i++)
    {
        Arr[i] = rand() % 100;
    }

    start = clock();
    qs(Arr, 0, size - 1);
    end = clock();
    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\ntime qs: %f s.", time_spent);

    for (int i = 0; i < size; i++)
    {
        Arr[i] = rand() % 100;
    }

    start = clock();
    qsort(Arr, size, sizeof(int), compare);
    end = clock();
    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\ntime standart qsort: %f s.", time_spent);
}

void exercise2(int* Arr, int size)
{
    printf("\n\nIncreasing sequence");
    fillArray(Arr, size, 0);
    clock_t start = clock();
    shell(Arr, size);
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\ntime shell: %f s.", time_spent);

    fillArray(Arr, size, 0);
    start = clock();
    qs(Arr, 0, size - 1);
    end = clock();
    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\ntime qs: %f s.", time_spent);

    fillArray(Arr, size, 0);
    start = clock();
    qsort(Arr, size, sizeof(int), compare);
    end = clock();
    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\ntime standart qsort: %f s.", time_spent);
}

void exercise3(int* Arr, int size)
{
    printf("\n\nDescending sequencee");
    fillArray(Arr, size, 1);
    clock_t start = clock();
    shell(Arr, size);
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\ntime shell: %f s.", time_spent);

    fillArray(Arr, size, 1);
    start = clock();
    qs(Arr, 0, size - 1);
    end = clock();
    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\ntime qs: %f s.", time_spent);

    fillArray(Arr, size, 1);
    start = clock();
    qsort(Arr, size, sizeof(int), compare);
    end = clock();
    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\ntime standart qsort: %f s.", time_spent);
}

void exercise4(int* Arr, int size)
{
    printf("\n\nHalf the numbers are increasing, half are decreasing");
    fillArrayHalf(Arr, size);
    clock_t start = clock();
    shell(Arr, size);
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\ntime shell: %f s.", time_spent);

    fillArrayHalf(Arr, size);
    start = clock();
    qs(Arr, 0, size - 1);
    end = clock();
    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\ntime qs: %f s.", time_spent);

    fillArrayHalf(Arr, size);
    start = clock();
    qsort(Arr, size, sizeof(int), compare);
    end = clock();
    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\ntime standart qsort: %f s.", time_spent);
}

void shell(int* items, int count)
{
    int i, j, gap, k;
    int x, a[5];

    a[0] = 9; a[1] = 5; a[2] = 3; a[3] = 2; a[4] = 1;

    for (k = 0; k < 5; k++) {
        gap = a[k];
        for (i = gap; i < count; ++i) {
            x = items[i];
            for (j = i - gap; (x < items[j]) && (j >= 0); j = j - gap)
                items[j + gap] = items[j];
            items[j + gap] = x;
        }
    }
}

void qs(int* items, int left, int right) //вызов функции: qs(items, 0, count-1);
{
    int i, j;
    int x, y;

    i = left; j = right;

    /* выбор компаранда */
    x = items[(left + right) / 2];

    do {
        while ((items[i] < x) && (i < right)) i++;
        while ((x < items[j]) && (j > left)) j--;

        if (i <= j) {
            y = items[i];
            items[i] = items[j];
            items[j] = y;
            i++; j--;
        }
    } while (i <= j);

    if (left < j) qs(items, left, j);
    if (i < right) qs(items, i, right);
}

void fillArray(int* Arr, int size, int direction)
{
    if(direction == 1)
    {
        for (int i = size-1; i > 0; i--)
        {
            Arr[size - 1 - i] = i;
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            Arr[i] = i;
        }
    }
    
}

void fillArrayHalf(int* Arr, int size)
{
     for (int i = 0; i < size/2; i++)
     {
        Arr[i] = i;
     }

     for (int i = size - 1; i > size/2 - 1; i--)
     {
         Arr[size - 1 - i + size/2] = i;
     }
}

int compare(const void* x1, const void* x2)
{
    return (*(int*)x1 - *(int*)x2);
}