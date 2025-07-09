#include <stdint.h>
#include <stdio.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>

#include "app/drivers/odroid_go_buttons.h"

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

void button_callback(const struct zbus_channel* channel) {
	if (channel != &button_event_channel) {
		return;
	}
	const struct ButtonEvent* event = zbus_chan_const_msg(channel);
	switch (event->type) {
		case BUTTON_A_PRESSED:
			gpio_pin_set_dt(&led, 1);
			break;
		case BUTTON_A_RELEASED:
			gpio_pin_set_dt(&led, 0);
			break;
		default:
			break;
	}
}

ZBUS_LISTENER_DEFINE(button_observer, button_callback);
ZBUS_CHAN_ADD_OBS(button_event_channel, ZBUS_OBSERVERS(button_observer), 0);

int main(void) {
	if (!gpio_is_ready_dt(&led)) {
		printf("Could not init GPIO\n");
		return 0;
	}

	int pin_config_error = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (pin_config_error < 0) {
		return 0;
	}
	gpio_pin_set_dt(&led, 0);

	while (1) {
		k_sleep(K_MSEC(50));
	}
	return 0;
}
