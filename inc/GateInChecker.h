#pragma once
#include "libhwtests_common.h"

class GateInChecker {
public:
	GateInChecker(uint8_t num_channels) {
//		assert(num_channels<32);
		if (num_channels>32)
			num_channels = 32;
		_num_channels = num_channels;
	}

	void assign_read_gate_func(ReadGateFuncType read_gate_func);
	void assign_indicator_func(IndicatorOnOffFuncType indicator_func);

	bool check();
	void reset();
	uint8_t num_gates_high();

private:
	ReadGateFuncType _read_gate;
	IndicatorOnOffFuncType _set_indicator;
	uint8_t _num_channels;
	uint32_t _coverage_high;
	uint32_t _coverage_low;
	uint8_t _num_gates_high;
};

