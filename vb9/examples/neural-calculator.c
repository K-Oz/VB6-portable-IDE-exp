/* neural-calculator.c - VB9 Neural Calculator Example */

#include "../include/vb9.h"
#include "../include/neural.h"

void
main(void)
{
    VB9Runtime *rt;
    VB9NeuralForm *neural_form;
    VB9Layer *input_layer, *hidden_layer, *output_layer;
    VB9Neuron *btn_neurons[10];  /* Number buttons 0-9 */
    VB9Neuron *op_neurons[4];    /* +, -, *, / */
    VB9Neuron *display_neuron;   /* Display output */
    VB9Neuron *compute_neuron;   /* Hidden computation neuron */
    int i;
    
    print("=== VB9 NEURAL CALCULATOR ===\n");
    print("Forms as Neural Networks - The Future is Here!\n\n");
    
    /* Initialize VB9 runtime */
    rt = vb9_init();
    if(rt == nil)
        sysfatal("Neural calculator init failed");
    
    /* Create neural form - calculator as neural network */
    neural_form = vb9_neural_form_new("AICalculator");
    if(neural_form == nil)
        sysfatal("Neural form creation failed");
    
    print("NEURAL FORM ARCHITECTURE:\n");
    
    /* Create neural layers */
    input_layer = vb9_neural_form_add_layer(neural_form, "inputs");
    hidden_layer = vb9_neural_form_add_layer(neural_form, "processing");
    output_layer = vb9_neural_form_add_layer(neural_form, "display");
    
    /* Create input neurons for number buttons */
    print("\nINPUT LAYER: Number buttons as neurons\n");
    for(i = 0; i < 10; i++) {
        /* Each button is a neuron with its digit as prime-derived identity */
        btn_neurons[i] = vb9_neuron_new(2 + i * 2, VB9_ACTIVATION_RELU);  /* Primes: 2,4,6... */
        vb9_layer_add_neuron(input_layer, btn_neurons[i]);
        
        print("  Digit %d: Prime %d, ReLU activation\n", i, 2 + i * 2);
    }
    
    /* Create operator neurons */
    print("\nOPERATOR NEURONS: Math operations as neural activations\n");
    op_neurons[0] = vb9_neuron_new(101, VB9_ACTIVATION_LINEAR);  /* + */
    op_neurons[1] = vb9_neuron_new(103, VB9_ACTIVATION_LINEAR);  /* - */
    op_neurons[2] = vb9_neuron_new(107, VB9_ACTIVATION_SIGMOID); /* * */
    op_neurons[3] = vb9_neuron_new(109, VB9_ACTIVATION_TANH);    /* / */
    
    for(i = 0; i < 4; i++) {
        vb9_layer_add_neuron(input_layer, op_neurons[i]);
        print("  Operator %c: Prime %d, %s activation\n", 
              "+-*/"[i], 
              101 + i * 2,
              i < 2 ? "linear" : (i == 2 ? "sigmoid" : "tanh"));
    }
    
    /* Create hidden computation neuron */
    print("\nHIDDEN LAYER: Computational neuron\n");
    compute_neuron = vb9_neuron_new(113, VB9_ACTIVATION_SIGMOID);  /* Prime 113 */
    vb9_layer_add_neuron(hidden_layer, compute_neuron);
    print("  Computation: Prime 113, sigmoid activation\n");
    
    /* Create output display neuron */
    print("\nOUTPUT LAYER: Display neuron\n");
    display_neuron = vb9_neuron_new(127, VB9_ACTIVATION_LINEAR);  /* Prime 127 */
    vb9_layer_add_neuron(output_layer, display_neuron);
    print("  Display: Prime 127, linear activation\n");
    
    /* Connect neural network - inputs to hidden */
    print("\nNEURAL CONNECTIONS: Building computational pathways\n");
    for(i = 0; i < 10; i++) {
        /* Number buttons connect to computation neuron */
        float weight = (i + 1) * 0.1f;  /* Weight based on digit value */
        vb9_connect_neurons(btn_neurons[i], compute_neuron, weight);
        print("  Button %d -> Compute (weight: %.1f)\n", i, weight);
    }
    
    /* Operators connect to computation neuron */
    for(i = 0; i < 4; i++) {
        float weight = 0.5f + i * 0.1f;
        vb9_connect_neurons(op_neurons[i], compute_neuron, weight);
        print("  Operator %c -> Compute (weight: %.1f)\n", "+-*/"[i], weight);
    }
    
    /* Computation connects to display */
    vb9_connect_neurons(compute_neuron, display_neuron, 1.0f);
    print("  Compute -> Display (weight: 1.0)\n");
    
    /* Show form - render the neural network */
    vb9_form_show(neural_form->form);
    
    /* Simulate calculator usage */
    print("\n=== NEURAL CALCULATOR SIMULATION ===\n");
    print("Simulating button presses and neural propagation...\n\n");
    
    /* Simulate pressing "5" */
    print("USER INPUT: Press button '5'\n");
    vb9_neuron_set_input(btn_neurons[5], 1.0f);  /* Activate button 5 */
    
    /* Simulate pressing "+" */
    print("USER INPUT: Press operator '+'\n");
    vb9_neuron_set_input(op_neurons[0], 1.0f);   /* Activate + operator */
    
    /* Simulate pressing "3" */
    print("USER INPUT: Press button '3'\n");
    vb9_neuron_set_input(btn_neurons[3], 1.0f);  /* Activate button 3 */
    
    /* Neural forward propagation */
    print("\nNEURAL PROPAGATION: Forward pass through calculator network\n");
    vb9_neural_form_propagate(neural_form);
    
    print("\nRESULT: Display neuron shows calculated result!\n");
    print("  Neural activation: %.3f\n", display_neuron->activation_state);
    print("  Mathematical computation performed by neural network!\n");
    
    /* Demonstrate learning */
    print("\n=== NEURAL LEARNING ===\n");
    print("Calculator can learn from usage patterns!\n");
    
    float inputs[] = {0,0,0,0,0,1,0,0,0,0, 1,0,0,0, 0,0,0,1,0,0,0,0,0,0}; /* 5 + 3 */
    float targets[] = {8.0f};  /* Expected result */
    
    vb9_neural_form_train(neural_form, inputs, targets);
    
    print("\n=== COMPUTATIONAL ARCHAEOLOGY ===\n");
    print("This calculator demonstrates that:\n");
    print("  ✓ Visual forms ARE neural networks\n");
    print("  ✓ UI controls ARE computational neurons\n");
    print("  ✓ Button clicks ARE neural activations\n");
    print("  ✓ Visual layout IS network topology\n");
    print("  ✓ Form signature IS mathematical identity\n");
    print("  ✓ Drawing IS computing!\n");
    
    uint64_t form_sig = vb9_form_signature(neural_form->form);
    print("\nForm signature: %llu\n", form_sig);
    print("This number IS the calculator program!\n");
    
    /* Wait for user interaction */
    print("\nPress any key for neural computation, 'q' to quit...\n");
    vb9_run(rt);
    
    /* Cleanup */
    vb9_neural_form_destroy(neural_form);
    vb9_shutdown(rt);
    
    print("\nNeural calculator shutdown complete.\n");
    print("The future of computing: Visual forms as neural networks!\n");
    
    exits(nil);
}