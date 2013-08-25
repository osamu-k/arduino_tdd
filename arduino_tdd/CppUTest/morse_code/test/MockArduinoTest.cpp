#include "CppUTest/TestHarness.h"
extern "C" {
#include "Arduino.h"
#include "MockArduino.h"
}

TEST_GROUP(MockArduino)
{

void setup()
{
    MockArduino_Create( 100 );
}

void teardown()
{
    MockArduino_Verify_Complete();
    MockArduino_Destroy();
}

};

TEST(MockArduino, TurnOnLed)
{
    MockArduino_Expect_digitalWrite( 13, HIGH );
    digitalWrite( 13, HIGH );
}

TEST(MockArduino, TurnOffLed)
{
    MockArduino_Expect_digitalWrite( 13, LOW );
    digitalWrite( 13, LOW );
}

TEST(MockArduino, Delay1000ms)
{
    MockArduino_Expect_delay( 1000 );
    delay( 1000 );
}

TEST(MockArduino, HighDelayLowDelay)
{
    MockArduino_Expect_digitalWrite( 13, HIGH );
    MockArduino_Expect_delay( 1000 );
    MockArduino_Expect_digitalWrite( 13, LOW );
    MockArduino_Expect_delay( 100 );

    digitalWrite( 13, HIGH );
    delay( 1000 );
    digitalWrite( 13, LOW );
    delay( 100 );
}

