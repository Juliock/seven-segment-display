/* Display library

License Here
Company Here
*/

#include <Display.h>

Display::Display(int *controlPins, int *dataPins, uint8_t type, int digits) {
	_controlPins = controlPins;
	_dataPins = dataPins;
	_type = type;
	_digits = digits;

	enable = true;

	scroll = false;

	scrollInterval = 500;

	dutyHigh = 500;

	dutyLow = 500;
}

void Display::begin(void) {

	// Serial.begin(9600);

	DEBUG_PRINTLN("Control Pins:");

	for (int pin = 0; pin < _digits; pin++) {
		pinMode(_controlPins[pin], OUTPUT);

		if (_type == commonCathode) {
			digitalWrite(_controlPins[pin], LOW);
		}else {
			digitalWrite(_controlPins[pin], HIGH);
		}

		DEBUG_PRINTLN(_controlPins[pin]);
	}

	DEBUG_PRINTLN("Data Pins:");

	for (int pin = 0; pin < 8; pin++) {
		pinMode(_dataPins[pin], OUTPUT);

		if (_type == commonCathode) {
			digitalWrite(_dataPins[pin], LOW);
		}else {
			digitalWrite(_dataPins[pin], HIGH);
		}

		DEBUG_PRINTLN(_dataPins[pin]);
	}
    
    DEBUG_PRINT("Display type: "); DEBUG_PRINTLN(_type);
	DEBUG_PRINT("Number of displays: "); DEBUG_PRINTLN(_digits);
}

void Display::write(int data) {
	SEND(data);
}

void Display::write(double data) {
	SEND(data);
}

void Display::write(float data) {
	SEND(data);
}

void Display::write(String data) {
	SEND(data);
}

void Display::brightness(int percentage) {
	float duty = (float) percentage / 100.0;

	dutyHigh = duty * 1000;

	dutyLow = 1000 - dutyHigh;
}

void Display::send(String data) {

	DEBUG_PRINT("Data: "); DEBUG_PRINTLN(data);

	uint32_t currentTime = millis();

	uint32_t lastScrollTime = currentTime;

	char character;
	char hash;
	int character_count = 0;
	int count = 0;

	_curr_digit = 0;
	
	for (int c = 0; c < data.length(); c++) {
		character = data.charAt(c);

		if ( findChar(character) ) {
			if (character != '.') {
				count++;
			}
		}else {
			data.setCharAt(c, ' ');		//Replace non-printable character with space
		}
	}

	if (!scroll) {
		if (count < _digits) {
			_curr_digit += _digits - count;
		}
	}

	while (true) {

		character = data.charAt(character_count);

		if ( (character == '.') && (_curr_digit != 0) ) {
			if (data.charAt(character_count - 1) != '.') {
				_curr_digit--;
			}
		}

		if ( (_curr_digit < _digits) && (character_count < data.length()) ) {
			DEBUG_PRINT("Character: "); DEBUG_PRINTLN(character);
		
			DEBUG_PRINT("Display: " ); DEBUG_PRINTLN(_curr_digit, DEC);

			if (findChar(character)) {
				hash = getCharHash(character);

				DEBUG_PRINT("Character Hash: " ); DEBUG_PRINTLN(hash, HEX);
		
				for (int segment = 0; segment < 8; segment++) {
					int b = (hash >> segment) & 1;
					
					if (b == 1) {
						if (_type == commonCathode) {
							digitalWrite(_dataPins[segment], HIGH);
						}else {
							digitalWrite(_dataPins[segment], LOW);
						}
					}else {
						if (_type == commonCathode) { 
							digitalWrite(_dataPins[segment], LOW);
						}else {
							digitalWrite(_dataPins[segment], HIGH);
						}
					}
				}

				if (_type == commonCathode) {
					digitalWrite(_controlPins[_curr_digit], HIGH);
				}else {
					digitalWrite(_controlPins[_curr_digit], LOW);
				}

				delayMicroseconds(dutyHigh);

				if (_type == commonCathode) {
					digitalWrite(_controlPins[_curr_digit], LOW);
				}else {
					digitalWrite(_controlPins[_curr_digit], HIGH);
				}

				delayMicroseconds(dutyLow);

				character_count++;
				_curr_digit++;
			}
		}else {
			if (scroll) {
				character_count = 0;
				_curr_digit = 0;

				currentTime = millis();

				if ((currentTime - lastScrollTime) > scrollInterval) {
					if (data.length() > 1) {
						String new_data = data.substring(1, data.length());

						if ( (data.charAt(0) != '.') && (new_data.charAt(0) == '.') ) {
							send(new_data.substring(1, new_data.length()));
						}else {
							send(new_data);
						}
					}
					
					break;
				}
			}else {
				break;
			}
		}
	}
	_curr_digit = 0;
}

bool Display::findChar(char c) {
	for (int h = 0; h < 51; h++) {
		if (_displayTable[h][0] == c) {
			return true;
		}
	}

	return false;
}

char Display::getCharHash(char c) {
	for (int h = 0; h < 51; h++) {
		if (_displayTable[h][0] == c) {
			return _displayTable[h][1];
		}
	}
}