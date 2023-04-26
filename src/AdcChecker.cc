#include "libhwtests/AdcChecker.hh"

void IAdcChecker::run_test() {
	for (uint32_t i = 0; i < num_adc; i++) {
		AdcType cur_adctype = i < num_pots ? Pot : i < (num_pots + num_bipolarCV) ? BipolarCV : UnipolarCV;

		auto adc_i = cur_adctype == Pot ? i : cur_adctype == BipolarCV ? i - num_pots : i - num_pots - num_bipolarCV;

		set_indicator(adc_i, cur_adctype, AdcCheckState::NoCoverage);

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
					set_indicator(adc_i, cur_adctype, AdcCheckState::NoCoverage);
				}
			}

			auto status = checker.check();

			set_indicator(adc_i, cur_adctype, status);
			if (status == AdcCheckState::FullyCovered) {
				done = true;
			}

			if (button_to_skip_step())
				done = true;
		}
		pause_between_steps();
		// delay_ms(100);
	}
}
