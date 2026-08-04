#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

struct Matrix{
    size_t rows;
    size_t cols;
    float *data;
};
typedef struct Matrix Matrix;

Matrix mat_create(size_t rows, size_t cols);
Matrix mat_zeros(size_t rows, size_t cols);
Matrix mat_ones(size_t rows, size_t cols);
Matrix mat_identity(size_t size);

void mat_free(Matrix *m);
Matrix mat_copy(const Matrix *m);


float mat_get(const Matrix *m, size_t row, size_t col);
void mat_set(Matrix *m, size_t row, size_t col, float value);

void mat_fill(Matrix *m, float value);
void mat_print(const Matrix *m);


Matrix mat_add(const Matrix *a, const Matrix *b);
Matrix mat_sub(const Matrix *a, const Matrix *b);
Matrix mat_scale(const Matrix *m, float scalar);
Matrix mat_hadamard(const Matrix *a, const Matrix *b);

Matrix mat_mul(const Matrix *a, const Matrix *b);
Matrix mat_transpose(const Matrix *m);

Matrix mat_random(size_t rows, size_t cols);
Matrix mat_random_normal(size_t rows, size_t cols);


float mat_sum(const Matrix *m);
float mat_mean(const Matrix *m);
float mat_min(const Matrix *m);
float mat_max(const Matrix *m);

#endif