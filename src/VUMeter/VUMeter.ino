float rapidStrength = 0.1;
float delayStrength = 0.01;
uint16_t inputLevel[2] = {0.0,0.0}; // 0 = right, 1 = left
uint16_t inputRaw[2][2];
uint16_t inputRawReady[2][2];

float measuredAverageVoltage = 1.76;
uint16_t offsetMidpoint = (measuredAverageVoltage / 5) * 1024;



void setup()
{
	Serial.begin(115200);
	resetRaw();
	inputRaw[0][0] = 0;
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
	if (input > offsetMidpoint)
		inputRaw[0][0] = iir(inputRaw[0][0],input-offsetMidpoint,0.5);
	//Serial.print(input);
	//Serial.print(" : ");
	Serial.print(inputRaw[0][0]);
	Serial.print(" : ");
	int scale = inputRaw[0][0] / 2;
	Serial.print(scale);
	Serial.print(" : ");

	for(int i = 0; i < scale ; ++i )
		Serial.print("=");

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
