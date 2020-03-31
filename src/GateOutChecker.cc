#include "GateOutChecker.h"
#include <cstdint>

//Todo: WIP, not complete
void GateOutChecker::update_gateouts() {
	_ctr++;
}

void GateOutChecker::assign_output_gate_func(OutputGateFuncType output_gate_func) {
	_output_gate = output_gate_func;
}

