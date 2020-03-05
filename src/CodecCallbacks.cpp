#include "CodecCallbacks.h"


TestRampUpOscillator::TestRampUpOscillator(float freqHz, float max, float min, float initial_phase, float sample_rate)
	: OutputStream(sample_rate)
	, _inc(freqHz/sample_rate)
	, _max(max)
	, _min(min)
	, _cur_phase(initial_phase)
{
}

float TestRampUpOscillator::update() {
	_cur_phase += _inc;
	while (_cur_phase > 1.f)
		_cur_phase -= 1.f;

	return ((_cur_phase * (_max-_min)) + _min);
}

SkewedTriOsc::SkewedTriOsc(float sample_rate)
	: OutputStream(sample_rate)
{}

SkewedTriOsc::SkewedTriOsc(float freqHz, float riseRatio, float max, float min, float initial_phase, float sample_rate)
	: OutputStream(sample_rate)
	, _rise_inc(freqHz / (sample_rate*riseRatio))
	, _fall_inc(freqHz / (sample_rate*(1.0f-riseRatio)))
	, _max(max)
	, _min(min)
	, _cur_phase(initial_phase)
{
	_is_rising = (_cur_phase < riseRatio);
}

void SkewedTriOsc::init(float freqHz, float riseRatio, float max, float min, float initial_phase, float sample_rate) {
	_sample_rate = sample_rate;
	_rise_inc = freqHz / (_sample_rate*riseRatio);
	_fall_inc = freqHz / (_sample_rate*(1.0f-riseRatio));
	_max = max;
	_min = min;
	_cur_phase = initial_phase;
	_is_rising = (_cur_phase < riseRatio);
}

float SkewedTriOsc::update() {
	if (_is_rising)
		_cur_phase += _rise_inc;
	else
		_cur_phase -= _fall_inc;

	if (_cur_phase >= 1.0f) {
		_is_rising = false;
		_cur_phase = 1.0f;
	}
	if (_cur_phase <= 0.0f) {
		_is_rising = true;
		_cur_phase = 0.0f;
	}
	return ((_cur_phase * (_max-_min)) + _min);
}

CenterFlatRamp::CenterFlatRamp(float sample_rate)
	: OutputStream(sample_rate)
{}

CenterFlatRamp::CenterFlatRamp(float freqHz, float flat_width, float max, float min, float initial_phase, float sample_rate)
	: OutputStream(sample_rate)
	, _inc(freqHz / sample_rate)
	, _flat_start(0.5f - flat_width*0.5f)
	, _flat_end(0.5f + flat_width*0.5f)
	, _nonflat_slope(0.5f/_flat_start)
	, _max(max)
	, _min(min)
	, _cur_phase((max-min)*initial_phase)
{}

void CenterFlatRamp::init(float freqHz, float flat_width, float max, float min, float initial_phase, float sample_rate) {
	_sample_rate = sample_rate;
	_inc = freqHz / _sample_rate;
	_flat_start = 0.5f - flat_width*0.5f;
	_flat_end = 0.5f + flat_width*0.5f;
	_nonflat_slope = 0.5f/_flat_start;
	_max = max;
	_min = min;
	_cur_phase = initial_phase;
}

float CenterFlatRamp::update() {
	_cur_phase += _inc;
	while (_cur_phase > 1.0f) {
		_cur_phase -= 1.0f;
	}
	
	float out;
	if (_cur_phase < _flat_start)
		out = _nonflat_slope * _cur_phase;

	else if (_cur_phase < _flat_end)
		out = 0.5f;

	else
		out = _nonflat_slope * (_cur_phase - _flat_end) + 0.5f;

	return ((out * (_max-_min)) + _min);
}

static OutputStream *testWaveLeft;
static OutputStream *testWaveRight;
static OutputStream *testWaveLeft_codec2;
static OutputStream *testWaveRight_codec2;

void assign_testWaveLeft(OutputStream *s) {
	testWaveLeft = s;
}

void assign_testWaveRight(OutputStream *s) {
	testWaveRight = s;
}

void assign_testWaveLeft_codec2(OutputStream *s) {
	testWaveLeft_codec2 = s;
}

void assign_testWaveRight_codec2(OutputStream *s) {
	testWaveRight_codec2 = s;
}

void test_audio_outs_cb(int16_t *src, int16_t *dst, uint16_t sz, uint8_t channel) {
	OutputStream *testWaveL, *testWaveR;
	if (channel==0) {
		testWaveL = testWaveLeft;
		testWaveR = testWaveRight;
	}
	else if (channel==1) {
		testWaveL = testWaveLeft_codec2;
		testWaveR = testWaveRight_codec2;
	}
	else return;

	if (testWaveL==nullptr || testWaveR==nullptr)
		return;

	for (uint16_t i=0; i<sz/2; i++)
	{
		float leftOut = testWaveL->update();
		*dst++ = (int16_t)leftOut;
		*dst++ = 0;

		float rightOut = testWaveR->update();
		*dst++ = (int16_t)rightOut;
		*dst++ = 0;
	}
	(void)(*src);//unused
}

void test_audio_ins_cb(int16_t *src, int16_t *dst, uint16_t sz, uint8_t channel) {
	OutputStream *testWaveL, *testWaveR;
	if (channel==0) {
		testWaveL = testWaveLeft;
		testWaveR = testWaveRight;
	}
	else if (channel==1) {
		testWaveL = testWaveLeft_codec2;
		testWaveR = testWaveRight_codec2;
	}
	else return;

	if (testWaveL==nullptr || testWaveR==nullptr)
		return;

	for (uint16_t i=0; i<sz/2; i++)
	{
		float leftOut = testWaveL->update();
		*dst++ = ((int16_t)leftOut)/2 + (*src++);
		*dst++ = *src++;

		float rightOut = testWaveR->update();
		*dst++ = ((int16_t)rightOut)/2 + (*src++);
		*dst++ = *src++;
	}
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

