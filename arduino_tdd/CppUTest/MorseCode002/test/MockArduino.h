#ifndef _MOCK_ARDUINO_H_
#define _MOCK_ARDUINO_H_

#include "Arduino.h"

void MockArduino_Create( int maxExpectations );
void MockArduino_Destroy( void );
void MockArduino_setMillis( unsigned long millis );
void MockArduino_Expect_digitalWrite( int pin_number, int level );
void MockArduino_Expect_delay( int msec );
void MockArduino_Verify_Complete( void );

#endif

