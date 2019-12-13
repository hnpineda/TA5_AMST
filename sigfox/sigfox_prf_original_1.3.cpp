// ORIGINAL1.3

// Include librairies
#include <WISOL.h>
#include <Tsensors.h>
#include <Wire.h>
#include <math.h>

Isigfox *Isigfox = new WISOL();
Tsensors *tSensors = new Tsensors();

typedef union{
uint16_t number;
uint8_t bytes[2];
} UINT16_t;

void setup() {
	Wire.begin();
	Wire.setClock(100000);
	// Init serial connection between Arduino and Modem
	Serial.begin(9600);
	UINT16_t tempt;
	// WISOL modem test
	Isigfox->initSigfox();
	Isigfox->testComms();
	// Init sensors on Thinxtra Module
	tSensors->initSensors();
	// Init an interruption on the button of the Xkit
	tSensors->setButton(buttonIR);
}

// Infinite loop of the program
void loop() {
	//obtenemos la aceleración en el eje x del thinxtra
	float axeX = getAxeX();
	Serial.print("Check Axe X: "); Serial.println(axeX);
	if (axeX <= -0.4 or axeX >= 0.4) {
		//obtenemos la temperatura que nos da un tipo de dato float
		float t = tSensors->getTemp();
		//hacemos un arreglo de bytes para poder enviar byte por byte mas adelante
		byte *float_byte = (byte *)&t;
		byte *float_axeXb = (byte *)&axeX;
		//indicamos el tamaño de nuestro mensaje sabiendo que el máximo tamaño es
		de 12 bytes
		const uint8_t payloadSize = 8;
		uint8_t buf_str[payloadSize];
		buf_str[0] = float_byte[0];
		buf_str[1] = float_byte[1];
		buf_str[2] = float_byte[2];
		buf_str[3] = float_byte[3];
		buf_str[4] = float_axeXb[0];
		buf_str[5] = float_axeXb[1];
		buf_str[6] = float_axeXb[2];
		buf_str[7] = float_axeXb[3];
		Send_Pload(buf_str, payloadSize);
		// Wait 20s
		delay(20000);
	}
	delay(1000);
}


// Return the acceleration on Axe X
float getAxeX() {
	acceleration_xyz *xyz_g;
	xyz_g = (acceleration_xyz *)malloc(sizeof(acceleration_xyz));
	tSensors->getAccXYZ(xyz_g);
	float axeX = (float)xyz_g->x_g;
	free(xyz_g);
	return axeX;
}


// Get the temperature
float getTemp() {
	float temp = round(tSensors->getTemp() * 10) / 10.0;
	Serial.print("Sending Temp: "); Serial.println(temp);
	return temp;
}


// SendPayload Function => Send messages to the Sigfox Network
void Send_Pload(uint8_t *sendData, int len) {
	recvMsg *RecvMsg;
	RecvMsg = (recvMsg *)malloc(sizeof(recvMsg));
	Isigfox->sendPayload(sendData, len, 0, RecvMsg);
	for (int i = 0; i < RecvMsg->len; i++) {
	Serial.print(RecvMsg->inData[i]);
	}
	Serial.println("");
	free(RecvMsg);
}


// Button Interruption
void buttonIR(){
	float temp = getTemp();
	Send_Pload((const char*)&temp, sizeof(temp));
}