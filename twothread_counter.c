#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <sched.h>


thread_attr_t myattr;
cpu_set_t cpuset;

pthread_attr_init(&myattr);
CPU_ZERO(&cpuset);
CPU_SET(0, &cpuset);
pthread_attr_setaffinity_np(&myattr, sizeof(cpu_set_t), &cpuset);
