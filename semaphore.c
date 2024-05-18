#include "semaphore.h"
#include <stdio.h>


int wait(struct Semaphore *s) {
    pthread_mutex_lock(&s->mutex);
    while (s->value <= 0) {
        pthread_cond_wait(&s->cond, &s->mutex);
        }
    s->value--;
    pthread_mutex_unlock(&s->mutex);
    return 0;
};

int signal(struct Semaphore *s) {
    pthread_mutex_lock(&s->mutex);
    s->value++;
    pthread_cond_signal(&s->cond);
    pthread_mutex_unlock(&s->mutex);
    return 0;
};