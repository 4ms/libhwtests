#pragma once


//Establishes the minimum needed to perform a hardware test: one button and one LED, and a way to delay
struct IHardwareTester {
    virtual bool main_button_pressed() = 0;
    virtual void delay_ms(uint32_t ms) = 0;
    virtual void set_main_button_led(bool turn_on) = 0;

    void pause_until_button_pressed(void) {
        delay_ms(10);
        while (!main_button_pressed()) { ; }
    }

    void pause_until_button_released(void) {
        delay_ms(10);
        while (main_button_pressed()) { ; }
    }

    void pause_until_button(void) {
        pause_until_button_pressed();
        pause_until_button_released();
    }

    void flash_mainbut_until_pressed(void) {
        while (1) {
            set_main_button_led(false);

            delay_ms(50);
            if (main_button_pressed()) break;
            delay_ms(50);
            if (main_button_pressed()) break;

            set_main_button_led(true);
            delay_ms(50);
            if (main_button_pressed()) break;
            delay_ms(50);
            if (main_button_pressed()) break;
        }
        pause_until_button_released();
    }

    bool check_for_longhold_button(void) {
        uint32_t press_tmr = 0;
        bool longhold_detected = false;
        set_main_button_led(false);
        while (main_button_pressed()) {
            if (++press_tmr > 20000000) {
                longhold_detected = true;
                set_main_button_led(true);
            }
        }
        return longhold_detected;
    }

};
