// Using the semaphore implementation on Linux, write a program to implement a competition among threads. Each thread has an associated semaphore. A thread in the competition goes through iterations.
// An iteration starts by incrementing a local variable, then the thread chooses another thread other than itself at random and signals it.
// Then, it waits on its own semaphore for the next iteration. The local variable is initialized to 0. A thread completes the competition after its local variable reaches 10,000.
// Your program should run the competition and print the order of the threads finishing. The competition is to be run with 8 threads.
// Write your code in C, compile and run. Include a screen shot of your run as part of your program. Ensure that your program does not hang.

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdbool.h>

struct thread_info
{
    sem_t *sema_list; // pointer to start of list
    int id;
    int *is_done_list;
};

pthread_mutex_t done_lock;

void *thread_function(void *arg)
{

    struct thread_info *cur_info = ((struct thread_info *)arg);

    int cur_id = cur_info->id;

    int count = 0;
    // printf("!!!!!starting thread %d", cur_id);

    while (count < 10000)
    {
        // sema down on OUR semaphore
        sem_wait(&(cur_info->sema_list[cur_id]));
        count += 1;

        if (count == 10000)
        {
            pthread_mutex_lock(&done_lock);
            cur_info->is_done_list[cur_id] = 1;
            pthread_mutex_unlock(&done_lock);
        }

        // signal some one else
        // gen rand number
        int x; // 0-7
        int done = 0;
        pthread_mutex_lock(&done_lock);
        for (int i = 0; i < 8; i++)
        {
            if (i != cur_id && cur_info->is_done_list[i])
            {
                done++;
            }
        }
        if (done == 7)
        {
            sem_post(&(cur_info->sema_list[cur_id]));
        }
        else
        {
            do
            {
                x = rand() % 8;
            } while (x == cur_id || cur_info->is_done_list[x]);
            sem_post(&(cur_info->sema_list[x]));
        }
        pthread_mutex_unlock(&done_lock);
        // printf("cout ! %d ", count);
    }

    printf("we finished %d , with value %d \n", cur_id, count);
}

int main()
{

    sem_t semalist[8];

    pthread_t threads[8];
    struct thread_info all_thread_infos[8];
    int is_done_list[8];

    pthread_mutex_init(&done_lock, NULL);

    for (int i = 0; i < 8; i++)
    {
        sem_init(&semalist[i], 0, 1); // 0 means shared between threads, initial value of 1

        struct thread_info info_block = {.id = i, .sema_list = semalist, .is_done_list = is_done_list};
        all_thread_infos[i] = info_block;
        is_done_list[i] = 0;
    }

    for (int i = 0; i < 8; i++)
    {
        pthread_create(&threads[i], NULL, thread_function, (void *)&all_thread_infos[i]);
    }

    // printf("yo");
    for (int i = 0; i < 8; i++)
    {
        pthread_join(threads[i], NULL);
        // printf("joining %d", i);
    }

    return 0;
}