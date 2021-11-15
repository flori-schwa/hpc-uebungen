#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define N (uint64_t) 8192l * (uint64_t) 8192l * (uint64_t) 8l

float* a;
float* b;
float* c;

void measure(char* label, void (*berechnung)(void), uint64_t n, uint32_t flop_pro_berechnung, uint32_t byte_pro_berechnung) {
    double mflop = n * flop_pro_berechnung / 1000000.0;
    double gb_mem = byte_pro_berechnung * n / 1000.0 / 1000.0 / 1000.0;
    double wtime = omp_get_wtime();
    (*berechnung)();
    wtime = omp_get_wtime() - wtime;
    double mflops = mflop / wtime;
    printf("%s:\n\t %.3f Sekunden\n", label, wtime);
    printf("\t%.2f MFLOP/s\n", mflops);
    printf("\t%.2f GByte/s\n", gb_mem / wtime);
}

void addition_seq() {
    for (int i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
    }
}

void addition_parallel() {
#pragma omp parallel for shared(a, b, c) default(none)
    for (int i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
    }
}

void veclen_seq() {
    for (int i = 0; i < N; ++i) {
        float ai = a[i];
        float bi = b[i];

        c[i] = sqrt(ai * ai + bi * bi);
    }
}

void veclen_parallel() {
#pragma omp parallel for shared(a, b, c) default(none)
    for (int i = 0; i < N; ++i) {
        float ai = a[i];
        float bi = b[i];

        c[i] = sqrt(ai * ai + bi * bi);
    }
}

void stencil_seq() {
    for (int i = 0; i < N - 8; ++i) {
        c[i] = a[i] * 0.9 + a[i + 1] * 0.8 + a[i + 2] * 0.7 + a[i + 3] * 0.6 + a[i + 4] * 0.5 + a[i + 5] * 0.4 + a[i + 6] * 0.3 + a[i + 7] * 0.2;
    }
}

void stencil_parallel() {
#pragma omp parallel for shared(a, c) default(none)
    for (int i = 0; i < N - 8; ++i) {
        c[i] = a[i] * 0.9 + a[i + 1] * 0.8 + a[i + 2] * 0.7 + a[i + 3] * 0.6 + a[i + 4] * 0.5 + a[i + 5] * 0.4 + a[i + 6] * 0.3 + a[i + 7] * 0.2;
    }
}

int main() {
    a = malloc(N * sizeof(float));
    b = malloc(N * sizeof(float));
    c = malloc(N * sizeof(float));

#pragma omp parallel for shared(a, b) default(none)
    for (int i = 0; i < N; i++) {
        a[i] = 1.0;
        b[i] = 2.0;
    }

    printf("##############\n");
    measure("addition_seq", &addition_seq, N, 1, 3 * sizeof(float));
    measure("addition_parallel", &addition_parallel, N, 1, 3 * sizeof(float));

    measure("veclen_seq", &veclen_seq, N, 4, 3 * sizeof(float));
    measure("veclen_parallel", &veclen_parallel, N, 4, 3 * sizeof(float));

    measure("stencil_seq", &stencil_seq, N - 8, 15, 9 * sizeof(float));
    measure("stencil_parallel", &stencil_parallel, N - 8, 15, 9 * sizeof(float));
}