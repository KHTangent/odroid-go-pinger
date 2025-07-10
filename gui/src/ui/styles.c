#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "screens.h"

//
// Style: Buttons
//

void init_style_buttons_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_font(style, &ui_font_comic);
};

lv_style_t *get_style_buttons_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_buttons_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_buttons_MAIN_PRESSED(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xff1c649d));
};

lv_style_t *get_style_buttons_MAIN_PRESSED() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_buttons_MAIN_PRESSED(style);
    }
    return style;
};

void init_style_buttons_MAIN_FOCUSED(lv_style_t *style) {
    lv_style_set_border_color(style, lv_color_hex(0xff1eff00));
    lv_style_set_border_width(style, 5);
    lv_style_set_border_side(style, LV_BORDER_SIDE_FULL);
    lv_style_set_border_opa(style, 255);
};

lv_style_t *get_style_buttons_MAIN_FOCUSED() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_buttons_MAIN_FOCUSED(style);
    }
    return style;
};

void add_style_buttons(lv_obj_t *obj) {
    lv_obj_add_style(obj, get_style_buttons_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_buttons_MAIN_PRESSED(), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_add_style(obj, get_style_buttons_MAIN_FOCUSED(), LV_PART_MAIN | LV_STATE_FOCUSED);
};

void remove_style_buttons(lv_obj_t *obj) {
    lv_obj_remove_style(obj, get_style_buttons_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_buttons_MAIN_PRESSED(), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_remove_style(obj, get_style_buttons_MAIN_FOCUSED(), LV_PART_MAIN | LV_STATE_FOCUSED);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_buttons,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_buttons,
    };
    remove_style_funcs[styleIndex](obj);
}

