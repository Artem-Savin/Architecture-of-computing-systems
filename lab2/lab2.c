#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define INT_MAX 2147483647

void matrix_vector_product_double(double* a, double* b, double* c, int m, int n)
{
    for (int i = 0; i < m; i++) {
        {
            c[i] = 0.0;
        }
        for (int j = 0; j < n; j++) {
            c[i] += a[i * n + j] * b[j];
        }
    }
}

void matrix_vector_product_float(float* a, float* b, float* c, int m, int n)
{
    for (int i = 0; i < m; i++) {
        {
            c[i] = 0.0;
        }
        for (int j = 0; j < n; j++) {
            c[i] += a[i * n + j] * b[j];
        }
    }
}

void matrix_vector_product_int(
        int* a, int* b, int* c, int m, int n) // m + (m*(n-1))
{
    for (int i = 0; i < m; i++) {
        {
            c[i] = 0.0;
        }
        for (int j = 0; j < n; j++) {
            c[i] += a[i * n + j] * b[j];
        }
    }
}

double wtime() // saod
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

double getrand(int min, int max) // [min;max)
{
    return (double)rand() / (RAND_MAX + 1.0) * (max - min) + min;
}

void init_matrix_d(double* a, double* b, int m, int n, int min, int max)
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            a[i * n + j] = getrand(min, max);
        }
    }
    for (int j = 0; j < n; j++) {
        b[j] = getrand(min, max);
    }
}

void init_matrix_f(float* a, float* b, int m, int n, int min, int max)
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            a[i * n + j] = getrand(min, max);
        }
    }
    for (int j = 0; j < n; j++) {
        b[j] = getrand(min, max);
    }
}

void init_matrix_i(int* a, int* b, int m, int n, int min, int max)
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            a[i * n + j] = getrand(min, max);
        }
    }
    for (int j = 0; j < n; j++) {
        b[j] = getrand(min, max);
    }
}

char* readln(FILE* stream)
{
    static char* str = NULL;
    static size_t i = 0;
    int ch = fgetc(stream);

    if ((ch == '\n') || (ch == EOF)) {
        str = malloc(i + 1);
        str[i] = 0;
    } else {
        i++;
        readln(stream);
        str[--i] = ch;
    }
    return str;
}

void output_result(
        FILE* out,
        char* optype,
        double i_time,
        int test_cnt,
        int m,
        int n,
        double max_time,
        double min_time)
{
    int insCount = m + (m * (n - 1));
    double avg_time = i_time / test_cnt;
    double performance = pow((1 / (insCount / avg_time)), -1);
    double abs_error, rel_error, delta_max, delta_min;
    delta_max = fabs(max_time - avg_time);
    delta_min = fabs(min_time - avg_time);
    if (delta_max >= delta_min) {
        abs_error = delta_max;
    } else {
        abs_error = delta_min;
    }
    rel_error = (abs_error / avg_time) * 100;

    // FILE *pipe_fp = popen("cat" , "w");
    // char* str;
    // size_t size = 100000;
    char* str2 = (char*)malloc(256);
    FILE* cpu_inf = fopen("/proc/cpuinfo", "r");
    fgets(str2, 255, cpu_inf);
    /* char* istr = strstr(str2,"model name");
    printf("%s\n", istr); */

    for (int i = 0; i < 4; i++) {
        str2 = readln(cpu_inf);
        if (i == 3) {
            str2 = str2 + 13;
        }
        /* printf("%s\n", str2);} */
    }

    // fprintf (out, "PModel - %s\n", pipe_fp);
    /* for(int i = 0; i < 5 ; i++){ */
    fprintf(out, "PModel - %s\n", str2);
    fprintf(out, "Task - multiplication of a matrix by a vector\n");
    fprintf(out, "OpType - %s\n", optype);
    fprintf(out, "Opt - O\n");
    fprintf(out, "InsCount - %d\n", insCount);
    fprintf(out, "Timer - gettimeofday\n");
    fprintf(out, "Time - %lf\n", i_time);
    fprintf(out, "LNum - %d\n", test_cnt);
    fprintf(out, "AvTime - %lf\n", avg_time);
    fprintf(out, "AbsError - %lf\n", abs_error);
    fprintf(out, "RelError - %lf %c\n", rel_error, 37);
    fprintf(out, "TaskPerf - %lf\n\n", performance);
}

int main()
{
    srand(time(NULL));
    FILE* out;
    out = fopen("result.csv", "w");
    int n, m, min, max, test_cnt;
    min = 0;
    max = 64;
    n = m = 5000;
    double *a, *b, *c, first_time, second_time, res_time, min_time, max_time,
            cur_time;
    a = (double*)malloc(sizeof(*a) * m * n);
    b = (double*)malloc(sizeof(*b) * n);
    c = (double*)malloc(sizeof(*c) * m);

    printf("Enter number of tests\n");
    scanf("%d", &test_cnt);
    if (test_cnt < 10) {
        return 0;
    }

    for (int i = 0; i < 3; i++) {
        min_time = INT_MAX;
        max_time = 0;
        if (i == 0) {
            res_time = 0;
            for (int j = 0; j < test_cnt; j++) {
                init_matrix_d(a, b, m, n, min, max);
                first_time = wtime();
                matrix_vector_product_double(a, b, c, m, n);
                second_time = wtime();
                cur_time = second_time - first_time;
                res_time += cur_time;
                if (cur_time > max_time)
                    max_time = cur_time;
                if (cur_time < min_time)
                    min_time = cur_time;
            }
            output_result(
                    out,
                    "double",
                    res_time,
                    test_cnt,
                    m,
                    n,
                    max_time,
                    min_time);
        }
        if (i == 1) {
            res_time = 0;
            for (int j = 0; j < test_cnt; j++) {
                init_matrix_f((float*)a, (float*)b, m, n, min, max);
                first_time = wtime();
                matrix_vector_product_float(
                        (float*)a, (float*)b, (float*)c, m, n);
                second_time = wtime();
                cur_time = second_time - first_time;
                res_time += cur_time;
                if (cur_time > max_time)
                    max_time = cur_time;
                if (cur_time < min_time)
                    min_time = cur_time;
            }
            output_result(
                    out, "float", res_time, test_cnt, m, n, max_time, min_time);
        }
        if (i == 2) {
            res_time = 0;
            for (int j = 0; j < test_cnt; j++) {
                init_matrix_i((int*)a, (int*)b, m, n, min, max);
                first_time = wtime();
                matrix_vector_product_int((int*)a, (int*)b, (int*)c, m, n);
                second_time = wtime();
                cur_time = second_time - first_time;
                res_time += cur_time;
                if (cur_time > max_time)
                    max_time = cur_time;
                if (cur_time < min_time)
                    min_time = cur_time;
            }
            output_result(
                    out,
                    "integer",
                    res_time,
                    test_cnt,
                    m,
                    n,
                    max_time,
                    min_time);
        }

        printf("%f\n", res_time);
    }
    free(a);
    free(b);
    free(c);
    return 0;
}