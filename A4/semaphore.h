#include <pthread.h>

struct Semaphore {
   int value;
   pthread_mutex_t mutex;
   pthread_cond_t cond;
};

int wait(struct Semaphore*);

int signal(struct Semaphore*);
