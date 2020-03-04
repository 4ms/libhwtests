#include <stdint.h>

typedef bool (*ButtonReadFuncType)(uint8_t button_num);
typedef void (*ButtonLEDSetFuncType)(uint8_t button_num, bool turn_on);

class LEDButtonChecker {
public:
	LEDButtonChecker(unsigned num_buttons);
	void assign_button_read_func(ButtonReadFuncType func);
	void assign_button_led_func(ButtonLEDSetFuncType func);

	void reset();
	bool run_check();

private:
	uint8_t _num_buttons;
	uint32_t _buttons_state;
	ButtonReadFuncType _button_read;
	ButtonLEDSetFuncType _set_led;
};

