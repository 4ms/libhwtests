#pragma once
#include <cstdint>

const unsigned kMaxChannels = 32;

class IButtonChecker {
public:
	IButtonChecker(uint8_t num_channels);
	virtual ~IButtonChecker() {
	}
	bool check();
	void reset();
	void set_min_steady_state_time(uint32_t min_hold_time);
	void set_allowable_noise(uint32_t allowable_noise);
	uint8_t button_under_test();

	enum class ErrorType {
		None,
		StuckDown,
		NoisyPress,
		NoisyRelease,
		MultipleHighs,
		Unknown,
	};
	ErrorType get_error(unsigned channel);

	void operator delete(void *, unsigned int) {
	}

protected:
	virtual bool _read_button(uint8_t channel) = 0;
	virtual void _set_error_indicator(uint8_t channel, ErrorType err) = 0;
	virtual void _set_indicator(uint8_t indicator_num, bool newstate) = 0;
	virtual void _check_max_one_pin_changed() = 0;

private:
	enum class WaitingFor {
		Press,
		StablePress,
		Release,
		StableRelease,
	};
	WaitingFor _test_state;
	ErrorType _channel_error[kMaxChannels];
	uint8_t _num_buttons;
	uint32_t _debounce_timer;
	uint8_t _cur_test_chan;
	unsigned _min_hold_time;
	unsigned _allowable_noise;
	bool _check_current_button();
};
