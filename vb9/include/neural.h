/* neural.h - VB9 Neural Form Architecture */

#ifndef VB9_NEURAL_H
#define VB9_NEURAL_H

#include "vb9.h"

/* Activation Functions */
typedef enum {
    VB9_ACTIVATION_SIGMOID = 1,
    VB9_ACTIVATION_RELU = 2,
    VB9_ACTIVATION_TANH = 3,
    VB9_ACTIVATION_LINEAR = 4,
    VB9_ACTIVATION_SOFTMAX = 5
} VB9ActivationFunc;

/* Neural Control Structure - Controls become neurons */
typedef struct VB9Neuron {
    int control_prime;        /* Prime ID (2, 3, 5, 7...) */
    float activation_state;   /* Current activation level (0.0-1.0) */
    VB9ActivationFunc func;   /* Activation function type */
    struct VB9Neuron **inputs; /* Connected input neurons */
    float *weights;           /* Connection weights */
    int ninputs;             /* Number of input connections */
    int maxinputs;           /* Maximum input connections */
    Control *control;        /* Associated VB9 control */
    char *name;              /* Neuron name */
} VB9Neuron;

/* Neural Layer - Groups of related neurons */
typedef struct VB9Layer {
    char *name;              /* Layer name (inputs, processing, display) */
    VB9Neuron **neurons;     /* Array of neurons in this layer */
    int nneurons;           /* Number of neurons */
    int maxneurons;         /* Maximum neurons in layer */
    struct VB9NeuralForm *form; /* Parent neural form */
} VB9Layer;

/* Neural Form - Entire form as neural network */
typedef struct VB9NeuralForm {
    char *name;              /* Neural form name */
    VB9Form *form;          /* Base VB9 form */
    VB9Layer **layers;      /* Neural layers */
    int nlayers;            /* Number of layers */
    int maxlayers;          /* Maximum layers */
    float learning_rate;     /* Network learning rate */
    int training_mode;       /* Training vs inference mode */
} VB9NeuralForm;

/* Function prototypes */

/* Activation functions */
float vb9_sigmoid(float x);
float vb9_relu(float x);
float vb9_tanh_activation(float x);
float vb9_linear(float x);
float vb9_activate(float x, VB9ActivationFunc func);

/* Neural control management */
VB9Neuron* vb9_neuron_new(int control_prime, VB9ActivationFunc func);
void vb9_neuron_destroy(VB9Neuron *neuron);
void vb9_connect_neurons(VB9Neuron *from, VB9Neuron *to, float weight);
void vb9_disconnect_neurons(VB9Neuron *from, VB9Neuron *to);
float vb9_neuron_compute(VB9Neuron *neuron);
void vb9_neuron_set_input(VB9Neuron *neuron, float value);

/* Neural layer management */
VB9Layer* vb9_layer_new(VB9NeuralForm *form, char *name);
void vb9_layer_destroy(VB9Layer *layer);
void vb9_layer_add_neuron(VB9Layer *layer, VB9Neuron *neuron);
void vb9_layer_propagate(VB9Layer *layer);

/* Neural form management */
VB9NeuralForm* vb9_neural_form_new(char *name);
void vb9_neural_form_destroy(VB9NeuralForm *neural_form);
VB9Layer* vb9_neural_form_add_layer(VB9NeuralForm *form, char *layer_name);
void vb9_neural_form_propagate(VB9NeuralForm *neural_form);
void vb9_neural_form_train(VB9NeuralForm *neural_form, float *inputs, float *targets);

/* Control-neuron integration */
VB9Neuron* vb9_control_to_neuron(Control *ctrl, VB9ActivationFunc func);
void vb9_neuron_update_control(VB9Neuron *neuron);
void vb9_control_update_neuron(Control *ctrl, VB9Neuron *neuron);

#endif /* VB9_NEURAL_H */