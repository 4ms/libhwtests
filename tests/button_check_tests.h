#include "ButtonChecker.h"

#define DEBUGMODE

#ifdef DEBUGMODE
#define debug_printf(...) printf(__VA_ARGS__)
#else
#define debug_printf(...)
#endif

const uint8_t kNumChans = 3;
class TestButtonChecker : public IButtonChecker {
public:
	TestButtonChecker()
		: IButtonChecker(kNumChans)
	{
		sim_time = 0;
		sim_press_period = 10;
	}

	~TestButtonChecker() {}

	virtual bool _read_button(uint8_t button_num) {
		bool bstate;
		if (sim_type[button_num] == SimulationType::CleanButton) {
			bstate = ((sim_time % sim_press_period) >= (sim_press_period/2));
		}
		else if (sim_type[button_num] == SimulationType::NoisyPressButton) {
			bstate = noisy_press_sim[sim_time % 30] == 1 ?  false : true;
		}
		else if (sim_type[button_num] == SimulationType::NoisyReleaseButton) {
			bstate = noisy_release_sim[sim_time % 30] == 1 ?  false : true;
		}
		else
			bstate = false;

		debug_printf("Time %d: ", sim_time);
		debug_printf("Button %d read as %s\n", button_num, bstate?"pressed":"released");
		return bstate;
	}

	virtual void _set_error_indicator(uint8_t channel, ErrorType err) {

		if (err != ErrorType::None)
			debug_printf("Button %d has an error: %s\n", channel, error_string(err).c_str());
	}

	virtual void _set_indicator(uint8_t indicate_num, bool newstate) {
		debug_printf("Indicator %d set to %s\n", indicate_num, newstate?"on":"off");
	}

	virtual void _check_max_one_pin_changed() {
	}

	std::string error_string(ErrorType err) {
		if (err == ErrorType::None) {
			return "None";
		} else if (err == ErrorType::StuckDown) {
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

	//Test parameters:
	enum class SimulationType {
		CleanButton,
		NoisyPressButton,
		NoisyReleaseButton,
		StuckDownButton,
		StuckUpButton
	};
	SimulationType sim_type[kNumChans];
	unsigned sim_time;
	unsigned sim_press_period;

private:
	const uint8_t noisy_press_sim[30] = {
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		0, 1, 0, 0, 0,
		0, 0, 0, 0, 0,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
	};
	const uint8_t noisy_release_sim[30] = {
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		1, 0, 1, 1, 1,
		1, 1, 1, 1, 1,
	};
};

