#include "libhwtests/AdcRangeChecker.hh"

AdcRangeChecker::AdcRangeChecker(const AdcRangeCheckerBounds &init)
	: _bounds(init) {
	reset();
}

void AdcRangeChecker::reset() {
	_hit_min = false;
	_hit_max = false;
	_min_val = 4095;
	_max_val = 0;
	_countdown = _bounds.center_check_counts;
}

AdcCheckState AdcRangeChecker::check() {
	AdcCheckState state;
	if (_cur_val < _min_val)
		_min_val = _cur_val;
	if (_cur_val > _max_val)
		_max_val = _cur_val;

	if (_cur_val < _bounds.min_val) {
		state = AdcCheckState::AtMin;
		_hit_min = true;
	} else if (_cur_val > _bounds.max_val) {
		state = AdcCheckState::AtMax;
		_hit_max = true;
	} else if (_cur_val > (_bounds.center_val - _bounds.center_width) &&
			   _cur_val < (_bounds.center_val + _bounds.center_width))
	{
		_countdown--;
		if (_countdown == 0 && _hit_max && _hit_min)
			state = AdcCheckState::FullyCovered;
		else
			state = AdcCheckState::AtCenter;
	} else {
		_countdown = _bounds.center_check_counts;
		state = AdcCheckState::Elsewhere;
	}

	return state;
}

void AdcRangeChecker::set_adcval(uint16_t adcval) { _cur_val = adcval; }
