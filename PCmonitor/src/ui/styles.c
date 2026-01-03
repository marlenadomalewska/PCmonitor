#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: secondary_arc
//

void init_style_secondary_arc_KNOB_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_opa(style, 0);
};

lv_style_t *get_style_secondary_arc_KNOB_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_secondary_arc_KNOB_DEFAULT(style);
    }
    return style;
};

void init_style_secondary_arc_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_arc_rounded(style, false);
};

lv_style_t *get_style_secondary_arc_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_secondary_arc_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_secondary_arc_INDICATOR_DEFAULT(lv_style_t *style) {
    lv_style_set_arc_rounded(style, false);
    lv_style_set_arc_color(style, lv_color_hex(0xff9000ff));
};

lv_style_t *get_style_secondary_arc_INDICATOR_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_secondary_arc_INDICATOR_DEFAULT(style);
    }
    return style;
};

void add_style_secondary_arc(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_secondary_arc_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_secondary_arc_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_secondary_arc_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
};

void remove_style_secondary_arc(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_secondary_arc_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_secondary_arc_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_secondary_arc_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
};

//
// Style: main_arc
//

void init_style_main_arc_KNOB_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_opa(style, 0);
};

lv_style_t *get_style_main_arc_KNOB_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_main_arc_KNOB_DEFAULT(style);
    }
    return style;
};

void init_style_main_arc_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_arc_rounded(style, false);
};

lv_style_t *get_style_main_arc_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_main_arc_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_main_arc_INDICATOR_DEFAULT(lv_style_t *style) {
    lv_style_set_arc_rounded(style, false);
    lv_style_set_arc_color(style, lv_color_hex(0xff00e0a5));
};

lv_style_t *get_style_main_arc_INDICATOR_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_main_arc_INDICATOR_DEFAULT(style);
    }
    return style;
};

void add_style_main_arc(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_main_arc_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_main_arc_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_main_arc_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
};

void remove_style_main_arc(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_main_arc_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_main_arc_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_main_arc_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
};

//
// Style: temperature_bar
//

void init_style_temperature_bar_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_radius(style, 0);
    lv_style_set_bg_color(style, lv_color_hex(0xff00ffaa));
};

lv_style_t *get_style_temperature_bar_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_temperature_bar_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_temperature_bar_INDICATOR_DEFAULT(lv_style_t *style) {
    lv_style_set_radius(style, 0);
    lv_style_set_bg_color(style, lv_color_hex(0xff00ffaa));
    lv_style_set_bg_grad_dir(style, LV_GRAD_DIR_HOR);
    lv_style_set_bg_grad_color(style, lv_color_hex(0xffcd03de));
    lv_style_set_bg_main_stop(style, 185);
    lv_style_set_bg_grad_stop(style, 255);
};

lv_style_t *get_style_temperature_bar_INDICATOR_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_temperature_bar_INDICATOR_DEFAULT(style);
    }
    return style;
};

void add_style_temperature_bar(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_temperature_bar_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_temperature_bar_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
};

void remove_style_temperature_bar(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_temperature_bar_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_temperature_bar_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
};

//
// Style: unit
//

void init_style_unit_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0xff636872));
};

lv_style_t *get_style_unit_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_unit_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_unit(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_unit_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_unit(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_unit_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: testbtn
//

void init_style_testbtn_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xff21f324));
};

lv_style_t *get_style_testbtn_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_testbtn_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_testbtn_MAIN_CHECKED(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xff8b2a40));
};

lv_style_t *get_style_testbtn_MAIN_CHECKED() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_testbtn_MAIN_CHECKED(style);
    }
    return style;
};

void add_style_testbtn(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_testbtn_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_testbtn_MAIN_CHECKED(), LV_PART_MAIN | LV_STATE_CHECKED);
};

void remove_style_testbtn(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_testbtn_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_testbtn_MAIN_CHECKED(), LV_PART_MAIN | LV_STATE_CHECKED);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_secondary_arc,
        add_style_main_arc,
        add_style_temperature_bar,
        add_style_unit,
        add_style_testbtn,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_secondary_arc,
        remove_style_main_arc,
        remove_style_temperature_bar,
        remove_style_unit,
        remove_style_testbtn,
    };
    remove_style_funcs[styleIndex](obj);
}

