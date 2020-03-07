#include "LEDTester.h"


void LEDTester::assign_led_onoff_func(IndicatorOnOffFuncType func) {
	_set_led = func;
}

void LEDTester::reset() {
	_next_led = 0;
	_cur_led = 0;
	for (uint8_t i=0; i<_num_leds; i++) {
		_set_led(i, false);
	}
}

bool LEDTester::is_done() {
	if (_next_led>=_num_leds)
		return true;
	else 
		return false;
}

void LEDTester::next_led() {
	if (is_done())
		return;
	_set_led(_cur_led, false);
	_set_led(_next_led, true);
	_cur_led = _next_led;
	_next_led++;
}

