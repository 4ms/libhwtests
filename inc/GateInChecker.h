#pragma once
#include "libhwtests_common.h"

class IGateInChecker {
public:
	IGateInChecker(uint8_t num_channels);
	virtual ~IGateInChecker() {}
	bool check();
	void reset();
	void set_num_toggles(uint32_t num_toggles);
	void clear_error();

	enum class ErrorType {
		None,
		StuckHigh,
		StuckLow,
		MultipleHighs
	};
	ErrorType get_error();


protected:
	virtual bool read_gate(uint8_t gate_num) = 0;
	virtual void set_test_signal(bool newstate) = 0;
	virtual void set_error_indicator(uint8_t channel, ErrorType err) = 0;
	virtual void set_indicator(uint8_t indicate_num, bool newstate) = 0;
	virtual void signal_jack_done(uint8_t chan) = 0;
	virtual bool is_ready_to_read_jack(uint8_t chan) = 0;

private:
	enum ErrorType _error;
	uint8_t _num_channels;
	uint8_t _num_gates_high;
	uint8_t _cur_test_chan;
	unsigned _cur_test_state;
	unsigned _num_repeats;
	void _check_current_gate_in();
	void _check_max_one_gate_high();
};

