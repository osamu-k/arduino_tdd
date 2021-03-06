#include "MorseCode.h"

#define LED_PIN_NUMBER 13
#define SHORT_CODE_PERIOD 200
#define LONG_CODE_PERIOD (SHORT_CODE_PERIOD * 3)
#define CODE_BOUNDARY_PERIOD SHORT_CODE_PERIOD
#define CHARACTER_BOUNDARY_PERIOD (SHORT_CODE_PERIOD * 3)
#define WORD_BOUNDARY_PERIOD (SHORT_CODE_PERIOD * 7)

MorseCode *morseCode = 0;

// the setup routine runs once when you press reset:
void setup() {                
    pinMode( LED_PIN_NUMBER, OUTPUT );     

    morseCode = new MorseCode();
    morseCode->setLedPinNumber( LED_PIN_NUMBER );
    morseCode->setLongCodePeriod( LONG_CODE_PERIOD );
    morseCode->setShortCodePeriod( SHORT_CODE_PERIOD );
    morseCode->setCodeBoundaryPeriod( CODE_BOUNDARY_PERIOD );
    morseCode->setCharacterBoundaryPeriod( CHARACTER_BOUNDARY_PERIOD );
    morseCode->setWordBoundaryPeriod( WORD_BOUNDARY_PERIOD );

    morseCode->showSentence( "   abc   xyz   " );    
}

// the loop routine runs over and over again forever:
void loop() {
    morseCode->loop();
}
