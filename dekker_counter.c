#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <sched.h>

const int N = 1000000;

int count = 0;

int wantp = 0, wantq = 0;
int turn = 1;

void *p_func(void*);
void *q_func(void*);

int main()
{
    const int TRIALS = 10;

    pthread_attr_t myattr;
    cpu_set_t cpuset;

    pthread_attr_init(&myattr);
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    pthread_attr_setaffinity_np(&myattr, sizeof(cpu_set_t), &cpuset);

    //printf("Successfully set processor affinity!\n");

    pthread_t p, q;
    
    int i;
    int wrongs = 0;
    for (i = 0; i < TRIALS; i++)
    {
        printf("Trial %d\n", i);

        count = 0;
        wantp = 0;
        wantq = 0;
        turn = 1;

        pthread_create(&p, &myattr, p_func, NULL);
        pthread_create(&q, &myattr, q_func, NULL);

        pthread_join(p, NULL);
        pthread_join(q, NULL);

        if (count != 2*N)
        {
            printf("Invalid count: %d\n", count);
            wrongs++;
        }
    }

    printf("%d incorrect runs out of %d trials\n", wrongs, TRIALS);



    return 0;

}

void *p_func(void *t)
{
    int i;
    for (i = 0; i < N; i++)
    {
        wantp = 1;
        while (wantq)
        {
            if (turn == 2)
            {
                wantp = 0;

                // await turn == 1
                while (turn != 1)
                    pthread_yield();

                wantp = 1;
            }
        }

        // Critical section
        int loc = count;
        loc++;
        count = loc;
        // end critical section

        turn = 2;
        wantp = 0;
    }
}

void *q_func(void *t)
{
    int i;
    for (i = 0; i < N; i++)
    {
        wantq = 1;
        while (wantp)
        {
            if (turn == 1)
            {
                wantq = 0;

                // await turn == 2
                while (turn != 2)
                    pthread_yield();

                wantq = 1;
            }
        }

        // Critical section
        int loc = count;
        loc++;
        count = loc;
        // end critical section

        turn = 1;
        wantq = 0;
    }
}
