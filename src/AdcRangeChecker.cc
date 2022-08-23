#include "libhwtests/AdcRangeChecker.hh"

AdcRangeChecker::AdcRangeChecker(const AdcRangeCheckerBounds &init)
	: _bounds(init) {
	reset();
}

void AdcRangeChecker::reset() {
	_coverage = 0;
	_countdown = _bounds.center_check_counts;
}

AdcCheckState AdcRangeChecker::check() {
	AdcCheckState state;

	if (_cur_val < _bounds.min_val) {
		state = AdcCheckState::AtMin;
		_coverage |= (0b10UL);
	} else if (_cur_val > _bounds.max_val) {
		state = AdcCheckState::AtMax;
		_coverage |= (0b01UL);
	} else if (_cur_val > (_bounds.center_val - _bounds.center_width) &&
			   _cur_val < (_bounds.center_val + _bounds.center_width))
	{
		_countdown--;
		if (_countdown == 0 && _coverage == 0b11)
			state = AdcCheckState::FullyCovered;
		else
			state = AdcCheckState::AtCenter;
	} else {
		_countdown = _bounds.center_check_counts;
		state = AdcCheckState::Elsewhere;
	}

	return state;
}

void AdcRangeChecker::set_adcval(uint16_t adcval) {
	_cur_val = adcval;
}
