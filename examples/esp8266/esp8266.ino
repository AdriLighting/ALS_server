// #define ALS_USE_LEDSFASTLEDRGBW      // Effect for rgb ws2812b strip
// #define ALS_USE_LEDSFASTLEDRGBW      // Effect for rgb SK6812 strip
#define ALS_USE_LEDSFASTLED

#define ALS_USE_ESPWEBSERVER
#define ALS_USE_UDP
// #define ALS_USE_UDPMULTI             // UdpMulticast used for the tft interface
// #define ALS_USE_SOCKETSERVER         // not used yet

#include <ALS_server.h>
#include <adri_tools.h>

// LIST
PROGMEM chooseEffectList chooseEffect [] = { 
	{chooseEffect_color},
    {chooseEffect_noise},
    {chooseEffect_noise_2},
    {chooseEffect_flock},
    {chooseEffect_bouncingBalls},
    {chooseEffect_sinusoid},
    {chooseEffect_plasma},
    {chooseEffect_fire18},
    {chooseEffect_fire},
    {chooseEffect_torch},
    {chooseEffect_rainbow_Matrix},
    {chooseEffect_rainbow_MatrixV},
    {chooseEffect_rainbow_MatrixH},
    {chooseEffect_rainbow_smoke},
    {chooseEffect_tower_h1},
    {chooseEffect_tower_h2},
    {chooseEffect_tower_h3},
};
uint8_t chooseEffectCnt = ARRAY_SIZE(chooseEffect);

leds <D4, GRB>myLed(16,14,NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,3);

ALS_main * _ASL;				// 

void setup() {
	Serial.begin(115200);
	delay(1000);
	fsprintf("\n");

	chooseEffect_itialize(chooseEffect, chooseEffectCnt);

	_ASL = new ALS_main();

	_ASL->setup("RGB4", &myLed);

	if (!ALS_wifiPtr_get()->_setupFromSpiff()) { 
		ALS_wifiPtr_get()->_setupAp(AWC_SETUP, AWCS_AP);
		ALS_wifiPtr_get()->_connect(AWC_SETUP);
	} else {
		ALS_wifiPtr_get()->_connect(AWC_LOOP);
	}
	
}   


void loop() {
	_ASL->loop();
	if(lampPeripheralsInstance()!=nullptr)lampPeripheralsInstance()->loop();
}