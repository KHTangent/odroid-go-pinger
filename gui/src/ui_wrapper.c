#include <lvgl.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>

#include "app/drivers/odroid_go_buttons.h"
#include "core/lv_group.h"
#include "indev/lv_indev.h"
#include "ui.h"

static const struct device* display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

lv_key_t last_pressed_key;
int64_t last_pressed_timestamp = 0;
lv_key_t last_released_key;
int64_t last_released_timestamp = 1;

void button_callback(const struct zbus_channel* channel) {
	if (channel != &button_event_channel) {
		return;
	}
	const struct ButtonEvent* event = zbus_chan_const_msg(channel);
	printf("UI thread received event %d\n", event->type);

	lv_key_t lv_key;
	switch (event->type) {
		case BUTTON_A_PRESSED:
		case BUTTON_A_RELEASED:
			lv_key = LV_KEY_ENTER;
			break;
		case BUTTON_B_PRESSED:
		case BUTTON_B_RELEASED:
			lv_key = LV_KEY_BACKSPACE;
			break;
		case BUTTON_UP_PRESSED:
		case BUTTON_UP_RELEASED:
			lv_key = LV_KEY_UP;
			break;
		case BUTTON_DOWN_PRESSED:
		case BUTTON_DOWN_RELEASED:
			lv_key = LV_KEY_DOWN;
			break;
		case BUTTON_LEFT_PRESSED:
		case BUTTON_LEFT_RELEASED:
			lv_key = LV_KEY_PREV;
			break;
		case BUTTON_RIGHT_PRESSED:
		case BUTTON_RIGHT_RELEASED:
			lv_key = LV_KEY_NEXT;
			break;
		default:
			lv_key = -1;
			break;
	}
	if (lv_key != -1 && button_event_is_press(event->type)) {
		last_pressed_timestamp = k_uptime_get();
		last_pressed_key = lv_key;
	}
	if (lv_key != -1 && button_event_is_release(event->type)) {
		last_released_timestamp = k_uptime_get();
		last_released_key = lv_key;
	}
}

ZBUS_LISTENER_DEFINE(button_observer, button_callback);
ZBUS_CHAN_ADD_OBS(button_event_channel, ZBUS_OBSERVERS(button_observer), 0);

void keypad_check_callback(lv_indev_t* indev, lv_indev_data_t* data) {
	if (last_pressed_timestamp > last_released_timestamp) {
		data->key = last_pressed_key;
		data->state = LV_INDEV_STATE_PRESSED;
	} else {
		data->key = last_released_key;
		data->state = LV_INDEV_STATE_RELEASED;
	}
}

void ui_thread_task(void* p1, void* p2, void* p3) {
	if (!device_is_ready(display)) {
		printf("Could not init display\n");
		return;
	}

	lv_indev_t* keypad_input = lv_indev_create();
	lv_indev_set_type(keypad_input, LV_INDEV_TYPE_KEYPAD);
	lv_indev_set_read_cb(keypad_input, keypad_check_callback);

	ui_create_groups();
	lv_indev_set_group(keypad_input, groups.menu_buttons_group);

	lv_task_handler();

	display_blanking_off(display);

	ui_init();

	while (true) {
		lv_task_handler();
		ui_tick();
		k_sleep(K_MSEC(50));
	}
}

K_THREAD_DEFINE(UI_THREAD, 4096, ui_thread_task, NULL, NULL, NULL, 4, 0, 0);
