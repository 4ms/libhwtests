#include "CppUTest/TestHarness.h"
#include <fff.h>
#include "button_check_tests.h"

#define DEBUGMODE

#ifdef DEBUGMODE
#define debug_printf(...) printf(__VA_ARGS__)
#else
#define debug_printf(...) 
#endif

const uint8_t kMinHoldTime = 5;

using ERRT = TestLEDButtonChecker::ErrorType;
using SIMT = TestLEDButtonChecker::SimulationType;

TEST_GROUP(button_checker)
{
	TestLEDButtonChecker checker;

	void setup()
	{
		debug_printf("Testing LEDButtonChecker\n");
		checker.reset();
		checker.set_min_steady_state_time(kMinHoldTime);
	}

	void teardown()
	{
	}
};

TEST(button_checker, no_errors_after_reset) {
	CHECK(checker.get_error() == ERRT::None);
}

TEST(button_checker, clean_buttons_pass) {
	checker.sim_press_period = 10;
	checker.sim_type[0] = SIMT::CleanButton;
	checker.sim_type[1] = SIMT::CleanButton;
	checker.sim_type[2] = SIMT::CleanButton;
	uint8_t current_button = 0;
	while (checker.check()) {
		checker.sim_time++;
		if (current_button !=checker.button_under_test()) {
			current_button = checker.button_under_test();
			checker.sim_time=0;
		}
	}

	auto err = checker.get_error();
	CHECK(err == ERRT::None);
}

TEST(button_checker, pressing_too_fast_fails) {
	checker.sim_press_period = 9;
	checker.sim_type[0] = SIMT::CleanButton;
	checker.sim_type[1] = SIMT::CleanButton;
	checker.sim_type[2] = SIMT::CleanButton;
	while (checker.check()) {
		checker.sim_time++;
	}
	CHECK_TRUE(checker.channel_error[0] == ERRT::NoisyPress
			|| checker.channel_error[0] == ERRT::NoisyRelease);
	CHECK_TRUE(checker.channel_error[1] == ERRT::NoisyPress
			|| checker.channel_error[1] == ERRT::NoisyRelease);
	CHECK_TRUE(checker.channel_error[2] == ERRT::NoisyPress
			|| checker.channel_error[2] == ERRT::NoisyRelease);
}

