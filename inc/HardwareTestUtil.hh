#pragma once
#include <stdint.h>

//Establishes the minimum needed to perform a hardware test: one button and one LED, and a way to delay
//Base class must define these:
// static bool main_button_pressed();
// static void delay_ms(uint32_t ms);
// static void set_main_button_led(bool turn_on);
template<typename Base>
struct IHardwareTestUtil {

	static void pause_until_button_pressed(void) {
		Base::delay_ms(10);
		while (!Base::main_button_pressed()) {
			;
		}
	}

	static void pause_until_button_released(void) {
		Base::delay_ms(10);
		while (Base::main_button_pressed()) {
			;
		}
	}

	static void pause_until_button(void) {
		pause_until_button_pressed();
		pause_until_button_released();
	}

	static void flash_mainbut_until_pressed(void) {
		while (1) {
			Base::set_main_button_led(false);

			Base::delay_ms(50);
			if (Base::main_button_pressed())
				break;
			Base::delay_ms(50);
			if (Base::main_button_pressed())
				break;

			Base::set_main_button_led(true);
			Base::delay_ms(50);
			if (Base::main_button_pressed())
				break;
			Base::delay_ms(50);
			if (Base::main_button_pressed())
				break;
		}
		pause_until_button_released();
	}

	static bool check_for_longhold_button(void) {
		uint32_t press_tmr = 0;
		bool longhold_detected = false;
		Base::set_main_button_led(false);
		while (Base::main_button_pressed()) {
			if (++press_tmr > 20000000) {
				longhold_detected = true;
				Base::set_main_button_led(true);
			}
		}
		return longhold_detected;
	}
};
