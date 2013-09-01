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
    int m_pinNumber;
    unsigned long m_longCodePeriod;
    unsigned long m_shortCodePeriod;
    unsigned long m_codeBoundaryPeriod;
    unsigned long m_characterBoundaryPeriod;
    unsigned long m_wordBoundaryPeriod;

    unsigned long m_lastTimeTurned;
};

#endif

