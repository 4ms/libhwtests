### libhwtests

A set of tools for writing hardware tests for embedded systems, focused mainly on testing front-end UI (buttons, switches, pots, jacks) and peripherals.

Many of the tests involve user interaction. For example to test an LED button the user must push the button and confirm the light turns on or off. Thus, these tests are not typically automatic and necessarily require a human to decide pass/fail. However, some tests like FLASH RAM read/write tests are automatic.

These tests are not meant to be used to test if driver code is written properly (hardware integration tests), nor are they to be run every time you compile (like unit tests might be). Instead, this library is useful for writing a test procedure to be performed on a production batch of hardware, in order to quickly give each unit a pass/fail result. In addition, a "fail" result also gives a specific error code which can be recorded by the testing person, and used in repair/rework.

The library relies heavily on function pointers, in order to make it easy to adapt to C.

Written with STM32 and ESP32 chips and gcc in mind.

Very much a WIP!

Todo:

  * ADC input range tester [done]
    * Helper callbacks for ADC input range testers.
  * ADC channel isolation tester
  * DAC/Codec output test helper callback functions
  * Generic I2C peripheral ping tester
  * Codec input helper callback functions
  * LED Button tester [done]
  * Toggle switch tester (2 or 3 position)
  * Gate input tester [done]
  * Gate output tester
  * FLASH RAM tester
  * LED tester 


To run tests:

```
make -f tests/Makefile
```

