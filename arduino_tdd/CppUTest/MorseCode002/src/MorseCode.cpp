#include "MorseCode.h"
#include "MorseCodeTable.h"
#include "Arduino.h"
#include <string.h>

MorseCode::MorseCode()
    : m_pinNumber(0)
    , m_longCodePeriod(0)
    , m_shortCodePeriod(0)
    , m_codeBoundaryPeriod(0)
    , m_characterBoundaryPeriod(0)
    , m_wordBoundaryPeriod(0)
    , m_status(STATUS_IDLE)
    , m_lastTimeTurned(0)
    , m_period(0)
    , m_sentence(0)
    , m_sentenceIndex(0)
    , m_currentCode(0)
    , m_currentCodeIndex(0)
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
    if( sentence == 0 ) return;
    m_sentence = sentence;
    for( m_sentenceIndex = 0; 
         (m_sentenceIndex < strlen(m_sentence)) && (m_sentence[m_sentenceIndex] == ' ');
         m_sentenceIndex ++ ){
    }
    showNextCharacter();
}

void MorseCode::showNextCharacter()
{
    if( isLastCharacter() ){
        m_status = STATUS_IDLE;
        return;
    }
    char nextChar = m_sentence[m_sentenceIndex++];
    m_currentCode = MorseCodeTable::codeForCharacter( nextChar );
    m_currentCodeIndex = 0;
    showNextCode();
}

void MorseCode::showNextCode()
{
    if( m_currentCodeIndex >= strlen(m_currentCode) ) return;

    unsigned long now = millis();
    switch( m_currentCode[m_currentCodeIndex++] ){
    case '.':
        turnOn( now, m_shortCodePeriod );
        break;
    case '-':
        turnOn( now, m_longCodePeriod );
        break;
    break;
        break;
    }
}

void MorseCode::turnOn(
    unsigned long startMillis,
    unsigned long period
)
{
    m_lastTimeTurned = startMillis;
    m_period = period;
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
    if( now < (m_lastTimeTurned + m_period) ) return;

    digitalWrite( m_pinNumber, LOW );

    if( m_currentCodeIndex < strlen( m_currentCode ) ){
        m_lastTimeTurned = now;
        m_period = m_codeBoundaryPeriod;
        m_status = STATUS_OFF;
    }
    else if( ! isLastCharacter() ){
        if( m_sentence[m_sentenceIndex] == ' ' ){
            m_sentenceIndex ++;
            while( (! isLastCharacter()) && (m_sentence[m_sentenceIndex] == ' ') ){
                m_sentenceIndex ++;
            }
            m_lastTimeTurned = now;
            m_period = m_wordBoundaryPeriod;
            m_status = STATUS_OFF;
        }
        else{
            m_lastTimeTurned = now;
            m_period = m_characterBoundaryPeriod;
            m_status = STATUS_OFF;
        }
    }
    else{
        m_lastTimeTurned = 0;
        m_period = 0;
        m_status = STATUS_IDLE;
    }
}

void MorseCode::checkTurnOn()
{
    unsigned long now = millis();
    if( now < (m_lastTimeTurned + m_period) ) return;

    if( m_currentCodeIndex < strlen( m_currentCode ) ){
        showNextCode();
    }
    else{
        showNextCharacter();
    }
}

bool MorseCode::isLastCharacter()
{
    for( unsigned int i = m_sentenceIndex; i < strlen(m_sentence); i++ ){
        if( m_sentence[i] != ' ' ){
            return false;
        }
    }
    return true;
}

