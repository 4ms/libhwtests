#include "ButtonChecker.h"

IButtonChecker::IButtonChecker(uint8_t num_buttons)
	: _num_buttons(num_buttons > kMaxChannels ? kMaxChannels : num_buttons)
	, _cur_test_chan(0)
	, _min_hold_time(200)
	, _allowable_noise(100)
	{
	}

void IButtonChecker::reset() {
	_cur_test_chan = 0;
	_test_state = WaitingFor::Press;

	for (uint8_t i=0; i<_num_buttons; i++) {
		_channel_error[i] = ErrorType::None;
		_set_indicator(i, false);
		_set_error_indicator(i, ErrorType::None);
	}
	_set_indicator(_cur_test_chan, true);
}

bool IButtonChecker::check() {
	bool is_new_error = _check_current_button();
	if (is_new_error)
		_set_error_indicator(_cur_test_chan, _channel_error[_cur_test_chan]);
	_check_max_one_pin_changed();
	bool is_done = (_cur_test_chan >= _num_buttons);
	return !is_done;
}

bool IButtonChecker::_check_current_button() {
	bool is_new_error = false;
	bool but_pressed = _read_button(_cur_test_chan);

	switch (_test_state) {
		case WaitingFor::Press:
			if (but_pressed) {
				_test_state = WaitingFor::StablePress;
				_debounce_timer = 1;
			}
			break;

		case WaitingFor::StablePress:
			_debounce_timer++;
			if (_debounce_timer >= _min_hold_time) {
				_test_state = WaitingFor::Release;
			}
			else if (!but_pressed) {
				if (_debounce_timer > _allowable_noise) {
					if (_channel_error[_cur_test_chan] != ErrorType::NoisyPress) {
						_channel_error[_cur_test_chan] = ErrorType::NoisyPress;
						is_new_error = true;
					}
				}
			}
			break;

		case WaitingFor::Release:
			if (!but_pressed) {
				_test_state = WaitingFor::StableRelease;
				_debounce_timer = 1;
			}
			break;

		case WaitingFor::StableRelease:
			_debounce_timer++;
			if (but_pressed) {
				if (_debounce_timer > _allowable_noise) {
					if (_channel_error[_cur_test_chan] != ErrorType::NoisyRelease) {
						_channel_error[_cur_test_chan] = ErrorType::NoisyRelease;
						is_new_error = true;
					}
				}
			}
			else if (_debounce_timer >= _min_hold_time) {
				_set_indicator(_cur_test_chan, false);
				_cur_test_chan++;
				_set_indicator(_cur_test_chan, true);
				_test_state = WaitingFor::Press;
			}
			break;
	}
	return is_new_error;
}

void IButtonChecker::set_min_steady_state_time(uint32_t min_hold_time) {
	_min_hold_time = min_hold_time;
}

void IButtonChecker::set_allowable_noise(uint32_t allowable_noise) {
	_allowable_noise = allowable_noise;
}

uint8_t IButtonChecker::button_under_test() {
	return _cur_test_chan;
}

IButtonChecker::ErrorType IButtonChecker::get_error(unsigned channel) {
	if (channel < _num_buttons)
		return _channel_error[channel];
	else
		return ErrorType::Unknown;
}

