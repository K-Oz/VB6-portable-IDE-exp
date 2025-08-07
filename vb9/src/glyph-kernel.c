/* glyph-kernel.c - VB9 Glyph Kernel Extraction Implementation */
/* Revealing the algorithmic DNA of characters */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/glyph-kernel.h"

/* Prime lookup table for runes - each character has a unique prime */
static uint64_t rune_prime_table[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
    157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 
    239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317,
    331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419,
    421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503
};

/* === GlyphKernel Operations === */

GlyphKernel* 
vb9_glyph_kernel_new(int rune)
{
    GlyphKernel *kernel = mallocz(sizeof(GlyphKernel), 1);
    if(!kernel) return nil;
    
    kernel->rune = rune;
    kernel->prime = vb9_rune_to_prime(rune);
    kernel->nops = 0;
    kernel->complexity_score = 0.0f;
    
    /* Initialize control points to origin */
    for(int i = 0; i < VB9_CONTROL_POINTS; i++) {
        kernel->controls[i].x = 0;
        kernel->controls[i].y = 0;
    }
    
    /* Set function pointers */
    kernel->draw_kernel = vb9_glyph_kernel_draw_visualization;
    kernel->draw_glyph = vb9_glyph_kernel_draw_glyph;
    
    return kernel;
}

void 
vb9_glyph_kernel_destroy(GlyphKernel *kernel)
{
    if(!kernel) return;
    
    if(kernel->name) 
        free(kernel->name);
    
    free(kernel);
}

uint64_t 
vb9_rune_to_prime(int rune)
{
    /* Map rune to prime number - each character gets unique prime */
    if(rune < 32) return 2;  /* Control characters get prime 2 */
    
    int index = (rune - 32) % (sizeof(rune_prime_table) / sizeof(uint64_t));
    return rune_prime_table[index];
}

GlyphKernel* 
vb9_extract_glyph_kernel(VB9Font *font, int rune)
{
    if(!font) return nil;
    
    GlyphKernel *kernel = vb9_glyph_kernel_new(rune);
    if(!kernel) return nil;
    
    kernel->source_font = font;
    
    /* Simulate drawing operations capture for the glyph */
    /* In a real implementation, this would hook into the font rendering pipeline */
    
    /* Generate simulated drawing operations based on character complexity */
    int op_count = 0;
    
    /* Simple heuristic: vowels have more curves, consonants have more lines */
    int is_vowel = (rune == 'a' || rune == 'e' || rune == 'i' || 
                   rune == 'o' || rune == 'u' || 
                   rune == 'A' || rune == 'E' || rune == 'I' || 
                   rune == 'O' || rune == 'U');
    
    if(is_vowel) {
        /* Vowels: More curves and arcs */
        kernel->ops[op_count++] = VB9_DRAW_OP_MOVE;
        kernel->ops[op_count++] = 10; /* x */
        kernel->ops[op_count++] = 20; /* y */
        
        kernel->ops[op_count++] = VB9_DRAW_OP_CURVE;
        kernel->ops[op_count++] = 15; /* width */
        kernel->ops[op_count++] = 25; /* height */
        
        kernel->ops[op_count++] = VB9_DRAW_OP_ARC;
        kernel->ops[op_count++] = 20; /* radius */
        kernel->ops[op_count++] = 180; /* angle */
        
        /* Set control points for curves */
        kernel->controls[0] = (Point){10, 20};
        kernel->controls[1] = (Point){25, 15};
        kernel->controls[2] = (Point){20, 45};
        kernel->controls[3] = (Point){5, 40};
        
    } else {
        /* Consonants: More lines and fills */
        kernel->ops[op_count++] = VB9_DRAW_OP_MOVE;
        kernel->ops[op_count++] = 5; /* x */
        kernel->ops[op_count++] = 10; /* y */
        
        kernel->ops[op_count++] = VB9_DRAW_OP_LINE;
        kernel->ops[op_count++] = 15; /* x2 */
        kernel->ops[op_count++] = 30; /* y2 */
        
        kernel->ops[op_count++] = VB9_DRAW_OP_FILL;
        kernel->ops[op_count++] = 8; /* width */
        kernel->ops[op_count++] = 12; /* height */
        
        /* Set control points for lines */
        kernel->controls[0] = (Point){5, 10};
        kernel->controls[1] = (Point){15, 30};
        kernel->controls[2] = (Point){10, 20};
        kernel->controls[3] = (Point){12, 25};
    }
    
    kernel->nops = op_count;
    kernel->complexity_score = vb9_glyph_kernel_complexity(kernel);
    
    return kernel;
}

float 
vb9_glyph_kernel_complexity(GlyphKernel *kernel)
{
    if(!kernel) return 0.0f;
    
    /* Calculate complexity based on number of operations and control points */
    float base_complexity = (float)kernel->nops / VB9_MAX_KERNEL_OPS;
    
    /* Count non-zero control points */
    int active_controls = 0;
    for(int i = 0; i < VB9_CONTROL_POINTS; i++) {
        if(kernel->controls[i].x != 0 || kernel->controls[i].y != 0) {
            active_controls++;
        }
    }
    
    float control_complexity = (float)active_controls / VB9_CONTROL_POINTS;
    
    /* Prime number influences complexity */
    float prime_factor = log((double)kernel->prime) / 10.0;
    
    return base_complexity + control_complexity + prime_factor;
}

/* === Visualization Functions === */

void 
vb9_glyph_kernel_draw_visualization(GlyphKernel *kernel, Point where)
{
    if(!kernel) return;
    
    print("KERNEL VISUALIZATION at (%d, %d):\n", where.x, where.y);
    print("  Rune: %c (0x%x)\n", kernel->rune, kernel->rune);
    print("  Prime: %llu\n", (unsigned long long)kernel->prime);
    print("  Operations: %d\n", kernel->nops);
    print("  Complexity: %.3f\n", kernel->complexity_score);
    
    /* Draw constellation of control points */
    vb9_draw_kernel_constellation(kernel, where);
    
    /* Draw bytecode as barcode */
    vb9_draw_bytecode_barcode(kernel, (Point){where.x, where.y + 50});
    
    /* Draw connections between control points */
    vb9_draw_kernel_connections(kernel, where);
}

void 
vb9_glyph_kernel_draw_glyph(GlyphKernel *kernel, Point where)
{
    if(!kernel) return;
    
    print("DRAWING GLYPH '%c' at (%d, %d) using kernel operations:\n", 
          kernel->rune, where.x, where.y);
    
    /* Execute the bytecode operations to draw the glyph */
    for(int i = 0; i < kernel->nops; i += 3) {
        if(i + 2 >= kernel->nops) break;
        
        VB9DrawOp op = (VB9DrawOp)kernel->ops[i];
        int param1 = kernel->ops[i+1];
        int param2 = kernel->ops[i+2];
        
        switch(op) {
            case VB9_DRAW_OP_MOVE:
                print("  MOVE to (%d, %d)\n", where.x + param1, where.y + param2);
                break;
            case VB9_DRAW_OP_LINE:
                print("  LINE to (%d, %d)\n", where.x + param1, where.y + param2);
                break;
            case VB9_DRAW_OP_CURVE:
                print("  CURVE %dx%d\n", param1, param2);
                break;
            case VB9_DRAW_OP_ARC:
                print("  ARC radius=%d angle=%d\n", param1, param2);
                break;
            case VB9_DRAW_OP_FILL:
                print("  FILL %dx%d\n", param1, param2);
                break;
        }
    }
}

void 
vb9_draw_kernel_constellation(GlyphKernel *kernel, Point where)
{
    if(!kernel) return;
    
    print("  CONSTELLATION (Control Points):\n");
    
    for(int i = 0; i < VB9_CONTROL_POINTS && i < 8; i++) {
        Point cp = kernel->controls[i];
        if(cp.x == 0 && cp.y == 0) continue;
        
        Point screen_pos = {where.x + cp.x, where.y + cp.y};
        
        /* Calculate hue based on position in sequence */
        int hue = (i * 360) / VB9_CONTROL_POINTS;
        
        print("    Point[%d]: (%d, %d) hue=%d\n", i, screen_pos.x, screen_pos.y, hue);
    }
}

void 
vb9_draw_bytecode_barcode(GlyphKernel *kernel, Point where)
{
    if(!kernel) return;
    
    print("  BYTECODE BARCODE:\n");
    print("    ");
    
    for(int i = 0; i < kernel->nops && i < 32; i++) {
        int intensity = kernel->ops[i];
        char bar_char = (intensity < 85) ? '.' : (intensity < 170) ? '|' : '#';
        print("%c", bar_char);
    }
    print("\n");
}

void 
vb9_draw_kernel_connections(GlyphKernel *kernel, Point where)
{
    if(!kernel) return;
    
    print("  CONTROL POINT CONNECTIONS:\n");
    
    for(int i = 1; i < VB9_CONTROL_POINTS && i < 6; i++) {
        Point prev = kernel->controls[i-1];
        Point curr = kernel->controls[i];
        
        if((prev.x == 0 && prev.y == 0) || (curr.x == 0 && curr.y == 0)) continue;
        
        Point prev_screen = {where.x + prev.x, where.y + prev.y};
        Point curr_screen = {where.x + curr.x, where.y + curr.y};
        
        print("    Connection[%d]: (%d,%d) -> (%d,%d)\n", 
              i, prev_screen.x, prev_screen.y, curr_screen.x, curr_screen.y);
    }
}

/* === KernelButton Operations === */

KernelButton* 
vb9_kernel_button_new(char *name, Point position, VB9Font *font)
{
    KernelButton *button = mallocz(sizeof(KernelButton), 1);
    if(!button) return nil;
    
    if(name) {
        button->name = malloc(strlen(name) + 1);
        if(button->name) strcpy(button->name, name);
    }
    
    button->position = position;
    button->size = (Point){120, 30}; /* Default button size */
    button->control_prime = 2; /* VB9_BUTTON */
    button->target_font = font;
    
    button->maxkernels = 256;
    button->kernels = mallocz(sizeof(GlyphKernel*) * button->maxkernels, 1);
    button->nkernels = 0;
    
    /* Set function pointers */
    button->extract = vb9_kernel_button_extract;
    button->convolve = vb9_convolve_kernels;
    button->click = vb9_kernel_button_click;
    button->draw = vb9_kernel_button_draw;
    
    return button;
}

void 
vb9_kernel_button_destroy(KernelButton *button)
{
    if(!button) return;
    
    if(button->name) free(button->name);
    
    /* Destroy all extracted kernels */
    for(int i = 0; i < button->nkernels; i++) {
        vb9_glyph_kernel_destroy(button->kernels[i]);
    }
    
    if(button->kernels) free(button->kernels);
    free(button);
}

void 
vb9_kernel_button_draw(KernelButton *button)
{
    if(!button) return;
    
    print("[KERNEL BUTTON: %s] at (%d, %d)\n", 
          button->name ? button->name : "unnamed",
          button->position.x, button->position.y);
    print("  Kernels extracted: %d\n", button->nkernels);
    print("  Target font: %s\n", 
          button->target_font ? button->target_font->name : "none");
}

void 
vb9_kernel_button_click(KernelButton *button, Point click_pos)
{
    if(!button) return;
    
    print("KERNEL BUTTON CLICKED at (%d, %d)!\n", click_pos.x, click_pos.y);
    print("Extracting kernels from ASCII printable characters...\n\n");
    
    /* Extract kernels for printable ASCII range */
    for(int rune = 33; rune <= 126 && button->nkernels < button->maxkernels; rune++) {
        GlyphKernel *kernel = vb9_extract_glyph_kernel(button->target_font, rune);
        if(kernel) {
            button->kernels[button->nkernels++] = kernel;
            
            /* Position kernels in prime-factorized arrangement */
            int x = ((kernel->prime % 31) * 25) + button->position.x;
            int y = (((kernel->prime / 31) % 31) * 25) + button->position.y + 50;
            
            print("EXTRACTED KERNEL for '%c':\n", rune);
            kernel->draw_kernel(kernel, (Point){x, y});
            print("\n");
        }
    }
    
    print("=== KERNEL EXTRACTION COMPLETE ===\n");
    print("Total kernels extracted: %d\n", button->nkernels);
    print("Font space revealed as kernel field!\n\n");
}

GlyphKernel* 
vb9_kernel_button_extract(KernelButton *button, int rune)
{
    if(!button || !button->target_font) return nil;
    
    return vb9_extract_glyph_kernel(button->target_font, rune);
}

/* === Kernel Convolution === */

GlyphKernel* 
vb9_convolve_kernels(GlyphKernel *a, GlyphKernel *b)
{
    if(!a || !b) return nil;
    
    GlyphKernel *child = vb9_glyph_kernel_new(0); /* Hybrid has no specific rune */
    if(!child) return nil;
    
    print("CONVOLVING KERNELS: '%c' (prime %llu) + '%c' (prime %llu)\n",
          a->rune, (unsigned long long)a->prime, 
          b->rune, (unsigned long long)b->prime);
    
    /* Child's prime is the product (true convolution) */
    child->prime = a->prime * b->prime;
    child->rune = '?'; /* Hybrid character */
    
    /* Merge control points using golden ratio interpolation */
    for(int i = 0; i < VB9_CONTROL_POINTS; i++) {
        child->controls[i].x = (int)(a->controls[i].x * VB9_GOLDEN_RATIO + 
                                   b->controls[i].x * (1.0f - VB9_GOLDEN_RATIO));
        child->controls[i].y = (int)(a->controls[i].y * VB9_GOLDEN_RATIO + 
                                   b->controls[i].y * (1.0f - VB9_GOLDEN_RATIO));
    }
    
    /* Interleave bytecode operations (genetic crossover) */
    vb9_kernel_genetic_crossover(a, b, child);
    
    child->complexity_score = vb9_glyph_kernel_complexity(child);
    
    print("HYBRID KERNEL created with prime %llu, complexity %.3f\n",
          (unsigned long long)child->prime, child->complexity_score);
    
    return child;
}

void 
vb9_kernel_genetic_crossover(GlyphKernel *parent_a, GlyphKernel *parent_b, 
                             GlyphKernel *child)
{
    if(!parent_a || !parent_b || !child) return;
    
    int j = 0;
    int max_ops = VB9_MAX_KERNEL_OPS - 1;
    
    /* Interleave operations from both parents */
    for(int i = 0; (i < parent_a->nops || i < parent_b->nops) && j < max_ops; i++) {
        if(i < parent_a->nops && j < max_ops) {
            child->ops[j++] = parent_a->ops[i];
        }
        if(i < parent_b->nops && j < max_ops) {
            child->ops[j++] = parent_b->ops[i];
        }
    }
    
    child->nops = j;
}

char* 
vb9_kernel_debug_info(GlyphKernel *kernel)
{
    if(!kernel) return nil;
    
    static char debug_buffer[512];
    snprint(debug_buffer, sizeof(debug_buffer),
            "GlyphKernel Debug Info:\n"
            "  Rune: '%c' (0x%x)\n"
            "  Prime: %llu\n" 
            "  Operations: %d\n"
            "  Complexity: %.3f\n"
            "  Control Points: %d active\n",
            kernel->rune, kernel->rune,
            (unsigned long long)kernel->prime,
            kernel->nops,
            kernel->complexity_score,
            4); /* Simplified count */
    
    return debug_buffer;
}