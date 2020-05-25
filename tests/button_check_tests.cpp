#include "CppUTest/TestHarness.h"
#include <fff.h>
#include "button_check_tests.h"

const uint8_t kMinHoldTime = 5;

using ERRT = TestButtonChecker::ErrorType;
using SIMT = TestButtonChecker::SimulationType;

TEST_GROUP(button_checker)
{
	TestButtonChecker checker;

	void setup()
	{
		checker.reset();
		checker.set_min_steady_state_time(kMinHoldTime);
		checker.sim_press_period = 10;
	}

	void teardown()
	{
	}
};

static void run_simulation(TestButtonChecker &checker) {
	uint8_t current_button = 0;
	while (checker.check()) {
		checker.sim_time++;
		if (current_button !=checker.button_under_test()) {
			current_button = checker.button_under_test();
			checker.sim_time=0;
		}
	}
}

TEST(button_checker, no_errors_after_reset) {
	CHECK_TRUE(checker.get_error(0) == ERRT::None);
	CHECK_TRUE(checker.get_error(1) == ERRT::None);
	CHECK_TRUE(checker.get_error(2) == ERRT::None);
}

TEST(button_checker, clean_buttons_pass) {
	checker.sim_type[0] = SIMT::CleanButton;
	checker.sim_type[1] = SIMT::CleanButton;
	checker.sim_type[2] = SIMT::CleanButton;

	run_simulation(checker);

	CHECK_TRUE(checker.get_error(0) == ERRT::None);
	CHECK_TRUE(checker.get_error(1) == ERRT::None);
	CHECK_TRUE(checker.get_error(2) == ERRT::None);
}

TEST(button_checker, pressing_too_fast_fails) {
	checker.sim_press_period = 9;
	checker.sim_type[0] = SIMT::CleanButton;
	checker.sim_type[1] = SIMT::CleanButton;
	checker.sim_type[2] = SIMT::CleanButton;

	run_simulation(checker);

	CHECK_TRUE(checker.get_error(0) == ERRT::NoisyPress
			|| checker.get_error(0) == ERRT::NoisyRelease);
	CHECK_TRUE(checker.get_error(1) == ERRT::NoisyPress
			|| checker.get_error(1) == ERRT::NoisyRelease);
	CHECK_TRUE(checker.get_error(2) == ERRT::NoisyPress
			|| checker.get_error(2) == ERRT::NoisyRelease);
}

TEST(button_checker, noisy_release_produces_error) {
	checker.sim_type[0] = SIMT::CleanButton;
	checker.sim_type[1] = SIMT::NoisyReleaseButton;
	checker.sim_type[2] = SIMT::CleanButton;

	run_simulation(checker);

	CHECK_TRUE(checker.get_error(0) == ERRT::None);
	CHECK_TRUE(checker.get_error(1) == ERRT::NoisyRelease);
	CHECK_TRUE(checker.get_error(2) == ERRT::None);
}

TEST(button_checker, noisy_press_produces_error) {
	checker.sim_type[0] = SIMT::CleanButton;
	checker.sim_type[1] = SIMT::NoisyPressButton;
	checker.sim_type[2] = SIMT::CleanButton;

	run_simulation(checker);

	CHECK_TRUE(checker.get_error(0) == ERRT::None);
	CHECK_TRUE(checker.get_error(1) == ERRT::NoisyPress);
	CHECK_TRUE(checker.get_error(2) == ERRT::None);
}

