#include <stdint.h>

class LEDButtonChecker {
public:
	LEDButtonChecker(unsigned num_buttons);
	void set_button_read_func(bool func(uint8_t));
	void set_button_led_off_func(void func(uint8_t));
	void set_button_led_on_func(void func(uint8_t));

	void reset();
	bool run_check();

private:
	uint8_t _num_buttons;
	uint32_t _buttons_state;
	void (*_led_on)(uint8_t);
	void (*_led_off)(uint8_t);
	bool (*_button_read)(uint8_t);
};

