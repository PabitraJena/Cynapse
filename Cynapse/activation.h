#pragma once

#include <math.h>

enum ActivationType{
    ACTIVATION_NONE,
    ACTIVATION_SIGMOID,
    ACTIVATION_RELU,
    ACTIVATION_SOFTMAX
};
typedef enum ActivationType ActivationType;

float ActivationForward(ActivationType type, float x){
    switch (type) {
        case ACTIVATION_SIGMOID:
            return 1.0f / (1.0f + expf(-x));

        case ACTIVATION_RELU:
            return x > 0.0f ? x : 0.0f;

        case ACTIVATION_NONE:
        default:
            return x;
    }
}

float ActivationDerivative(ActivationType type, float x){
    switch (type) {
        case ACTIVATION_SIGMOID: {
            float y = ActivationForward(type, x);
            return y * (1.0f - y);
        }

        case ACTIVATION_RELU:
            return x > 0.0f ? 1.0f : 0.0f;

        case ACTIVATION_NONE:
        default:
            return 1.0f;
    }
}

void Softmax(const Matrix *input, Matrix *output){
    if (!input || !output || !input->data || !output->data){
        return;
    }

    if (input->rows != output->rows || input->cols != output->cols){
        return;
    }

    float max_value = input->data[0];
    for (int i = 1; i < input->rows; i++){
        if (input->data[i] > max_value) {
            max_value = input->data[i];
        }
    }

    float sum = 0.0f;
    for (int i = 0; i < input->rows; i++){
        output->data[i] = expf(input->data[i] - max_value);
        sum += output->data[i];
    }

    for (int i = 0; i < input->rows; i++) {
        output->data[i] /= sum;
    }
}
