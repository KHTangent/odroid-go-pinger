#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: Buttons
lv_style_t *get_style_buttons_MAIN_DEFAULT();
lv_style_t *get_style_buttons_MAIN_PRESSED();
lv_style_t *get_style_buttons_MAIN_FOCUSED();
void add_style_buttons(lv_obj_t *obj);
void remove_style_buttons(lv_obj_t *obj);



#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/