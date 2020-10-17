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

class ManualValue : public OutputStream {
	float _val;
	public:
		ManualValue() : OutputStream(48000.f) { _val=0; }
		virtual float update() { return _val; }
		void set_val(const float val) { _val = val; }
};

struct CodecCallbacks_TwoCodecs {
	using SampleT = int16_t;

	static void testWavesOut(SampleT *src, SampleT *dst, uint16_t sz, uint8_t channel);
	static void passthruPlusTestWave(SampleT *src, SampleT *dst, uint16_t sz, uint8_t channel);

	static inline OutputStream *leftOutCodec1 = nullptr;
	static inline OutputStream *rightOutCodec1 = nullptr;
	static inline OutputStream *leftOutCodec2 = nullptr;
	static inline OutputStream *rightOutCodec2 = nullptr;
};

//Todo: make it more clear how to use:
//SampleT <-- int16_t for 16-bit samples within frames with 32-bits per channel (padding will be added after sample data)
//SampleT <-- int32_t for 24-bit samples within frames with 32-bits per channel (must pad before calling)
template <typename SampleT, int BlockSz>
struct CodecCallbacks {
	static inline OutputStream *leftStream = nullptr;
	static inline OutputStream *rightStream = nullptr;

	static void testWavesOut(SampleT *src, SampleT *dst) {
		if (leftStream==nullptr || rightStream==nullptr) return;

		for (int i=0; i<BlockSz; i++)
		{
			float l_sample = leftStream->update();
			*dst++ = (SampleT)l_sample;
			if (sizeof(SampleT) <= 2) *dst++ = 0;

			float r_sample = rightStream->update();
			*dst++ = (SampleT)r_sample;
			if (sizeof(SampleT) <= 2) *dst++ = 0;
		}
		
		(void)(*src);//unused
	}

	static void passthruPlusTestWave(SampleT *src, SampleT *dst) {
		for (int i=0; i<BlockSz; i++)
		{
			float l_sample = leftStream->update();
			*dst++ = ((SampleT)l_sample)/2 + (*src++);
			if (sizeof(SampleT) <= 2) *dst++ = *src++;

			float r_sample = rightStream->update();
			*dst++ = ((SampleT)r_sample)/2 + (*src++);
			if (sizeof(SampleT) <= 2) *dst++ = *src++;
		}

	}
};

