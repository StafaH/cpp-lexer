#pragma once

enum TokenType
{
	TokenType_IDENTIFIER,

	//Literals
	TokenType_DIGIT,
	TokenType_FLOAT,
	TokenType_STRING,
	TokenType_BOOL,

	//Operators
	TokenType_PLUS,
	TokenType_MINUS,
	TokenType_TIMES,
	TokenType_SLASH,
	TokenType_PERIOD,
	TokenType_EQL,
	TokenType_NOT,
	TokenType_LSS,
	TokenType_GTR,
	TokenType_LEQ,
	TokenType_GEQ,

	//Seperators
	TokenType_LPAREN,
	TokenType_RPAREN,
	TokenType_LBRACK,
	TokenType_RBRACK,
	TokenType_LBRACE,
	TokenType_RBRACE,
	TokenType_SEMICOLON,
	TokenType_COMMA,
	TokenType_BECOMES,
	

	//Keywords
	TokenType_IF,
	TokenType_WHILE,
	TokenType_FOR,
	TokenType_RETURN,
	TokenType_DO,
	TokenType_NEW,
	TokenType_DELETE,
	TokenType_NULL,


	//Unique
	TokenType_UNKNOWN,
	TokenType_EOF
};

//TODO: Another implementation might be to use pointers into the original string,
// this will eliminate the overhead of copying + storing, as long as we hold the original
// in memory, otherwise the tokens become invalid 
struct Token
{
	TokenType type;
	char contents[256];
	int length;
}; 

struct Tokenizer
{
	char* location;
	int count;
};