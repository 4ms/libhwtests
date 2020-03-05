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

SkewedTriOsc::SkewedTriOsc(float sample_rate)
	: OutputStream(sample_rate)
{}

SkewedTriOsc::SkewedTriOsc(float freqHz, float riseRatio, float max, float min, float initial_phase, float sample_rate)
	: OutputStream(sample_rate)
	, _rise_inc(((max-min) * freqHz) / (sample_rate*riseRatio))
	, _fall_inc(((max-min) * freqHz) / (sample_rate*(1.0f-riseRatio)))
	, _max(max)
	, _min(min)
	, _cur_phase(initial_phase)
{
	_is_rising = (_cur_phase < ((_max-_min)*riseRatio));
}

void SkewedTriOsc::init(float freqHz, float riseRatio, float max, float min, float initial_phase) {
	_rise_inc = (max-min) * freqHz / (_sample_rate*riseRatio);
	_fall_inc = (max-min) * freqHz / (_sample_rate*(1.0f-riseRatio));
	_max = max;
	_min = min;
	_cur_phase = initial_phase;
	_is_rising = (_cur_phase < ((_max-_min)*riseRatio));
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

