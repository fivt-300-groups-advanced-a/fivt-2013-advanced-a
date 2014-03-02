#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define maxn (int)(1e8)
int a[maxn];
int perm[maxn];
int n, mode, shift;

void initArray()
{
    int j;
    for (j = 0; j < n; j++)
        a[j] = rand();
}

void run()
{
    int sum;
    clock_t startTime, finishTime;
/*    struct timeval start_t;*/
    startTime = clock();
    initArray();
    sum = 0;
    /*printf("startTime = %d\n", startTime);*/
    switch(mode)
    {
       
        case 0:
            {

                int pos, it;
                pos = 0;
                for (it = 0; it < n; it++)
                {
                    sum += a[pos];
                    pos += shift;
                    /*if (it < 10)
                        printf("pos = %d\n", pos);*/
                    if (pos >= n)
                        pos -= n;
                }
                break;
            }
        case 1:
            {
                int pos, it;
                for (it = 0; it < n; it++)
                {
                    pos = rand() % n;
                    sum += a[pos];     
                }
                break;                
            }
        default:
            {
                printf("wrong mode!\n");
                break;
            }
    }
    finishTime = clock();
 /*   printf("finishTime = %d\n", finishTime);*/
    /*printf("clocks_per_sec %d\n", CLOCKS_PER_SEC);*/
    /*printf("RAND_MAX = %d\n", RAND_MAX);*/
    printf("time = %d\n", (int)(finishTime - startTime));
}

int main(int argv, char ** argc)
{
    if (argv > 4 || argv < 3)
    {
        printf("usage <n> <mode> <shift>");
        exit(0);
    }
    n = atoi(argc[1]);
    if (n <= 0 || n > maxn)
    {
        printf("n should be in [1;%d]", maxn);
        exit(0);
    }
    printf("first argument n = %d\n", n);
    mode = atoi(argc[2]);
    if (mode < 0 || mode > 1)
    {
        printf("mode should be 0 or 1");
        exit(0);
    }
    printf("second argument mode = %d\n", mode);
    if (mode == 0)
    {
        if (argv == 3)
        {
             printf("usage <n> <mode> <shift>");
             exit(0);
        }
        shift = atoi(argc[3]);
        printf("third argument shift = %d\n", shift);
    }
    else if (argv == 4)
    {
        printf("usage <n> <mode> <shift>");
        exit(0);

    }
    run();
    return 0;
}
