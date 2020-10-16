#include "GateInChecker.h"

//Todo: integrate num_gates_high>1 as a fail for check()
//Todo: check for 0->1 and 1->0 transition, instead of just 0 and 1

IGateInChecker::IGateInChecker(uint8_t num_channels) 
	: _num_channels(num_channels)
	, _num_repeats(10)
{
}

void IGateInChecker::reset() {
	_cur_test_chan = 0;
	_cur_test_state = 0;
	_error = ErrorType::None;

	_set_test_signal(false);
	for (int i=0; i<_num_channels; i++) {
		_set_indicator(i, false);
		_set_error_indicator(i, ErrorType::None);
	}
}

bool IGateInChecker::check()
{
	_check_current_gate_in();
	_check_max_one_gate_high();
	bool is_done = _cur_test_chan >= _num_channels;
	return !is_done;
}

void IGateInChecker::_check_current_gate_in()
{
	bool gatestate = _read_gate(_cur_test_chan);
	_set_indicator(_cur_test_chan, gatestate);

	if ((_cur_test_state & 0b1) == 0)
	{
		if (gatestate==false) {
			_cur_test_state++;
			_set_test_signal(true);
		}
		else if (_cur_test_state>=2)
			_error = ErrorType::StuckHigh;
	}
	else if ((_cur_test_state & 0b1 ) == 1)
	{
		if (gatestate==true) {
			_cur_test_state++;
			_set_test_signal(false);
		}
		else if (_cur_test_state>=2)
			_error = ErrorType::StuckLow;
	}

	_set_error_indicator(_cur_test_chan, _error);

	if (_cur_test_state >= _num_repeats)
	{
		_error = ErrorType::None;
		_set_indicator(_cur_test_chan, false);
		_cur_test_chan++;
		_cur_test_state = 0;
		_set_test_signal(false);
	}
}

void IGateInChecker::_check_max_one_gate_high()
{
	_num_gates_high = 0;

	for (int i=0; i<_num_channels; i++) {
		if (_read_gate(i))
			_num_gates_high++;
	}

	if (_num_gates_high > 1)
		_error = ErrorType::MultipleHighs;

	_set_error_indicator(_cur_test_chan, _error);
}

void IGateInChecker::set_num_toggles(uint32_t num_toggles) {
	_num_repeats = num_toggles;
}

IGateInChecker::ErrorType IGateInChecker::get_error() {
	return _error;
}

