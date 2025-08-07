/* quantum.h - VB9 Quantum UI States */

#ifndef VB9_QUANTUM_H
#define VB9_QUANTUM_H

#include "vb9.h"

/* Quantum States - Controls can exist in superposition */
typedef enum {
    VB9_STATE_CLICKED = 1,
    VB9_STATE_UNCLICKED = 2,
    VB9_STATE_ENABLED = 4,
    VB9_STATE_DISABLED = 8,
    VB9_STATE_VISIBLE = 16,
    VB9_STATE_HIDDEN = 32,
    VB9_STATE_FOCUSED = 64,
    VB9_STATE_UNFOCUSED = 128
} VB9State;

/* Quantum Amplitude - Probability amplitude for each state */
typedef struct VB9Amplitude {
    VB9State state;         /* Which state this amplitude represents */
    float real;             /* Real part of complex amplitude */
    float imag;             /* Imaginary part of complex amplitude */
    float probability;      /* |amplitude|^2 - computed probability */
} VB9Amplitude;

/* Quantum Control - Control in superposition of states */
typedef struct VB9QuantumControl {
    char *name;                     /* Control name */
    int control_prime;              /* Prime number ID */
    VB9Amplitude *amplitudes;       /* Array of state amplitudes */
    int nstates;                   /* Number of states in superposition */
    int maxstates;                 /* Maximum states capacity */
    int measured;                   /* Has this control been measured? */
    VB9State measured_state;        /* Result of last measurement */
    Control *classical_control;     /* Associated classical control */
    struct VB9QuantumControl **entangled_with;  /* Entangled controls */
    int nentangled;                /* Number of entangled controls */
    int maxentangled;              /* Maximum entanglement capacity */
} VB9QuantumControl;

/* Quantum Form - Form with quantum controls */
typedef struct VB9QuantumForm {
    char *name;                     /* Form name */
    VB9Form *classical_form;        /* Classical base form */
    VB9QuantumControl **qcontrols;  /* Quantum controls */
    int nqcontrols;                /* Number of quantum controls */
    int maxqcontrols;              /* Maximum quantum controls */
    float coherence_time;           /* How long superposition lasts */
    int measurement_count;          /* Number of measurements performed */
} VB9QuantumForm;

/* Function prototypes */

/* Quantum amplitude operations */
VB9Amplitude vb9_amplitude(VB9State state, float real, float imag);
float vb9_amplitude_probability(VB9Amplitude amp);
void vb9_normalize_amplitudes(VB9Amplitude *amplitudes, int nstates);

/* Quantum control management */
VB9QuantumControl* vb9_quantum_control_new(int control_prime);
void vb9_quantum_control_destroy(VB9QuantumControl *qctrl);
void vb9_quantum_superposition(VB9QuantumControl *qctrl, VB9State states, 
                              float prob1, float prob2);
VB9State vb9_quantum_measure(VB9QuantumControl *qctrl);
void vb9_quantum_set_state(VB9QuantumControl *qctrl, VB9State state);

/* Quantum entanglement */
void vb9_quantum_entangle(VB9QuantumControl *qctrl1, VB9QuantumControl *qctrl2);
void vb9_quantum_disentangle(VB9QuantumControl *qctrl1, VB9QuantumControl *qctrl2);
void vb9_quantum_collapse_entangled(VB9QuantumControl *qctrl, VB9State measured_state);

/* Quantum form management */
VB9QuantumForm* vb9_quantum_form_new(char *name);
void vb9_quantum_form_destroy(VB9QuantumForm *qform);
void vb9_quantum_form_add_control(VB9QuantumForm *qform, VB9QuantumControl *qctrl);
void vb9_quantum_form_evolve(VB9QuantumForm *qform, float dt);
void vb9_quantum_form_decohere(VB9QuantumForm *qform);

/* Integration with classical controls */
VB9QuantumControl* vb9_control_to_quantum(Control *ctrl);
void vb9_quantum_update_classical(VB9QuantumControl *qctrl);

#endif /* VB9_QUANTUM_H */