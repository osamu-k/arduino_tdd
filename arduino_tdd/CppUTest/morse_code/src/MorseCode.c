#include "Arduino.h"
#include "MorseCode.h"

#include "string.h"

#define LED_PIN_NUMBER 13

#define LONG_CODE_DURATION 600
#define SHORT_CODE_DURATION 200
#define CODE_INTERVAL_DURATION 200
#define CHARACTER_INTERVAL_DURATION 400
#define WORD_INTERVAL_DURATION 1200

typedef struct {
    char c;
    const char *code;
} CodeDefinition;

static CodeDefinition codeDefinitions[] = 
{
    { '1', ".----" },
    { '2', "..---" },
    { '3', "...--" },
    { '4', "....-" },
    { '5', "....." },
    { '6', "-...." },
    { '7', "--..." },
    { '8', "---.." },
    { '9', "----." },
    { '0', "-----" },

    { 'A', ".-" },
    { 'B', "-..." },
    { 'C', "-.-." },
    { 'D', "-.." },
    { 'E', "." },
    { 'F', "..-." },
    { 'G', "--." },
    { 'H', "...." },
    { 'I', ".." },
    { 'J', ".---" },
    { 'K', "-.-" },
    { 'L', ".-.." },
    { 'M', "--" },
    { 'N', "-." },
    { 'O', "---" },
    { 'P', ".--." },
    { 'Q', "--.-" },
    { 'R', ".-." },
    { 'S', "..." },
    { 'T', "-" },
    { 'U', "..-" },
    { 'V', "...-" },
    { 'W', ".--" },
    { 'X', "-..-" },
    { 'Y', "-.--" },
    { 'Z', "--.." },

    { 'a', ".-" },
    { 'b', "-..." },
    { 'c', "-.-." },
    { 'd', "-.." },
    { 'e', "." },
    { 'f', "..-." },
    { 'g', "--." },
    { 'h', "...." },
    { 'i', ".." },
    { 'j', ".---" },
    { 'k', "-.-" },
    { 'l', ".-.." },
    { 'm', "--" },
    { 'n', "-." },
    { 'o', "---" },
    { 'p', ".--." },
    { 'q', "--.-" },
    { 'r', ".-." },
    { 's', "..." },
    { 't', "-" },
    { 'u', "..-" },
    { 'v', "...-" },
    { 'w', ".--" },
    { 'x', "-..-" },
    { 'y', "-.--" },
    { 'z', "--.." },

    { '.', ".-.-.-" },
    { ',', "--..--" },
    { '?', "..--.." },
    { '!', "-.-.--" },
    { '-', "-....-" },
    { '/', "-..-." },
    { '@', ".--.-." },
    { '(', "-.--." },
    { ')', "-.--.-" }
};

static const char *findCode( char c )
{
    const char *code = "";
    for( int i = 0; i < (int)(sizeof(codeDefinitions) / sizeof(codeDefinitions[0])); i++ ){
        if( codeDefinitions[i].c == c ){
            code = codeDefinitions[i].code;
            break;
        }
    }
    return code;
}

static void writeLongCode()
{
    digitalWrite( LED_PIN_NUMBER, HIGH );
    delay( LONG_CODE_DURATION );
    digitalWrite( LED_PIN_NUMBER, LOW );
    delay( CODE_INTERVAL_DURATION );
}

static void writeShortCode()
{
    digitalWrite( LED_PIN_NUMBER, HIGH );
    delay( SHORT_CODE_DURATION );
    digitalWrite( LED_PIN_NUMBER, LOW );
    delay( CODE_INTERVAL_DURATION );
}

static void writeCode( const char *code ){
    for( int i = 0; i < (int)strlen(code); i++ ){
        switch( code[i] ){
        case '.':
            writeShortCode();
            break;
        case '-':
            writeLongCode();
            break;
        default:
            break;
        }
    }
}

void writeMorseCodeCharacter( char c )
{
    const char *code = findCode( c );
    writeCode( code );
}

void writeMorseCodeString( const char *string )
{
    for( int i = 0; i < (int)strlen(string); i++ ){
        if( string[i] == ' ' ){
            delay( WORD_INTERVAL_DURATION );
        }
        else{
            writeMorseCodeCharacter( string[i] );
            delay( CHARACTER_INTERVAL_DURATION );
        }
    }
}

