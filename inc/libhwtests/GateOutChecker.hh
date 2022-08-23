#pragma once
#include <cstdint>

class IGateOutChecker {
public:
	IGateOutChecker(uint8_t num_channels) {
		if (num_channels < 1)
			num_channels = 1;
		_num_channels = num_channels;
		_num_toggles = 50;
	}

	bool check();
	void reset();
	void skip();
	void set_num_toggles(uint32_t num_toggles);

	enum class ErrorType {
		None,
		StuckHigh,
		StuckLow,
	};
	ErrorType get_error();

protected:
	virtual bool read_gate() = 0;
	virtual void set_gate(uint32_t chan, bool newstate) = 0;
	virtual void set_indicator(uint32_t indicate_num, bool newstate) = 0;
	virtual void signal_jack_done(uint32_t chan) = 0;

private:
	uint32_t _num_channels;
	uint32_t _num_toggles;
	uint32_t _cur_chan;
	ErrorType _err;

	bool _cur_out_state;
	uint32_t _toggle_ctr;
};
