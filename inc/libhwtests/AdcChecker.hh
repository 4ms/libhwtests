#pragma once
#include "AdcRangeChecker.hh"
#include <cstdint>

struct IAdcChecker {
	enum AdcType {
		Pot,
		BipolarCV,
		UnipolarCV,
	};

	IAdcChecker(const AdcRangeCheckerBounds bounds,
				const uint8_t num_pots,
				const uint8_t num_bipolarCV,
				const uint8_t num_unipolarCV)
		: checker(bounds)
		, num_pots(num_pots)
		, num_bipolarCV(num_bipolarCV)
		, num_unipolarCV(num_unipolarCV)
		, num_adc(num_pots + num_bipolarCV + num_unipolarCV) {
	}

	~IAdcChecker() = default;

	virtual uint32_t get_adc_reading(uint8_t adc_i, AdcType adctype) = 0;
	virtual void set_indicator(uint8_t adc_i, AdcType adctype, AdcCheckState state) = 0;
	virtual void pause_between_steps() = 0;
	virtual void show_multiple_nonzeros_error() = 0;
	virtual bool button_to_skip_step() = 0;
	virtual void delay_ms(uint32_t x) = 0;

	void run_test();

private:
	AdcRangeChecker checker;
	const uint8_t num_pots;
	const uint8_t num_bipolarCV;
	const uint8_t num_unipolarCV;
	const uint8_t num_adc;

	bool _check_max_one_cv_is_nonzero(uint32_t width, uint32_t center) {
		uint8_t num_nonzero = 0;

		for (unsigned adc_i = 0; adc_i < num_bipolarCV; adc_i++) {
			auto val = get_adc_reading(adc_i, BipolarCV);
			if (val > (center + width / 2) || val < (center - width / 2))
				num_nonzero++;
		}

		return (num_nonzero <= 1);
	}
};
