#ifndef ODROID_GO_BUTTONS_H
#define ODROID_GO_BUTTONS_H

#include <stdbool.h>
#include <zephyr/zbus/zbus.h>

enum ButtonEventType {
	BUTTON_UNKNOWN = 0,
	BUTTON_A_PRESSED = 1,
	BUTTON_A_RELEASED = 11,
	BUTTON_B_PRESSED = 2,
	BUTTON_B_RELEASED = 22,
	BUTTON_MENU_PRESSED = 3,
	BUTTON_MENU_RELEASED = 33,
	BUTTON_VOLUME_PRESSED = 4,
	BUTTON_VOLUME_RELEASED = 44,
	BUTTON_SELECT_PRESSED = 5,
	BUTTON_SELECT_RELEASED = 55,
	BUTTON_START_PRESSED = 6,
	BUTTON_START_RELEASED = 66,

	BUTTON_UP_PRESSED = 7,
	BUTTON_UP_RELEASED = 77,
	BUTTON_DOWN_PRESSED = 8,
	BUTTON_DOWN_RELEASED = 88,
	BUTTON_LEFT_PRESSED = 9,
	BUTTON_LEFT_RELEASED = 99,
	BUTTON_RIGHT_PRESSED = 10,
	BUTTON_RIGHT_RELEASED = 1010,
};

bool button_event_is_press(enum ButtonEventType event);
bool button_event_is_release(enum ButtonEventType event);

struct ButtonEvent {
	enum ButtonEventType type;
};

extern const struct zbus_channel button_event_channel;

#endif  // ODROID_GO_BUTTONS_H
