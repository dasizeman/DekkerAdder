#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <sched.h>

const int N = 1000000;

int count = 0;

void *increment(void*);

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
        pthread_create(&p, &myattr, increment, NULL);
        pthread_create(&q, &myattr, increment, NULL);

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

void *increment(void *t)
{
    int i;
    for (i = 0; i < N; i++)
    {
        int loc = count;
        loc++;
        count = loc;
    }
}
