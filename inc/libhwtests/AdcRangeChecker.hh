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

	uint32_t get_cur() { return _cur_val; }
	uint32_t get_max() { return _max_val; }
	uint32_t get_min() { return _min_val; }

private:
	const AdcRangeCheckerBounds _bounds;
	bool _hit_max = false;
	bool _hit_min = false;
	uint32_t _countdown;
	uint16_t _cur_val;

	uint32_t _min_val = 4095;
	uint32_t _max_val = 0;
};
