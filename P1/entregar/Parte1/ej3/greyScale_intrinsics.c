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
        /*int r[4], g[4], b[4];
        __m128 factor1 = {0.2989,0.2989,0.2989,0.2989};
        __m128 factor2 = {0.5870,0.5870,0.5870,0.5870};
        __m128 factor3 = {0.1140,0.1140,0.1140,0.1140};
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j+=4)
            {
                getRGB(rgb_image, width, height, 4, j, i, &r[0], &g[0], &b[0]);
                getRGB(rgb_image, width, height, 4, j, i, &r[1], &g[1], &b[1]);
                getRGB(rgb_image, width, height, 4, j, i, &r[2], &g[2], &b[2]);
                getRGB(rgb_image, width, height, 4, j, i, &r[3], &g[3], &b[3]);
                __m128 tmp_r = {r[0], r[1], r[2], r[3]};
                __m128 tmp_g = {g[0], g[1], g[2], g[3]};
                __m128 tmp_b = {b[0], b[1], b[2], b[3]};
                //grey_image[j * width + i] = (int)(0.2989 * r + 0.5870 * g + 0.1140 * b);
                __m128 mul1 = _mm_mul_ps(factor1, tmp_r);
                __m128 mul2 = _mm_mul_ps(factor2, tmp_g);
                __m128 mul3 = _mm_mul_ps(factor3, tmp_b);

                __m128 sum1 = _mm_add_ps(mul1, mul2);
                __m128 sumt = _mm_add_ps(sum1, mul3);

                printf("%d, %d\n",i,j);
                _mm_store_ps((float*)&grey_image[i * width + j], sumt);
            }
        }
        */

        __m128i datal, datah;
        __m256i intl, inth;
        __m256 floatl, floath;
        __m256 coefficients = {0.2989,0.5870,0.1140,0.0,0.2989,0.5870,0.1140,0.0};
        __m256 greyl, greyh;
        __m256 partial_result, result;
        __m256 reordered;
        __m128i grey32;
        __m256i idx = {7, 3, 6, 2, 0, 0, 0, 0};
        __m128i mask = {3,1,2,0};
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j+=4)
            {
                //getRGB(rgb_image, width, height, 4, j, i, &r, &g, &b);
                uint8_t *image = rgb_image + (i * width + j)*4;
                datal = _mm_load_epi64((__m128i *)image);
                datah = _mm_load_epi64((__m128i *)image+8);
                intl = _mm256_cvtepu8_epi32(datal);
                inth = _mm256_cvtepu8_epi32(datah);
                floatl = _mm256_cvtepi32_ps(intl);
                floath = _mm256_cvtepi32_ps(inth);
                greyl = _mm256_mul_ps(floatl,coefficients);
                greyh = _mm256_mul_ps(floath,coefficients);
                partial_result = _mm256_hadd_ps(greyl,greyh);
                result = _mm256_hadd_ps(partial_result,partial_result);
                reordered = _mm256_permutevar8x32_ps(result,idx);
                grey32 = _mm_cvtps_epi32(_mm256_extractf128_ps(reordered,1));

                grey_image[i * width + j] = (int)_mm_shuffle_epi8(grey32,mask);
                //grey_image[i * width + j] = (int)(0.2989 * r + 0.5870 * g + 0.1140 * b);
            }
        }
        
        stbi_write_jpg(grey_image_filename, width, height, 1, grey_image, 10);
        free(rgb_image);

        gettimeofday(&fin,NULL);

	printf("Tiempo: %f\n", ((fin.tv_sec*1000000+fin.tv_usec)-(ini.tv_sec*1000000+ini.tv_usec))*1.0/1000000.0);
        free(grey_image_filename);
    }
}
