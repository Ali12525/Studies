#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "Present.h"
#include "Constants.h"
#include "ArrayOperations.h"
#include "FileWork.h"

using namespace std;

int n = 5;

Present** a;      // Матрица эффективности a[разраб][задача]
vector<int> xy, yx;             // Паросочетания: xy[разраб], yx[задача]
vector<char> vx, vy;            // Альтернирующее дерево vx[разраб], vy[задача]
vector<int> maxrow, mincol;     // Способности, изученность

bool dotry(int i)
{
    if (vx[i])
        return false;

    vx[i] = true;

    for (int j = 0; j < n; ++j)
        if (a[i][j].Weight() - maxrow[i] - mincol[j] == 0)
            vy[j] = true;

    for (int j = 0; j < n; ++j)
        if (a[i][j].Weight() - maxrow[i] - mincol[j] == 0 && yx[j] == -1)
        {
            if (i < TWEENS_COUNT)
            {
                int koe = j + 1;
                if (koe >= n)
                    koe = j - 1;

                xy[i + 1] = koe;
                yx[koe] = i + 1;
            }

            xy[i] = j;
            yx[j] = i;
            return true;
        }

    for (int j = 0; j < n; ++j)
        if (a[i][j].Weight() - maxrow[i] - mincol[j] == 0 && dotry(yx[j]))
        {
            if (i < TWEENS_COUNT)
            {
                int koe = j + 1;
                if (koe >= n)
                    koe = j - 1;

                xy[i + 1] = koe;
                yx[koe] = i + 1;
            }

            xy[i] = j;
            yx[j] = i;
            return true;
        }

    return false;
}

//Present* CreatePresent(int num, int weight)
//{
//    Present* tmp = new Present(num, weight);
//
//    return tmp;
//}

int main() {

    srand(time(NULL));

    n = PRESENTS_COUNT * PRESENTS_PER_TYPE;

    a = (Present**)malloc(n * sizeof(Present*));
    for (size_t i = 0; i < n; i++)
    {
        a[i] = (Present*)malloc(n * sizeof(Present));
    }

    int** arrChild = readMatrixFromFile("kids_wish.csv", CHILD_COUNT, CHILD_PRESENTS);
    int** arrPresent = readMatrixFromFile("ded_moroz_wish.csv", PRESENTS_COUNT, PRESENTS_CHILD);

    AddWeightChild(a, arrChild);
    AddWeightPresent(a, arrPresent);

    cout << "end fill" << endl;

    for (size_t i = 0; i < TWEENS_COUNT; i += 2)
    {
        for (size_t j = 0; j < n; j++)
        {
            int weight = a[i][j].Weight() + a[i + 1][j].Weight();
            if (weight > 0)
                weight += 1000;

            a[i][j].SetWeight(weight);
            a[i + 1][j].SetWeight(-10);
        }
    }

    mincol.assign(n, 0);
    maxrow.assign(n, 0);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            maxrow[i] = max(maxrow[i], a[i][j].Weight());

    // razr
    xy.assign(n, -1);
    // zad
    yx.assign(n, -1);

    for (int c = 0; c < CHILD_COUNT; ) {
        // razr
        vx.assign(n, 0);
        // zad
        vy.assign(n, 0);

        int k = 0;
        for (int i = 0; i < n; ++i)
        {
            if (xy[i] == -1 && dotry(i))
            {
                if (i < TWEENS_COUNT)
                {
                    k += 2;
                    i++;
                }
                else
                    k++;
            }
        }

        c += k;
        cout << c << endl;

        if (k == 0) {
            int z = INF;
            for (int i = 0; i < n; ++i)
            {
                if (i < TWEENS_COUNT)
                {
                    continue;
                }
                if (vx[i])
                {
                    for (int j = 0; j < n; ++j)
                    {
                        if (!vy[j])
                            z = min(z, maxrow[i] + mincol[j] - a[i][j].Weight());
                    }
                }
            }

            for (int i = 0; i < n; ++i) {
                if (i < TWEENS_COUNT)
                {
                    continue;
                }
                if (vx[i])
                    maxrow[i] -= z;
                if (vy[i])
                    mincol[i] += z;

            }
        }
    }

    for (size_t i = 0; i < TWEENS_COUNT; i += 2)
    {
        for (size_t j = 0; j < n; j++)
        {
            //a[i + 1][j].SetWeight(a[i][j])
            xy[i + 1] = xy[i];
        }
    }


    int ans = 0;
    for (int i = 0; i < n; ++i)
        ans += a[i][xy[i]].Weight();

    printf("%d\n", ans);

    for (int i = 0; i < n; ++i)
        cout << "r: " << i << " p: " << a[i][xy[i]].Id() << " w: " << a[i][xy[i]].Weight() << " n: " << xy[i] << endl;
}