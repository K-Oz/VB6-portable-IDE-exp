/* hello.c - Hello World in VB9 - 30 seconds to working distributed system */

#include "../include/vb9.h"
#include "../include/controls.h"

void
main(void)
{
    VB9Runtime *rt;
    VB9Form *form;
    Control *lbl, *btn;
    
    /* Initialize - connect to the computation substrate */
    rt = vb9_init();
    if(rt == nil)
        sysfatal("VB9 init failed");
    
    /* Create form - a computational container */  
    form = vb9_form_new("HelloForm", "VB9 Hello World");
    
    /* Add controls - each IS a computation */
    lbl = vb9_label_new(form, "Label1", "Hello from VB9!");
    btn = vb9_button_new(form, "OkButton", "OK");
    
    /* Position like VB6 - simple and visual */
    vb9_control_move(lbl, vb9_point(50, 50));
    vb9_control_move(btn, vb9_point(50, 80));
    
    /* Show form - render the computation */
    vb9_form_show(form);
    
    print("VB9 Hello World - Drawing IS Computing!\n");
    print("Click OK or press 'q' to exit\n");
    
    /* Run - execute the computation */  
    vb9_run(rt);
    
    /* Shutdown */
    vb9_shutdown(rt);
    exits(nil);
}