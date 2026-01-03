#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *obj0;
    lv_obj_t *obj0__wifi;
    lv_obj_t *slider_brightness;
    lv_obj_t *tabview;
    lv_obj_t *test;
    lv_obj_t *obj1;
    lv_obj_t *obj2;
    lv_obj_t *obj3;
    lv_obj_t *bar_cpu_temperature;
    lv_obj_t *arc_cpu_load_core_max;
    lv_obj_t *arc_cpu_load_total;
    lv_obj_t *label_cpu_load_core_max;
    lv_obj_t *label_cpu_load_total;
    lv_obj_t *label_cpu_power;
    lv_obj_t *obj4;
    lv_obj_t *label_cpu_temperature;
    lv_obj_t *bar_gpu_temp_core;
    lv_obj_t *bar_gpu_temp_hot_spot;
    lv_obj_t *arc_gpu_load_memory;
    lv_obj_t *arc_gpu_load_core;
    lv_obj_t *label_gpu_load_memory;
    lv_obj_t *label_gpu_load_core;
    lv_obj_t *label_gpu_power;
    lv_obj_t *label_gpu_framerate;
    lv_obj_t *obj5;
    lv_obj_t *label_gpu_temp_core;
    lv_obj_t *arc_ram_percentage_virtual_details;
    lv_obj_t *arc_ram_percentage_details;
    lv_obj_t *label_ram_percentage_virtual_details;
    lv_obj_t *label_ram_percentage_details;
    lv_obj_t *label_ram_used;
    lv_obj_t *label_ram_available;
    lv_obj_t *label_ram_virtual_available;
    lv_obj_t *label_ram_virtual_used;
    lv_obj_t *obj6;
    lv_obj_t *label_date;
    lv_obj_t *label_second;
    lv_obj_t *label_minute;
    lv_obj_t *label_hour;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
};

void create_screen_main();
void tick_screen_main();

void create_user_widget_wifi_widget(lv_obj_t *parent_obj, void *flowState, int startWidgetIndex);
void tick_user_widget_wifi_widget(void *flowState, int startWidgetIndex);

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/