#include "libhwtests/AdcChecker.hh"

void IAdcChecker::run_test() {
	for (uint32_t i = 0; i < num_adc; i++) {
		AdcType cur_adctype = i < num_pots ? Pot : i < (num_pots + num_bipolarCV) ? BipolarCV : UnipolarCV;

		auto adc_i = cur_adctype == Pot ? i : cur_adctype == BipolarCV ? i - num_pots : i - num_pots - num_bipolarCV;

		pause_between_steps();
		set_indicator(adc_i, cur_adctype, ADCCHECK_NO_COVERAGE);

		bool done = false;
		bool zeroes_ok = true;

		checker.reset();

		while (!done) {
			uint16_t adcval = get_adc_reading(adc_i, cur_adctype);
			checker.set_adcval(adcval);

			if (cur_adctype != Pot) {
				zeroes_ok = _check_max_one_cv_is_nonzero(300, 2048);
				if (!zeroes_ok) {
					show_multiple_nonzeros_error();
					checker.reset();
					set_indicator(adc_i, cur_adctype, ADCCHECK_NO_COVERAGE);
				}
			}

			auto status = checker.check();
			if (status == ADCCHECK_AT_MIN) {
				set_indicator(adc_i, cur_adctype, ADCCHECK_AT_MIN);
			} else if (status == ADCCHECK_AT_MAX) {
				set_indicator(adc_i, cur_adctype, ADCCHECK_AT_MAX);
			} else if (status == ADCCHECK_AT_CENTER) {
				set_indicator(adc_i, cur_adctype, ADCCHECK_AT_CENTER);
			} else if (status == ADCCHECK_ELSEWHERE) {
				set_indicator(adc_i, cur_adctype, ADCCHECK_ELSEWHERE);
			} else if (status == ADCCHECK_FULLY_COVERED) {
				done = true;
			}
			if (button_to_skip_step())
				done = true;
		}
		delay_ms(100);
	}
}
