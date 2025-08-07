/* quantum-buttons.c - VB9 Quantum Button Example */

#include "../include/vb9.h"
#include "../include/quantum.h"

void
main(void)
{
    VB9Runtime *rt;
    VB9QuantumForm *qform;
    VB9QuantumControl *qbutton1, *qbutton2, *qbutton3;
    VB9State measured_state;
    int i;
    
    print("=== VB9 QUANTUM BUTTONS ===\n");
    print("Controls in Superposition - Quantum Computing Meets UI!\n\n");
    
    /* Initialize VB9 runtime */
    rt = vb9_init();
    if(rt == nil)
        sysfatal("Quantum button init failed");
    
    /* Create quantum form */
    qform = vb9_quantum_form_new("QuantumInterface");
    if(qform == nil)
        sysfatal("Quantum form creation failed");
    
    print("QUANTUM FORM: '%s' created\n", qform->name);
    print("  Coherence time: %.1f seconds\n", qform->coherence_time);
    print("  Form exists in superposition of all possible states!\n\n");
    
    /* Create quantum buttons */
    print("QUANTUM CONTROLS: Creating quantum buttons\n");
    
    qbutton1 = vb9_quantum_control_new(VB9_BUTTON);  /* Prime 2 */
    qbutton1->name = strdup("QuantumButton1");
    vb9_quantum_form_add_control(qform, qbutton1);
    
    qbutton2 = vb9_quantum_control_new(VB9_BUTTON);  /* Prime 2 */
    qbutton2->name = strdup("QuantumButton2");
    vb9_quantum_form_add_control(qform, qbutton2);
    
    qbutton3 = vb9_quantum_control_new(VB9_BUTTON);  /* Prime 2 */
    qbutton3->name = strdup("QuantumButton3");
    vb9_quantum_form_add_control(qform, qbutton3);
    
    print("  Created %d quantum buttons\n", qform->nqcontrols);
    print("  Each button exists in quantum superposition!\n\n");
    
    /* Put buttons in superposition */
    print("=== QUANTUM SUPERPOSITION ===\n");
    
    print("Button1: Superposition of CLICKED and UNCLICKED\n");
    vb9_quantum_superposition(qbutton1, 
                             VB9_STATE_CLICKED | VB9_STATE_UNCLICKED, 
                             0.7, 0.3);  /* 70% clicked, 30% unclicked */
    
    print("Button2: Superposition of ENABLED and DISABLED\n");
    vb9_quantum_superposition(qbutton2, 
                             VB9_STATE_ENABLED | VB9_STATE_DISABLED, 
                             0.5, 0.5);  /* 50% enabled, 50% disabled */
    
    print("Button3: Superposition of VISIBLE and HIDDEN\n");
    vb9_quantum_superposition(qbutton3, 
                             VB9_STATE_VISIBLE | VB9_STATE_HIDDEN, 
                             0.8, 0.2);  /* 80% visible, 20% hidden */
    
    print("\nAll buttons now exist in MULTIPLE STATES SIMULTANEOUSLY!\n");
    print("Quantum mechanics applied to user interfaces!\n\n");
    
    /* Quantum entanglement */
    print("=== QUANTUM ENTANGLEMENT ===\n");
    print("Entangling Button1 and Button2...\n");
    vb9_quantum_entangle(qbutton1, qbutton2);
    
    print("Entangling Button2 and Button3...\n");
    vb9_quantum_entangle(qbutton2, qbutton3);
    
    print("Quantum entanglement established!\n");
    print("  Button1 <===> Button2 <===> Button3\n");
    print("  Measuring one will instantly affect the others!\n");
    print("  Spooky action at a distance in user interfaces!\n\n");
    
    /* Show form - render quantum interface */
    vb9_form_show(qform->classical_form);
    
    /* Quantum evolution simulation */
    print("=== QUANTUM EVOLUTION ===\n");
    print("Letting quantum states evolve over time...\n");
    
    for(i = 0; i < 5; i++) {
        float dt = 0.1f;  /* 0.1 second time step */
        print("Time step %d: dt = %.1f seconds\n", i + 1, dt);
        vb9_quantum_form_evolve(qform, dt);
    }
    
    print("Quantum evolution complete!\n");
    print("States have rotated in complex quantum phase space\n\n");
    
    /* Quantum measurements */
    print("=== QUANTUM MEASUREMENTS ===\n");
    print("WARNING: Measurement will collapse quantum superposition!\n");
    print("Observer effect will destroy quantum nature!\n\n");
    
    print("Measuring Button1...\n");
    measured_state = vb9_quantum_measure(qbutton1);
    print("  Button1 collapsed to state: %d\n", measured_state);
    print("  Quantum superposition DESTROYED by observation!\n");
    
    print("\nDue to entanglement, other buttons collapsed too:\n");
    if(qbutton2->measured) {
        print("  Button2 entangled collapse to state: %d\n", qbutton2->measured_state);
    }
    if(qbutton3->measured) {
        print("  Button3 entangled collapse to state: %d\n", qbutton3->measured_state);
    }
    
    print("\nSPOOKY ACTION AT A DISTANCE: Instantaneous correlation!\n");
    print("Einstein called this 'spukhafte Fernwirkung'\n");
    print("Now available in your UI toolkit!\n\n");
    
    /* Demonstrate quantum reset */
    print("=== QUANTUM RESET ===\n");
    print("Creating new superposition after measurement...\n");
    
    vb9_quantum_superposition(qbutton1, 
                             VB9_STATE_VISIBLE | VB9_STATE_HIDDEN,
                             0.6, 0.4);
    
    print("Button1 back in superposition (60% visible, 40% hidden)\n");
    print("Quantum nature restored!\n\n");
    
    /* Decoherence simulation */
    print("=== QUANTUM DECOHERENCE ===\n");
    print("Environmental interaction destroys quantum coherence...\n");
    vb9_quantum_form_decohere(qform);
    print("All quantum states collapsed to classical reality\n");
    print("Quantum computing reduced to classical computing\n\n");
    
    /* Computational archaeology */
    print("=== COMPUTATIONAL ARCHAEOLOGY ===\n");
    print("This quantum interface demonstrates:\n");
    print("  ✓ Controls can exist in superposition of states\n");
    print("  ✓ Quantum entanglement between UI elements\n");
    print("  ✓ Observer effect collapses UI superposition\n");
    print("  ✓ Quantum evolution of interface states\n");
    print("  ✓ Spooky action at a distance in distributed UIs\n");
    print("  ✓ User interface as quantum computer!\n\n");
    
    uint64_t form_sig = vb9_form_signature(qform->classical_form);
    print("Classical form signature: %llu\n", form_sig);
    print("But quantum signature would be complex-valued!\n");
    print("Quantum computing embedded in visual forms!\n\n");
    
    /* Wait for user interaction */
    print("Click buttons to collapse their quantum states...\n");
    print("Press 'q' to exit quantum realm\n");
    
    vb9_run(rt);
    
    /* Cleanup */
    vb9_quantum_form_destroy(qform);
    vb9_shutdown(rt);
    
    print("\nQuantum interface shutdown complete.\n");
    print("Welcome back to classical reality!\n");
    print("But remember: underneath, everything is quantum...\n");
    
    exits(nil);
}