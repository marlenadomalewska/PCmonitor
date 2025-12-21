#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *tabview;
    lv_obj_t *all;
    lv_obj_t *sensor_value;
    lv_obj_t *cpu;
    lv_obj_t *gpu;
    lv_obj_t *gpu_voltage_max;
    lv_obj_t *gpu_voltage;
    lv_obj_t *ram;
    lv_obj_t *obj0;
    lv_obj_t *ram_virtual_available;
    lv_obj_t *ram_virtual_used;
    lv_obj_t *ram_available;
    lv_obj_t *ram_used;
    lv_obj_t *ram_percentage_virtual_details_arc;
    lv_obj_t *ram_percentage_details_arc;
    lv_obj_t *ram_percentage_virtual_details;
    lv_obj_t *ram_percentage_details;
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