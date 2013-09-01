#include "CppUTest/TestHarness.h"
#include "MorseCode.h"
#include "MockArduino.h"

#define MAX_EXPECTATIONS 100

#define LED_PIN_NUMBER 13
#define LONG_CODE_PERIOD 600
#define SHORT_CODE_PERIOD 200
#define CODE_BOUNDARY_PERIOD 200
#define CHARACTER_BOUNDARY_PERIOD 400
#define WORD_BOUNDARY_PERIOD 1200

#define UNSIGNED_LONG_MAX 0xffffffff

TEST_GROUP( MorseCode )
{

MorseCode *morseCode;

void setup()
{
    MockArduino_Create( MAX_EXPECTATIONS );

    morseCode = new MorseCode();
    morseCode->setLedPinNumber( LED_PIN_NUMBER );
    morseCode->setLongCodePeriod( LONG_CODE_PERIOD );
    morseCode->setShortCodePeriod( SHORT_CODE_PERIOD );
    morseCode->setCodeBoundaryPeriod( CODE_BOUNDARY_PERIOD );
    morseCode->setCharacterBoundaryPeriod( CHARACTER_BOUNDARY_PERIOD );
    morseCode->setWordBoundaryPeriod( WORD_BOUNDARY_PERIOD );
}

void teardown()
{
    delete morseCode;
    morseCode = 0;

    MockArduino_Destroy();
}

};

TEST( MorseCode, ShowSentence_e )
{
    unsigned long startMillis = 1000;

    MockArduino_setMillis( startMillis );
    MockArduino_Expect_digitalWrite( LED_PIN_NUMBER, HIGH );
    morseCode->showSentence( "e" );
    MockArduino_Verify_Complete();

    MockArduino_setMillis( startMillis );
    morseCode->loop();
    MockArduino_Verify_Complete();

    MockArduino_setMillis( startMillis + SHORT_CODE_PERIOD - 1 );
    morseCode->loop();
    MockArduino_Verify_Complete();

    MockArduino_setMillis( startMillis + SHORT_CODE_PERIOD );
    MockArduino_Expect_digitalWrite( LED_PIN_NUMBER, LOW );
    morseCode->loop();
    MockArduino_Verify_Complete();

    MockArduino_setMillis( UNSIGNED_LONG_MAX );
    morseCode->loop();
    MockArduino_Verify_Complete();
}

