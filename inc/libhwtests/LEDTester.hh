#pragma once
#include <cstdint>

// LED Tester base class
// Create a derived class that implements set_led(int, bool)
// and pause_between_steps()
class ILEDTester {
public:
	ILEDTester(uint8_t num_leds)
		: _num_leds(num_leds) {
	}

	virtual void set_led(int led_id, bool turn_on) = 0;
	virtual void pause_between_steps() = 0;
	void reset();
	bool is_done();
	void next_led();
	void run_test();

private:
	uint8_t _num_leds;
	uint8_t _next_led;
	uint8_t _cur_led;
};

//Deprecated (non-virtual class using function pointer)
class LEDTester {
	typedef void (*IndicatorOnOffFuncType)(uint8_t indicator_num, bool newstate);

public:
	LEDTester(uint8_t num_leds)
		: _num_leds(num_leds) {
	}

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
