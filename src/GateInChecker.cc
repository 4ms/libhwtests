#include "libhwtests/GateInChecker.h"

//Todo: integrate num_gates_high>1 as a fail for check()
//Todo: check for 0->1 and 1->0 transition, instead of just 0 and 1

IGateInChecker::IGateInChecker(uint8_t num_channels)
	: _num_channels(num_channels)
	, _num_repeats(10) {
}

void IGateInChecker::clear_error() {
	_error = ErrorType::None;
}

void IGateInChecker::reset() {
	_cur_test_chan = 0;
	_cur_test_state = 0;
	_error = ErrorType::None;

	set_test_signal(false);
	for (int i = 0; i < _num_channels; i++) {
		set_indicator(i, false);
		set_error_indicator(i, ErrorType::None);
	}
}

bool IGateInChecker::check() {
	_check_current_gate_in();
	_check_max_one_gate_high();
	bool is_done = _cur_test_chan >= _num_channels;
	return !is_done;
}

void IGateInChecker::skip() {
	set_indicator(_cur_test_chan, false);
	_cur_test_chan++;
}

void IGateInChecker::_check_current_gate_in() {
	set_indicator(_cur_test_chan, true);

	if ((_cur_test_state == 1) && (!is_ready_to_read_jack(_cur_test_chan)))
		return;

	bool gatestate = read_gate(_cur_test_chan);

	if ((_cur_test_state & 0b1) == 0) {
		if (gatestate == false) {
			_cur_test_state++;
			set_test_signal(true);
		} else if (_cur_test_state >= 2)
			_error = ErrorType::StuckHigh;
	} else if ((_cur_test_state & 0b1) == 1) {
		if (gatestate == true) {
			_cur_test_state++;
			set_test_signal(false);
		} else if (_cur_test_state >= 2) {
			_cur_test_state = 1;
			_error = ErrorType::StuckLow;
		}
	}

	set_error_indicator(_cur_test_chan, _error);

	if (_cur_test_state >= _num_repeats) {
		_error = ErrorType::None;
		set_indicator(_cur_test_chan, false);
		_cur_test_chan++;
		_cur_test_state = 0;
		set_test_signal(false);
		signal_jack_done(_cur_test_chan);
	}
}

void IGateInChecker::_check_max_one_gate_high() {
	_num_gates_high = 0;

	for (int i = 0; i < _num_channels; i++) {
		if (read_gate(i))
			_num_gates_high++;
	}

	if (_num_gates_high > 1)
		_error = ErrorType::MultipleHighs;

	set_error_indicator(_cur_test_chan, _error);
}

void IGateInChecker::set_num_toggles(uint32_t num_toggles) {
	_num_repeats = num_toggles;
}

IGateInChecker::ErrorType IGateInChecker::get_error() {
	return _error;
}
