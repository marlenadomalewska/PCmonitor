#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;

void create_screen_main() {
    void *flowState = getFlowState(0, 0);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 320);
    {
        lv_obj_t *parent_obj = obj;
        {
            // tabview
            lv_obj_t *obj = lv_tabview_create(parent_obj);
            objects.tabview = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 480, 320);
            lv_tabview_set_tab_bar_position(obj, LV_DIR_TOP);
            lv_tabview_set_tab_bar_size(obj, 50);
            lv_obj_set_style_text_font(obj, &ui_font_michroma_15, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // tabALL
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "ALL");
                    objects.tab_all = obj;
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // test
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.test = obj;
                            lv_obj_set_pos(obj, 0, 92);
                            lv_obj_set_size(obj, 445, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Text");
                        }
                    }
                }
                {
                    // tabCPU
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "CPU");
                    objects.tab_cpu = obj;
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 5, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_michroma_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Power");
                        }
                        {
                            // label_cpu_power
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.label_cpu_power = obj;
                            lv_obj_set_pos(obj, 5, 72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &ui_font_michroma_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "0,0 W");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 5, 114);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_michroma_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Temperature");
                        }
                        {
                            // label_cpu_temperature
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.label_cpu_temperature = obj;
                            lv_obj_set_pos(obj, 5, 145);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &ui_font_michroma_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "100 C");
                        }
                        {
                            // arc_cpu_load_core_max
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.arc_cpu_load_core_max = obj;
                            lv_obj_set_pos(obj, 223, 10);
                            lv_obj_set_size(obj, 214, 214);
                            lv_arc_set_value(obj, 25);
                            lv_arc_set_mode(obj, LV_ARC_MODE_REVERSE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_rounded(obj, false, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_rounded(obj, false, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_color(obj, lv_color_hex(0xff5e00a6), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                        }
                        {
                            // arc_cpu_load_total
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.arc_cpu_load_total = obj;
                            lv_obj_set_pos(obj, 213, 0);
                            lv_obj_set_size(obj, 235, 235);
                            lv_arc_set_value(obj, 25);
                            lv_arc_set_mode(obj, LV_ARC_MODE_REVERSE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_rounded(obj, false, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_rounded(obj, false, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_color(obj, lv_color_hex(0xff00805e), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 44, 48);
                                    lv_obj_set_size(obj, 148, LV_SIZE_CONTENT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "Total");
                                }
                                {
                                    // label_cpu_load_core_max
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.label_cpu_load_core_max = obj;
                                    lv_obj_set_pos(obj, 44, 120);
                                    lv_obj_set_size(obj, 148, 37);
                                    lv_obj_set_style_text_font(obj, &ui_font_michroma_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "100");
                                }
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 44, 102);
                                    lv_obj_set_size(obj, 148, LV_SIZE_CONTENT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "Max");
                                }
                                {
                                    // label_cpu_load_total
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.label_cpu_load_total = obj;
                                    lv_obj_set_pos(obj, 44, 66);
                                    lv_obj_set_size(obj, 148, 37);
                                    lv_obj_set_style_text_font(obj, &ui_font_michroma_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "100");
                                }
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 104, 191);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "%");
                                }
                            }
                        }
                    }
                }
                {
                    // tabGPU
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "GPU");
                    objects.tab_gpu = obj;
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // label_gpu_temp_core
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.label_gpu_temp_core = obj;
                            lv_obj_set_pos(obj, 5, 200);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &ui_font_michroma_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "99,9 C");
                        }
                        {
                            // bar_gpu_temp_core
                            lv_obj_t *obj = lv_bar_create(parent_obj);
                            objects.bar_gpu_temp_core = obj;
                            lv_obj_set_pos(obj, 5, 174);
                            lv_obj_set_size(obj, 181, 17);
                            lv_bar_set_value(obj, 100, LV_ANIM_OFF);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff00ffaa), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff00ffaa), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xffcd03de), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_grad_stop(obj, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_main_stop(obj, 185, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                        }
                        {
                            // bar_gpu_temp_hot_spot
                            lv_obj_t *obj = lv_bar_create(parent_obj);
                            objects.bar_gpu_temp_hot_spot = obj;
                            lv_obj_set_pos(obj, 5, 174);
                            lv_obj_set_size(obj, 181, 17);
                            lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
                            lv_bar_set_value(obj, 25, LV_ANIM_OFF);
                            lv_bar_set_start_value(obj, 24, LV_ANIM_OFF);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffcd03de), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_shadow_spread(obj, 3, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xffcd03de), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_shadow_opa(obj, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_shadow_width(obj, 20, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 5, 144);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_michroma_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Temperature");
                        }
                        {
                            // label_gpu_framerate
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.label_gpu_framerate = obj;
                            lv_obj_set_pos(obj, 5, 102);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &ui_font_michroma_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "100 FPS");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 5, 71);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_michroma_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Framerate");
                        }
                        {
                            // label_gpu_power
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.label_gpu_power = obj;
                            lv_obj_set_pos(obj, 5, 29);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &ui_font_michroma_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "0,0 W");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 5, -1);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_michroma_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Power");
                        }
                        {
                            // arc_gpu_load_memory
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.arc_gpu_load_memory = obj;
                            lv_obj_set_pos(obj, 223, 10);
                            lv_obj_set_size(obj, 214, 214);
                            lv_arc_set_value(obj, 25);
                            lv_arc_set_mode(obj, LV_ARC_MODE_REVERSE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_rounded(obj, false, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_rounded(obj, false, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_color(obj, lv_color_hex(0xff5e00a6), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                        }
                        {
                            // arc_gpu_load_core
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.arc_gpu_load_core = obj;
                            lv_obj_set_pos(obj, 213, 0);
                            lv_obj_set_size(obj, 235, 235);
                            lv_arc_set_value(obj, 25);
                            lv_arc_set_mode(obj, LV_ARC_MODE_REVERSE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_rounded(obj, false, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_rounded(obj, false, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_color(obj, lv_color_hex(0xff00805e), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 44, 48);
                                    lv_obj_set_size(obj, 148, LV_SIZE_CONTENT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "Core");
                                }
                                {
                                    // label_gpu_load_memory
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.label_gpu_load_memory = obj;
                                    lv_obj_set_pos(obj, 44, 120);
                                    lv_obj_set_size(obj, 148, 37);
                                    lv_obj_set_style_text_font(obj, &ui_font_michroma_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "100");
                                }
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 44, 102);
                                    lv_obj_set_size(obj, 148, LV_SIZE_CONTENT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "Memory");
                                }
                                {
                                    // label_gpu_load_core
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.label_gpu_load_core = obj;
                                    lv_obj_set_pos(obj, 44, 66);
                                    lv_obj_set_size(obj, 148, 37);
                                    lv_obj_set_style_text_font(obj, &ui_font_michroma_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "100");
                                }
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 104, 191);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "%");
                                }
                            }
                        }
                    }
                }
                {
                    // tabRAM
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "RAM");
                    objects.tab_ram = obj;
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            objects.obj0 = obj;
                            lv_obj_set_pos(obj, 385, 110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            static lv_point_precise_t line_points[] = {
                                { 0, 0 },
                                { 70, 0 }
                            };
                            lv_line_set_points(obj, line_points, 2);
                            lv_obj_set_style_line_width(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0xff00fffa), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff00fffa), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_shadow_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_shadow_spread(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 294, 178);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Available");
                        }
                        {
                            // label_ram_virtual_available
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.label_ram_virtual_available = obj;
                            lv_obj_set_pos(obj, 294, 197);
                            lv_obj_set_size(obj, 152, 32);
                            lv_obj_set_style_text_font(obj, &ui_font_michroma_25, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "24,0 GB");
                        }
                        {
                            // label_ram_virtual_used
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.label_ram_virtual_used = obj;
                            lv_obj_set_pos(obj, 294, 144);
                            lv_obj_set_size(obj, 152, 34);
                            lv_obj_set_style_text_font(obj, &ui_font_michroma_25, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "24,0 GB");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 294, 126);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Used");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 294, 98);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &ui_font_michroma_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Virtual");
                        }
                        {
                            // label_ram_available
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.label_ram_available = obj;
                            lv_obj_set_pos(obj, 294, 66);
                            lv_obj_set_size(obj, 152, 32);
                            lv_obj_set_style_text_font(obj, &ui_font_michroma_25, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "24,0 GB");
                        }
                        {
                            // label_ram_used
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.label_ram_used = obj;
                            lv_obj_set_pos(obj, 294, 13);
                            lv_obj_set_size(obj, 152, 34);
                            lv_obj_set_style_text_font(obj, &ui_font_michroma_25, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "24,0 GB");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 294, 47);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Available");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 294, -5);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Used");
                        }
                        {
                            // arc_ram_percentage_virtual_details
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.arc_ram_percentage_virtual_details = obj;
                            lv_obj_set_pos(obj, 19, 12);
                            lv_obj_set_size(obj, 214, 214);
                            lv_arc_set_value(obj, 25);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_rounded(obj, false, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_rounded(obj, false, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_color(obj, lv_color_hex(0xff00fffa), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                        }
                        {
                            // arc_ram_percentage_details
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.arc_ram_percentage_details = obj;
                            lv_obj_set_pos(obj, 9, 2);
                            lv_obj_set_size(obj, 235, 235);
                            lv_arc_set_value(obj, 25);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_rounded(obj, false, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_rounded(obj, false, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_color(obj, lv_color_hex(0xffec00ff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 95, 48);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Core");
                                }
                                {
                                    // label_ram_percentage_virtual_details
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.label_ram_percentage_virtual_details = obj;
                                    lv_obj_set_pos(obj, 44, 122);
                                    lv_obj_set_size(obj, 148, 37);
                                    lv_obj_set_style_text_font(obj, &ui_font_michroma_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "100");
                                }
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 87, 102);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Virtual");
                                }
                                {
                                    // label_ram_percentage_details
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.label_ram_percentage_details = obj;
                                    lv_obj_set_pos(obj, 44, 66);
                                    lv_obj_set_size(obj, 148, 37);
                                    lv_obj_set_style_text_font(obj, &ui_font_michroma_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "100");
                                }
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 104, 191);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "%");
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
    void *flowState = getFlowState(0, 0);
    (void)flowState;
}


static const char *screen_names[] = { "Main" };
static const char *object_names[] = { "main", "tabview", "tab_all", "test", "tab_cpu", "label_cpu_power", "label_cpu_temperature", "arc_cpu_load_core_max", "arc_cpu_load_total", "label_cpu_load_core_max", "label_cpu_load_total", "tab_gpu", "label_gpu_temp_core", "bar_gpu_temp_core", "bar_gpu_temp_hot_spot", "label_gpu_framerate", "label_gpu_power", "arc_gpu_load_memory", "arc_gpu_load_core", "label_gpu_load_memory", "label_gpu_load_core", "tab_ram", "obj0", "label_ram_virtual_available", "label_ram_virtual_used", "label_ram_available", "label_ram_used", "arc_ram_percentage_virtual_details", "arc_ram_percentage_details", "label_ram_percentage_virtual_details", "label_ram_percentage_details" };


typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    eez_flow_init_screen_names(screen_names, sizeof(screen_names) / sizeof(const char *));
    eez_flow_init_object_names(object_names, sizeof(object_names) / sizeof(const char *));
    
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
}
