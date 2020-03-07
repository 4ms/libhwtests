#pragma once
#include "libhwtests_common.h"

class OutputStream {
public: 
	OutputStream(float sample_rate=48000.0f) 
	: _sample_rate(sample_rate)
	{}
	virtual float update() = 0;

protected:
	float _sample_rate;
};

class TestRampUpOscillator : public OutputStream {
public:
	TestRampUpOscillator(float freqHz, float max, float min, float initial_phase, float sample_rate);
	void init(float freqHz, float max, float min, float initial_phase, float sample_rate);
	float update();

protected:
	float _inc;
	float _max;
	float _min;
	float _cur_phase;
};

class SkewedTriOsc : public OutputStream {
public:
	SkewedTriOsc(float sample_rate=48000.0f);
	SkewedTriOsc(float freqHz, float riseRatio, float max, float min, float initial_phase, float sample_rate);
	void init(float freqHz, float riseRatio, float max, float min, float initial_phase, float sample_rate);
	float update();

private:
	float _rise_inc;
	float _fall_inc;
	float _max;
	float _min;
	float _cur_phase;
	bool _is_rising;
};

class CenterFlatRamp : public OutputStream {
public:
	CenterFlatRamp(float sample_rate=48000.0f);
	CenterFlatRamp(float freqHz, float flat_width, float max, float min, float initial_phase, float sample_rate);
	void init(float freqHz, float flat_width, float max, float min, float initial_phase, float sample_rate);
	float update();

private:
	float _inc;
	float _flat_start;
	float _flat_end;
	float _nonflat_slope;
	float _max;
	float _min;
	float _cur_phase;
};

class TestGateOscillator : public OutputStream {
public:
	TestGateOscillator(float sample_rate=48000.0f);
	TestGateOscillator(float freqHz, float pw, float max, float min, float initial_phase, float sample_rate);
	void init(float freqHz, float pw, float max, float min, float initial_phase, float sample_rate);
	float update();

private:
	float _inc;
	float _pw;
	float _max;
	float _min;
	float _cur_phase;
};

void test_audio_outs_cb(int16_t *src, int16_t *dst, uint16_t sz, uint8_t channel);
void test_audio_passthruPlusTestWave_cb(int16_t *src, int16_t *dst, uint16_t sz, uint8_t channel);

void assign_testWaveLeft(OutputStream *s);
void assign_testWaveRight(OutputStream *s);
void assign_testWaveLeft_codec2(OutputStream *s);
void assign_testWaveRight_codec2(OutputStream *s);


struct CodecCallbacks {
	//static void testwave_out_stereo_multicodec_16(int16_t *src, int16_t *dst, uint16_t sz, uint8_t channel);
	static void passthruPlusTestWave_2codecs(int16_t *src, int16_t *dst, uint16_t sz, uint8_t channel);

	static inline OutputStream *leftOut;
	static inline OutputStream *rightOut;
	static inline OutputStream *leftOutCodec2;
	static inline OutputStream *rightOutCodec2;
};


