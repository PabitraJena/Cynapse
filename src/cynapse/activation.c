#include <math.h>
#include "cynapse/activation.h"

float sigmoid(float x){
    return 1.0f / (1.0f + expf(-x));
}

Matrix mat_sigmoid(Matrix *m){
    Matrix out = mat_create(m->rows, m->cols);

    for (int i = 0; i < m->rows; i++){
        for (int j = 0; j < m->cols; j++){
            float x = mat_get(m, i, j);
            mat_set(&out, i, j, sigmoid(x));
        }
    }

    return out;
}
