/* vb9.h - Core VB9 structures and definitions */

#ifndef VB9_H
#define VB9_H

#include <u.h>
#include <libc.h>
#include <draw.h>
#include <event.h>
#include <keyboard.h>

typedef struct Point Point;
typedef struct Rectangle Rectangle;
typedef struct Image Image;
typedef struct Font Font;

/* Control types - matching VB6 basics */
enum {
    VB9_BUTTON   = 1,
    VB9_TEXTBOX  = 2, 
    VB9_LABEL    = 3,
    VB9_LISTBOX  = 4,
    VB9_FORM     = 5
};

/* Event types */
enum {
    VB9_CLICK    = 1,
    VB9_CHANGE   = 2,
    VB9_KEYPRESS = 3,
    VB9_LOAD     = 4
};

/* Control structure - the visual IS the program */
typedef struct Control {
    char    *name;          /* maps to /form/name */
    int     type;           /* VB9_BUTTON, VB9_TEXTBOX, etc */
    Rectangle r;            /* position IS memory address */
    char    *text;          /* current text/caption */
    char    *value;         /* current value */
    int     visible;        /* visibility flag */
    int     enabled;        /* enabled/disabled */
    void    (*draw)(struct Control*);    /* drawing IS execution */
    void    (*click)(struct Control*);   /* events ARE file writes */
    void    (*change)(struct Control*);  /* change handler */
} Control;

/* Form structure - container for controls */
typedef struct VB9Form {
    char    *name;          /* form name */
    char    *title;         /* window title */
    Rectangle r;            /* form bounds */
    Image   *image;         /* /dev/draw handle */
    int     ncontrols;      /* number of controls */
    Control controls[128];  /* max 128, like ttffs glyphs! */
    Font    *font;          /* default font */
    int     visible;        /* form visibility */
} VB9Form;

/* Runtime context */
typedef struct VB9Runtime {
    Display *display;       /* connection to /dev/draw */
    VB9Form *forms[16];     /* max 16 forms */
    int     nforms;         /* current form count */
    VB9Form *activeform;    /* currently active form */
    Font    *defaultfont;   /* system default font */
    Image   *background;    /* desktop background */
} VB9Runtime;

/* Global runtime context - accessible from all modules */
extern VB9Runtime *g_runtime;

/* Function prototypes */

/* Core runtime */
VB9Runtime* vb9_init(void);
void        vb9_shutdown(VB9Runtime *rt);
void        vb9_run(VB9Runtime *rt);

/* Form management */
VB9Form*    vb9_form_new(char *name, char *title);
void        vb9_form_show(VB9Form *form);
void        vb9_form_hide(VB9Form *form);
void        vb9_form_destroy(VB9Form *form);

/* Control management */
Control*    vb9_control_new(VB9Form *form, int type, char *name);
void        vb9_control_move(Control *ctrl, Point p);
void        vb9_control_resize(Control *ctrl, Point size);
void        vb9_control_set_text(Control *ctrl, char *text);
char*       vb9_control_get_text(Control *ctrl);

/* Event system */
void        vb9_event_loop(VB9Runtime *rt);
void        vb9_handle_click(VB9Runtime *rt, Point p);
void        vb9_handle_key(VB9Runtime *rt, Rune r);

/* File system integration - the Plan 9 magic */
int         vb9_serve_control(Control *ctrl, char *mountpoint);
void        vb9_handle_9p_request(Control *ctrl, char *op, char *data);

/* Drawing primitives - drawing IS computing */
void        vb9_draw_form(VB9Form *form);
void        vb9_draw_control(Control *ctrl);
void        vb9_redraw_all(VB9Runtime *rt);

/* Utility functions */
Rectangle   vb9_rect(int x, int y, int w, int h);
Point       vb9_point(int x, int y);
Point       vb9_string_size(Font *font, char *s);

#endif /* VB9_H */