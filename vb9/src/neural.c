/* neural.c - VB9 Neural Form Architecture Implementation */

#include "../include/vb9.h"
#include "../include/neural.h"
#include <math.h>

/* Activation Functions */

float
vb9_sigmoid(float x)
{
    return 1.0f / (1.0f + exp(-x));
}

float
vb9_relu(float x)
{
    return x > 0.0f ? x : 0.0f;
}

float
vb9_tanh_activation(float x)
{
    return tanh(x);
}

float
vb9_linear(float x)
{
    return x;
}

float
vb9_activate(float x, VB9ActivationFunc func)
{
    switch(func) {
    case VB9_ACTIVATION_SIGMOID:
        return vb9_sigmoid(x);
    case VB9_ACTIVATION_RELU:
        return vb9_relu(x);
    case VB9_ACTIVATION_TANH:
        return vb9_tanh_activation(x);
    case VB9_ACTIVATION_LINEAR:
        return vb9_linear(x);
    case VB9_ACTIVATION_SOFTMAX:
        /* TODO: Implement softmax for multi-class output */
        return vb9_sigmoid(x);
    default:
        return x;
    }
}

/* Neural control management */

VB9Neuron*
vb9_neuron_new(int control_prime, VB9ActivationFunc func)
{
    VB9Neuron *neuron;
    
    neuron = mallocz(sizeof(VB9Neuron), 1);
    if(neuron == nil)
        return nil;
    
    neuron->control_prime = control_prime;
    neuron->activation_state = 0.0f;
    neuron->func = func;
    neuron->inputs = nil;
    neuron->weights = nil;
    neuron->ninputs = 0;
    neuron->maxinputs = 8;  /* Initial capacity */
    neuron->control = nil;
    neuron->name = nil;
    
    print("NEURAL CREATION: Neuron born with prime %d\n", control_prime);
    print("  Activation function: %s\n", 
          func == VB9_ACTIVATION_SIGMOID ? "sigmoid" :
          func == VB9_ACTIVATION_RELU ? "relu" :
          func == VB9_ACTIVATION_TANH ? "tanh" :
          func == VB9_ACTIVATION_LINEAR ? "linear" : "unknown");
    
    return neuron;
}

void
vb9_neuron_destroy(VB9Neuron *neuron)
{
    if(neuron == nil)
        return;
    
    if(neuron->inputs)
        free(neuron->inputs);
    if(neuron->weights)
        free(neuron->weights);
    if(neuron->name)
        free(neuron->name);
    
    free(neuron);
}

void
vb9_connect_neurons(VB9Neuron *from, VB9Neuron *to, float weight)
{
    if(from == nil || to == nil)
        return;
    
    /* Resize input arrays if needed */
    if(to->ninputs >= to->maxinputs) {
        to->maxinputs *= 2;
        to->inputs = realloc(to->inputs, to->maxinputs * sizeof(VB9Neuron*));
        to->weights = realloc(to->weights, to->maxinputs * sizeof(float));
        if(to->inputs == nil || to->weights == nil)
            sysfatal("vb9_connect_neurons: realloc failed");
    }
    
    /* Add connection */
    to->inputs[to->ninputs] = from;
    to->weights[to->ninputs] = weight;
    to->ninputs++;
    
    print("NEURAL CONNECTION: Prime %d -> Prime %d (weight: %.2f)\n", 
          from->control_prime, to->control_prime, weight);
    print("  Mathematical bond created between controls!\n");
}

void
vb9_disconnect_neurons(VB9Neuron *from, VB9Neuron *to)
{
    int i, j;
    
    if(from == nil || to == nil)
        return;
    
    /* Find and remove connection */
    for(i = 0; i < to->ninputs; i++) {
        if(to->inputs[i] == from) {
            /* Shift remaining connections */
            for(j = i; j < to->ninputs - 1; j++) {
                to->inputs[j] = to->inputs[j + 1];
                to->weights[j] = to->weights[j + 1];
            }
            to->ninputs--;
            print("NEURAL DISCONNECTION: Prime %d -/-> Prime %d\n", 
                  from->control_prime, to->control_prime);
            break;
        }
    }
}

float
vb9_neuron_compute(VB9Neuron *neuron)
{
    float sum = 0.0f;
    int i;
    
    if(neuron == nil)
        return 0.0f;
    
    /* Weighted sum of inputs */
    for(i = 0; i < neuron->ninputs; i++) {
        sum += neuron->inputs[i]->activation_state * neuron->weights[i];
    }
    
    /* Apply activation function */
    neuron->activation_state = vb9_activate(sum, neuron->func);
    
    print("NEURAL COMPUTE: Prime %d computed %.3f from %d inputs\n", 
          neuron->control_prime, neuron->activation_state, neuron->ninputs);
    
    return neuron->activation_state;
}

void
vb9_neuron_set_input(VB9Neuron *neuron, float value)
{
    if(neuron == nil)
        return;
    
    neuron->activation_state = value;
    print("NEURAL INPUT: Prime %d set to %.3f\n", neuron->control_prime, value);
}

/* Neural layer management */

VB9Layer*
vb9_layer_new(VB9NeuralForm *form, char *name)
{
    VB9Layer *layer;
    
    if(form == nil || name == nil)
        return nil;
    
    layer = mallocz(sizeof(VB9Layer), 1);
    if(layer == nil)
        return nil;
    
    layer->name = strdup(name);
    layer->neurons = nil;
    layer->nneurons = 0;
    layer->maxneurons = 16;  /* Initial capacity */
    layer->form = form;
    
    layer->neurons = mallocz(layer->maxneurons * sizeof(VB9Neuron*), 1);
    if(layer->neurons == nil) {
        free(layer);
        return nil;
    }
    
    print("NEURAL LAYER: Layer '%s' created for form '%s'\n", name, form->name);
    
    return layer;
}

void
vb9_layer_destroy(VB9Layer *layer)
{
    int i;
    
    if(layer == nil)
        return;
    
    /* Destroy all neurons in layer */
    for(i = 0; i < layer->nneurons; i++) {
        if(layer->neurons[i])
            vb9_neuron_destroy(layer->neurons[i]);
    }
    
    if(layer->neurons)
        free(layer->neurons);
    if(layer->name)
        free(layer->name);
    
    free(layer);
}

void
vb9_layer_add_neuron(VB9Layer *layer, VB9Neuron *neuron)
{
    if(layer == nil || neuron == nil)
        return;
    
    /* Resize if needed */
    if(layer->nneurons >= layer->maxneurons) {
        layer->maxneurons *= 2;
        layer->neurons = realloc(layer->neurons, 
                               layer->maxneurons * sizeof(VB9Neuron*));
        if(layer->neurons == nil)
            sysfatal("vb9_layer_add_neuron: realloc failed");
    }
    
    layer->neurons[layer->nneurons] = neuron;
    layer->nneurons++;
    
    print("NEURAL LAYER: Added prime %d to layer '%s' (%d neurons total)\n", 
          neuron->control_prime, layer->name, layer->nneurons);
}

void
vb9_layer_propagate(VB9Layer *layer)
{
    int i;
    
    if(layer == nil)
        return;
    
    print("NEURAL PROPAGATION: Forward pass through layer '%s'\n", layer->name);
    
    /* Compute all neurons in parallel (conceptually) */
    for(i = 0; i < layer->nneurons; i++) {
        vb9_neuron_compute(layer->neurons[i]);
    }
}

/* Neural form management */

VB9NeuralForm*
vb9_neural_form_new(char *name)
{
    VB9NeuralForm *neural_form;
    
    if(name == nil)
        return nil;
    
    neural_form = mallocz(sizeof(VB9NeuralForm), 1);
    if(neural_form == nil)
        return nil;
    
    neural_form->name = strdup(name);
    neural_form->form = vb9_form_new(name, name);  /* Create base form */
    neural_form->layers = nil;
    neural_form->nlayers = 0;
    neural_form->maxlayers = 8;  /* Typical neural networks have few layers */
    neural_form->learning_rate = 0.01f;  /* Default learning rate */
    neural_form->training_mode = 0;      /* Start in inference mode */
    
    neural_form->layers = mallocz(neural_form->maxlayers * sizeof(VB9Layer*), 1);
    if(neural_form->layers == nil) {
        vb9_form_destroy(neural_form->form);
        free(neural_form->name);
        free(neural_form);
        return nil;
    }
    
    print("NEURAL FORM: Created neural form '%s'\n", name);
    print("  Base form signature will grow as neurons are added\n");
    print("  Neural networks as visual forms - the future is here!\n");
    
    return neural_form;
}

void
vb9_neural_form_destroy(VB9NeuralForm *neural_form)
{
    int i;
    
    if(neural_form == nil)
        return;
    
    /* Destroy all layers */
    for(i = 0; i < neural_form->nlayers; i++) {
        if(neural_form->layers[i])
            vb9_layer_destroy(neural_form->layers[i]);
    }
    
    if(neural_form->layers)
        free(neural_form->layers);
    if(neural_form->form)
        vb9_form_destroy(neural_form->form);
    if(neural_form->name)
        free(neural_form->name);
    
    free(neural_form);
}

VB9Layer*
vb9_neural_form_add_layer(VB9NeuralForm *form, char *layer_name)
{
    VB9Layer *layer;
    
    if(form == nil || layer_name == nil)
        return nil;
    
    /* Resize if needed */
    if(form->nlayers >= form->maxlayers) {
        form->maxlayers *= 2;
        form->layers = realloc(form->layers, 
                              form->maxlayers * sizeof(VB9Layer*));
        if(form->layers == nil)
            sysfatal("vb9_neural_form_add_layer: realloc failed");
    }
    
    layer = vb9_layer_new(form, layer_name);
    if(layer == nil)
        return nil;
    
    form->layers[form->nlayers] = layer;
    form->nlayers++;
    
    print("NEURAL FORM: Added layer '%s' to form '%s' (%d layers total)\n", 
          layer_name, form->name, form->nlayers);
    
    return layer;
}

void
vb9_neural_form_propagate(VB9NeuralForm *neural_form)
{
    int i;
    uint64_t form_signature;
    
    if(neural_form == nil)
        return;
    
    print("NEURAL FORM PROPAGATION: Forward pass through entire network\n");
    print("  Form: %s\n", neural_form->name);
    print("  Layers: %d\n", neural_form->nlayers);
    
    /* Forward propagate through all layers */
    for(i = 0; i < neural_form->nlayers; i++) {
        print("  Layer %d: %s\n", i, neural_form->layers[i]->name);
        vb9_layer_propagate(neural_form->layers[i]);
    }
    
    /* Calculate form signature - now includes neural complexity */
    form_signature = vb9_form_signature(neural_form->form);
    print("  Neural form signature: %llu\n", form_signature);
    print("  The visual form IS the neural network!\n");
}

void
vb9_neural_form_train(VB9NeuralForm *neural_form, float *inputs, float *targets)
{
    /* TODO: Implement backpropagation training */
    /* For now, just a stub that demonstrates the concept */
    
    if(neural_form == nil || inputs == nil || targets == nil)
        return;
    
    print("NEURAL TRAINING: Training neural form '%s'\n", neural_form->name);
    print("  Learning rate: %.3f\n", neural_form->learning_rate);
    print("  Training mode: %s\n", neural_form->training_mode ? "ON" : "OFF");
    print("  Visual forms learning from experience!\n");
    
    /* Set training mode */
    neural_form->training_mode = 1;
    
    /* Forward pass with inputs */
    vb9_neural_form_propagate(neural_form);
    
    /* TODO: Implement backward pass and weight updates */
    print("  Training iteration complete (backprop not yet implemented)\n");
}

/* Control-neuron integration */

VB9Neuron*
vb9_control_to_neuron(Control *ctrl, VB9ActivationFunc func)
{
    VB9Neuron *neuron;
    
    if(ctrl == nil)
        return nil;
    
    neuron = vb9_neuron_new(ctrl->type, func);  /* Use control type as prime */
    if(neuron == nil)
        return nil;
    
    neuron->control = ctrl;
    neuron->name = strdup(ctrl->name);
    
    /* Initialize activation based on control state */
    if(ctrl->enabled && ctrl->visible) {
        neuron->activation_state = 1.0f;
    } else {
        neuron->activation_state = 0.0f;
    }
    
    print("NEURAL INTEGRATION: Control '%s' (prime %d) -> Neuron\n", 
          ctrl->name, ctrl->type);
    print("  Initial activation: %.3f\n", neuron->activation_state);
    
    return neuron;
}

void
vb9_neuron_update_control(VB9Neuron *neuron)
{
    Control *ctrl;
    
    if(neuron == nil || neuron->control == nil)
        return;
    
    ctrl = neuron->control;
    
    /* Update control state based on neuron activation */
    if(neuron->activation_state > 0.5f) {
        ctrl->enabled = 1;
        ctrl->visible = 1;
    } else {
        ctrl->enabled = 0;
        /* Keep visible but disabled for low activation */
    }
    
    print("NEURAL->CONTROL: Neuron %.3f -> Control '%s' (enabled: %d)\n", 
          neuron->activation_state, ctrl->name, ctrl->enabled);
}

void
vb9_control_update_neuron(Control *ctrl, VB9Neuron *neuron)
{
    if(ctrl == nil || neuron == nil)
        return;
    
    /* Update neuron activation based on control interaction */
    if(ctrl->enabled && ctrl->visible) {
        neuron->activation_state = 1.0f;
    } else {
        neuron->activation_state = 0.0f;
    }
    
    print("CONTROL->NEURAL: Control '%s' -> Neuron %.3f\n", 
          ctrl->name, neuron->activation_state);
}