#include "CppUTest/TestHarness_c.h"
#include <stdio.h>
#include "Arduino.h"

typedef enum {
    FUNCTION_TYPE_DIGITAL_WRIGHT,
    FUNCTION_TYPE_DELAY
} FunctionType;

typedef struct{
    FunctionType type;
    union {
        struct {
            int pin_number;
            int level;
        } digitalWright;
        struct {
            int msec;
        } delay;
    } parameters;
} Expectation;

static unsigned long timeInMillis;

static int maxExpectationCount;
static int expectationCount;
static int nextExpectation;

static Expectation *expectations;

void MockArduino_Create( int maxExpectations )
{
    maxExpectationCount = maxExpectations;
    expectationCount = 0;
    nextExpectation = 0;
    expectations = new Expectation[maxExpectationCount];
}

void MockArduino_Destroy( void )
{
    if( expectations ){
        delete[] expectations;
        expectations = 0;
    }
}

void MockArduino_setMillis( unsigned long millis )
{
    timeInMillis = millis;
}

void MockArduino_Expect_digitalWrite( int pin_number, int level )
{
    Expectation *exp = &expectations[ expectationCount ++ ];
    exp->type = FUNCTION_TYPE_DIGITAL_WRIGHT;
    exp->parameters.digitalWright.pin_number = pin_number;
    exp->parameters.digitalWright.level = level; 
}

void MockArduino_Expect_delay( int msec )
{
    Expectation *exp = &expectations[ expectationCount ++ ];
    exp->type = FUNCTION_TYPE_DELAY;
    exp->parameters.delay.msec = msec;
}

static void expectationStringDigitalWright(
    Expectation *expectation, 
    char *buffer,
    size_t maxLength )
{
    char buff[128];
    sprintf( buff, "digitalWright(%d,%s)",
        expectation->parameters.digitalWright.pin_number,
        expectation->parameters.digitalWright.level == 0 ? "LOW" : "HIGH"
    );
    strncat( buffer, buff, maxLength );
}

static void expectationStringDelay(
    Expectation *expectation, 
    char *buffer,
    size_t maxLength )
{
    char buff[128];
    sprintf( buff, "delay(%d)", expectation->parameters.delay.msec );
    strncat( buffer, buff, maxLength );
}

static void expectationString(
    Expectation *expectation,
    char *buffer,
    size_t maxLength )
{
    switch( expectation->type ){
    case FUNCTION_TYPE_DIGITAL_WRIGHT:
        expectationStringDigitalWright( expectation, buffer, maxLength );
        break;
    case FUNCTION_TYPE_DELAY:
        expectationStringDelay( expectation, buffer, maxLength );
        break;
    default:
        break;
    }
}

void MockArduino_Verify_Complete( void )
{
    if( nextExpectation < expectationCount ){
        char message[4096];
        strcpy( message, "Expectations not happened..." );
        for( int i = nextExpectation; i < expectationCount; i++ ){
            strncat( message, "\n          ", (size_t)(sizeof(message) - strlen(message) - 1) );
            expectationString( &expectations[i], message, (size_t)(sizeof(message) - strlen(message) - 1) );
        } 
        strncat( message, "\n", (size_t)(sizeof(message) - strlen(message) - 1) );
        FAIL_TEXT_C( message );
    }
}

static void unexpectedDigitalWright( int pin_number, int level )
{
    char message[128];
    sprintf( message, 
        "unexpected call 'digitalWright(%d,%s)'",
        pin_number, (level == 0) ? "LOW" : "HIGH" );
    FAIL_TEXT_C( message );
} 

void digitalWrite( int pin_number, int level )
{
    if( nextExpectation >= expectationCount ){
        unexpectedDigitalWright( pin_number, level );
    }
    Expectation *exp = &expectations[ nextExpectation ++ ];
    if( exp->type != FUNCTION_TYPE_DIGITAL_WRIGHT ){
        unexpectedDigitalWright( pin_number, level );
    }
    CHECK_EQUAL_C_INT( exp->parameters.digitalWright.pin_number, pin_number );
    CHECK_EQUAL_C_INT( exp->parameters.digitalWright.level, level );
}

static void unexpectedDelay( int msec )
{
    char message[128];
    sprintf( message, "unexpected call 'delay(%d)'", msec );
    FAIL_TEXT_C( message );
} 

void delay( int msec )
{
    if( nextExpectation >= expectationCount ){
        unexpectedDelay( msec );
    }
    Expectation *exp = &expectations[ nextExpectation ++ ];
    if( exp->type != FUNCTION_TYPE_DELAY ){
        unexpectedDelay( msec );
    }
    CHECK_EQUAL_C_INT( exp->parameters.delay.msec, msec );
}

unsigned long millis()
{
    return timeInMillis;
}

