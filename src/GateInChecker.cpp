#include "GateInChecker.h"

void GateInChecker::reset() {
	uint32_t coverage;
	if (_num_channels==32)
		coverage=0xFFFFFFFF;
	else
		coverage = (1<<_num_channels)-1;

	_coverage_low = coverage;
	_coverage_high = coverage;
}

uint8_t GateInChecker::num_gates_high() {
	return _num_gates_high;
}

bool GateInChecker::check() {
	_num_gates_high = 0;

	for (int i=0; i<_num_channels; i++) {
		if (_read_gate(i)){
			_set_indicator(i, true);
			_coverage_high &= ~(1<<i);
			_num_gates_high++;
		}
		else {
			_set_indicator(i, false);
			_coverage_low &= ~(1<<i);
		}
	}
	return (_coverage_high!=0 || _coverage_low!=0);
}


void GateInChecker::assign_read_gate_func(ReadGateFuncType read_gate_func) {
	_read_gate = read_gate_func;
}

void GateInChecker::assign_output_gate_func(OutputGateFuncType output_gate_func) {
	_output_gate = output_gate_func;
}

void GateInChecker::assign_indicator_func(IndicatorOnOffFuncType indicator_func){
	_set_indicator = indicator_func;
}

