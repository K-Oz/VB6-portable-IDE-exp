/* controls.h - VB9 Control definitions */

#ifndef CONTROLS_H
#define CONTROLS_H

#include "vb9.h"

/* Control creation functions - VB6 style simplicity */
Control* vb9_button_new(VB9Form *form, char *name, char *caption);
Control* vb9_textbox_new(VB9Form *form, char *name, char *text);
Control* vb9_label_new(VB9Form *form, char *name, char *caption);
Control* vb9_listbox_new(VB9Form *form, char *name);

/* Control drawing functions - drawing IS execution */
void vb9_button_draw(Control *ctrl);
void vb9_textbox_draw(Control *ctrl);
void vb9_label_draw(Control *ctrl);
void vb9_listbox_draw(Control *ctrl);

/* Control event handlers - events ARE file operations */
void vb9_button_click(Control *ctrl);
void vb9_textbox_change(Control *ctrl);
void vb9_listbox_select(Control *ctrl);

/* Control property accessors */
void vb9_button_set_caption(Control *ctrl, char *caption);
char* vb9_button_get_caption(Control *ctrl);
void vb9_textbox_set_text(Control *ctrl, char *text);
char* vb9_textbox_get_text(Control *ctrl);
void vb9_label_set_caption(Control *ctrl, char *caption);
void vb9_listbox_add_item(Control *ctrl, char *item);

/* Default control sizes - reasonable defaults like VB6 */
enum {
    VB9_BUTTON_WIDTH    = 75,
    VB9_BUTTON_HEIGHT   = 23,
    VB9_TEXTBOX_WIDTH   = 100,
    VB9_TEXTBOX_HEIGHT  = 20,
    VB9_LABEL_WIDTH     = 100,
    VB9_LABEL_HEIGHT    = 15,
    VB9_LISTBOX_WIDTH   = 120,
    VB9_LISTBOX_HEIGHT  = 95
};

/* Control colors - simple palette */
enum {
    VB9_COLOR_BUTTON_FACE    = DPalebluegreen,
    VB9_COLOR_BUTTON_TEXT    = DBlack,
    VB9_COLOR_TEXTBOX_BACK   = DWhite,
    VB9_COLOR_TEXTBOX_TEXT   = DBlack,
    VB9_COLOR_LABEL_BACK     = DPaleyellow,
    VB9_COLOR_LABEL_TEXT     = DBlack,
    VB9_COLOR_LISTBOX_BACK   = DWhite,
    VB9_COLOR_LISTBOX_TEXT   = DBlack,
    VB9_COLOR_FORM_BACK      = DPaleyellow
};

#endif /* CONTROLS_H */