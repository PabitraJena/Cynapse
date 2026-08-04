#include "cynapse/matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static int valid_index(const Matrix *m, size_t r, size_t c){
    return m && r < m->rows && c < m->cols;
}

Matrix mat_create(size_t rows, size_t cols){
    Matrix m;
    m.rows = rows;
    m.cols = cols;

    m.data = calloc(rows * cols, sizeof(float));
    return m;
}

Matrix mat_zeros(size_t rows, size_t cols){
    return mat_create(rows, cols);
}

Matrix mat_ones(size_t rows, size_t cols){
    Matrix m = mat_create(rows, cols);
    for (size_t i = 0; i < rows * cols; i++){
        m.data[i] = 1.0f;
    }
    return m;
}

Matrix mat_identity(size_t size){
    Matrix m = mat_create(size, size);

    for (size_t i = 0; i < size; i++){
        m.data[i * size + i] = 1.0f;
    }
    return m;
}

void mat_free(Matrix *m){
    if (m && m->data){
        free(m->data);

        m->data = NULL;
        m->rows = 0;
        m->cols = 0;
    }
}

Matrix mat_copy(const Matrix *m){
    Matrix res = mat_create(m->rows, m->cols);

    for (size_t i = 0; i < m->rows * m->cols; i++){
        res.data[i] = m->data[i];
    }
    return res;
}


float mat_get(const Matrix *m, size_t row, size_t col){
    if (!valid_index(m, row, col)){
        return 0.0f;
    }
    return m->data[row * m->cols + col];
}

void mat_set(Matrix *m, size_t row, size_t col, float value){
    if(!valid_index(m,row,col)){
        return;
    }

    m->data[row*m->cols + col] = value;
}

void mat_fill(Matrix *m, float value){
    if (!m){
        return;
    }
    for (size_t i = 0; i < m->rows * m->cols; i++){
        m->data[i] = value;
    }
}

void mat_print(const Matrix *m){
    if (!m){
        return;
    }
    for (size_t r = 0; r < m->rows; r++){
        for (size_t c = 0; c < m->cols; c++){
            printf ("%8.4f ", m->data[r * m->cols + c]);
        }
        printf("\n");
    }
}


Matrix mat_add(const Matrix *a, const Matrix *b){
    Matrix res = mat_create(a->rows, a->cols);
    if (a->rows != b->rows || a->cols != b->cols){
        return res;
    }
    for (size_t i = 0; i < a->rows * a->cols; i++){
        res.data[i] = a->data[i] + b->data[i];
    }
    return res;
}

Matrix mat_sub(const Matrix *a, const Matrix *b){
    Matrix res = mat_create(a->rows, a->cols);
    if (a->rows != b->rows || a->cols != b->cols){
        return res;
    }
    for (size_t i = 0; i < a->rows * a->cols; i++){
        res.data[i] = a->data[i] - b->data[i];
    }
    return res;
}

Matrix mat_scale(const Matrix *m, float scalar){
    Matrix res = mat_copy(m);
    for (size_t i = 0; i < m->rows * m->cols; i++){
        res.data[i] *= scalar;
    }
    return res;
}

Matrix mat_hadamard(const Matrix *a, const Matrix *b){
    Matrix res = mat_create(a->rows, a->cols);
    if (a->rows != b->rows || a->cols != b->cols){
        return res;
    }
    for (size_t i = 0; i < a->rows * a->cols; i++){
        res.data[i] = a->data[i] * b->data[i];
    }
    return res;
}

Matrix mat_mul(const Matrix *a, const Matrix *b){
    Matrix res;
    if (a->cols != b->rows){
        return mat_create(0,0);
    }
    res = mat_create(a->rows, b->cols);

    for (size_t i = 0; i < a->rows; i++){
        for (size_t j = 0; j < b->cols; j++){
            float sum = 0;
            for (size_t k = 0; k < a->cols; k++){
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            res.data[i * res.cols + j] = sum;
        }
    }
    return res;
}

Matrix mat_transpose(const Matrix *m){
    Matrix res = mat_create(m->cols,m->rows);
    for(size_t r = 0; r < m->rows; r++){
        for(size_t c = 0;c < m->cols; c++){
            res.data[c * res.cols + r] = m->data[r * m->cols + c];
        }
    }
    return res;
}

Matrix mat_random(size_t rows, size_t cols){
    Matrix m = mat_create(rows, cols);

    for (size_t i = 0; i < rows * cols; i++){
        m.data[i] = (float)rand() / RAND_MAX;
    }
    return m;
}

Matrix mat_random_normal(size_t rows, size_t cols){
    Matrix m = mat_create(rows, cols);
    for (size_t i = 0; i < rows * cols; i++){
        float a = ((float)rand() + 1.0f) / ((float)RAND_MAX + 1.0f);
        float b = ((float)rand() + 1.0f) / ((float)RAND_MAX + 1.0f);

        m.data[i] = sqrtf(-2.0f * logf(a)) * cosf(2.0f * M_PI * b);
    }
    return m;
}


float mat_sum(const Matrix *m){
    float sum = 0;

    for (size_t i = 0; i < m->rows * m->cols; i++){
        sum += m->data[i];
    }
    return sum;
}

float mat_mean(const Matrix *m){
    return mat_sum(m) / (float)(m->rows * m->cols);
}

float mat_min(const Matrix *m){
    float min = m->data[0];

    for (size_t i = 0; i < m->rows * m->cols; i++){
        if (m->data[i] < min){
            min = m->data[i];
        }
    }
    return min;
}

float mat_max(const Matrix *m){
    float max = m->data[0];

    for (size_t i = 0; i < m->rows * m->cols; i++){
        if (m->data[i] > max){
            max = m->data[i];
        }
    }
    return max;
}