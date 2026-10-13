#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "logger.h"

struct Matrix{
    int rows;
    int cols;
    float *data;
};
typedef struct Matrix Matrix;

Matrix CreateMatrix(int rows, int cols){
    Matrix m = {0};

    if (rows <= 0 || cols <= 0){
        log_message(INFO, "Matrix dimensions are [0, 0]");
        return m;
    }

    m.rows = rows;
    m.cols = cols;
    m.data = (float *)calloc((size_t)rows * cols, sizeof(*m.data));
    
    if (m.data == NULL){
        log_message(INFO, "Failed to calloc");
        m.rows = 0;
        m.cols = 0;
    }
    return m;
}

void FreeMatrix(Matrix *m){
    if (m == NULL){
        log_message(INFO, "Matrix is invalid");
        return;
    }

    free(m->data);
    m->data = NULL;
    m->rows = 0;
    m->cols = 0;
}

void MatrixMul(Matrix *result, const Matrix *a, const Matrix *b){
    if (!result || !a || !b){
        log_message(INFO, "Invalid matrices passed to Multiply");
        return;
    }

    if (!a->data || !b->data || !result->data){
        log_message(INFO, "Invalid data in matrices");
        return;
    }

    if (a->cols != b->rows){
        log_message(INFO, "Incompatible dimensions of multiplication");
        return;
    }

    if (result->rows != a->rows || result->cols != b->cols){
        log_message(INFO, "Invalid result matrix dimensions");
        return;
    }

    for (int i = 0; i < a->rows; i++){
        for (int j = 0; j < b->cols; j++){
            float sum = 0.0f;

            for (int k = 0; k < a->cols; k++){
                sum += a->data[i * a->cols + k] *
                       b->data[k * b->cols + j];
            }

            result->data[i * result->cols + j] = sum;
        }
    }
}


void TransposeMatrix(Matrix *result, Matrix *m){
    
    for(int r = 0; r < m->rows; r++){
        for(int c = 0;c < m->cols; c++){
            result->data[c * result->cols + r] = m->data[r * m->cols + c];
        }
    }
}

void AddMatrix(Matrix *a, Matrix *b){
    if (!a || !b){
        log_message(INFO, "Invalid matrices passed to Multiply");
        return;
    }

    if (!a->data || !b->data){
        log_message(INFO, "Invalid data in matrices");
        return;
    }

    if (a->rows != b->rows || a->cols != b->cols){
        log_message(INFO, "Incompatiable dimensions of multiplication");

        return;
    }

    size_t count = (size_t)a->rows * a->cols;

    for (size_t i = 0; i < count; i++){
        a->data[i] += b->data[i];
    }
}



float MatrixGet(Matrix *m, int row, int col){
    if (!m || !m->data){
        log_message(INFO, "Invalid matrix or data");
        return 0.0f;
    }
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols){
        log_message(INFO, "Invalid matrix index");
        return 0.0f;
    }

    return m->data[(size_t)row * m->cols + col];
}

void MatrixSet(Matrix *m, int row, int col, float value){
    if (!m || !m->data){
        log_message(INFO, "Invalid matrix or data");
        return;
    }
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols){
        log_message(INFO, "Invalid matrix index");
        return;
    }

    m->data[(size_t)row * m->cols + col] = value;
}

int ArgMax(Matrix *m){
    int index = 0;
    for (int i = 1; i < m->rows; i++){
        if (m->data[i] > m->data[index]){
            index = i;
        }
    }

    return index;
}

