#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <sys/time.h>
#include <xmmintrin.h>
#include <x86intrin.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static inline void getRGB(uint8_t *im, int width, int height, int nchannels, int x, int y, int *r, int *g, int *b)
{

    unsigned char *offset = im + (x + width * y) * nchannels;
    *r = offset[0];
    *g = offset[1];
    *b = offset[2];
}

int main(int nargs, char **argv)
{
    int width, height, nchannels;
    struct timeval fin,ini;

    if (nargs < 2)
    {
        printf("Usage: %s <image1> [<image2> ...]\n", argv[0]);
    }
    // For each image
    // Bucle 0
    for (int file_i = 1; file_i < nargs; file_i++)
    {
        printf("[info] Processing %s\n", argv[file_i]);
        /****** Reading file ******/
        uint8_t *rgb_image = stbi_load(argv[file_i], &width, &height, &nchannels, 4);
        if (!rgb_image)
        {
            perror("Image could not be opened");
        }

        /****** Allocating memory ******/
        // - RGB2Grey
        uint8_t *grey_image = calloc(width, height);
        if (!grey_image)
        {
            perror("Could not allocate memory");
        }

        // - Filenames 
        for (int i = strlen(argv[file_i]) - 1; i >= 0; i--)
        {
            if (argv[file_i][i] == '.')
            {
                argv[file_i][i] = 0;
                break;
            }
        }

        char *grey_image_filename = 0;
        asprintf(&grey_image_filename, "%s_grey.jpg", argv[file_i]);
        if (!grey_image_filename)
        {
            perror("Could not allocate memory");
            exit(-1);
        }

        /****** Computations ******/
        printf("[info] %s: width=%d, height=%d, nchannels=%d\n", argv[file_i], width, height, nchannels);

        if (nchannels != 3 && nchannels != 4)
        {
            printf("[error] Num of channels=%d not supported. Only three (RGB), four (RGBA) are supported.\n", nchannels);
            continue;
        }

        gettimeofday(&ini,NULL);
        // RGB to grey scale
        int r[4], g[4], b[4];
        __m128d factor1 = {0.2989,0.2989,0.2989,0.2989};
        __m128d factor2 = {0.5870,0.5870,0.5870,0.5870};
        __m128d factor3 = {0.1140,0.1140,0.1140,0.1140};
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j+=4)
            {
                getRGB(rgb_image, width, height, 4, j, i, &r[0], &g[0], &b[0]);
                getRGB(rgb_image, width, height, 4, j, i, &r[1], &g[1], &b[1]);
                getRGB(rgb_image, width, height, 4, j, i, &r[2], &g[2], &b[2]);
                getRGB(rgb_image, width, height, 4, j, i, &r[3], &g[3], &b[3]);
                __m128s tmp_r = {r[0], r[1], r[2], r[3]};
                __m128s tmp_g = {g[0], g[1], g[2], g[3]};
                __m128s tmp_b = {b[0], b[1], b[2], b[3]};
                //grey_image[j * width + i] = (int)(0.2989 * r + 0.5870 * g + 0.1140 * b);
                __m128s mul1 = _mm128_mul_ps(factor1, tmp_r);
                __m128s mul2 = _mm128_mul_ps(factor2, tmp_g);
                __m128s mul3 = _mm128_mul_ps(factor3, tmp_b);

                __m128s sum1 = _mm128_add_ps(mul1, mul2);
                __m128s sumt = _mm128_add_ps(sum1, mul3);

                printf("%d, %d\n",i,j);
                _mm128_store_ps((float*)&grey_image[i * width + j], sumt);
                printf("d\n");
            }
        }

        stbi_write_jpg(grey_image_filename, width, height, 1, grey_image, 10);
        free(rgb_image);

        gettimeofday(&fin,NULL);

	printf("Tiempo: %f\n", ((fin.tv_sec*1000000+fin.tv_usec)-(ini.tv_sec*1000000+ini.tv_usec))*1.0/1000000.0);
        free(grey_image_filename);
    }
}
