#pragma once

#include "matrix.h"
#include "activation.h"
#include "weight_init.h"

struct Layer{
    int input_size;
    int output_size;

    Matrix weights;
    Matrix biases;

    Matrix input;
    Matrix z;
    Matrix output;

    Matrix d_weights;
    Matrix d_biases;
    Matrix d_inputs;

    ActivationType activation;
};
typedef struct Layer Layer;

Layer CreateLayer(int input_size, int output_size, ActivationType activation){
    Layer layer = {0};
    
    layer.input_size = input_size;
    layer.output_size = output_size;
    layer.activation = activation;

    layer.weights = CreateMatrix(output_size, input_size);
    layer.biases = CreateMatrix(output_size, 1);

    layer.input = CreateMatrix(input_size, 1);
    layer.z = CreateMatrix(output_size, 1);
    layer.output = CreateMatrix(output_size, 1);

    layer.d_weights = CreateMatrix(output_size, input_size);
    layer.d_biases = CreateMatrix(output_size, 1);
    layer.d_inputs = CreateMatrix(input_size, 1);

    for (int i = 0; i < output_size; i++){
        for (int j = 0; j < input_size; j++){
            layer.weights.data[ i * input_size + j] = rand_float();
        }
    }

    return layer;
}

void FreeLayer(Layer *layer){
    if (!layer){
        log_message(INFO, "Invalid layer");
        return;
    }
    
    FreeMatrix(&layer->weights);
    FreeMatrix(&layer->biases);
    
    FreeMatrix(&layer->input);
    FreeMatrix(&layer->z);
    FreeMatrix(&layer->output);
    
    FreeMatrix(&layer->d_weights);
    FreeMatrix(&layer->d_biases);
    FreeMatrix(&layer->d_inputs);
}

void ForwardLayer(Layer *layer, Matrix* input){
    if (!layer || !input){
        log_message(INFO, "Invalid layer or input matrix");
        return;
    }
    
    for (int i = 0; i < input->rows; i++){
        layer->input.data[i] = input->data[i];
    }
    
    // z = W * x + b
    MatrixMul(&layer->z, &layer->weights, input);
    AddMatrix(&layer->z, &layer->biases);

    for (int i = 0; i < layer->output_size; i++){
        layer->output.data[i] = ActivationForward(layer->activation, layer->z.data[i]);
    }
}

void BackwardLayer(Layer *layer, Matrix *gradient){
    if (!layer || !gradient){
        log_message(INFO, "Invalid layer or gradient matrix");
        return;
    }

    // dZ = gradient * activation'(Z)
    Matrix dz = CreateMatrix(layer->output_size, 1);

    for (int i = 0; i < layer->output_size; i++){
        dz.data[i] = gradient->data[i] * ActivationDerivative( layer->activation, layer->z.data[i]);
    }

    // dW = dZ * X^T
    for (int i = 0; i < layer->output_size; i++){
        layer->d_biases.data[i] = dz.data[i];

        for (int j = 0; j < layer->input_size; j++){
            layer->d_weights.data[ i * layer->input_size + j] = dz.data[i] * layer->input.data[j];
        }
    }

    // dX = W^T * dZ
    Matrix weight_transpose = CreateMatrix( layer->input_size, layer->output_size);

    TransposeMatrix(&weight_transpose, &layer->weights);
    MatrixMul( &layer->d_inputs, &weight_transpose, &dz);

    FreeMatrix(&weight_transpose);
    FreeMatrix(&dz);
}


void UpdateLayer(Layer *layer, float learning_rate){
    if (!layer){
        log_message(INFO, "Invalid layer");
        return;
    }

    for (int i = 0; i < layer->output_size; i++){
        layer->biases.data[i] -= learning_rate * layer->d_biases.data[i];
        for (int j = 0; j < layer->input_size; j++){
            int index = i * layer->input_size + j;
            layer->weights.data[index] -= learning_rate * layer->d_weights.data[index];
        }
    }
}