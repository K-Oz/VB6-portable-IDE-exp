/* glyph-kernel.h - VB9 Glyph Kernel Extraction System */
/* Every character is a program - this reveals their algorithmic DNA */

#ifndef VB9_GLYPH_KERNEL_H
#define VB9_GLYPH_KERNEL_H

#include "vb9.h"
#include "font-coding.h"

/* Forward declarations */
typedef struct GlyphKernel GlyphKernel;
typedef struct KernelButton KernelButton;

/* Drawing operation types for kernel bytecode */
typedef enum {
    VB9_DRAW_OP_LINE = 1,
    VB9_DRAW_OP_CURVE = 2,
    VB9_DRAW_OP_ARC = 3,
    VB9_DRAW_OP_FILL = 4,
    VB9_DRAW_OP_MOVE = 5
} VB9DrawOp;

/* GlyphKernel - The DNA of a character */
struct GlyphKernel {
    int rune;                    /* The character this kernel generates */
    uint64_t prime;              /* Prime factorization signature */
    Point controls[17];          /* Bezier control points (17 is prime) */
    unsigned char ops[256];      /* Drawing operations as bytecode */
    int nops;                    /* Number of operations */
    
    /* Function pointers for drawing */
    void (*draw_kernel)(GlyphKernel *self, Point where);
    void (*draw_glyph)(GlyphKernel *self, Point where);
    
    /* Metadata */
    char *name;                  /* Optional name for this kernel */
    VB9Font *source_font;        /* Font this kernel was extracted from */
    float complexity_score;      /* Computational complexity measure */
};

/* KernelButton - Button that extracts and manipulates glyph kernels */  
struct KernelButton {
    /* Base control properties */
    char *name;
    Point position;
    Point size;
    int control_prime;           /* VB9_BUTTON = 2 */
    
    /* Kernel extraction properties */
    VB9Font *target_font;        /* Font to extract kernels from */
    GlyphKernel **kernels;       /* Array of extracted kernels */
    int nkernels;                /* Number of kernels */
    int maxkernels;              /* Maximum kernel capacity */
    
    /* Function pointers */
    GlyphKernel* (*extract)(KernelButton *self, int rune);
    GlyphKernel* (*convolve)(GlyphKernel *a, GlyphKernel *b);
    void (*click)(KernelButton *self, Point where);
    void (*draw)(KernelButton *self);
};

/* Font Field - Collection of interacting kernels */
typedef struct FontField {
    GlyphKernel *kernels[65536]; /* Unicode space */
    int populated[65536];        /* Which kernels are populated */
    int nkernels;               /* Total number of kernels */
    
    /* Kernel interaction functions */
    float (*interaction)(GlyphKernel *a, GlyphKernel *b);
    void (*evolve)(struct FontField *self);
} FontField;

/* Neural Kernel - Adaptive version of GlyphKernel */
typedef struct NeuralKernel {
    GlyphKernel base;           /* Base kernel functionality */
    float weights[17*17];       /* Connections between control points */
    float learning_rate;        /* Adaptation rate */
    
    /* Learning functions */
    void (*learn)(struct NeuralKernel *self, int prev_rune, int next_rune);
    GlyphKernel* (*dream)(struct NeuralKernel *self);
} NeuralKernel;

/* === Function Prototypes === */

/* GlyphKernel operations */
GlyphKernel* vb9_glyph_kernel_new(int rune);
void vb9_glyph_kernel_destroy(GlyphKernel *kernel);
GlyphKernel* vb9_extract_glyph_kernel(VB9Font *font, int rune);
void vb9_glyph_kernel_draw_visualization(GlyphKernel *kernel, Point where);
void vb9_glyph_kernel_draw_glyph(GlyphKernel *kernel, Point where);
uint64_t vb9_rune_to_prime(int rune);
float vb9_glyph_kernel_complexity(GlyphKernel *kernel);

/* KernelButton operations */
KernelButton* vb9_kernel_button_new(char *name, Point position, VB9Font *font);
void vb9_kernel_button_destroy(KernelButton *button);
void vb9_kernel_button_draw(KernelButton *button);
void vb9_kernel_button_click(KernelButton *button, Point click_pos);
GlyphKernel* vb9_kernel_button_extract(KernelButton *button, int rune);

/* Kernel convolution and evolution */
GlyphKernel* vb9_convolve_kernels(GlyphKernel *a, GlyphKernel *b);
void vb9_kernel_genetic_crossover(GlyphKernel *parent_a, GlyphKernel *parent_b, 
                                  GlyphKernel *child);

/* FontField operations */
FontField* vb9_font_field_new(void);
void vb9_font_field_destroy(FontField *field);
void vb9_font_field_add_kernel(FontField *field, GlyphKernel *kernel);
void vb9_font_field_evolve(FontField *field);
float vb9_kernel_interaction_strength(GlyphKernel *a, GlyphKernel *b);

/* NeuralKernel operations */
NeuralKernel* vb9_neural_kernel_new(int rune);
void vb9_neural_kernel_destroy(NeuralKernel *neural);
void vb9_neural_kernel_learn(NeuralKernel *neural, int prev_rune, int next_rune);
GlyphKernel* vb9_neural_kernel_dream(NeuralKernel *neural);

/* Visualization and debugging */
void vb9_draw_kernel_constellation(GlyphKernel *kernel, Point where);
void vb9_draw_bytecode_barcode(GlyphKernel *kernel, Point where);
void vb9_draw_kernel_connections(GlyphKernel *kernel, Point where);
char* vb9_kernel_debug_info(GlyphKernel *kernel);

/* Integration with VB9 forms */
void vb9_form_add_kernel_button(VB9Form *form, KernelButton *button);
void vb9_form_add_font_field(VB9Form *form, FontField *field);

/* Mathematical constants for kernel operations */
#define VB9_GOLDEN_RATIO 0.618034f
#define VB9_PRIME_17 17
#define VB9_MAX_KERNEL_OPS 256
#define VB9_CONTROL_POINTS 17

#endif /* VB9_GLYPH_KERNEL_H */