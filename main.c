#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX 10240
#define NUM_THREADS 10

int total = 0;
int n1, n2;
char *s1, *s2;
pthread_mutex_t lock;

// Thread argument structure
typedef struct {
    int thread_id;
    int start;
    int end;
} thread_arg_t;

void *thread_func(void *arg) {
    thread_arg_t *t_arg = (thread_arg_t *)arg;
    int local_total = 0;
    int i, j, k, count;

    for (i = t_arg->start; i <= t_arg->end - n2; i++) {
        count = 0;
        for (j = i, k = 0; k < n2; j++, k++) {
            if (*(s1 + j) != *(s2 + k)) {
                break;
            } else {
                count++;
            }
        }
        if (count == n2)
            local_total++;
    }

    pthread_mutex_lock(&lock);
    total += local_total;
    printf("The number of substrings thread %d find is %d\n", t_arg->thread_id + 1, local_total);

    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int readf() {
    FILE *fp;
    if ((fp = fopen("strings.txt", "r")) == NULL) {
        printf("ERROR: can't open strings.txt!\n");
        return 0;
    }

    s1 = (char *)malloc(sizeof(char) * MAX);
    s2 = (char *)malloc(sizeof(char) * MAX);
    if (s1 == NULL || s2 == NULL) {
        printf("ERROR: Out of memory!\n");
        return -1;
    }

    fgets(s1, MAX, fp);
    fgets(s2, MAX, fp);
    fclose(fp);

    n1 = strlen(s1);
    if (s1[n1 - 1] == '\n') s1[--n1] = '\0';
    n2 = strlen(s2);
    if (s2[n2 - 1] == '\n') s2[--n2] = '\0';

    if (n1 < n2) return -1;
    return 1;
}

int main() {
    pthread_t threads[NUM_THREADS];
    thread_arg_t args[NUM_THREADS];
    pthread_mutex_init(&lock, NULL);

    if (readf() < 0) {
        printf("Failed to read strings or invalid input.\n");
        return -1;
    }

    int chunk = n1 / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].thread_id = i;
        args[i].start = i * chunk;
        args[i].end = (i == NUM_THREADS - 1) ? n1 : (i + 1) * chunk + n2 - 1;
        pthread_create(&threads[i], NULL, thread_func, (void *)&args[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("The number of substrings is: %d\n", total);

    pthread_mutex_destroy(&lock);
    free(s1);
    free(s2);
    return 0;
}
