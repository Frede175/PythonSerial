/*
 Name:		PythonSerial.ino
 Created:	12/28/2016 1:55:53 AM
 Author:	fsr19
*/

#include <Servo.h>

Servo servos[12];

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	
}

// the loop function runs over and over again until power down or reset
void loop() {
	char c; int pin; int value;
	String command = Serial.readStringUntil('\n');
	switch (command.charAt(0))
	{
	case 'W':
		pin = getValue(command, ':', 2).toInt();
		value = getValue(command, ':', 3).toInt();
		c = command.charAt(2);
		if (c == 'D') {
			digitalWrite(pin, value);
		}
		else if (c == 'A') {
			analogWrite(pin, value);
		}
		else if (c == 'S') {
			servos[pin].write(value);
		}
		break;
	case 'R':
		pin = getValue(command, ':', 2).toInt();
		c = command.charAt(2);
		if (c == 'D') {
			value = digitalRead(pin);
			Serial.println(value);
		}
	case 'S':
		c = command.charAt(2);
		pin = getValue(command, ':', 2).toInt();
		value = getValue(command, ':', 3).toInt();
		if (c == 'P') {
			
			pinMode(pin, value);
		}
		else if (c == 'S') {
			servos[pin].attach(value);
		}
		break;
			
	default:
		break;
	}
}






String getValue(String data, char separator, int index)
{
	int found = 0;
	int strIndex[] = { 0, -1 };
	int maxIndex = data.length() - 1;

	for (int i = 0; i <= maxIndex && found <= index; i++) {
		if (data.charAt(i) == separator || i == maxIndex) {
			found++;
			strIndex[0] = strIndex[1] + 1;
			strIndex[1] = (i == maxIndex) ? i + 1 : i;
		}
	}
	return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}