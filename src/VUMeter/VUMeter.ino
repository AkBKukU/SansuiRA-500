float rapidStrength = 0.1;
float delayStrength = 0.01;
uint16_t inputLevel[2] = {0.0,0.0}; // 0 = right, 1 = left
uint16_t inputRaw[2][2];
uint16_t inputRawReady[2][2];

float measuredAverageVoltage = 1.76;
uint16_t offsetMidpoint = (measuredAverageVoltage / 5) * 1024;


uint16_t iir(uint16_t oldData, uint16_t newData, float strength);

void setup()
{
	Serial.begin(115200);
	resetRaw();
}


void loop()
{
	Serial.print(inputRaw[0][1]);
	Serial.print(",");
	Serial.print(inputRaw[0][0]);
	Serial.print(" = ");
	Serial.println(amplitudeValue(analogRead(A0)));
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
	if (input < inputRaw[0][0]) {
		inputRaw[0][0] = iir(inputRaw[0][0],input,0.2);	
	}else if(inputRaw[0][0] != offsetMidpoint) {
		inputRawReady[0][0] = true;
	}

	if (input > inputRaw[0][1]) {
		inputRaw[0][1] = iir(inputRaw[0][1],input,0.2);	
	}else if(inputRaw[0][1] != offsetMidpoint) {
		inputRawReady[0][1] = true;
	}

	
	if (inputRawReady[0][1] && inputRawReady[0][0]) {
		uint16_t value = inputRaw[0][1] - inputRaw[0][0];
		resetRaw();
		return value;
	}else{
		return 0;
	}
}

uint16_t iir(uint16_t oldData, uint16_t newData, float strength)
{
	return oldData += strength * (oldData - newData);
}
