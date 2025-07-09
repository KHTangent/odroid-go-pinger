#include <lvgl.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>

#include "ui.h"

static const struct device* display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

void ui_thread_task(void* p1, void* p2, void* p3) {
	if (!device_is_ready(display)) {
		printf("Could not init display\n");
		return;
	}

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
