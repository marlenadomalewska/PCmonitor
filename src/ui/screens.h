#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *cpu_core_1;
    lv_obj_t *cpu_core_1__clock_mhz;
    lv_obj_t *cpu_core_1__core_name;
    lv_obj_t *cpu_core_1__clock;
    lv_obj_t *cpu_core_1__temp;
    lv_obj_t *cpu_core_1__temp_c;
    lv_obj_t *cpu_core_2;
    lv_obj_t *cpu_core_2__clock_mhz;
    lv_obj_t *cpu_core_2__core_name;
    lv_obj_t *cpu_core_2__clock;
    lv_obj_t *cpu_core_2__temp;
    lv_obj_t *cpu_core_2__temp_c;
    lv_obj_t *cpu_core_3;
    lv_obj_t *cpu_core_3__clock_mhz;
    lv_obj_t *cpu_core_3__core_name;
    lv_obj_t *cpu_core_3__clock;
    lv_obj_t *cpu_core_3__temp;
    lv_obj_t *cpu_core_3__temp_c;
    lv_obj_t *cpu_core_4;
    lv_obj_t *cpu_core_4__clock_mhz;
    lv_obj_t *cpu_core_4__core_name;
    lv_obj_t *cpu_core_4__clock;
    lv_obj_t *cpu_core_4__temp;
    lv_obj_t *cpu_core_4__temp_c;
    lv_obj_t *cpu_core_5;
    lv_obj_t *cpu_core_5__clock_mhz;
    lv_obj_t *cpu_core_5__core_name;
    lv_obj_t *cpu_core_5__clock;
    lv_obj_t *cpu_core_5__temp;
    lv_obj_t *cpu_core_5__temp_c;
    lv_obj_t *hour;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
};

void create_screen_main();
void tick_screen_main();

void create_user_widget_core_cpu(lv_obj_t *parent_obj, void *flowState, int startWidgetIndex);
void tick_user_widget_core_cpu(void *flowState, int startWidgetIndex);

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/