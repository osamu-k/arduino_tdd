#ifndef _MORSE_CODE_H_
#define _MORSE_CODE_H_

class MorseCode{

public:
    MorseCode();
    ~MorseCode();

    void setLedPinNumber( int number );
    void setLongCodePeriod( unsigned long period );
    void setShortCodePeriod( unsigned long period );
    void setCodeBoundaryPeriod( unsigned long period );
    void setCharacterBoundaryPeriod( unsigned long period );
    void setWordBoundaryPeriod( unsigned long period );

    void showSentence( const char *sentence );

    void loop();

private:
    void showNextCharacter();
    void showNextCode();
    void turnOn( unsigned long startMillis, unsigned long period );
    const char *characterCode( char c );
    void checkTurnOff();
    void checkTurnOn();
    bool isLastCharacter();

    int m_pinNumber;
    unsigned long m_longCodePeriod;
    unsigned long m_shortCodePeriod;
    unsigned long m_codeBoundaryPeriod;
    unsigned long m_characterBoundaryPeriod;
    unsigned long m_wordBoundaryPeriod;

    enum Status{
        STATUS_IDLE,
        STATUS_ON,
        STATUS_OFF
    };
    Status m_status;
    unsigned long m_lastTimeTurned;
    unsigned long m_period;
    const char *m_sentence;
    unsigned int m_sentenceIndex;
    const char *m_currentCode;
    unsigned int m_currentCodeIndex;
};

#endif

