#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <sys/time.h>

#define CHUNK_WIDTH 4

#define N (CHUNK_WIDTH * CHUNK_WIDTH)

void mem_to_thread(int thread[N][N]) {
#pragma omp parallel for shared(thread) default(none)
    for (int chunk = 0; chunk < N; ++chunk) {
        int chunk_x = (chunk % CHUNK_WIDTH) * CHUNK_WIDTH;
        int chunk_y = (chunk / CHUNK_WIDTH) * CHUNK_WIDTH;

        for (int i = 0; i < N; ++i) {
            thread[chunk_y + (i / CHUNK_WIDTH)][chunk_x + (i % CHUNK_WIDTH)] = omp_get_thread_num();
        }
    }
}

void print_thread_nums(int thread[N][N]) {
    int x, y;
    printf("    |");
    for (x = 0; x < N; x++)
        printf("| %2i ", x);
    printf("|\n");
    printf("------");
    for (x = 0; x < N; x++)
        printf("-----");
    printf("-\n");
    for (y = 0; y < N; y++) {
        printf(" %2i |", y);
        for (x = 0; x < N; x++)
            printf("| %2i ", thread[y][x]);
        printf("|\n");
    }
}

int main() {
    int x, y;
    int thread[N][N];

    omp_set_num_threads(N);

    struct timeval start;
    struct timeval end;

    gettimeofday(&start, 0);

    mem_to_thread(thread);

    gettimeofday(&end, 0);
    long lsec = end.tv_sec - start.tv_sec;
    long lusec = end.tv_usec - start.tv_usec;
    double sec = (lsec + lusec / 1000000.0);

    printf("%f seconds\n", sec);

    print_thread_nums(thread);
}
