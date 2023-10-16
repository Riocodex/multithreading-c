#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *threadFunction(void *p) {
    int thread_id = *((int *)p); // Cast the argument to an integer pointer
    for (int i = 0; i < 10; i++) {
        printf("Thread ID %d: i=%d\n", thread_id, i);
        usleep(1000);
    }
    return NULL;
}

typedef struct {
    int start;
    int end;
    int thread_id;
} ThreadArgs;

// Function to check if a number is triangular
int isTriangular(int n) {
    int sum = 0;
    int i = 1;
    while (sum < n) {
        sum += i;
        i++;
    }
    return sum == n;
}

// Function executed by each thread
void* findTriangular(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int start = args->start;
    int end = args->end;
    int thread_id = args->thread_id;

    char filename[20];
    sprintf(filename, "triangular_thread_%d.txt", thread_id);
    FILE* file = fopen(filename, "w");

    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    for (int i = start; i <= end; i++) {
        if (isTriangular(i)) {
            fprintf(file, "%d\n", i);
        }
    }

    fclose(file);
    free(args);
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {

    //Number 6 workshop
    // if (argc != 2) {
    //     printf("Usage: %s <num_threads>\n", argv[0]);
    //     return 1;
    // }

    // int num_threads = atoi(argv[1]); // Convert the command line argument to an integer

    // pthread_t *threads = malloc(num_threads * sizeof(pthread_t));

    // if (threads == NULL) {
    //     perror("Thread creation failed");
    //     return 1;
    // }

    // for (int i = 0; i < num_threads; i++) {
    //     int *thread_id = malloc(sizeof(int));
    //     if (thread_id == NULL) {
    //         perror("Thread ID allocation failed");
    //         return 1;
    //     }
    //     *thread_id = i;
    //     if (pthread_create(&threads[i], NULL, threadFunction, (void *)thread_id) != 0) {
    //         perror("Thread creation failed");
    //         return 1;
    //     }
    // }

    // for (int i = 0; i < num_threads; i++) {
    //     pthread_join(threads[i], NULL);
    //     free(threads[i]); // Free the memory allocated for thread_id
    // }

    // free(threads); // Free the array of thread handles


    //number 4 main workshop

// Struct to pass arguments to each thread


// Function to check if a number is triangular
    if (argc != 2) {
        printf("Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);

    if (num_threads <= 0) {
        printf("Number of threads should be a positive integer.\n");
        return 1;
    }

    pthread_t threads[num_threads];
    ThreadArgs* thread_args[num_threads];
    int range_per_thread = 1000000 / num_threads;

    for (int i = 0; i < num_threads; i++) {
        thread_args[i] = (ThreadArgs*)malloc(sizeof(ThreadArgs));
        thread_args[i]->start = i * range_per_thread;
        thread_args[i]->end = (i == num_threads - 1) ? 1000000 : (i + 1) * range_per_thread - 1;
        thread_args[i]->thread_id = i;

        if (pthread_create(&threads[i], NULL, findTriangular, thread_args[i]) != 0) {
            perror("Thread creation failed");
            return 1;
        }
    }

    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Thread join failed");
            return 1;
        }
    }

    printf("Triangular numbers found and saved in separate files.\n");

    return 0;
}
