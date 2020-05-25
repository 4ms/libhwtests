#include "LEDButtonChecker.h"

#define DEBUGMODE

#ifdef DEBUGMODE
#define debug_printf(...) printf(__VA_ARGS__)
#else
#define debug_printf(...) 
#endif

const uint8_t kNumChans = 3;
class TestLEDButtonChecker : public ILEDButtonChecker {
public:
	TestLEDButtonChecker() 
		: ILEDButtonChecker(kNumChans)
	{
		sim_time = 0;
		sim_press_period = 10;
		for (uint8_t i=0; i<kNumChans; i++)
			channel_error[i] = ErrorType::None;
	}

	~TestLEDButtonChecker() {}

	virtual bool _read_button(uint8_t button_num) {
		if (sim_type[button_num] == SimulationType::CleanButton) {
			bool bstate = ((sim_time % sim_press_period) < (sim_press_period/2));
			debug_printf("time: %d: ", sim_time % sim_press_period);
			debug_printf("Button %d was %s\n", button_num, bstate?"pressed":"released");
			return bstate;
		}

		else if (sim_type[button_num] == SimulationType::NoisyPress) {
			return false;
		}

		else
			return false;
	}
	virtual void _set_error_indicator(uint8_t channel, ErrorType err) {
		if (channel_error[channel] == ErrorType::None)
			channel_error[channel] = err;
		
		debug_printf("Button %d got an error: %s\n", channel, error_string(err).c_str());
	}
	virtual void _set_indicator(uint8_t indicate_num, bool newstate) {
		debug_printf("Indicator %d went %s\n", indicate_num, newstate?"on":"off");
	}
	virtual void _check_max_one_pin_changed() {
	}

	std::string error_string(ErrorType err) {
		if (err == ErrorType::StuckDown) {
			return "StuckDown";
		} else if (err == ErrorType::NoisyPress) {
			return "NoisyPress";
		} else if (err == ErrorType::NoisyRelease) {
			return "NoisyRelease";
		} else if (err == ErrorType::MultipleHighs) {
			return "MultipleHighs";
		} else 
			return "Unknown";
	}
	ErrorType channel_error[kNumChans];

	//Test parameters:
	enum class SimulationType {
		CleanButton,
		NoisyPress,
		NoisyRelease,
		StuckDown,
		StuckUp
	};
	SimulationType sim_type[kNumChans];
	unsigned sim_time;
	unsigned sim_press_period;
};

