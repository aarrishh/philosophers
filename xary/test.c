#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define PHILO_COUNT 5

// Each philosopher will run this function
void *philosopher(void *arg) {
    int id = *(int *)arg;  // Get philosopher number
    free(arg);  // Free malloc'd memory

    while (1) {
        printf("Philosopher %d is thinking 🧠\n", id);
        usleep(1000000);  // Think for 1 sec

        printf("Philosopher %d is hungry 🍽️\n", id);

        printf("Philosopher %d is eating 🍝\n", id);
        usleep(1000000);  // Eat for 1 sec
    }
    return NULL;
}

int main() {
    pthread_t threads[PHILO_COUNT];

    for (int i = 0; i < PHILO_COUNT; i++) {
        int *id = malloc(sizeof(int));  // Each thread needs its own id copy
        *id = i + 1;

        if (pthread_create(&threads[i], NULL, philosopher, id) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // Wait for all threads (they actually never finish here)
    for (int i = 0; i < PHILO_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
