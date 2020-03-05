#pragma once
#include <stdint.h>

class OutputStream {
public: 
	OutputStream(float sample_rate) 
	: _sample_rate(sample_rate)
	{}
	float update();

private:
	float _sample_rate;
};

class TestRampUpOscillator : public OutputStream {
	TestRampUpOscillator(float freqHz, float max, float min, float initial_phase, float sample_rate);

	float update();

private:
	float _inc;
	float _max;
	float _min;
	float _cur_phase;
};

class SkewedTriOsc : public OutputStream {
	SkewedTriOsc(float freqHz, float riseRatio, float max, float min, float initial_phase, float sample_rate);

	float update();

private:
	const float _rise_inc;
	const float _fall_inc;
	const float _max;
	const float _min;
	float _cur_phase;
	bool _is_rising;
};

class DualCodecOutputCallback {
public:
	DualCodecOutputCallback(OutputStream &ch1LeftOut,
							OutputStream &ch1RightOut,
							OutputStream &ch2LeftOut,
							OutputStream &ch2RightOut)
		; _ch1LeftOut(ch1LeftOut)
		, _ch1RigthOut(ch1RightOut)
		, _ch2LeftOut(ch1LeftOut)
		, _ch2RightOut(ch1RightOut)
		{}

private:
	OutputStream &_ch1LeftOut;
	OutputStream &_ch1RightOut;
	OutputStream &_ch2LeftOut;
	OutputStream &_ch2RightOut;
};

class CodecCallbacks {
public:


	static void testwave_out_stereo_multicodec_16(int16_t *src, int16_t *dst, uint16_t sz, uint8_t channel);

	static OutputStream *leftOut;
	static OutputStream *rightOut;
	static OutputStream *leftOutMulti;
	static OutputStream *rightOutMulti;
};

