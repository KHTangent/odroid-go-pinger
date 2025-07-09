#include <zephyr/kernel.h>


int odroid_go_buttons_init() {
	return 0;
}

void odroid_go_buttons_task(void* p1, void* p2, void* p3) {

}


K_THREAD_DEFINE(ODROID_GO_BUTTONS_THREAD, 1024, odroid_go_buttons_task, NULL, NULL, NULL, 4, 0, 0);
