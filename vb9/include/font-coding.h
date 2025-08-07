/* font-coding.h - VB9 Font-Based Programming */

#ifndef VB9_FONT_CODING_H
#define VB9_FONT_CODING_H

#include "vb9.h"

/* Font-as-Code Structures */
typedef struct VB9Font {
    char *name;              /* Font name */
    char *filename;          /* Font file path */
    void *font_data;         /* Raw font data */
    int font_size;           /* Font data size */
    int glyph_count;         /* Number of glyphs */
    int is_executable;       /* Is this font executable code? */
    char *language;          /* Programming language this font represents */
} VB9Font;

/* Glyph as Language Construct */
typedef struct VB9Glyph {
    int unicode;             /* Unicode codepoint */
    char *construct;         /* Language construct (if, while, func, etc) */
    void *bytecode;          /* Compiled bytecode for this glyph */
    int bytecode_size;       /* Size of bytecode */
    int execution_prime;     /* Prime number for computational identity */
} VB9Glyph;

/* Font Program - Text that IS executable */
typedef struct VB9Program {
    char *source_text;       /* Original source text */
    VB9Font *font;          /* Font used to compile */
    VB9Glyph **glyphs;      /* Compiled glyphs */
    int nglyph_instances;   /* Number of glyph instances */
    void *executable;       /* Final executable */
    int executable_size;    /* Size of executable */
    uint64_t program_signature; /* Prime signature of program */
} VB9Program;

/* Language-specific font loaders */
typedef struct VB9LanguageFont {
    char *language_name;     /* "golang", "lisp", "forth", etc */
    VB9Font *font;          /* Font for this language */
    int (*compile_glyph)(char c, VB9Glyph *glyph);  /* Glyph compiler */
    int (*execute_program)(VB9Program *prog);       /* Program executor */
} VB9LanguageFont;

/* Visual Programming Context */
typedef struct VB9VisualProgramming {
    VB9LanguageFont **languages;  /* Available programming languages */
    int nlanguages;               /* Number of languages */
    VB9LanguageFont *current;     /* Currently selected language */
    VB9Font *display_font;        /* Font for visual display */
    VB9Program *active_program;   /* Currently edited program */
} VB9VisualProgramming;

/* Function prototypes */

/* Font loading and management */
VB9Font* vb9_font_load(char *filename);
void vb9_font_destroy(VB9Font *font);
int vb9_font_is_executable(VB9Font *font);
char* vb9_font_get_language(VB9Font *font);

/* Glyph operations */
VB9Glyph* vb9_glyph_new(int unicode, char *construct);
void vb9_glyph_destroy(VB9Glyph *glyph);
int vb9_glyph_compile(VB9Glyph *glyph, VB9Font *font);

/* Font compilation */
VB9Program* vb9_font_compile(VB9Font *code_font, char *program_text);
void vb9_program_destroy(VB9Program *prog);
int vb9_execute_font_program(VB9Program *prog);
uint64_t vb9_program_signature(VB9Program *prog);

/* Visual programming through typography */
char* vb9_render_as_font(VB9Program *prog, VB9Font *display_font);
VB9Program* vb9_parse_font_text(char *text, VB9Font *language_font);
void vb9_visual_edit(VB9Program *prog, Point click_pos, char new_char);

/* Language-specific implementations */
VB9LanguageFont* vb9_language_font_new(char *language_name, char *font_path);
void vb9_language_font_destroy(VB9LanguageFont *lang_font);
int vb9_golang_compile_glyph(char c, VB9Glyph *glyph);
int vb9_lisp_compile_glyph(char c, VB9Glyph *glyph);
int vb9_forth_compile_glyph(char c, VB9Glyph *glyph);

/* Visual programming environment */
VB9VisualProgramming* vb9_visual_programming_new(void);
void vb9_visual_programming_destroy(VB9VisualProgramming *vp);
void vb9_visual_programming_add_language(VB9VisualProgramming *vp, VB9LanguageFont *lang);
void vb9_visual_programming_set_language(VB9VisualProgramming *vp, char *language_name);
void vb9_visual_programming_render(VB9VisualProgramming *vp, VB9Form *form);

/* Integration with VB9 forms */
void vb9_form_add_font_editor(VB9Form *form, VB9VisualProgramming *vp);
void vb9_font_editor_handle_click(Control *editor_ctrl, Point p);
void vb9_font_editor_handle_key(Control *editor_ctrl, Rune r);

#endif /* VB9_FONT_CODING_H */