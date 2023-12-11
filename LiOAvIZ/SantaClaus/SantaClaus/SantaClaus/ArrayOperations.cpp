#include <stdlib.h>
#include <algorithm>

#include "ArrayOperations.h"
#include "Constants.h"

using std::find;

Present* CreatePresent(int num, int weight)
{
    Present* tmp = new Present(num, weight);
    return tmp;
}

Present** CreateEmptyArray(int size)
{
    Present** tmp = (Present**)malloc(size * sizeof(Present*));
    for (size_t i = 0; i < size; i++)
    {
        tmp[i] = (Present*)malloc(size * sizeof(Present));

        for (size_t j = 0; j < size; j += PRESENTS_PER_TYPE)
        {
            int weight = -1;
            if (i >= CHILD_COUNT)
                weight = -100;

            for (size_t k = 0; k < PRESENTS_PER_TYPE; k++)
            {
                tmp[i][j + k] = *CreatePresent(j, weight);
            }
        }
    }

    return tmp;
}

int IndexOf(int* arr, int element)
{
    int* p;
    p = find(arr, arr + PRESENTS_PER_TYPE, element);
    if (p != arr + PRESENTS_PER_TYPE)
        return *p;
    return -1;
}

void AddWeightChild(Present** a, int** arrChild)
{
    for (size_t i = 0; i < CHILD_COUNT; i++)
    {
        for (size_t j = 0; j < PRESENTS_COUNT * PRESENTS_PER_TYPE; j += PRESENTS_PER_TYPE)
        {
            int index = IndexOf(arrChild[i], a[i][j].Id());
            if (index != -1)
            {
                int weight = 2 * (100 - index);

                for (size_t k = 0; k < PRESENTS_PER_TYPE; k++)
                {
                    a[i][j + k].SetWeight(weight);
                }
            }
        }
    }
}

void AddWeightPresent(Present** a, int** arrPresent)
{
    for (size_t i = 0; i < PRESENTS_COUNT; i++)
    {
        for (size_t j = 1; j < 184; j++)
        {
            int child = arrPresent[i][j];
            int weight = 2 * (184 - j);
            if (a[child][i].Weight() < 0)
                weight -= 1;
            else
                weight += a[child][i].Weight();

            for (size_t k = 0; k < PRESENTS_PER_TYPE; k++)
            {
                a[child][i + k].SetWeight(weight);
            }
        }
    }
}