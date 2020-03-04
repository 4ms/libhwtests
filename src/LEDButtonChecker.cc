#include "LEDButtonChecker.h"

LEDButtonChecker::LEDButtonChecker(unsigned num_buttons) 
	: _num_buttons(num_buttons) {
		//static_assert(num_buttons<31,"Max 31 buttons");		
		if (num_buttons>31)
			num_buttons=31;
		_buttons_state = 0;
	}

void LEDButtonChecker::assign_button_read_func(ButtonReadFuncType func) {
	_button_read = func;
}

void LEDButtonChecker::assign_button_led_func(ButtonLEDSetFuncType func) {
	_set_led = func;
}

void LEDButtonChecker::reset() {
	_buttons_state = 0;
	for (unsigned i=0;i<_num_buttons;i++) {
		_set_led(i, true);
	}
}

bool LEDButtonChecker::run_check() {
	uint8_t buttons_pressed = 0;
	for (uint8_t button_num=0; button_num<_num_buttons; button_num++) {
		if (_button_read(button_num))
			buttons_pressed++;
	}
	if (buttons_pressed == 1) {
		for (uint8_t button_num=0; button_num<_num_buttons; button_num++) {
			if (_button_read(button_num)) {
				_set_led(button_num, false);
				_buttons_state &= ~(1<<button_num);
			}
		}
	}
	return (_buttons_state != 0);
}

