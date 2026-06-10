#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

double GetTime(){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

void fibo(mpz_t *nums, long long n_fibbo);

int main(int argc, char *argv[])
{
    long long n = 100000;

    if (argc > 1)
        n = atoll(argv[1]);

    if (n < 1)
    {
        printf("Value must be greater than 0\n");
        return 1;
    }

    mpz_t *nums = malloc(n * sizeof(mpz_t));

    if (nums == NULL)
    {
        fprintf(stderr, "Error allocating memory\n");
        return 1;
    }

    for (long long i = 0; i < n; i++)
        mpz_init(nums[i]);

    double start = GetTime();
    fibo(nums, n);
    double end = GetTime();
    printf("Time taken: %f seconds\n", end - start);
    for (long long i = 0; i < n; i++)
    {
        gmp_printf("%Zd\n", nums[i]);
    }

    for (long long i = 0; i < n; i++)
        mpz_clear(nums[i]);

    free(nums);

    return 0;
}

void fibo(mpz_t *nums, long long n_fibbo)
{
    if (n_fibbo >= 1)
        mpz_set_ui(nums[0], 0);

    if (n_fibbo >= 2)
        mpz_set_ui(nums[1], 1);

    for (long long i = 2; i < n_fibbo; i++)
    {
        mpz_add(nums[i], nums[i - 1], nums[i - 2]);
    }

}
