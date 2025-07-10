#include "app/drivers/odroid_go_buttons.h"

#include <stdint.h>
#include <stdio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/dt-bindings/input/input-event-codes.h>
#include <zephyr/input/input.h>
#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>

static const struct device* const buttons_input = DEVICE_DT_GET(DT_COMPAT_GET_ANY_STATUS_OKAY(gpio_keys));

static const struct device* const dpad_adc_dev = DEVICE_DT_GET(DT_ALIAS(dpad_adc));
#define DPAD_X_CHANNEL DT_ALIAS(dpad_x_adc_channel)
static const struct adc_channel_cfg dpad_x_channel = ADC_CHANNEL_CFG_DT(DPAD_X_CHANNEL);
#define DPAD_Y_CHANNEL DT_ALIAS(dpad_y_adc_channel)
static const struct adc_channel_cfg dpad_y_channel = ADC_CHANNEL_CFG_DT(DPAD_Y_CHANNEL);

static int odroid_go_buttons_init() {
	printf("Button driver init\n");
	adc_channel_setup(dpad_adc_dev, &dpad_x_channel);
	adc_channel_setup(dpad_adc_dev, &dpad_y_channel);
	return 0;
}

enum DpadState {
	DPAD_STATE_NONE = 0,
	DPAD_STATE_PLUS = 1,
	DPAD_STATE_MINUS = -1,
};

static enum DpadState state_from_value(int16_t value) {
	if (value > 1000 && value < 2000) {
		return DPAD_STATE_MINUS;
	} else if (value > 2000) {
		return DPAD_STATE_PLUS;
	} else {
		return DPAD_STATE_NONE;
	}
}

static void handle_dpad_buttons(int16_t value_x, int16_t value_y) {
	static enum DpadState last_state_x = DPAD_STATE_NONE;
	static enum DpadState last_state_y = DPAD_STATE_NONE;

	enum DpadState state_x = state_from_value(value_x);
	enum DpadState state_y = state_from_value(value_y);

	struct ButtonEvent event = {
		.type = BUTTON_UNKNOWN,
	};
	if (state_x != last_state_x) {
		switch (last_state_x) {
			case DPAD_STATE_NONE:
				event.type = state_x == DPAD_STATE_PLUS ? BUTTON_LEFT_PRESSED : BUTTON_RIGHT_PRESSED;
				break;
			case DPAD_STATE_PLUS:
				event.type = BUTTON_LEFT_RELEASED;
				break;
			case DPAD_STATE_MINUS:
				event.type = BUTTON_RIGHT_RELEASED;
				break;
		}
	}
	if (event.type != BUTTON_UNKNOWN) {
		zbus_chan_pub(&button_event_channel, &event, K_MSEC(100));
	}

	event.type = BUTTON_UNKNOWN;
	if (state_y != last_state_y) {
		switch (last_state_y) {
			case DPAD_STATE_NONE:
				event.type = state_y == DPAD_STATE_PLUS ? BUTTON_UP_PRESSED : BUTTON_DOWN_PRESSED;
				break;
			case DPAD_STATE_PLUS:
				event.type = BUTTON_UP_RELEASED;
				break;
			case DPAD_STATE_MINUS:
				event.type = BUTTON_DOWN_RELEASED;
				break;
		}
	}
	if (event.type != BUTTON_UNKNOWN) {
		zbus_chan_pub(&button_event_channel, &event, K_MSEC(100));
	}

	last_state_x = state_x;
	last_state_y = state_y;
}

static void odroid_go_button_event_callback(struct input_event* ev, void* user_data) {
	struct ButtonEvent event;
	switch (ev->code) {
		case INPUT_KEY_A:
			event.type = ev->value == 1 ? BUTTON_A_PRESSED : BUTTON_A_RELEASED;
			break;
		case INPUT_KEY_B:
			event.type = ev->value == 1 ? BUTTON_B_PRESSED : BUTTON_B_RELEASED;
			break;
		case INPUT_KEY_MENU:
			event.type = ev->value == 1 ? BUTTON_MENU_PRESSED : BUTTON_MENU_RELEASED;
			break;
		case INPUT_KEY_0:
			event.type = ev->value == 1 ? BUTTON_VOLUME_PRESSED : BUTTON_VOLUME_RELEASED;
			break;
		case INPUT_BTN_SELECT:
			event.type = ev->value == 1 ? BUTTON_SELECT_PRESSED : BUTTON_SELECT_RELEASED;
			break;
		case INPUT_BTN_START:
			event.type = ev->value == 1 ? BUTTON_START_PRESSED : BUTTON_START_RELEASED;
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

	int16_t buffer_x;
	struct adc_sequence sequence_x = {
		.channels = BIT(dpad_x_channel.channel_id),
		.buffer = &buffer_x,
		.buffer_size = sizeof(buffer_x),
		.resolution = DT_PROP(DPAD_X_CHANNEL, zephyr_resolution)
	};
	int16_t buffer_y;
	struct adc_sequence sequence_y = {
		.channels = BIT(dpad_y_channel.channel_id),
		.buffer = &buffer_y,
		.buffer_size = sizeof(buffer_y),
		.resolution = DT_PROP(DPAD_Y_CHANNEL, zephyr_resolution)
	};

	while (1) {
		adc_read(dpad_adc_dev, &sequence_x);
		adc_read(dpad_adc_dev, &sequence_y);
		handle_dpad_buttons(buffer_x, buffer_y);
		k_sleep(K_MSEC(20));
	}
}

bool button_event_is_press(enum ButtonEventType event) {
	return event <= 10;
}
bool button_event_is_release(enum ButtonEventType event) {
	return event > 10;
}

INPUT_CALLBACK_DEFINE(buttons_input, odroid_go_button_event_callback, NULL);

K_THREAD_DEFINE(ODROID_GO_BUTTONS_THREAD, 1024, odroid_go_buttons_task, NULL, NULL, NULL, 4, 0, 0);

ZBUS_CHAN_DEFINE(
	button_event_channel,
	struct ButtonEvent,
	NULL,
	NULL,
	ZBUS_OBSERVERS_EMPTY,
	ZBUS_MSG_INIT(0)
);
