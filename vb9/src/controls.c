/* controls.c - VB9 Control implementations - Each control IS a computation */

#include "../include/vb9.h"
#include "../include/controls.h"

/* Forward declaration - defined in runtime.c */
extern VB9Runtime *g_runtime;

/* Create new control in form */
Control*
vb9_control_new(VB9Form *form, int type, char *name)
{
    Control *ctrl;
    
    if(form == nil || form->ncontrols >= 128)
        return nil;
        
    ctrl = &form->controls[form->ncontrols];
    memset(ctrl, 0, sizeof(Control));
    
    ctrl->name = strdup(name);
    ctrl->type = type;
    ctrl->visible = 1;
    ctrl->enabled = 1;
    
    /* Set default position and size based on type */
    switch(type) {
    case VB9_BUTTON:
        ctrl->r = vb9_rect(10, 30 + form->ncontrols * 30, 
                          VB9_BUTTON_WIDTH, VB9_BUTTON_HEIGHT);
        ctrl->draw = vb9_button_draw;
        ctrl->click = vb9_button_click;
        break;
        
    case VB9_TEXTBOX:
        ctrl->r = vb9_rect(100, 30 + form->ncontrols * 30,
                          VB9_TEXTBOX_WIDTH, VB9_TEXTBOX_HEIGHT);
        ctrl->draw = vb9_textbox_draw;
        ctrl->change = vb9_textbox_change;
        ctrl->text = strdup("");
        break;
        
    case VB9_LABEL:
        ctrl->r = vb9_rect(10, 30 + form->ncontrols * 30,
                          VB9_LABEL_WIDTH, VB9_LABEL_HEIGHT);
        ctrl->draw = vb9_label_draw;
        ctrl->text = strdup("Label");
        break;
        
    case VB9_LISTBOX:
        ctrl->r = vb9_rect(10, 30 + form->ncontrols * 30,
                          VB9_LISTBOX_WIDTH, VB9_LISTBOX_HEIGHT);
        ctrl->draw = vb9_listbox_draw;
        ctrl->click = vb9_listbox_select;
        break;
    }
    
    form->ncontrols++;
    return ctrl;
}

/* Button creation - VB6 style simplicity */
Control*
vb9_button_new(VB9Form *form, char *name, char *caption)
{
    Control *ctrl;
    
    ctrl = vb9_control_new(form, VB9_BUTTON, name);
    if(ctrl == nil)
        return nil;
        
    if(caption)
        ctrl->text = strdup(caption);
    else
        ctrl->text = strdup("Button");
        
    return ctrl;
}

/* TextBox creation */
Control*
vb9_textbox_new(VB9Form *form, char *name, char *text)
{
    Control *ctrl;
    
    ctrl = vb9_control_new(form, VB9_TEXTBOX, name);
    if(ctrl == nil)
        return nil;
        
    if(text) {
        free(ctrl->text);
        ctrl->text = strdup(text);
    }
        
    return ctrl;
}

/* Label creation */
Control*
vb9_label_new(VB9Form *form, char *name, char *caption)
{
    Control *ctrl;
    
    ctrl = vb9_control_new(form, VB9_LABEL, name);
    if(ctrl == nil)
        return nil;
        
    if(caption) {
        free(ctrl->text);
        ctrl->text = strdup(caption);
    }
        
    return ctrl;
}

/* ListBox creation */
Control*
vb9_listbox_new(VB9Form *form, char *name)
{
    Control *ctrl;
    
    ctrl = vb9_control_new(form, VB9_LISTBOX, name);
    if(ctrl == nil)
        return nil;
        
    return ctrl;
}

/* Button drawing - render button computation */
void
vb9_button_draw(Control *ctrl)
{
    VB9Form *form;
    Rectangle r;
    Point textpos;
    
    if(ctrl == nil || g_runtime == nil || g_runtime->activeform == nil)
        return;
        
    form = g_runtime->activeform;
    r = rectaddpt(ctrl->r, form->image->r.min);
    
    /* Draw button face */
    draw(form->image, r, display->paleyellow, nil, ZP);
    
    /* Draw border - simple 3D effect like VB6 */
    line(form->image, r.min, Pt(r.max.x-1, r.min.y), 
         0, 0, 0, display->black, ZP);  /* Top */
    line(form->image, r.min, Pt(r.min.x, r.max.y-1), 
         0, 0, 0, display->black, ZP);  /* Left */
    line(form->image, Pt(r.max.x-1, r.min.y), Pt(r.max.x-1, r.max.y-1), 
         0, 0, 0, display->darkgrey, ZP);  /* Right */
    line(form->image, Pt(r.min.x, r.max.y-1), Pt(r.max.x-1, r.max.y-1), 
         0, 0, 0, display->darkgrey, ZP);  /* Bottom */
    
    /* Draw text centered */
    if(ctrl->text) {
        textpos.x = r.min.x + (Dx(r) - stringwidth(form->font, ctrl->text)) / 2;
        textpos.y = r.min.y + (Dy(r) - form->font->height) / 2 + form->font->ascent;
        string(form->image, textpos, display->black, ZP, form->font, ctrl->text);
    }
}

/* TextBox drawing - render text input computation */
void
vb9_textbox_draw(Control *ctrl)
{
    VB9Form *form;
    Rectangle r;
    Point textpos;
    
    if(ctrl == nil || g_runtime == nil || g_runtime->activeform == nil)
        return;
        
    form = g_runtime->activeform;
    r = rectaddpt(ctrl->r, form->image->r.min);
    
    /* Draw white background */
    draw(form->image, r, display->white, nil, ZP);
    
    /* Draw inset border */
    line(form->image, r.min, Pt(r.max.x-1, r.min.y), 
         0, 0, 0, display->darkgrey, ZP);  /* Top */
    line(form->image, r.min, Pt(r.min.x, r.max.y-1), 
         0, 0, 0, display->darkgrey, ZP);  /* Left */
    line(form->image, Pt(r.max.x-1, r.min.y), Pt(r.max.x-1, r.max.y-1), 
         0, 0, 0, display->black, ZP);  /* Right */
    line(form->image, Pt(r.min.x, r.max.y-1), Pt(r.max.x-1, r.max.y-1), 
         0, 0, 0, display->black, ZP);  /* Bottom */
    
    /* Draw text */
    if(ctrl->text) {
        textpos.x = r.min.x + 3;
        textpos.y = r.min.y + (Dy(r) - form->font->height) / 2 + form->font->ascent;
        string(form->image, textpos, display->black, ZP, form->font, ctrl->text);
    }
}

/* Label drawing - render static text computation */
void
vb9_label_draw(Control *ctrl)
{
    VB9Form *form;
    Rectangle r;
    Point textpos;
    
    if(ctrl == nil || g_runtime == nil || g_runtime->activeform == nil)
        return;
        
    form = g_runtime->activeform;
    r = rectaddpt(ctrl->r, form->image->r.min);
    
    /* Draw text - no background, transparent like VB6 */
    if(ctrl->text) {
        textpos.x = r.min.x;
        textpos.y = r.min.y + form->font->ascent;
        string(form->image, textpos, display->black, ZP, form->font, ctrl->text);
    }
}

/* ListBox drawing - render list computation */
void
vb9_listbox_draw(Control *ctrl)
{
    VB9Form *form;
    Rectangle r;
    
    if(ctrl == nil || g_runtime == nil || g_runtime->activeform == nil)
        return;
        
    form = g_runtime->activeform;
    r = rectaddpt(ctrl->r, form->image->r.min);
    
    /* Draw white background */
    draw(form->image, r, display->white, nil, ZP);
    
    /* Draw border */
    line(form->image, r.min, Pt(r.max.x-1, r.min.y), 
         0, 0, 0, display->black, ZP);  /* Top */
    line(form->image, r.min, Pt(r.min.x, r.max.y-1), 
         0, 0, 0, display->black, ZP);  /* Left */
    line(form->image, Pt(r.max.x-1, r.min.y), Pt(r.max.x-1, r.max.y-1), 
         0, 0, 0, display->black, ZP);  /* Right */
    line(form->image, Pt(r.min.x, r.max.y-1), Pt(r.max.x-1, r.max.y-1), 
         0, 0, 0, display->black, ZP);  /* Bottom */
    
    /* TODO: Draw list items */
}

/* Button click handler - execute button computation */
void
vb9_button_click(Control *ctrl)
{
    if(ctrl == nil)
        return;
        
    /* Default button action - print to console for now */
    print("Button '%s' clicked!\n", ctrl->name);
    
    /* TODO: Execute user-defined event handler */
    /* This is where we'd read from /form/buttonname/click */
}

/* TextBox change handler */
void
vb9_textbox_change(Control *ctrl)
{
    if(ctrl == nil)
        return;
        
    print("TextBox '%s' changed to: %s\n", ctrl->name, ctrl->text);
}

/* ListBox select handler */
void
vb9_listbox_select(Control *ctrl)
{
    if(ctrl == nil)
        return;
        
    print("ListBox '%s' selection changed\n", ctrl->name);
}

/* Control property accessors */
void
vb9_control_set_text(Control *ctrl, char *text)
{
    if(ctrl == nil)
        return;
        
    if(ctrl->text)
        free(ctrl->text);
    ctrl->text = strdup(text ? text : "");
}

char*
vb9_control_get_text(Control *ctrl)
{
    return (ctrl && ctrl->text) ? ctrl->text : "";
}

void
vb9_control_move(Control *ctrl, Point p)
{
    if(ctrl == nil)
        return;
        
    ctrl->r = rectaddpt(ctrl->r, subpt(p, ctrl->r.min));
}

void
vb9_control_resize(Control *ctrl, Point size)
{
    if(ctrl == nil)
        return;
        
    ctrl->r.max = addpt(ctrl->r.min, size);
}