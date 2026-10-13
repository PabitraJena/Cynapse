#pragma once

#include "layer.h"

struct NeuralNetwork{
    int num_layers;
    Layer *layers;
};
typedef struct NeuralNetwork NeuralNetwork;

NeuralNetwork CreateNetwork(int num_layers, int *layer_sizes, ActivationType *activations){
    NeuralNetwork nn = {0};

    nn.num_layers = num_layers;
    nn.layers = (Layer *)malloc(sizeof(Layer) * num_layers);
    if (!nn.layers) {
        log_message(INFO, "Invalid Layer in Net");
        nn.num_layers = 0;
        return nn;
    }

    for (int i = 0; i < num_layers; i++){
        nn.layers[i] = CreateLayer( layer_sizes[i], layer_sizes[i + 1], activations[i] );
    }
    return nn;
}

void FreeNetwork(NeuralNetwork *nn){
    if (!nn){
        log_message(INFO, "Invalid Net");
        return;
    }

    for (int i = 0; i < nn->num_layers; i++){
        FreeLayer(&nn->layers[i]);
    }

    free(nn->layers);

    nn->layers = NULL;
    nn->num_layers = 0;
}

void NetworkForward( NeuralNetwork *nn, Matrix *input){
    if (!nn || !input){
        log_message(INFO, "Invalid Net or input");
        return;
    }

    Matrix *current = input;
    for (int i = 0; i < nn->num_layers; i++){
        ForwardLayer( &nn->layers[i], current);
        current = &nn->layers[i].output;
    }
}

void NetworkBackward( NeuralNetwork *nn, Matrix *gradient){
    if (!nn || !gradient){
        log_message(INFO, "Invalid Net or gradient");
        return;
    }
    Matrix *current_gradient = gradient;

    for (int i = nn->num_layers - 1; i >= 0; i--){
        BackwardLayer( &nn->layers[i],current_gradient);
        current_gradient = &nn->layers[i].d_inputs;
    }
}

void NetworkUpdate( NeuralNetwork *nn, float learning_rate){
    if (!nn){
        log_message(INFO, "Invalid Net");
        return;
    }

    for (int i = 0; i < nn->num_layers; i++){
        UpdateLayer( &nn->layers[i], learning_rate);
    }
}