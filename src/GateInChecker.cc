#include "GateInChecker.h"

//Todo: integrate num_gates_high>1 as a fail for check()
//Todo: check for 0->1 and 1->0 transition, instead of just 0 and 1 


void GateInChecker::reset() {
	uint32_t coverage;
	if (_num_channels==32)
		coverage=0xFFFFFFFF;
	else
		coverage = (1<<_num_channels)-1;

	_coverage_low = coverage;
	_coverage_high = coverage;
}

//Useful for showing an error if more than one gate goes high at a time
uint8_t GateInChecker::num_gates_high() {
	return _num_gates_high;
}

//Returns true when all gate inputs have full coverage (high and low)
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

void GateInChecker::assign_indicator_func(IndicatorOnOffFuncType indicator_func){
	_set_indicator = indicator_func;
}

