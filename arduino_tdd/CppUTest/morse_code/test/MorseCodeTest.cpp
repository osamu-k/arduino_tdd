#include "CppUTest/TestHarness.h"
extern "C" {
#include "Arduino.h"
#include "MockArduino.h"
#include "MorseCode.h"
}

#define LED_PIN_NUMBER 13

#define LONG_CODE_DURATION 600
#define SHORT_CODE_DURATION 200
#define CODE_INTERVAL_DURATION 200
#define CHARACTER_INTERVAL_DURATION 400
#define WORD_INTERVAL_DURATION 1200

TEST_GROUP(MorseCode)
{

void setup()
{
    MockArduino_Create( 1000 );
}

void teardown()
{
    MockArduino_Verify_Complete();
    MockArduino_Destroy();
}

void expectLongCode()
{
    MockArduino_Expect_digitalWrite( LED_PIN_NUMBER, HIGH );
    MockArduino_Expect_delay( LONG_CODE_DURATION );
    MockArduino_Expect_digitalWrite( LED_PIN_NUMBER, LOW );
    MockArduino_Expect_delay( CODE_INTERVAL_DURATION );
}

void expectShortCode()
{
    MockArduino_Expect_digitalWrite( LED_PIN_NUMBER, HIGH );
    MockArduino_Expect_delay( SHORT_CODE_DURATION );
    MockArduino_Expect_digitalWrite( LED_PIN_NUMBER, LOW );
    MockArduino_Expect_delay( CODE_INTERVAL_DURATION );
}

};

TEST(MorseCode, Write1 )
{
    expectShortCode();
    expectLongCode();
    expectLongCode();
    expectLongCode();
    expectLongCode();
    
    writeMorseCodeCharacter( '1' );
}

TEST(MorseCode, WriteA )
{
    expectShortCode();
    expectLongCode();
    
    writeMorseCodeCharacter( 'a' );
}

TEST(MorseCode, WriteZ )
{
    expectLongCode();
    expectLongCode();
    expectShortCode();
    expectShortCode();

    writeMorseCodeCharacter( 'z' );
}

TEST(MorseCode, WriteRightParenthesis )
{
    expectLongCode();
    expectShortCode();
    expectLongCode();
    expectLongCode();
    expectShortCode();
    expectLongCode();

    writeMorseCodeCharacter( ')' );
}

TEST( MorseCode, WriteWordAZ )
{
    expectShortCode();
    expectLongCode();

    MockArduino_Expect_delay( CHARACTER_INTERVAL_DURATION );

    expectLongCode();
    expectLongCode();
    expectShortCode();
    expectShortCode();

    MockArduino_Expect_delay( CHARACTER_INTERVAL_DURATION );

    writeMorseCodeString( "az" );
}

TEST( MorseCode, writeABC_XYZ )
{
    // 'A'
    expectShortCode();
    expectLongCode();
    MockArduino_Expect_delay( CHARACTER_INTERVAL_DURATION );
    // 'B'
    expectLongCode();
    expectShortCode();
    expectShortCode();
    expectShortCode();
    MockArduino_Expect_delay( CHARACTER_INTERVAL_DURATION );
    // 'C'
    expectLongCode();
    expectShortCode();
    expectLongCode();
    expectShortCode();
    MockArduino_Expect_delay( CHARACTER_INTERVAL_DURATION );
    // Word boundary
    MockArduino_Expect_delay( WORD_INTERVAL_DURATION );
    // 'X'
    expectLongCode();
    expectShortCode();
    expectShortCode();
    expectLongCode();
    MockArduino_Expect_delay( CHARACTER_INTERVAL_DURATION );
    // 'Y'
    expectLongCode();
    expectShortCode();
    expectLongCode();
    expectLongCode();
    MockArduino_Expect_delay( CHARACTER_INTERVAL_DURATION );
    // 'Z'
    expectLongCode();
    expectLongCode();
    expectShortCode();
    expectShortCode();
    MockArduino_Expect_delay( CHARACTER_INTERVAL_DURATION );

    writeMorseCodeString( "ABC XYZ" );
}

