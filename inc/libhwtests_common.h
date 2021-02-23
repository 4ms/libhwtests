///DEPRECATED!
#pragma once
#include <stdint.h>

typedef void (*OutputGateFuncType)(uint8_t gate_num, bool newstate);
typedef bool (*ReadGateFuncType)(uint8_t gate_num);
typedef void (*IndicatorOnOffFuncType)(uint8_t indicator_num, bool newstate);
typedef void (*GateOnOffFuncType)(bool newstate);
typedef bool (*ButtonReadFuncType)(uint8_t button_num);
typedef void (*ButtonLEDSetFuncType)(uint8_t button_num, bool turn_on);

