#include <iostream> // for input/output
#include <fstream>  // for file operations
#include <cctype>    // for character classification (isdigit, isalpha, etc.)
#include <cstring>   // for string manipulation (strcpy, etc.)

using namespace std; // use standard namespace to avoid prefixing std::

/* Global declarations */
int charClass;               // holds the character class (LETTER, DIGIT, etc.)
char lexeme[100];            // array to store the current lexeme (token string)
char nextChar;               // holds the current character being analyzed
int lexLen = 0;              // length of the current lexeme
int token;                   // placeholder for current token (not actively used)
int nextToken;               // holds the next token to be returned
ifstream in_fp;              // input file stream to read source code

/* Function declarations */
void addChar();              // adds nextChar to lexeme
void getChar();              // reads the next character from file and sets charClass
void getNonBlank();          // skips whitespace characters
int lex();                   // the main lexical analyzer function
int lookup(char ch);         // identifies operators and parentheses

/* Character classes */
#define LETTER 0             // letter character class
#define DIGIT 1              // digit character class
#define UNKNOWN 99           // unknown character class (e.g., symbols)

/* Token codes */
#define INT_LIT 10           // integer literal token
#define IDENT 11             // identifier token
#define ASSIGN_OP 20         // assignment operator token (not implemented)
#define ADD_OP 21            // addition operator token
#define SUB_OP 22            // subtraction operator token
#define MULT_OP 23           // multiplication operator token
#define DIV_OP 24            // division operator token
#define LEFT_PAREN 25        // left parenthesis token
#define RIGHT_PAREN 26       // right parenthesis token

/*****************************************************/
/* main driver */
int main() {
    in_fp.open("front.in");                                 // open input file named "front.in"
    if (!in_fp.is_open()) {                                  // check if file was opened successfully
        cout << "ERROR - cannot open front.in" << endl;      // print error if file could not be opened
        return 1;                                            // return with error code
    }
    else {
        getChar();                                           // read the first character from file
        do {
            lex();                                          // analyze and tokenize input
        } while (nextToken != EOF);                          // repeat until end-of-file is reached
    }

    in_fp.close();                                           // close the file after processing
    return 0;                                                // indicate successful execution
}

/*****************************************************/
/* lookup - identifies and returns tokens for symbols */
int lookup(char ch) {
    switch (ch) {
    case '(': addChar(); nextToken = LEFT_PAREN; break;  // left parenthesis
    case ')': addChar(); nextToken = RIGHT_PAREN; break; // right parenthesis
    case '+': addChar(); nextToken = ADD_OP; break;      // addition operator
    case '-': addChar(); nextToken = SUB_OP; break;      // subtraction operator
    case '*': addChar(); nextToken = MULT_OP; break;     // multiplication operator
    case '/': addChar(); nextToken = DIV_OP; break;      // division operator
    default:  addChar(); nextToken = EOF; break;         // unknown symbol (treated as EOF here)
    }
    return nextToken;                                        // return identified token
}

/*****************************************************/
/* addChar - adds nextChar to the lexeme array */
void addChar() {
    if (lexLen <= 98) {                                      // prevent buffer overflow
        lexeme[lexLen++] = nextChar;                         // append nextChar to lexeme
        lexeme[lexLen] = '\0';                              // null-terminate the string
    }
    else {
        cout << "Error - lexeme is too long" << endl;        // print error for overflow
    }
}

/*****************************************************/
/* getChar - reads next character and sets charClass */
void getChar() {
    if (in_fp.get(nextChar)) {                               // read next character from file
        if (isalpha(nextChar))                               // check if it's a letter
            charClass = LETTER;
        else if (isdigit(nextChar))                          // check if it's a digit
            charClass = DIGIT;
        else
            charClass = UNKNOWN;                             // otherwise, mark as unknown
    }
    else {
        charClass = EOF;                                     // if no more characters, set to EOF
    }
}

/*****************************************************/
/* getNonBlank - skips all whitespace characters */
void getNonBlank() {
    while (isspace(nextChar))                                // loop until non-space character found
        getChar();                                           // read next character
}

/*****************************************************/
/* lex - performs lexical analysis and returns tokens */
int lex() {
    lexLen = 0;                                              // reset lexeme length
    getNonBlank();                                           // skip whitespace
    switch (charClass) {                                     // choose action based on char class

        // Parse identifiers (starting with a letter)
    case LETTER:
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT) { // continue if letter or digit
            addChar();
            getChar();
        }
        nextToken = IDENT;                               // set token as identifier
        break;

        // Parse integer literals (numbers)
    case DIGIT:
        addChar();
        getChar();
        while (charClass == DIGIT) {                     // read until non-digit
            addChar();
            getChar();
        }
        nextToken = INT_LIT;                             // set token as integer literal
        break;

        // Parse symbols using lookup table
    case UNKNOWN:
        lookup(nextChar);                                // identify symbol token
        getChar();                                       // read next character
        break;

        // End-of-file condition
    case EOF:
        nextToken = EOF;                                 // set token to EOF
        strcpy(lexeme, "EOF");                           // set lexeme string
        break;
    }

    cout << "Next token is: " << nextToken << ", Next lexeme is " << lexeme << endl; // print result
    return nextToken;                                        // return the token type
}
