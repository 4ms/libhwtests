#pragma once
#include "libhwtests_common.h"

class LEDTester {
public:
	LEDTester(uint8_t num_leds) 
	: _num_leds(num_leds)
	{}

	void assign_led_onoff_func(IndicatorOnOffFuncType func);
	void reset();
	bool is_done();
	void next_led();

private:
	uint8_t _num_leds;
	uint8_t _next_led;
	uint8_t _cur_led;
	IndicatorOnOffFuncType _set_led;
};

