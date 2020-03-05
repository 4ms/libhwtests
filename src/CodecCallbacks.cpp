#include "CodecCallbacks.h"


TestRampUpOscillator::TestRampUpOscillator(float freqHz, float max, float min, float initial_phase, float sample_rate)
	: OutputStream(sample_rate)
	, _inc((max-min) * freqHz/sample_rate)
	, _max(max)
	, _min(min)
	, _cur_phase(initial_phase)
{
}

float TestRampUpOscillator::update() {
	_cur_phase += _inc;
	if (_cur_phase > _max)
		_cur_phase = _min;

	return _cur_phase;
}

SkewedTriOsc::SkewedTriOsc(float freqHz, float riseRatio, float max, float min, float initial_phase, float sample_rate)
	: OutputStream(sample_rate)
	, _rise_inc(((max-min) * freqHz) / (sample_rate*riseRatio))
	, _fall_inc(((max-min) * freqHz) / (sample_rate*(1.0f-riseRatio)))
	, _max(max)
	, _min(min)
	, _cur_phase(initial_phase)
{
	if (_cur_phase > ((_max-_min)*riseRatio))
		_is_rising = false;
	else
		_is_rising = true;
}

float SkewedTriOsc::update() {
	if (_is_rising)
		_cur_phase += _rise_inc;
	else
		_cur_phase -= _fall_inc;

	if (_cur_phase >= _max) {
		_is_rising = false;
		_cur_phase = _max;
	} 
	if (_cur_phase <= _min) {
		_is_rising = true;
		_cur_phase = _min;
	}

	return _cur_phase;
}

/*
void CodecCallbacks::testwave_out_stereo_multicodec_15(int16_t *src, int16_t *dst, uint16_t sz, uint8_t channel) {
	int32_t *dst32 = (int32_t*)dst;

	for (uint16_t i=0; i<sz; i++)
	{
		*dst32++ = (int32_t)(leftOutMulti[channel].update());
		*dst32++ = (int32_t)(rightOutMulti[channel].update());
	}
	(void)(*src);//unused
}
*/

