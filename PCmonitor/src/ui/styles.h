#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: secondary_arc
lv_style_t *get_style_secondary_arc_KNOB_DEFAULT();
lv_style_t *get_style_secondary_arc_MAIN_DEFAULT();
lv_style_t *get_style_secondary_arc_INDICATOR_DEFAULT();
void add_style_secondary_arc(lv_obj_t *obj);
void remove_style_secondary_arc(lv_obj_t *obj);

// Style: main_arc
lv_style_t *get_style_main_arc_KNOB_DEFAULT();
lv_style_t *get_style_main_arc_MAIN_DEFAULT();
lv_style_t *get_style_main_arc_INDICATOR_DEFAULT();
void add_style_main_arc(lv_obj_t *obj);
void remove_style_main_arc(lv_obj_t *obj);

// Style: temperature_bar
lv_style_t *get_style_temperature_bar_MAIN_DEFAULT();
lv_style_t *get_style_temperature_bar_INDICATOR_DEFAULT();
void add_style_temperature_bar(lv_obj_t *obj);
void remove_style_temperature_bar(lv_obj_t *obj);

// Style: unit
lv_style_t *get_style_unit_MAIN_DEFAULT();
void add_style_unit(lv_obj_t *obj);
void remove_style_unit(lv_obj_t *obj);

// Style: testbtn
lv_style_t *get_style_testbtn_MAIN_DEFAULT();
lv_style_t *get_style_testbtn_MAIN_CHECKED();
void add_style_testbtn(lv_obj_t *obj);
void remove_style_testbtn(lv_obj_t *obj);



#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/