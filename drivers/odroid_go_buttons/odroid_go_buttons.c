#include "app/drivers/odroid_go_buttons.h"

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/dt-bindings/input/input-event-codes.h>
#include <zephyr/input/input.h>
#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>

static const struct device* const buttons_input =
	DEVICE_DT_GET(DT_COMPAT_GET_ANY_STATUS_OKAY(gpio_keys));

static int odroid_go_buttons_init() {
	printf("Button driver init\n");
	return 0;
}

static void
odroid_go_button_event_callback(struct input_event* ev, void* user_data) {
	struct ButtonEvent event;
	switch (ev->code) {
		case INPUT_KEY_A:
			event.type = ev->value == 1 ? BUTTON_A_PRESSED : BUTTON_A_RELEASED;
			break;
		case INPUT_KEY_B:
			event.type = ev->value == 1 ? BUTTON_B_PRESSED : BUTTON_B_RELEASED;
			break;
		default:
			event.type = BUTTON_UNKNOWN;
	}

	if (event.type != BUTTON_UNKNOWN) {
		zbus_chan_pub(&button_event_channel, &event, K_MSEC(100));
	}
}

void odroid_go_buttons_task(void* p1, void* p2, void* p3) {
	if (!device_is_ready(buttons_input)) {
		printf("Warning: Button device not ready\n");
		while (1) {
			k_sleep(K_MSEC(100));
		}
	}

	odroid_go_buttons_init();

	while (1) {
		k_sleep(K_MSEC(100));
	}
}

INPUT_CALLBACK_DEFINE(buttons_input, odroid_go_button_event_callback, NULL);

K_THREAD_DEFINE(
	ODROID_GO_BUTTONS_THREAD,
	1024,
	odroid_go_buttons_task,
	NULL,
	NULL,
	NULL,
	4,
	0,
	0
);

ZBUS_CHAN_DEFINE(
	button_event_channel,
	struct ButtonEvent,
	NULL,
	NULL,
	ZBUS_OBSERVERS_EMPTY,
	ZBUS_MSG_INIT(0)
);
