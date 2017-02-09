/*
 Name:		PythonSerial.ino
 Created:	12/28/2016 1:55:53 AM
 Author:	fsr19
*/

#include <Servo.h>
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

Servo servos[12];

// the setup function runs once when you press reset or power the board
void setup() {
	lcd.begin(16, 2);
	lcd.backlight();
	lcd.setCursor(0, 0);
	printToLCD("Not setup!");
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
		else if (c == 'A') {
			value = analogRead(pin);
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
	case 'D':
		c = command.charAt(2);
		if (c == 'T') {
			printToLCD(getValue(command, ':', 2));
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



void printToLCD(String text) {
	lcd.clear();
	lcd.setCursor(0, 0);
	int strlength = text.length();
	if (strlength > 32) {
	printToLCD("Error: Failed to print");
	return;
	}
	if (strlength <= 16) {
	lcd.print(text);
	}
	//Convert to double line
	else {
	int index = 0;
	index = text.indexOf(' ', index);
	int oldIndex;
	while (index <= 16) {
	oldIndex = index;
	index = text.indexOf(' ', index + 1);
	}
	lcd.print(text.substring(0, oldIndex));
	lcd.setCursor(0, 1);
	lcd.print(text.substring(oldIndex + 1, strlength));
	}
}

