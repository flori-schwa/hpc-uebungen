#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define X 8192
#define Y 8192

void **malloc2d(int x, int y, size_t size)
{
    void **array = malloc(x * sizeof(void *) + x * y * size);
    for (int i = 0; i < x; i++)
    {
        size_t offset = x * sizeof(void *) + i * y * size;
        array[i] = (void *)((unsigned char *)array + offset);
    }
    return array;
}

int main() {
    float** a = (float**) malloc2d(X, Y, sizeof(float));
    float** b = (float**) malloc2d(X, Y, sizeof(float));

    int i, j;

    printf("sizeof(float) = %llu\n", sizeof(float));
    printf("Groesse der Matrix: %i x %i = %i Elemente\n", X, Y, X * Y);
    printf("Groesse einer Matrix: %llu kBytes\n", sizeof(float) * X * Y / 1024);
    printf("Groesse einer Reihe: %llu kBytes\n", sizeof(float) * X / 1024);

    for (i = 0; i < X; i++) {
        for (j = 0; j < Y; j++) {
            a[i][j] = 1.0f;
            b[i][j] = 2.0f;
        }
    }

    // Measure time
    struct timeval start, end;
    gettimeofday(&start, 0);

    // TODO: Optimize this loop
    for (i = 0; i < X; i++) {
        float* aRow = a[i];
        float* bRow = b[i];

        for (j = 0; j < Y; j++) {
            aRow[j] += bRow[j];
        }
    }

    gettimeofday(&end, 0);

    long sec = end.tv_sec - start.tv_sec;
    long usec = end.tv_usec - start.tv_usec;
    double secs = (sec + usec / 1000000.0);
    double flop = X * Y;

    printf("%.2f MFLOP\n%.2f sec\n", flop / 1000000.0, secs);
    printf("%.2f MFLOPs\n", flop / secs / 1000000.0);

    free(a);
    free(b);

    return 0;
}