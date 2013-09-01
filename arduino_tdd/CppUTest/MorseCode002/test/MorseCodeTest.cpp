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

void checkTurnOn(
    const char *sentence
)
{
    MockArduino_Expect_digitalWrite( LED_PIN_NUMBER, HIGH );
    morseCode->showSentence( sentence );
    MockArduino_Verify_Complete();
}

void checkOnPeriod(
    unsigned long startMillis,
    unsigned long period
)
{
    MockArduino_setMillis( startMillis );
    morseCode->loop();
    MockArduino_Verify_Complete();

    MockArduino_setMillis( startMillis + period - 1 );
    morseCode->loop();
    MockArduino_Verify_Complete();

    MockArduino_setMillis( startMillis + period );
    MockArduino_Expect_digitalWrite( LED_PIN_NUMBER, LOW );
    morseCode->loop();
    MockArduino_Verify_Complete();
}

void checkFinished()
{
    MockArduino_setMillis( UNSIGNED_LONG_MAX );
    morseCode->loop();
    MockArduino_Verify_Complete();
}

};

TEST( MorseCode, ShowSentence_e )
{
    unsigned long startMillis = 0;
    MockArduino_setMillis( startMillis );

    checkTurnOn( "e"  );
    checkOnPeriod( startMillis, SHORT_CODE_PERIOD );
    checkFinished();
}

TEST( MorseCode, ShowSentence_t )
{
    unsigned long startMillis = 0;
    MockArduino_setMillis( startMillis );

    checkTurnOn( "t"  );
    checkOnPeriod( startMillis, LONG_CODE_PERIOD );
    checkFinished();
}

