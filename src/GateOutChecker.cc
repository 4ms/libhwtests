#include "libhwtests/GateOutChecker.hh"
#include <cstdint>

void IGateOutChecker::run_test() {
	reset();
	while (check())
		;
}

bool IGateOutChecker::check() {
	if (_cur_chan >= _num_channels)
		return false;

	set_gate(_cur_chan, _cur_out_state);
	bool r = read_gate();
	if (r != _cur_out_state) {
		_err = _cur_out_state ? ErrorType::StuckHigh : ErrorType::StuckLow;
		return false;
	}
	_toggle_ctr++;
	_cur_out_state = !_cur_out_state;
	if (_toggle_ctr >= _num_toggles) {
		_cur_chan++;
	}

	return true;
}

void IGateOutChecker::reset() {
	_cur_chan = 0;
	_toggle_ctr = 0;
	_err = ErrorType::None;
	for (uint32_t i = 0; i < _num_channels; i++)
		set_indicator(i, i == _cur_chan);
}

void IGateOutChecker::skip() { _cur_chan++; }

void IGateOutChecker::set_num_toggles(uint32_t num_toggles) { _num_toggles = num_toggles; }

IGateOutChecker::ErrorType IGateOutChecker::get_error() { return _err; }
