#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX 10240
#define NUM_THREADS  10

int n1, n2;
char *s1, *s2;
FILE *fp;
int countArray[NUM_THREADS] = {0};

int readf(FILE *fp) {
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
    return 0;
}

int num_substring(int tid) {
    int local_count = 0;
    int chunk = n1 / NUM_THREADS;
    int start = tid * chunk;
    int end = (tid == NUM_THREADS - 1) ? n1 : start + chunk + n2 - 1;

    for (int i = start; i <= end - n2; i++) {
        int match = 1;
        for (int j = 0; j < n2; j++) {
            if (s1[i + j] != s2[j]) {
                match = 0;
                break;
            }
        }
        if (match) local_count++;
    }
    countArray[tid] = local_count;
    return local_count;
}

void *calSubStringThread(void *threadid) {
    long tid = (long)threadid;
    int num = num_substring(tid);
    printf("The number of substrings thread %ld find is: %d\n", tid + 1, num);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    int t, rc;
    int totalNum = 0;

    if (readf(fp) < 0) {
        printf("Failed to read strings.\n");
        return -1;
    }

    for (t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, calSubStringThread, (void *)(size_t)t);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
        totalNum += countArray[t];
    }

    printf("Total substrings is: %d\n", totalNum);
    free(s1);
    free(s2);
    return 0;
}
