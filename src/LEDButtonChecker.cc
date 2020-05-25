#include "LEDButtonChecker.h"

ILEDButtonChecker::ILEDButtonChecker(uint8_t num_buttons)
	: _num_buttons(num_buttons > 32 ? 32 : num_buttons)
	, _min_hold_time(100)
	{
		//assert(num_buttons<=32)
	}

void ILEDButtonChecker::reset() {
	uint32_t coverage;
	if (_num_buttons==32)
		coverage=0xFFFFFFFF;
	else
		coverage = (1<<_num_buttons)-1;

	_cur_test_chan = 0;
	_test_state = 0;
	_error = ErrorType::None;

	for (uint8_t i=0; i<_num_buttons; i++) {
		_set_indicator(i, false);
		_set_error_indicator(i, ErrorType::None);
	}
}

bool ILEDButtonChecker::check() {
	_check_current_button();
	_set_error_indicator(_cur_test_chan, _error);
	_check_max_one_pin_changed();
	bool is_done = (_cur_test_chan >= _num_buttons);
	return !is_done;
}

void ILEDButtonChecker::_check_current_button() {
	bool but_pressed = _read_button(_cur_test_chan);
	if (_test_state == 0) {
		_set_indicator(_cur_test_chan, true);
		if (but_pressed) {
			_test_state = 1;
			_debounce_timer = 0;
		}
	}
	else if (_test_state == 1) {
		_debounce_timer++;
		if (!but_pressed) {
			if (_debounce_timer < _min_hold_time)
				_error = ErrorType::NoisyPress;
			else {
				_test_state = 2;
				_debounce_timer = 0;
			}
		}
	}
	else if (_test_state == 2) {
		_debounce_timer++;
		if (_debounce_timer >= _min_hold_time) {
			_set_indicator(_cur_test_chan, false);
			_cur_test_chan++;
			_test_state = 0;
		}
		else if (but_pressed)
			_error = ErrorType::NoisyRelease;
	}
}

void ILEDButtonChecker::set_min_steady_state_time(uint32_t min_hold_time) {
	_min_hold_time = min_hold_time;
}

uint8_t ILEDButtonChecker::button_under_test() {
	return _cur_test_chan;
}

ILEDButtonChecker::ErrorType ILEDButtonChecker::get_error() {
	return _error;
}

