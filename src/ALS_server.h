


#include <ALS_leds_effects.h>

#ifdef ALS_USE_LEDSFASTLED
	#include <ALS_leds_fastled.h>
	#include <ALS_effectsRegister_fastled.h>
#endif
#ifdef ALS_USE_LEDSFASTLEDRGBW
	#include <ALS_leds_fastledRGBW.h>
	#include <ALS_effectsRegister_fastledRGBW.h>
#endif
#ifdef ALS_USE_LEDSADAFRUITNEO
	#include <ALS_leds_adafruitneopixel.h>
	#include <ALS_effectsRegister_adaneo.h>
#endif

	#include <ALS_leds_segment.h>

#include <ALS_leds.h>

#include "ALS_main.h"

