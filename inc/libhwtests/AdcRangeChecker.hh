#pragma once
#include <cstdint>

struct AdcRangeCheckerBounds {
	const uint16_t center_val;
	const uint16_t center_width;
	const uint32_t center_check_counts;
	const uint16_t min_val;
	const uint16_t max_val;
};

enum class AdcCheckState {
	NoCoverage,
	AtMin,
	AtMax,
	AtCenter,
	Elsewhere,
	FullyCovered,
};

class AdcRangeChecker {
public:
	AdcRangeChecker(const AdcRangeCheckerBounds &init);
	void reset();
	void set_adcval(uint16_t adcval);
	AdcCheckState check();

private:
	const AdcRangeCheckerBounds _bounds;
	uint32_t _coverage;
	uint32_t _countdown;
	uint16_t _cur_val;
};
