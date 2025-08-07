/* primordial.c - The Primordial Button - Archaeological Proof of Computation */

#include "../include/vb9.h"
#include "../include/controls.h"

/* The primordial button that exists simultaneously as:
 * - A rectangle in /dev/draw
 * - A directory in /form/button1/
 * - An event stream that IS its execution
 */

void
primordial_button_click(Control *ctrl)
{
    print("PRIMORDIAL EVENT: Button click IS file write!\n");
    print("  Control: %s (prime %d)\n", ctrl->name, ctrl->type);
    print("  This would be: echo 'click' > /form/PrimordialForm/%s/click\n", ctrl->name);
    print("  The file write IS the execution!\n");
    print("  The button doesn't 'trigger' code - the button IS a computational portal!\n");
    print("\n");
}

void
main(void)
{
    VB9Runtime *rt;
    VB9Form *form;
    Control *btn;
    uint64_t signature;
    
    print("=== VB9 PRIMORDIAL BUTTON DEMONSTRATION ===\n");
    print("Archaeological Proof: Drawing and Computing are the Same Operation\n\n");
    
    /* Initialize - connect to the computation substrate */
    rt = vb9_init();
    if(rt == nil)
        sysfatal("VB9 init failed");
    
    /* Create form - a computational container */  
    form = vb9_form_new("PrimordialForm", "The Primordial Button");
    
    /* Create the primordial button - it simultaneously exists as: */
    btn = vb9_button_new(form, "PrimordialButton", "The Origin");
    btn->click = primordial_button_click;  /* Custom event handler */
    
    /* Position the button */
    vb9_control_move(btn, vb9_point(50, 50));
    
    /* Calculate form signature - this IS the program */
    signature = vb9_form_signature(form);
    
    print("FORM ANALYSIS:\n");
    print("  Form has 1 button (prime 2)\n");
    print("  Form signature: %llu\n", signature);
    print("  Prime factorization: 2^1 = 2\n");
    print("  This signature IS the computational identity!\n\n");
    
    print("THE PRIMORDIAL INSIGHT:\n");
    print("  The button exists simultaneously as:\n");
    print("  1. Rectangle at coordinates (%d,%d) to (%d,%d)\n", 
          btn->r.min.x, btn->r.min.y, btn->r.max.x, btn->r.max.y);
    print("  2. Directory at /form/PrimordialForm/PrimordialButton/\n");
    print("  3. Event stream that IS its execution\n\n");
    
    print("SIZE ANALYSIS:\n");
    print("  VB6 Portable.exe: 6.0MB (entire IDE + runtime)\n");
    print("  VB9 Source: ~25KB (reveals the same computational power)\n");
    print("  Every byte removed reveals underlying simplicity!\n\n");
    
    /* Show form - render the computation */
    vb9_form_show(form);
    
    print("INTERACTION INSTRUCTIONS:\n");
    print("  Click the button to see file write = execution\n");
    print("  Press 'q' to exit and witness the computational archaeology\n");
    print("  Each click demonstrates: Drawing IS Computing!\n\n");
    
    /* Run - execute the computation */  
    vb9_run(rt);
    
    print("\nCOMPUTATIONAL ARCHAEOLOGY COMPLETE:\n");
    print("  The button was simultaneously:\n");
    print("  - Visual pixels on screen\n");
    print("  - Files in namespace\n"); 
    print("  - Computational events\n");
    print("  Drawing and computing are the same operation!\n");
    
    /* Shutdown */
    vb9_shutdown(rt);
    exits(nil);
}