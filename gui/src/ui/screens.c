#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

groups_t groups;
static bool groups_created = false;

objects_t objects;
lv_obj_t *tick_value_change_obj;

static void event_handler_cb_main_main(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_SCREEN_LOAD_START) {
        // group: menu_buttons_group
        lv_group_remove_all_objs(groups.menu_buttons_group);
        lv_group_add_obj(groups.menu_buttons_group, objects.stop_scan_button);
        lv_group_add_obj(groups.menu_buttons_group, objects.start_scan_button);
    }
}

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_add_event_cb(obj, event_handler_cb_main_main, LV_EVENT_ALL, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0066ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 5, 173);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Odroid Go Pinger");
            lv_obj_set_style_text_font(obj, &ui_font_comic, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // networks
            lv_obj_t *obj = lv_list_create(parent_obj);
            objects.networks = obj;
            lv_obj_set_pos(obj, 117, 20);
            lv_obj_set_size(obj, 180, 100);
        }
        {
            // start scan button
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.start_scan_button = obj;
            lv_obj_set_pos(obj, 5, 20);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_scan_pressed, LV_EVENT_PRESSED, (void *)0);
            add_style_buttons(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Scan");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // stop scan button
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.stop_scan_button = obj;
            lv_obj_set_pos(obj, 5, 95);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_stop_scan_pressed, LV_EVENT_PRESSED, (void *)0);
            add_style_buttons(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Stop");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
    }
}

void tick_screen_main() {
}


void ui_create_groups() {
    if (!groups_created) {
        groups.menu_buttons_group = lv_group_create();
        groups_created = true;
    }
}

void create_screens() {
    ui_create_groups();
    
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
}

typedef void (*tick_screen_func_t)();

tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
};

void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
