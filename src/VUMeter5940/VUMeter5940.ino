#include "SparkFun_Tlc5940.h"

float rapidStrength = 0.1;
float delayStrength = 0.01;
uint16_t inputLevel[2] = {0.0,0.0}; // 0 = right, 1 = left
uint16_t inputRaw[2];
uint16_t inputRawReady[2][2];

#define SIDE_PIN (4)

float measuredAverageVoltage = 2;
uint16_t offsetMidpoint = (measuredAverageVoltage / 5) * 1024;

#define LED_SETS 4
#define LED_LINE_COUNT 5
uint16_t leds[LED_SETS][LED_LINE_COUNT];
uint16_t center = 0;
uint16_t center2 = 0;

#define OUTPUT_SETS 2
#define OUTPUT_5940 11
uint16_t *outputs[OUTPUT_SETS][OUTPUT_5940];

volatile void tlc_animationXLATCallback(void);

#define UPDATE_DELAY (0)
uint32_t startUpdate = 0;

#define PWM_MAX (4095)

bool side = false;

void mapOutputs()
{
	outputs[0][0] = &leds[0][0];
	outputs[0][1] = &leds[0][1];
	outputs[0][2] = &leds[0][2];
	outputs[0][3] = &leds[0][3];
	outputs[0][4] = &leds[0][4];

	outputs[0][5] = &center;

	outputs[0][6] = &leds[1][4];
	outputs[0][7] = &leds[1][3];
	outputs[0][8] = &leds[1][2];
	outputs[0][9] = &leds[1][1];
	outputs[0][10] = &leds[1][0];

	outputs[1][0] = &leds[2][0];
	outputs[1][1] = &leds[2][1];
	outputs[1][2] = &leds[2][2];
	outputs[1][3] = &leds[2][3];
	outputs[1][4] = &leds[2][4];

	outputs[1][5] = &center2;

	outputs[1][6] = &leds[3][4];
	outputs[1][7] = &leds[3][3];
	outputs[1][8] = &leds[3][2];
	outputs[1][9] = &leds[3][1];
	outputs[1][10] = &leds[3][0];


}

volatile void tlc_animationXLATCallback(void)
{
	side = !side;

	uint8_t i = (side?1:0);
	for (uint8_t j = 0; j < OUTPUT_5940; ++j) {
		Tlc.set(j,*outputs[i][j]);
	}

	digitalWrite(SIDE_PIN,side?HIGH:LOW);
	startUpdate = millis() + UPDATE_DELAY;
}

void setup()
{
	tlc_onUpdateFinished = tlc_animationXLATCallback;
	//Serial.begin(115200);
	resetLEDs();

	mapOutputs();

	pinMode(SIDE_PIN,OUTPUT);
	digitalWrite(SIDE_PIN,LOW);


	Tlc.init();

	//center = 4000;

	leds[3][0] = 4000;
	leds[3][1] = 2000;
	leds[3][2] = 1000;
	//leds[1][0] = 4000;
	//leds[2][0] = 4000;
	//leds[2][1] = 4000;

/*
	leds[2][0] = 1000;
	leds[2][1] = 2000;
	leds[2][2] = 3000;
	leds[2][3] = 4000;
	leds[2][4] = PWM_MAX;
	*/
}

void resetLEDs()
{
	for (uint8_t i = 0; i < LED_SETS; ++i) {
		for (uint8_t j = 0; j < LED_LINE_COUNT; ++j) {
			leds[i][j] = 0;
		}
	}
}


void imageCycle()
{
	if (Tlc.update())
		return;

	delay(500);

	delay(500);
}


void loop()
{
/*
	Serial.print(inputRaw[0][1]);
	Serial.print(" , ");
	Serial.print(inputRaw[0][0]);
	Serial.print(" , ");
	Serial.println(amplitudeValue(analogRead(A0)));
	*/

	//Serial.print(input);
	//Serial.print(" : ");
/*	Serial.print(inputRaw[0][0]);
	Serial.print(" : ");
	int scale = input / 2;
	Serial.print(scale);
	Serial.print(" : ");

	for(int i = 0; i < scale ; ++i )
		Serial.print("=");

	resetLEDs();
	int ledScale = input / 15;
	for(int i = 0; i < ledScale ; ++i)
		digitalWrite(leds[i], HIGH);

	Serial.println();
	*/
	if (millis() > startUpdate)
		Tlc.update();

	readPot();
}

void readPot()
{
	float smoothing = 0.1;
	uint16_t input = analogRead(A7)-25;

	inputRaw[0] = iir(inputRaw[0],(input>1024?0:input),smoothing);
	uint32_t output = (inputRaw[0]>1024?0:inputRaw[0]) / 250.0 * (PWM_MAX*5);
	/*
	Serial.print("Input: ");
	Serial.print(input);
	Serial.print(", Output: ");
	Serial.println(output);
	*/

	for (uint8_t j = 0; j < LED_LINE_COUNT; ++j) {
		if (output > PWM_MAX) {
			leds[0][j] = PWM_MAX;
			leds[2][j] = PWM_MAX;
			output -= PWM_MAX;
		} else {
			leds[0][j] = output;
			leds[2][j] = output;
			output = 0;
		}
	}

	input = analogRead(A6)-25;
	inputRaw[1] = iir(inputRaw[1],(input>1024?0:input),smoothing);
	output = (inputRaw[1]>1024?0:inputRaw[1]) / 250.0 * (PWM_MAX*5);

	for (uint8_t j = 0; j < LED_LINE_COUNT; ++j) {
		if (output > PWM_MAX) {
			leds[1][j] = PWM_MAX;
			leds[3][j] = PWM_MAX;
			output -= PWM_MAX;
		} else {
			leds[1][j] = output;
			leds[3][j] = output;
			output = 0;
		}
	}

}


uint16_t iir(int32_t oldData, int32_t newData, float strength)
{
/*	Serial.print("Old Data: ");
	Serial.println(oldData);
	Serial.print("New Data: ");
	Serial.println(newData);
	Serial.print("Difference: ");
	Serial.println(newData-oldData);
	Serial.print("Change: ");
	Serial.println(strength*(newData-oldData));
	Serial.print("New Value: ");
	Serial.println(oldData + strength*(newData-oldData));*/

	return oldData + strength * (newData - oldData);
}
