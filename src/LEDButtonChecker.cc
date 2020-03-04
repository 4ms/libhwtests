#include "LEDButtonChecker.hh"

LEDButtonChecker::LEDButtonChecker(unsigned num_buttons) 
	: _num_buttons(num_buttons) {
		//static_assert(num_buttons<31,"Max 31 buttons");		
		if (num_buttons>31)
			num_buttons=31;
		_buttons_state = 0;
	}

void LEDButtonChecker::set_button_read_func(bool func(uint8_t)) {
	_button_read = func;
}

void LEDButtonChecker::set_button_led_off_func(void func(uint8_t)) {
	_led_off = func;
}

void LEDButtonChecker::set_button_led_on_func(void func(uint8_t)) {
	_led_on = func;
}

void LEDButtonChecker::reset() {
	_buttons_state = 0;
	for (unsigned i=0;i<_num_buttons;i++) {
		_led_on(i);
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
				_led_off(button_num);
				_buttons_state &= ~(1<<button_num);
			}
		}
	}
	return (_buttons_state != 0);
}

