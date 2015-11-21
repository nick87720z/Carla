// generated by Fast Light User Interface Designer (fluid) version 1.0107f

#include <cmath>
#include <FL/fl_draw.H>
#include "../Misc/Util.h"
#include "WidgetPDial.h"
//Copyright (c) 2003-2005 Nasca Octavian Paul
//License: GNU GPL version 2 or later


//static int numobj = 0;

WidgetPDial::WidgetPDial(int x, int y, int w, int h, const char *label)
    :Fl_Dial(x, y, w, h, label), reset_value(0), integer_step(true),
     oldvalue(0.0f), pos(false), textset(false), value_offset(0.0),
     value_scale(1.0)
{
    //cout << "[" << label << "] There are now " << ++numobj << endl;
    Fl_Group *save = Fl_Group::current();
    tipwin = new TipWin();
    tipwin->hide();
    Fl_Group::current(save);
}

WidgetPDial::~WidgetPDial()
{
    //cout << "There are now " << --numobj << endl;
    delete tipwin;
}

void WidgetPDial::setRounding(unsigned int digits)
{
    tipwin->setRounding(digits);
}

#define MOD_MASK (FL_CTRL | FL_SHIFT)

int WidgetPDial::handle(int event)
{
    double dragsize, min = minimum(), max = maximum(), result;
    int dy;

    if (event == FL_RELEASE && Fl::event_clicks() == 1) {
        Fl::event_clicks(0);
        value(reset_value);
        tipwin->hide();
        value_damage();
        if (this->when() != 0)
            do_callback();
        return 1;
    }

    int old_mod_state;

    switch(event) {
        case FL_PUSH:
            mod_state = Fl::event_state() & MOD_MASK;
            if (integer_step)
                setRounding(0);
            else if (mod_state == MOD_MASK)
                setRounding(5);
            else if (mod_state == FL_SHIFT)
                setRounding(4);
            else
                setRounding((Fl::event_button3() || mod_state & FL_CTRL)
                            ? 3 : 2);
            oldvalue = value();
            old_y = Fl::event_y();
        case FL_DRAG:
            getPos();
            old_mod_state = mod_state;
            mod_state = Fl::event_state() & MOD_MASK;
            if (old_mod_state != mod_state) {
                oldvalue = value();
                old_y = Fl::event_y();
                if (integer_step)
                    setRounding(0);
                else if (mod_state == MOD_MASK)
                    setRounding(5);
                else if (mod_state == FL_SHIFT)
                    setRounding(4);
                else
                    setRounding((Fl::event_button3() || mod_state & FL_CTRL)
                                ? 3 : 2);
                break;
            }
            dy = old_y - Fl::event_y();

            if (!integer_step && mod_state == MOD_MASK)
                dragsize = 200000.0f;
            else if (!integer_step && mod_state == FL_SHIFT)
                dragsize = 20000.0f;
            else
                dragsize = (Fl::event_button3() || mod_state & MOD_MASK)
                    ? 1000.0f : 200.0f;

            value(clamp(oldvalue + dy / dragsize * (max - min)));
            tipwin->showValue(transform(value()));
            value_damage();
            if(this->when() != 0)
                do_callback();
            return 1;
        case FL_MOUSEWHEEL:
            if (Fl::event_buttons() || Fl::belowmouse() != this)
                return 1;
            mod_state = Fl::event_state() & MOD_MASK;
            dy = - Fl::event_dy();

            if (integer_step) {
                setRounding(0);
                result = (int)(value() +
                               dy * ((Fl::event_ctrl() ||
                                      Fl::event_shift()) ? 1 : 8));
            } else {
                float dragsize;
                if (mod_state == MOD_MASK) {
                    dragsize = 100000.0;
                    setRounding(5);
                } else if (mod_state == FL_SHIFT) {
                    dragsize = 10000.0;
                    setRounding(4);
                } else if (mod_state == FL_CTRL) {
                    dragsize = 1000.0;
                    setRounding(3);
                } else {
                    dragsize = 100.0;
                    setRounding(2);
                }
                result = value() + dy / dragsize * (max - min);
            }
            value(clamp(result));

            tipwin->showValue(transform(value()));
            value_damage();
            if(this->when() != 0)
                do_callback();
            return 1;
        case FL_ENTER:
            getPos();
            tipwin->showText();
            return 1;
        case FL_HIDE:
        case FL_LEAVE:
            tipwin->hide();
            resetPos();
            break;
        case FL_RELEASE:
            if (integer_step) {
                float rounded = floorf(value() + 0.5);
                value(clamp(rounded));
            }
            tipwin->hide();
            resetPos();
            if(this->when() == 0)
                do_callback();
            return 1;
    }
    return 0;
//#endif
}

void WidgetPDial::draw()
{
#ifdef NTK_GUI
    box( FL_NO_BOX );

    Fl_Dial::draw();
    
    return;
#else
    const int cx = x(), cy = y(), sx = w(), sy = h();
    const double a1 = angle1(), a2 = angle2();
    const double val = (value() - minimum()) / (maximum() - minimum());
    // even radius produces less artifacts if no antialiasing is avail
    const int rad = (sx > sy ? sy : sx) &~1;

    /* clears the button background */
    pdialcolor(160, 160, 160);
    fl_pie(cx - 2, cy - 2, rad + 4, rad + 4, 0, 360);

    /* dark outline */
    fl_color(60, 60, 60);
    fl_pie(cx - 1, cy - 1, rad + 2, rad + 2, 0, 360);

    /* Draws the button faceplate, min/max */
    pdialcolor(110, 110, 115);
    fl_pie(cx, cy, rad, rad, 270 - a2, 270 - a1);

    /* knob center */
    if (rad > 8) {
        pdialcolor(140, 140, 145);
        fl_pie(cx + 4, cy + 4, rad - 8, rad - 8, 0, 360);
    }

    /* value circle */
    double a = -(a2 - a1) * val - a1;
    fl_line_style(0, 2, 0);
    pdialcolor(0, 200, 0);
    fl_arc(cx + 1, cy + 1, rad - 2, rad - 2, a - 90, a1 - 180);
    fl_line_style(0);

    /* draw value line */
    int ll = rad/4;
    if (ll < 2) ll = 2;

    fl_push_matrix();

    fl_translate(cx + rad / 2, cy + rad / 2);
    fl_rotate(a - 90.0f);

    fl_translate(rad / 2, 0);

    fl_begin_polygon();
    pdialcolor(0, 0, 0);
    fl_vertex(-ll, 0);
    fl_vertex(0, 0);
    fl_end_polygon();

    fl_pop_matrix();

#endif
}

void WidgetPDial::pdialcolor(int r, int g, int b)
{
    if(active_r())
        fl_color(r, g, b);
    else
        fl_color(160 - (160 - r) / 3, 160 - (160 - b) / 3, 160 - (160 - b) / 3);
}

void WidgetPDial::tooltip(const char *c)
{
    tipwin->setText(c);
    textset = true;
}

void WidgetPDial::getPos()
{
    if(!pos) {
        tipwin->position(Fl::event_x_root(), Fl::event_y_root() + 20);
        pos = true;
    }
}

void WidgetPDial::resetPos()
{
    pos = false;
}

void WidgetPDial::set_transform(float scale, float offset)
{
    value_offset = offset;
    value_scale = scale;
}

float WidgetPDial::transform(float x)
{
    return value_scale * x + value_offset;
}
