### libhwtests

A set of tools for writing hardware tests for embedded systems, focused mainly
on testing front-end UI (buttons, switches, pots, jacks) and peripherals.

Many of the tests involve user interaction. For example to test an LED button
the user must push the button and confirm the light turns on or off. Thus,
these tests are not typically automatic, and often require a human to
decide pass/fail. However, some tests like FLASH RAM read/write tests are
automatic.

These tests are not meant to be used to test if driver code is written properly,
nor are they to be run every time you compile (like unit tests might be).
Instead, this library is useful for writing a test procedure to be performed 
to determine if the hardware is working properly. Some uses might be:

  * Board bring-up for a new prototype revision
  * Quality assurance for production units
  * Allowing a customer to verify if their unit is working in the "field" 

A "fail" result also gives a specific error code which can be recorded by the
testing person, and used in repair/rework.

To use, typically you create a class which defines certain virtual functions
that interact with the hardware. 

Written with STM32 and ESP32 chips and gcc in mind.

Very much a WIP!

Todo:
  * Convert unit tests to doctest, and write more
  * Examples

Done:
  * ADC input range tester: tests if a pot or CV input can go full range
    * Helper callbacks for ADC input range testers.
  * ADC channel isolation tester
  * DAC/Codec output test helper callback functions
  * Generic I2C peripheral ping tester
  * Codec input helper callback functions
  * LED Button tester 
  * LED element tester
  * Toggle switch tester (2 or 3 position)
  * Gate input tester
  * Gate output tester
  * FLASH RAM tester

Example:

```
#include "libhwtests/ButtonChecker.hh"
#include "my_board_config.hh"

struct TestButtons : IButtonChecker {
    constexpr static number_of_buttons = 3;

	TestButtons()
		: IButtonChecker{number_of_buttons} {
		reset();
		set_allowable_noise(10);
		set_min_steady_state_time(10);
	}

	bool _read_button(uint8_t channel) override {
		if (channel == 0)
			return Board::BankButton::read();
		if (channel == 1)
			return Board::PlayButton::read();
		if (channel == 2)
			return Board::RevButton::read();
		return false;
	}

	void _set_error_indicator(uint8_t channel, ErrorType err) override { 
        Board::PlayLED.set_color(Colors::red);

        // Print the error to a terminal
        // printf("Button Test Failed! Button %d, Error Code %d\n", channel, (uint32_t)err);
    }

	void _set_indicator(uint8_t indicator_num, bool newstate) override {
        auto color = newstate ? Colors::green : Colors::off;

		if (indicator_num == 0)
			Board::BankLED.set_color(color);
		if (indicator_num == 1)
			Board::PlayLED.set_color(color);
		if (indicator_num == 2)
			Board::RevLED.set_color(color);

        // printf("Press button %d\n");
	}

	void _check_max_one_pin_changed() override {
		//...
	}
};

void run_hardware_tests() {
    TestButtons tester;
    tester.run_tests();

    //run more tests...
}

```


To run unit tests:

```
make -f tests/Makefile
```

