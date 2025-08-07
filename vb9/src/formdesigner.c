/* formdesigner.c - Visual Form Designer - Draw the Computation */

#include "../include/vb9.h"
#include "../include/controls.h"

void
usage(void)
{
    fprint(2, "usage: %s [formname.frm]\n", argv0);
    exits("usage");
}

void
create_sample_form(char *name)
{
    VB9Runtime *rt;
    VB9Form *form;
    Control *btn, *lbl, *txt;
    
    /* Initialize VB9 runtime - connect to computational substrate */
    rt = vb9_init();
    if(rt == nil)
        sysfatal("failed to initialize VB9 runtime");
    
    /* Create form - a computational container */
    form = vb9_form_new(name, "VB9 Sample Form");
    if(form == nil)
        sysfatal("failed to create form");
    
    /* Add controls - each control IS a computation */
    lbl = vb9_label_new(form, "Label1", "Hello VB9!");
    btn = vb9_button_new(form, "Command1", "Click Me");  
    txt = vb9_textbox_new(form, "Text1", "Enter text here");
    
    /* Position controls like VB6 */
    if(lbl) vb9_control_move(lbl, vb9_point(20, 40));
    if(btn) vb9_control_move(btn, vb9_point(20, 70));
    if(txt) vb9_control_move(txt, vb9_point(120, 70));
    
    /* Show form - render the computation */
    vb9_form_show(form);
    
    print("VB9 Form Designer - Drawing IS Computing!\n");
    print("Form: %s created with %d controls\n", form->name, form->ncontrols);
    print("Press any key to interact, 'q' to quit\n");
    
    /* Run the computation */
    vb9_run(rt);
    
    /* Shutdown */
    vb9_shutdown(rt);
}

void
main(int argc, char *argv[])
{
    char *formname = "Form1";
    
    ARGBEGIN {
    default:
        usage();
    } ARGEND
    
    if(argc > 0)
        formname = argv[0];
    
    create_sample_form(formname);
    exits(nil);
}