#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <windows.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define IMAGE_PENCILS "tamanna-rumee-vaTsR-ghLog-unsplash.jpg"
#define IMAGE_CITY_LARGE "Large-Sample-Image-download-for-Testing"

#define TEST_IMAGE IMAGE_PENCILS

struct grayscale_args {
    unsigned char* src;
    int pixels;
    int channels;
    unsigned char* target;
};

void* grayscale_calc(void* v_args) {
    struct grayscale_args* args = (struct grayscale_args*) v_args;

    if (args->channels < 3) {
        printf("Image already grascale\n");
        return NULL;
    }

    for (int pixel = 0; pixel < args->pixels; pixel++) {
        int srcOffset = pixel * args->channels;
        float grayscale = 0.2126f * args->src[srcOffset] + 0.7152f * args->src[srcOffset + 1] + 0.0722f * args->src[srcOffset + 2];
        args->target[pixel] = (int) grayscale;

        // usleep(20);
    }

    return NULL;
}

int main()
{
    // Read color JPG into byte array "img"
    // Array contains "width" x "height" pixels each consisting of "channels" colors/bytes
    int width, height, channels;
    unsigned char *img = stbi_load(TEST_IMAGE, &width, &height, &channels, 3);
    if (img == NULL)
    {
        printf("Err: loading image\n");
        exit(1);
    }

    printf("w: %d ; h: %d ; c: %d\n", width, height, channels);
    
    // Allocate target array for grayscale image
    unsigned char *gray = malloc(width * height);

    struct timeval start;
    struct timeval end;

    gettimeofday(&start, 0);

#define MULTI

#ifdef SINGLE
    struct grayscale_args args = {
        .src = img,
        .channels = channels,
        .pixels = width * height,
        .target = gray
    };

    grayscale_calc(&args);
#elif defined(MULTI)
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    int amountProcessors = (int) sysinfo.dwNumberOfProcessors;

    struct grayscale_args* thread_args = malloc(sizeof(struct grayscale_args) * amountProcessors);
    pthread_t threads[amountProcessors];

    int threadPixels = (width * height) / amountProcessors;

    for (int i = 0; i < amountProcessors; i++) {
        thread_args[i].src = img + (i * threadPixels * channels);
        thread_args[i].channels = channels;
        thread_args[i].target = gray + (i * threadPixels);

        if (i == (amountProcessors - 1)) {
            thread_args[i].pixels = (width * height) - ((amountProcessors - 1) * threadPixels);
        } else {
            thread_args[i].pixels = threadPixels;
        }

        int err = pthread_create(&threads[i], NULL, grayscale_calc, &thread_args[i]);

        if (err) {
            printf("Error starting thread #%d: %d\n", (i + 1), err);
            exit(err);
        }
    }

    for (int i = 0; i < amountProcessors; i++) {
        int err = pthread_join(threads[i], NULL);

        if (err) {
            printf("Error in thread #%d: %d\n", (i + 1), err);
            exit(err);
        }
    }

    free(thread_args);
#endif

    gettimeofday(&end, 0);
    long lsec = end.tv_sec - start.tv_sec;
    long lusec = end.tv_usec - start.tv_usec;
    double sec = (lsec + lusec / 1000000.0);

    long flop = width * height * (3 + 2);
    double mflops = flop / 1000000.0 / sec;

    printf("%8.6f seconds\n", sec);
    printf("%8.2f MFLOP/s\n", mflops);

    stbi_write_jpg("grayscale.jpg", width, height, 1, gray, 95);

    free(img);
    free(gray);
}