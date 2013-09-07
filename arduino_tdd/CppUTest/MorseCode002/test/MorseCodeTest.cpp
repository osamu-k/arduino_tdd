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

void checkOffPeriod(
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
    MockArduino_Expect_digitalWrite( LED_PIN_NUMBER, HIGH );
    morseCode->loop();
    MockArduino_Verify_Complete();
}

void checkCodeSequence(
    unsigned long startMillis,
    const char *codeSequence
)
{
    for( const char *cp = codeSequence; *cp != '\0'; cp++ ){
        if( cp > codeSequence ){
            checkOffPeriod( startMillis, CODE_BOUNDARY_PERIOD );
            startMillis += CODE_BOUNDARY_PERIOD;
        }
        switch( *cp ){
        case '.':
            checkOnPeriod( startMillis, SHORT_CODE_PERIOD );
            startMillis += SHORT_CODE_PERIOD;
            break;
        case '-':
            checkOnPeriod( startMillis, LONG_CODE_PERIOD );
            startMillis += LONG_CODE_PERIOD;
            break;
        default:
            FAIL( "unexpected character in code sequence !" );
            break;
        }
    }
}

void checkFinished()
{
    MockArduino_setMillis( UNSIGNED_LONG_MAX );
    morseCode->loop();
    MockArduino_Verify_Complete();
}

void checkSentence(
    const char *sentence,
    const char *code
)
{
    unsigned long startMillis = 0;
    MockArduino_setMillis( startMillis );

    checkTurnOn( sentence );
    checkCodeSequence( startMillis, code );
    checkFinished();
}
};

TEST( MorseCode, ShowSentence_e )
{
    checkSentence( "e", "." );
}

TEST( MorseCode, ShowSentence_t )
{
    checkSentence( "t", "-" );
}

TEST( MorseCode, ShowSentence_a )
{
    checkSentence( "a", ".-" );
}

TEST( MorseCode, ShowSentence_b )
{
    checkSentence( "b", "-..." );
}

TEST( MorseCode, ShowFirstCharacterInCodeTable )
{
    checkSentence( "1", ".----" );
}

TEST( MorseCode, ShowLastCharacterInCodeTable )
{
    checkSentence( ")", "-.--.-" );
}

