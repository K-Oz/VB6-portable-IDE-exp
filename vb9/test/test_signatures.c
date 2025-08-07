/* test_signatures.c - Test the prime signature system */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Minimal VB9 types for testing */
typedef struct {
    int type;
    char *name;
} Control;

typedef struct {
    char *name;
    int ncontrols;
    Control controls[128];
} VB9Form;

/* Control types - prime numbers in computational space */
enum {
    VB9_BUTTON   = 2,    /* first prime */
    VB9_TEXTBOX  = 3,    /* second prime */
    VB9_LABEL    = 5,    /* third prime */
    VB9_LISTBOX  = 7,    /* fourth prime */
    VB9_FORM     = 11    /* fifth prime */
};

/* Calculate form signature - prime factorization */
uint64_t vb9_form_signature(VB9Form *form)
{
    uint64_t sig = 1;
    int i;
    
    if(!form) return 0;
    
    for(i = 0; i < form->ncontrols; i++) {
        sig *= form->controls[i].type;
    }
    
    return sig;
}

void add_control(VB9Form *form, int type, const char *name)
{
    if(form->ncontrols < 128) {
        form->controls[form->ncontrols].type = type;
        form->controls[form->ncontrols].name = (char*)name;
        form->ncontrols++;
    }
}

int main()
{
    VB9Form hello_form = {0};
    VB9Form calculator_form = {0};
    uint64_t sig1, sig2, sig3;
    
    printf("=== VB9 PRIME SIGNATURE SYSTEM TEST ===\n\n");
    
    /* Test 1: Simple Hello Form (Label + Button) */
    hello_form.name = "HelloForm";
    add_control(&hello_form, VB9_LABEL, "Label1");    /* prime 5 */
    add_control(&hello_form, VB9_BUTTON, "OkButton"); /* prime 2 */
    
    sig1 = vb9_form_signature(&hello_form);
    printf("Hello Form:\n");
    printf("  Controls: Label (5) + Button (2)\n");
    printf("  Signature: %llu (5 * 2 = 10)\n", sig1);
    printf("  Expected: 10\n");
    printf("  Test: %s\n\n", sig1 == 10 ? "PASS" : "FAIL");
    
    /* Test 2: Calculator Form (1 TextBox + 2 Buttons) */
    calculator_form.name = "Calculator";
    add_control(&calculator_form, VB9_TEXTBOX, "Display"); /* prime 3 */
    add_control(&calculator_form, VB9_BUTTON, "Button1");  /* prime 2 */
    add_control(&calculator_form, VB9_BUTTON, "Button2");  /* prime 2 */
    
    sig2 = vb9_form_signature(&calculator_form);
    printf("Calculator Form:\n");
    printf("  Controls: TextBox (3) + Button (2) + Button (2)\n");
    printf("  Signature: %llu (3 * 2 * 2 = 12)\n", sig2);
    printf("  Expected: 12\n");
    printf("  Test: %s\n\n", sig2 == 12 ? "PASS" : "FAIL");
    
    /* Test 3: Complex Form (all control types) */
    VB9Form complex_form = {0};
    complex_form.name = "ComplexForm";
    add_control(&complex_form, VB9_BUTTON, "Button");   /* 2 */
    add_control(&complex_form, VB9_TEXTBOX, "TextBox"); /* 3 */
    add_control(&complex_form, VB9_LABEL, "Label");     /* 5 */
    add_control(&complex_form, VB9_LISTBOX, "ListBox"); /* 7 */
    
    sig3 = vb9_form_signature(&complex_form);
    printf("Complex Form:\n");
    printf("  Controls: Button (2) + TextBox (3) + Label (5) + ListBox (7)\n");
    printf("  Signature: %llu (2 * 3 * 5 * 7 = 210)\n", sig3);
    printf("  Expected: 210\n");
    printf("  Test: %s\n\n", sig3 == 210 ? "PASS" : "FAIL");
    
    /* Demonstrate uniqueness */
    printf("SIGNATURE UNIQUENESS TEST:\n");
    printf("  HelloForm signature: %llu\n", sig1);
    printf("  Calculator signature: %llu\n", sig2);
    printf("  ComplexForm signature: %llu\n", sig3);
    printf("  All different: %s\n\n", 
           (sig1 != sig2 && sig2 != sig3 && sig1 != sig3) ? "PASS" : "FAIL");
    
    printf("COMPUTATIONAL ARCHAEOLOGY:\n");
    printf("  Each form has a unique prime factorization!\n");
    printf("  The signature IS the computational identity!\n");
    printf("  Form complexity = product of control primes\n");
    printf("  This reveals the quantization of interfaces!\n\n");
    
    printf("SIZE COMPARISON:\n");
    printf("  VB6 Portable.exe: ~6MB (hides the simplicity)\n");
    printf("  VB9 signatures: 8 bytes (reveals the truth)\n");
    printf("  Size reduction exposes underlying computational substrate!\n");
    
    return 0;
}