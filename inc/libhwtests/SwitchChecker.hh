#pragma once
#include <stdint.h>

template<unsigned Num3posSwitches>
class ISwitchChecker {
public:
	static constexpr unsigned MaxSwitches = 32;
	static_assert(Num3posSwitches < MaxSwitches, "Trying to check too many switches!");

	enum class SwitchPosition {
		Middle,
		Down,
		Up,
		Initial,
		Unknown,
	};

	ISwitchChecker() {}
	virtual ~ISwitchChecker() {}
	//FixMe: Why is this required?
	void operator delete(void *, unsigned int) {}

	virtual SwitchPosition read_switch_state(uint32_t sw_num) = 0;
	virtual void delay_ms(uint32_t ms) = 0;
	virtual void set_indicator(uint32_t sw_num, SwitchPosition pos) = 0;
	virtual void set_error_indicator(uint32_t sw_num) = 0;

	bool run_test() {
		reset();
		while (!is_done()) {
			check();
		}
		return !bad_switch;
	}

	void reset() {
		for (uint8_t sw_num = 0; sw_num < Num3posSwitches; sw_num++) {
			switch_state[sw_num] = SwitchPosition::Initial;
			switch_last_state[sw_num] = read_switch_state(sw_num);

			for (int pos_i = 0; pos_i < 3; pos_i++)
				pos_checked[pos_i][sw_num] |= (1 << sw_num);

			set_indicator(sw_num, SwitchPosition::Initial);
		}
	}

	void check() {
		for (uint8_t sw_num = 0; sw_num < Num3posSwitches; sw_num++) {
			switch_state[sw_num] = read_switch_state(sw_num);

			for (int pos_i = 0; pos_i < 3; pos_i++) {
				SwitchPosition pos = pos_i == 0 ? SwitchPosition::Up :
									 pos_i == 1 ? SwitchPosition::Middle :
													SwitchPosition::Down;

				if (switch_state[sw_num] == pos && switch_last_state[sw_num] != pos) {
					set_indicator(sw_num, pos);
					delay_ms(100);
					pos_checked[pos_i][sw_num] &= ~(1 << sw_num);
				}
			}
			if (switch_state[sw_num] == SwitchPosition::Initial || switch_state[sw_num] == SwitchPosition::Unknown) {
				set_error_indicator(sw_num);
				delay_ms(100);
				bad_switch = true;
			}
			switch_last_state[sw_num] = switch_state[sw_num];
		}
	}

	bool is_done() {
		uint8_t tot = 0;
		for (uint8_t sw_num = 0; sw_num < Num3posSwitches; sw_num++) {
			for (uint8_t pos_i = 0; pos_i < 3; pos_i++)
				tot += pos_checked[pos_i][sw_num];
		}
		return (tot == 0);
	}

private:
	SwitchPosition switch_state[Num3posSwitches];
	SwitchPosition switch_last_state[Num3posSwitches];
	uint8_t pos_checked[3][Num3posSwitches] = {0};
	bool bad_switch;
};
