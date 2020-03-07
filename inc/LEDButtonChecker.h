#pragma once
#include "libhwtests_common.h"

class LEDButtonChecker {
public:
	LEDButtonChecker(unsigned num_buttons);
	void assign_button_read_func(ButtonReadFuncType func);
	void assign_button_led_func(ButtonLEDSetFuncType func);

	void reset();
	void run_check();
	bool check_done();

private:
	uint8_t _num_buttons;
	uint32_t _buttons_state;
	ButtonReadFuncType _button_read;
	ButtonLEDSetFuncType _set_led;
};

