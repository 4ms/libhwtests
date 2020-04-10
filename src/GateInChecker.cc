#include "GateInChecker.h"

//Todo: integrate num_gates_high>1 as a fail for check()
//Todo: check for 0->1 and 1->0 transition, instead of just 0 and 1

IGateInChecker::IGateInChecker(uint8_t num_channels) 
	: _num_channels(num_channels)
{
}

void IGateInChecker::reset() {
	uint32_t coverage;
	if (_num_channels==32)
		coverage=0xFFFFFFFF;
	else
		coverage = (1<<_num_channels)-1;

	_cur_test_chan = 0;
	_cur_test_state = 0;
	_error = ErrorType::None;

	_set_test_signal(false);
	_set_error_indicator(_error);
	for (int i=0; i<_num_channels; i++)
		_set_indicator(i, false);
}

IGateInChecker::ErrorType IGateInChecker::get_error() {
	return _error;
}

uint8_t IGateInChecker::num_gates_high() {
	return _num_gates_high;
}

bool IGateInChecker::check() {

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

	if (_cur_test_state >= 10)
	{
		_cur_test_chan++;
		_cur_test_state = 0;
		_set_test_signal(false);
	}

	_num_gates_high = 0;

	for (int i=0; i<_num_channels; i++) {
		if (_read_gate(i))
			_num_gates_high++;
	}

	if (num_gates_high() > 1)
		_error = ErrorType::MultipleHighs;

	_set_error_indicator(_error);

	return (_cur_test_chan >= _num_channels);
}

