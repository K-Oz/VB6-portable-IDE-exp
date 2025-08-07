/* quantum.c - VB9 Quantum UI States Implementation */

#include "../include/vb9.h"
#include "../include/quantum.h"
#include <math.h>
#include <stdlib.h>

/* Random number generator for quantum measurements */
static unsigned int quantum_seed = 1;

static float
quantum_random(void)
{
    /* Simple linear congruential generator for quantum randomness */
    quantum_seed = quantum_seed * 1664525 + 1013904223;
    return (quantum_seed & 0x7FFFFFFF) / (float)0x7FFFFFFF;
}

/* Quantum amplitude operations */

VB9Amplitude
vb9_amplitude(VB9State state, float real, float imag)
{
    VB9Amplitude amp;
    
    amp.state = state;
    amp.real = real;
    amp.imag = imag;
    amp.probability = real * real + imag * imag;  /* |amplitude|^2 */
    
    return amp;
}

float
vb9_amplitude_probability(VB9Amplitude amp)
{
    return amp.real * amp.real + amp.imag * amp.imag;
}

void
vb9_normalize_amplitudes(VB9Amplitude *amplitudes, int nstates)
{
    float total_prob = 0.0f;
    float norm_factor;
    int i;
    
    if(amplitudes == nil || nstates <= 0)
        return;
    
    /* Calculate total probability */
    for(i = 0; i < nstates; i++) {
        total_prob += vb9_amplitude_probability(amplitudes[i]);
    }
    
    if(total_prob <= 0.0f)
        return;
    
    /* Normalize to ensure total probability = 1 */
    norm_factor = sqrt(1.0f / total_prob);
    for(i = 0; i < nstates; i++) {
        amplitudes[i].real *= norm_factor;
        amplitudes[i].imag *= norm_factor;
        amplitudes[i].probability = vb9_amplitude_probability(amplitudes[i]);
    }
    
    print("QUANTUM NORMALIZATION: %d states normalized (total prob: %.3f)\n", 
          nstates, total_prob);
}

/* Quantum control management */

VB9QuantumControl*
vb9_quantum_control_new(int control_prime)
{
    VB9QuantumControl *qctrl;
    
    qctrl = mallocz(sizeof(VB9QuantumControl), 1);
    if(qctrl == nil)
        return nil;
    
    qctrl->name = nil;
    qctrl->control_prime = control_prime;
    qctrl->amplitudes = nil;
    qctrl->nstates = 0;
    qctrl->maxstates = 8;  /* Initial capacity for states */
    qctrl->measured = 0;
    qctrl->measured_state = 0;
    qctrl->classical_control = nil;
    qctrl->entangled_with = nil;
    qctrl->nentangled = 0;
    qctrl->maxentangled = 4;  /* Initial entanglement capacity */
    
    qctrl->amplitudes = mallocz(qctrl->maxstates * sizeof(VB9Amplitude), 1);
    qctrl->entangled_with = mallocz(qctrl->maxentangled * sizeof(VB9QuantumControl*), 1);
    
    if(qctrl->amplitudes == nil || qctrl->entangled_with == nil) {
        if(qctrl->amplitudes) free(qctrl->amplitudes);
        if(qctrl->entangled_with) free(qctrl->entangled_with);
        free(qctrl);
        return nil;
    }
    
    print("QUANTUM CREATION: Quantum control born with prime %d\n", control_prime);
    print("  Exists in pure quantum superposition until measured\n");
    print("  Spooky action at a distance enabled!\n");
    
    return qctrl;
}

void
vb9_quantum_control_destroy(VB9QuantumControl *qctrl)
{
    int i;
    
    if(qctrl == nil)
        return;
    
    /* Disentangle from all controls */
    for(i = 0; i < qctrl->nentangled; i++) {
        vb9_quantum_disentangle(qctrl, qctrl->entangled_with[i]);
    }
    
    if(qctrl->name)
        free(qctrl->name);
    if(qctrl->amplitudes)
        free(qctrl->amplitudes);
    if(qctrl->entangled_with)
        free(qctrl->entangled_with);
    
    free(qctrl);
}

void
vb9_quantum_superposition(VB9QuantumControl *qctrl, VB9State states, 
                         float prob1, float prob2)
{
    VB9Amplitude amp1, amp2;
    
    if(qctrl == nil)
        return;
    
    /* Reset quantum state */
    qctrl->nstates = 0;
    qctrl->measured = 0;
    
    /* Create superposition of two states */
    amp1 = vb9_amplitude(states & VB9_STATE_CLICKED ? VB9_STATE_CLICKED : 
                        (states & VB9_STATE_ENABLED ? VB9_STATE_ENABLED : 
                         VB9_STATE_VISIBLE), sqrt(prob1), 0.0f);
    
    amp2 = vb9_amplitude(states & VB9_STATE_UNCLICKED ? VB9_STATE_UNCLICKED :
                        (states & VB9_STATE_DISABLED ? VB9_STATE_DISABLED :
                         VB9_STATE_HIDDEN), sqrt(prob2), 0.0f);
    
    /* Add amplitudes */
    qctrl->amplitudes[0] = amp1;
    qctrl->amplitudes[1] = amp2;
    qctrl->nstates = 2;
    
    /* Normalize */
    vb9_normalize_amplitudes(qctrl->amplitudes, qctrl->nstates);
    
    print("QUANTUM SUPERPOSITION: Control prime %d in superposition\n", 
          qctrl->control_prime);
    print("  State 1: probability %.3f\n", amp1.probability);
    print("  State 2: probability %.3f\n", amp2.probability);
    print("  Control exists in BOTH states simultaneously!\n");
    print("  Observer effect: measurement will collapse wavefunction\n");
}

VB9State
vb9_quantum_measure(VB9QuantumControl *qctrl)
{
    float random_val, cumulative_prob;
    int i;
    
    if(qctrl == nil || qctrl->nstates == 0)
        return 0;
    
    /* If already measured, return cached result */
    if(qctrl->measured)
        return qctrl->measured_state;
    
    /* Quantum measurement - collapse wavefunction */
    random_val = quantum_random();
    cumulative_prob = 0.0f;
    
    for(i = 0; i < qctrl->nstates; i++) {
        cumulative_prob += qctrl->amplitudes[i].probability;
        if(random_val <= cumulative_prob) {
            qctrl->measured_state = qctrl->amplitudes[i].state;
            qctrl->measured = 1;
            break;
        }
    }
    
    print("QUANTUM MEASUREMENT: Prime %d wavefunction collapsed!\n", 
          qctrl->control_prime);
    print("  Random value: %.3f\n", random_val);
    print("  Measured state: %d\n", qctrl->measured_state);
    print("  Superposition destroyed by observation!\n");
    
    /* Collapse entangled controls */
    vb9_quantum_collapse_entangled(qctrl, qctrl->measured_state);
    
    return qctrl->measured_state;
}

void
vb9_quantum_set_state(VB9QuantumControl *qctrl, VB9State state)
{
    VB9Amplitude amp;
    
    if(qctrl == nil)
        return;
    
    /* Set definite state (no superposition) */
    amp = vb9_amplitude(state, 1.0f, 0.0f);
    qctrl->amplitudes[0] = amp;
    qctrl->nstates = 1;
    qctrl->measured = 1;
    qctrl->measured_state = state;
    
    print("QUANTUM STATE SET: Prime %d set to definite state %d\n", 
          qctrl->control_prime, state);
    
    /* Update entangled controls */
    vb9_quantum_collapse_entangled(qctrl, state);
}

/* Quantum entanglement */

void
vb9_quantum_entangle(VB9QuantumControl *qctrl1, VB9QuantumControl *qctrl2)
{
    if(qctrl1 == nil || qctrl2 == nil)
        return;
    
    /* Resize entanglement arrays if needed */
    if(qctrl1->nentangled >= qctrl1->maxentangled) {
        qctrl1->maxentangled *= 2;
        qctrl1->entangled_with = realloc(qctrl1->entangled_with,
                                       qctrl1->maxentangled * sizeof(VB9QuantumControl*));
    }
    
    if(qctrl2->nentangled >= qctrl2->maxentangled) {
        qctrl2->maxentangled *= 2;
        qctrl2->entangled_with = realloc(qctrl2->entangled_with,
                                       qctrl2->maxentangled * sizeof(VB9QuantumControl*));
    }
    
    /* Create bidirectional entanglement */
    qctrl1->entangled_with[qctrl1->nentangled] = qctrl2;
    qctrl1->nentangled++;
    
    qctrl2->entangled_with[qctrl2->nentangled] = qctrl1;
    qctrl2->nentangled++;
    
    print("QUANTUM ENTANGLEMENT: Prime %d <===> Prime %d\n", 
          qctrl1->control_prime, qctrl2->control_prime);
    print("  Spooky action at a distance activated!\n");
    print("  Measuring one will instantly affect the other!\n");
    print("  Even across different machines on the network!\n");
}

void
vb9_quantum_disentangle(VB9QuantumControl *qctrl1, VB9QuantumControl *qctrl2)
{
    int i, j;
    
    if(qctrl1 == nil || qctrl2 == nil)
        return;
    
    /* Remove qctrl2 from qctrl1's entanglement list */
    for(i = 0; i < qctrl1->nentangled; i++) {
        if(qctrl1->entangled_with[i] == qctrl2) {
            for(j = i; j < qctrl1->nentangled - 1; j++) {
                qctrl1->entangled_with[j] = qctrl1->entangled_with[j + 1];
            }
            qctrl1->nentangled--;
            break;
        }
    }
    
    /* Remove qctrl1 from qctrl2's entanglement list */
    for(i = 0; i < qctrl2->nentangled; i++) {
        if(qctrl2->entangled_with[i] == qctrl1) {
            for(j = i; j < qctrl2->nentangled - 1; j++) {
                qctrl2->entangled_with[j] = qctrl2->entangled_with[j + 1];
            }
            qctrl2->nentangled--;
            break;
        }
    }
    
    print("QUANTUM DISENTANGLEMENT: Prime %d -/-/-> Prime %d\n", 
          qctrl1->control_prime, qctrl2->control_prime);
    print("  Quantum correlation broken\n");
}

void
vb9_quantum_collapse_entangled(VB9QuantumControl *qctrl, VB9State measured_state)
{
    VB9QuantumControl *entangled;
    VB9State opposite_state;
    int i;
    
    if(qctrl == nil)
        return;
    
    /* Determine opposite state for entangled controls */
    switch(measured_state) {
    case VB9_STATE_CLICKED:
        opposite_state = VB9_STATE_UNCLICKED;
        break;
    case VB9_STATE_UNCLICKED:
        opposite_state = VB9_STATE_CLICKED;
        break;
    case VB9_STATE_ENABLED:
        opposite_state = VB9_STATE_DISABLED;
        break;
    case VB9_STATE_DISABLED:
        opposite_state = VB9_STATE_ENABLED;
        break;
    case VB9_STATE_VISIBLE:
        opposite_state = VB9_STATE_HIDDEN;
        break;
    case VB9_STATE_HIDDEN:
        opposite_state = VB9_STATE_VISIBLE;
        break;
    default:
        opposite_state = measured_state;
        break;
    }
    
    /* Collapse all entangled controls */
    for(i = 0; i < qctrl->nentangled; i++) {
        entangled = qctrl->entangled_with[i];
        if(entangled != nil && !entangled->measured) {
            entangled->measured = 1;
            entangled->measured_state = opposite_state;
            entangled->nstates = 1;
            entangled->amplitudes[0] = vb9_amplitude(opposite_state, 1.0f, 0.0f);
            
            print("ENTANGLEMENT COLLAPSE: Prime %d -> state %d (instant!)\n", 
                  entangled->control_prime, opposite_state);
        }
    }
}

/* Quantum form management */

VB9QuantumForm*
vb9_quantum_form_new(char *name)
{
    VB9QuantumForm *qform;
    
    if(name == nil)
        return nil;
    
    qform = mallocz(sizeof(VB9QuantumForm), 1);
    if(qform == nil)
        return nil;
    
    qform->name = strdup(name);
    qform->classical_form = vb9_form_new(name, name);
    qform->qcontrols = nil;
    qform->nqcontrols = 0;
    qform->maxqcontrols = 16;
    qform->coherence_time = 1.0f;  /* 1 second coherence by default */
    qform->measurement_count = 0;
    
    qform->qcontrols = mallocz(qform->maxqcontrols * sizeof(VB9QuantumControl*), 1);
    if(qform->qcontrols == nil) {
        vb9_form_destroy(qform->classical_form);
        free(qform->name);
        free(qform);
        return nil;
    }
    
    print("QUANTUM FORM: Created quantum form '%s'\n", name);
    print("  Coherence time: %.1f seconds\n", qform->coherence_time);
    print("  Form exists in quantum superposition of all possible states!\n");
    
    return qform;
}

void
vb9_quantum_form_destroy(VB9QuantumForm *qform)
{
    int i;
    
    if(qform == nil)
        return;
    
    /* Destroy all quantum controls */
    for(i = 0; i < qform->nqcontrols; i++) {
        if(qform->qcontrols[i])
            vb9_quantum_control_destroy(qform->qcontrols[i]);
    }
    
    if(qform->qcontrols)
        free(qform->qcontrols);
    if(qform->classical_form)
        vb9_form_destroy(qform->classical_form);
    if(qform->name)
        free(qform->name);
    
    free(qform);
}

void
vb9_quantum_form_add_control(VB9QuantumForm *qform, VB9QuantumControl *qctrl)
{
    if(qform == nil || qctrl == nil)
        return;
    
    /* Resize if needed */
    if(qform->nqcontrols >= qform->maxqcontrols) {
        qform->maxqcontrols *= 2;
        qform->qcontrols = realloc(qform->qcontrols,
                                  qform->maxqcontrols * sizeof(VB9QuantumControl*));
        if(qform->qcontrols == nil)
            sysfatal("vb9_quantum_form_add_control: realloc failed");
    }
    
    qform->qcontrols[qform->nqcontrols] = qctrl;
    qform->nqcontrols++;
    
    print("QUANTUM FORM: Added quantum control prime %d to form '%s'\n", 
          qctrl->control_prime, qform->name);
}

void
vb9_quantum_form_evolve(VB9QuantumForm *qform, float dt)
{
    int i;
    
    if(qform == nil)
        return;
    
    print("QUANTUM EVOLUTION: Form '%s' evolving over time %.3f\n", 
          qform->name, dt);
    print("  Quantum states naturally evolve according to Schrödinger equation\n");
    
    /* Simple time evolution - rotate phases */
    for(i = 0; i < qform->nqcontrols; i++) {
        VB9QuantumControl *qctrl = qform->qcontrols[i];
        int j;
        
        if(qctrl->measured)
            continue;  /* Measured controls don't evolve */
        
        /* Rotate quantum phases */
        for(j = 0; j < qctrl->nstates; j++) {
            float phase = dt * qctrl->control_prime;  /* Phase proportional to prime */
            float old_real = qctrl->amplitudes[j].real;
            float old_imag = qctrl->amplitudes[j].imag;
            
            qctrl->amplitudes[j].real = old_real * cos(phase) - old_imag * sin(phase);
            qctrl->amplitudes[j].imag = old_real * sin(phase) + old_imag * cos(phase);
            qctrl->amplitudes[j].probability = vb9_amplitude_probability(qctrl->amplitudes[j]);
        }
    }
    
    print("  Quantum evolution complete - superposition states rotated\n");
}

void
vb9_quantum_form_decohere(VB9QuantumForm *qform)
{
    int i;
    
    if(qform == nil)
        return;
    
    print("QUANTUM DECOHERENCE: Form '%s' losing quantum coherence\n", qform->name);
    print("  Environmental interaction destroys superposition\n");
    
    /* Force measurement of all unmeasured controls */
    for(i = 0; i < qform->nqcontrols; i++) {
        VB9QuantumControl *qctrl = qform->qcontrols[i];
        if(!qctrl->measured) {
            vb9_quantum_measure(qctrl);
        }
    }
    
    print("  All quantum states collapsed to classical reality\n");
}

/* Integration with classical controls */

VB9QuantumControl*
vb9_control_to_quantum(Control *ctrl)
{
    VB9QuantumControl *qctrl;
    
    if(ctrl == nil)
        return nil;
    
    qctrl = vb9_quantum_control_new(ctrl->type);
    if(qctrl == nil)
        return nil;
    
    qctrl->name = strdup(ctrl->name);
    qctrl->classical_control = ctrl;
    
    /* Initialize with current control state */
    if(ctrl->enabled && ctrl->visible) {
        vb9_quantum_set_state(qctrl, VB9_STATE_ENABLED | VB9_STATE_VISIBLE);
    } else {
        vb9_quantum_set_state(qctrl, VB9_STATE_DISABLED | VB9_STATE_HIDDEN);
    }
    
    print("CLASSICAL->QUANTUM: Control '%s' promoted to quantum realm\n", ctrl->name);
    
    return qctrl;
}

void
vb9_quantum_update_classical(VB9QuantumControl *qctrl)
{
    Control *ctrl;
    VB9State state;
    
    if(qctrl == nil || qctrl->classical_control == nil)
        return;
    
    ctrl = qctrl->classical_control;
    
    /* Measure quantum state if not already measured */
    state = vb9_quantum_measure(qctrl);
    
    /* Update classical control based on quantum measurement */
    ctrl->enabled = (state & VB9_STATE_ENABLED) != 0;
    ctrl->visible = (state & VB9_STATE_VISIBLE) != 0;
    
    print("QUANTUM->CLASSICAL: Quantum state %d -> Classical control '%s'\n", 
          state, ctrl->name);
    print("  Quantum measurement collapsed into visual reality\n");
}