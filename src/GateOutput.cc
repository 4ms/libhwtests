#include "GateOutput.h"
#include <cstdint>

void GateOutput::assign_gate_onoff_func(GateOnOffFuncType gate_onoff_func) {
	_gate_out = gate_onoff_func;
}

void GateOutput::reset() {
	_phase = 0;
	_gate_out(false);
}

void GateOutput::update() {
	_phase += _freq;
	_gate_out(_phase < _pulse_width);
}

