#pragma once
#include "libhwtests_common.h"

//Todo: WIP, not complete
class GateOutChecker {
public:
	GateOutChecker(uint8_t num_channels) {
		if (num_channels<1)
			num_channels = 1;
		_num_channels = num_channels;
	}
	void assign_output_gate_func(OutputGateFuncType output_gate_func);
	void update_gateouts();
private:
	OutputGateFuncType _output_gate;
	uint8_t _num_channels;
	uint8_t _ctr;
};

