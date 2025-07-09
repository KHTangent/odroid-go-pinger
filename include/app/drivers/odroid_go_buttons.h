#ifndef ODROID_GO_BUTTONS_H

#include <zephyr/zbus/zbus.h>


enum ButtonEventType {
	BUTTON_A_PRESSED = 1,
	BUTTON_A_RELEASED = 11,
	BUTTON_B_PRESSED = 2,
	BUTTON_B_RELEASED = 22,
};

struct ButtonEvent {
	enum ButtonEventType type;
};

// extern const struct zbus_channel button_event_channel;


#endif // ODROID_GO_BUTTONS_H
