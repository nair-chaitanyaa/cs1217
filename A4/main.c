#include <stdio.h>
#include <stdlib.h>
#include "semaphore.h"
#include <pthread.h>
#include <unistd.h>
#include <syscall.h>

struct Stack {
    int TOP;  //index of topmost element
    int stack[5]; 
};

struct Stack *stack;  //shared stack
struct Semaphore full, empty;  //empty and full semaphores
pthread_mutex_t mutex;  //mutex to protect shared stack and semaphores
pthread_cond_t cond;  //condition variable to signal when the stack is full
int phase_complete = 0;  //flag to indicate that the leech thread is done

void pushStack(struct Stack *s, int value) {
    s->stack[s->TOP] = value;
    printf("Produced: %d\n", value);
    s->TOP++;
}

int popStack(struct Stack *s) {
    s->TOP--;
    int value = s->stack[s->TOP];
    printf("Consumed: %d\n", value);
    return value;
}

void *leech(void *arg) {
    //leech should consume all items from the stack in a FIFO order and should not block the producer or consumer.
    //one-one code
    for (int i = 0; i < 5; i++) {
        wait(&full);  //wait for the stack to be not full
        popStack(stack);  //pop an item from the stack
        signal(&empty);  //signal that the stack is not full
    }

    //signal that leech has completed the first phase
    pthread_mutex_lock(&mutex);
    phase_complete++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

   //burst code
    for (int burst = 0; burst < 5; burst++) {
        //consume all items in the burst
        for (int i = 0; i < 5; i++) {
            wait(&full); //wait for full
            popStack(stack); //pop the stack
        }
        signal(&empty); //signal empty
    }
    return NULL;
}

void *produce(void *arg) {
    //one-one code
    for (int i = 0; i < 5; i++) {
        signal(&full);  //signal that stack is not full
        pushStack(stack, i); //push onto stack
        wait(&empty);  //wait for stack to not be empty
    }

    //wait for one-one to finish
    pthread_mutex_lock(&mutex);
    while (phase_complete < 1) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    //burst codes
    for (int burst = 0; burst < 5; burst++) {
        for (int i = 0; i < 5; i++) {
            pushStack(stack, i);
            signal(&full);  //let consumer know stack is not empty
        }
        wait(&empty);  //wait for consumer to consume all items in burst
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->TOP = 0;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    printf("Main\n");
    pthread_create(&thread2, NULL, produce, NULL);
    pthread_create(&thread1, NULL, leech, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    free(stack);
    pthread_exit(NULL);
}

