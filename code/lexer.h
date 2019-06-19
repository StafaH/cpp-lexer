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

struct Token
{
	TokenType type;
	char* contents;
	int length;
}; 

struct TokenArray
{
	Token* tokens;
	int count;
	int capacity;
};

struct Tokenizer
{
	char* location;
	int count;
};

bool IsWhiteSpace(char c);

bool IsEndOfLine(char c);

bool IsLetter(char c);

void IgnoreCommentsAndWhiteSpace(Tokenizer& tokenizer);

Token GetToken(Tokenizer& Tokenizer);

void DeleteTokenContents(TokenArray token_array);

void DeleteTokens(TokenArray token_array);

void InitializeTokenArray(TokenArray& token_array, unsigned int size);

void ResizeTokenArray(TokenArray& token_array, unsigned int size);

TokenArray LexInput(char *input);

void DebugPrintTokenArray(TokenArray token_array)



