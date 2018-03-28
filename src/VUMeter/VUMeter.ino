float rapidStrength = 0.1;
float delayStrength = 0.01;
uint16_t inputLevel[2] = {0.0,0.0}; // 0 = right, 1 = left
uint16_t inputRaw[2][2];
uint16_t inputRawReady[2][2];

float measuredAverageVoltage = 2;
uint16_t offsetMidpoint = (measuredAverageVoltage / 5) * 1024;

#define LED_COUNT 10
uint8_t leds[LED_COUNT] = {2,3,4,5,6,7,8,9,10,11};

void setup()
{
	Serial.begin(115200);
	resetRaw();
	inputRaw[0][0] = 0;
	
	
	for (uint8_t i = 0; i < LED_COUNT; ++i) {
		pinMode(leds[i], OUTPUT);
	}
	
	for (uint8_t i = 0; i < LED_COUNT; ++i) {
		digitalWrite(leds[i], HIGH);
	}
}

void resetLEDs()
{
	for (uint8_t i = 0; i < LED_COUNT; ++i) {
		digitalWrite(leds[i], LOW);
	}
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

	uint16_t input = analogRead(A0);
	inputRaw[0][0] = iir(inputRaw[0][0],input,0.9);
	//Serial.print(input);
	//Serial.print(" : ");
/*	Serial.print(inputRaw[0][0]);
	Serial.print(" : ");
	int scale = input / 2;
	Serial.print(scale);
	Serial.print(" : ");
*/
	for(int i = 0; i < scale ; ++i )
		Serial.print("=");
	
	resetLEDs();
	int ledScale = input / 15;
	for(int i = 0; i < ledScale ; ++i)
		digitalWrite(leds[i], HIGH);

	Serial.println();
}

void resetRaw()
{
	inputRaw[0][0] = offsetMidpoint;
	inputRaw[0][1] = offsetMidpoint;
	inputRaw[1][0] = offsetMidpoint;
	inputRaw[1][1] = offsetMidpoint;

	inputRawReady[0][0] = false;
	inputRawReady[0][1] = false;
	inputRawReady[1][0] = false;
	inputRawReady[1][1] = false;
}

uint16_t amplitudeValue(uint16_t input)
{
	if (input < offsetMidpoint) {
		inputRaw[0][0] = iir(inputRaw[0][0],input,0.2);	
	}

	if (input > offsetMidpoint) {
		inputRaw[0][1] = iir(inputRaw[0][1],input,0.2);	
	}

	uint16_t value = inputRaw[0][1] - inputRaw[0][0];
	return value;
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
