#pragma once
#include "libhwtests_common.h"

class ILEDButtonChecker {
public:
	ILEDButtonChecker(uint8_t num_channels);
	bool check();
	void reset();
	void set_min_steady_state_time(uint32_t min_hold_time);

	enum class ErrorType {
		None,
		StuckDown,
		NoisyPress,
		NoisyRelease,
		MultipleHighs
	};
	ErrorType get_error();

protected:
	virtual bool _read_button(uint8_t gate_num) = 0;
	virtual void _set_error_indicator(uint8_t channel, ErrorType err) = 0;
	virtual void _set_indicator(uint8_t indicate_num, bool newstate) = 0;
	virtual void _check_max_one_pin_changed() = 0;

private:
	enum ErrorType _error;
	uint8_t _num_buttons;
	uint8_t _cur_test_chan;
	unsigned _test_state;
	unsigned _min_hold_time;
	void _check_current_button();
};

