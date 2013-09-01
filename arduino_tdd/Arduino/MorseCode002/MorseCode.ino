#include "MorseCode.h"
#include "Arduino.h"

MorseCode::MorseCode()
    : m_pinNumber(0)
    , m_longCodePeriod(0)
    , m_shortCodePeriod(0)
    , m_codeBoundaryPeriod(0)
    , m_characterBoundaryPeriod(0)
    , m_wordBoundaryPeriod(0)
    , m_status(STATUS_IDLE)
    , m_lastTimeTurned(0)
{
}

MorseCode::~MorseCode()
{
}

void MorseCode::setLedPinNumber( int number )
{
    m_pinNumber = number;
}

void MorseCode::setLongCodePeriod( unsigned long period )
{
    m_longCodePeriod = period;
}

void MorseCode::setShortCodePeriod( unsigned long period )
{
    m_shortCodePeriod = period;
}

void MorseCode::setCodeBoundaryPeriod( unsigned long period )
{
    m_codeBoundaryPeriod = period;
}

void MorseCode::setCharacterBoundaryPeriod( unsigned long period )
{
    m_characterBoundaryPeriod = period;
}

void MorseCode::setWordBoundaryPeriod( unsigned long period )
{
    m_wordBoundaryPeriod = period;
}

void MorseCode::showSentence( const char *sentence )
{
    (void)sentence;
    m_lastTimeTurned = millis();
    m_status = STATUS_ON;
    digitalWrite( m_pinNumber, HIGH );
}

void MorseCode::loop()
{
    switch( m_status ){
    case STATUS_IDLE:
        break;
    case STATUS_ON:
        checkTurnOff();
        break;
    case STATUS_OFF:
        checkTurnOn();
        break;
    default:
        break;
    }
}

void MorseCode::checkTurnOff()
{
    unsigned long now = millis();
    if( now >= (m_lastTimeTurned + m_shortCodePeriod) ){
        digitalWrite( m_pinNumber, LOW );
        m_lastTimeTurned = 0;
        m_status = STATUS_IDLE;
    }
}

void MorseCode::checkTurnOn()
{
}

