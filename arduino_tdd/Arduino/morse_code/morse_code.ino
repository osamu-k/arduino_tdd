/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
  Serial.begin(9600);
}

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

#define MAX_INPUT_LENGTH 50

// the loop routine runs over and over again forever:
void loop() {
    static char inputString[MAX_INPUT_LENGTH];
    static int inputLength = 0;
 
    while( inputLength < (MAX_INPUT_LENGTH - 1) ){
        inputString[ inputLength ] = Serial.read();
        if( inputString[ inputLength ] < 0 ){
            break;
        }
        inputLength++;
    }
    inputString[ inputLength ] = 0;

    if( (inputLength > 0) && (inputString[inputLength-1] == '.') ){
        Serial.print(inputString);
        Serial.print("\n");
        writeMorseCodeString( inputString );
        inputLength = 0;
    }
}

