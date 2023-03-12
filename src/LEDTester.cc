#include "libhwtests/LEDTester.hh"

void ILEDTester::reset() {
	_cur_led = 0;
	for (uint8_t i = 0; i < _num_leds; i++) {
		set_led(i, false);
	}
	// Make sure to set this after turning all LEDs off
	// In case some "led" elements are actually multiple
	// elements to test color blending
	set_led(0, true);
}

bool ILEDTester::is_done() {
	if (_cur_led >= _num_leds)
		return true;
	else
		return false;
}

void ILEDTester::next_led() {
	set_led(_cur_led, false);
	_cur_led++;
	if (is_done())
		return;
	set_led(_cur_led, true);
}

void ILEDTester::run_test() {
	reset();
	while (!is_done()) {
		pause_between_steps();
		next_led();
	}
}

// [[deprecated]] void LEDTester::assign_led_onoff_func(IndicatorOnOffFuncType func) { _set_led = func; }

// [[deprecated]] void LEDTester::reset() {
// 	_next_led = 0;
// 	_cur_led = 0;
// 	for (uint8_t i = 0; i < _num_leds; i++) {
// 		_set_led(i, false);
// 	}
// }

// [[deprecated]] bool LEDTester::is_done() {
// 	if (_next_led >= _num_leds)
// 		return true;
// 	else
// 		return false;
// }

// [[deprecated]] void LEDTester::next_led() {
// 	if (is_done())
// 		return;
// 	_set_led(_cur_led, false);
// 	_set_led(_next_led, true);
// 	_cur_led = _next_led;
// 	_next_led++;
// }
