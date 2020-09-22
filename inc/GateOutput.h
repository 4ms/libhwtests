#pragma once
#include "libhwtests_common.h"

class IGateOutput {
public:
	IGateOutput(uint32_t freq, float pulse_width, float initial_phase, uint32_t sample_rate)
		: _freq(freq*sample_rate)
		, _phase (initial_phase*(float)(0xFFFFFFFFUL))
		, _pulse_width(pulse_width*(float)(0xFFFFFFFFUL))
	{}
	
	virtual void gate_out(bool state) = 0;
	void reset();
	void update();

protected:
	uint32_t _freq;
	uint32_t _phase;
	uint32_t _pulse_width;
};

class GateOutput {
public:
	GateOutput(uint32_t freq, float pulse_width, float initial_phase, uint32_t sample_rate)
		: _freq(freq*sample_rate)
		, _phase (initial_phase*(float)(0xFFFFFFFFUL))
		, _pulse_width(pulse_width*(float)(0xFFFFFFFFUL))
	{}
	
	void assign_gate_onoff_func(GateOnOffFuncType gate_onoff_func);
	void reset();
	void update();

private:
	uint32_t _freq;
	uint32_t _phase;
	uint32_t _pulse_width;
	GateOnOffFuncType _gate_out;
};

