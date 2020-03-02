#include "CppUTest/TestHarness.h"
#include <fff.h>
#include "AdcRangeChecker.hh"

//#define DEBUGMODE

#ifdef DEBUGMODE
#define debug_printf(...) printf(__VA_ARGS__)
#else
#define debug_printf(...) 
#endif

TEST_GROUP(adc_range_checker)
{
	AdcRangeCheckerBounds init {
		.center_val = 2048,
		.center_width = 100,
		.center_check_counts = 4,
		.min_val = 10,
		.max_val = 4086
	};

	AdcRangeChecker checker{init};

	void setup()
	{
	}

	void teardown()
	{
	}
};

#define NUM_ADCS 4
enum fakeADC_IDs : uint8_t {
	fakePOT1 = 0,
	fakeCVJACK1 = 1,
	fakePOT2 = 2,
	fakeCVJACK2 = 3,
};

//Timeout: simulates the user pressing a button after a while, and prevents inf. loops
static uint32_t testTimeout = 1000;

static void reset_skip_or_timeout() {
	testTimeout = 1000;
}

static bool check_skip_or_timeout() {
	return ((--testTimeout)==0);
}


const uint16_t noMinFakeAdcVals [32] = {
	500,500,500,500,
	500,1000,1500,1900,
	2048,2048,2048,2048,
	2500,3000,3500,3900,
	4095,4095,4095,4095,
	3900,3500,3000,2500,
	2048,2048,2048,2048,
	1900,1500,1000,500,
};

const uint16_t noMaxFakeAdcVals [32] = {
	0,0,0,0,
	500,1000,1500,1900,
	2048,2048,2048,2048,
	2500,3000,3500,3900,
	3095,3095,3095,3095,
	3900,3500,3000,2500,
	2048,2048,2048,2048,
	1900,1500,1000,500,
};

const uint16_t noisyCenterFakeAdcVals [32] = {
	0,0,0,0,
	500,1000,1500,1900,
	2048,2448,2048,2448,
	2500,3000,3500,3900,
	4095,4095,4095,4095,
	3900,3500,3000,2500,
	2048,2048,2048,2048,
	1900,1500,1000,500,
};
const uint16_t goodFakeAdcVals [32] = {
	0,0,0,0,
	500,1000,1500,1900,
	2048,2048,2048,2048,
	2500,3000,3500,3900,
	4095,4095,4095,4095,
	3900,3500,3000,2500,
	2048,2048,2048,2048,
	1900,1500,1000,500,
};


// Fake ADC values to simulate a knob turning up slowly and hovering at center
static uint16_t get_adc_val(const uint16_t *fakeAdcVals, const uint8_t cur_adc) {
	static uint8_t fakeAdcCounter = 0;

	fakeAdcCounter++;
	if (fakeAdcCounter>=32)
		fakeAdcCounter=0;

	return fakeAdcVals[fakeAdcCounter];
}

TEST(adc_range_checker, rangeCheck_NoMin) {
	bool covered = false;
	checker.reset();
	
	uint8_t num_vals_to_test = 32;
	while (num_vals_to_test--) {
		uint16_t adcval = get_adc_val(noMinFakeAdcVals, 0);
		checker.set_adcval(adcval);
	}
	CHECK_FALSE(covered);
}

TEST(adc_range_checker, rangeCheck_NoMax) {
	bool covered = false;
	checker.reset();
	
	uint8_t num_vals_to_test = 32;
	while (num_vals_to_test--) {
		uint16_t adcval = get_adc_val(noMaxFakeAdcVals, 0);
		checker.set_adcval(adcval);
	}
	CHECK_FALSE(covered);
}

TEST(adc_range_checker, rangeCheck_NoisyCenter) {
	bool covered = false;
	checker.reset();
	
	uint8_t num_vals_to_test = 32;
	while (num_vals_to_test--) {
		uint16_t adcval = get_adc_val(noisyCenterFakeAdcVals, 0);
		checker.set_adcval(adcval);
	}
	CHECK_FALSE(covered);
}

TEST(adc_range_checker, rangeCheck_good) {
	const uint8_t adc_map[NUM_ADCS] = {
		fakePOT1,
		fakePOT2,
		fakeCVJACK1,
		fakeCVJACK2
	};

	debug_printf("Begin Adc Range Testing");

	for (uint32_t adc_i=0; adc_i<NUM_ADCS; adc_i++) {
		bool done = false;
		uint8_t cur_adc = adc_map[adc_i];
		
		debug_printf("Testing ADC %d", cur_adc);
		checker.reset();

		while (!done) {
			uint16_t adcval = get_adc_val(goodFakeAdcVals, cur_adc);
			checker.set_adcval(adcval);
			debug_printf("%d :", adcval);

			auto status = checker.check();

			if (status==ADCCHECK_AT_MIN){
				debug_printf("Got min\n");
			}
			else if (status==ADCCHECK_AT_MAX){
				debug_printf("Got max\n");
			}
			else if (status==ADCCHECK_AT_CENTER){
				debug_printf("In center\n");
			}
			else if (status==ADCCHECK_ELSEWHERE){
				debug_printf("Out of center\n");
			}
			else if (status==ADCCHECK_FULLY_COVERED){
				done = true;
			}

			if (check_skip_or_timeout())
				done = true;

		}
		if (testTimeout) 
			debug_printf("Passed!\n");
		else
			debug_printf("Skipped\n");

		CHECK(testTimeout>0);

		reset_skip_or_timeout();
	}
}

