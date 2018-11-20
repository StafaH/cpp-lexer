#include <stdio.h>

/*
Simple lexer to tokenize an input
string. Useful to solve variety of 
problems.
*/

char* strcopy(char* destination, char* source)
{
	char* temp = destination;

	while (*source != '\0')
	{
		*destination = *source;
		destination++;
		source++;
	}

	*destination = '\0';

	return temp;
}

void GetInput(char* dest) 
{
	char buffer[256];
	gets_s(buffer, 256); //Note: gets_s returns a null terminated string
	strcopy(dest, buffer);
}

enum TokenType
{
	//Literals
	TokenType_LETTER,
	TokenType_DIGIT,

	//Operators
	TokenType_PLUS,
	TokenType_MINUS,
	TokenType_TIMES,
	TokenType_SLASH,
	TokenType_LPAREN,
	TokenType_RPAREN,
	TokenType_LBRACK,
	TokenType_RBRACK,
	TokenType_LBRACE,
	TokenType_RBRACE,
	TokenType_SEMICOLON,
	TokenType_COMMA,
	TokenType_PERIOD,
	TokenType_BECOMES,
	TokenType_EQL,
	TokenType_NOT,
	TokenType_LSS,
	TokenType_GTR,
	TokenType_LEQ,
	TokenType_GEQ,

	//Conditionals

	//Unique
	TokenType_EOF
};

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

bool IsWhiteSpace(char c)
{
	bool result = (c == ' ') || (c == '\t') || (c == '\f') || (c == '\v');
	return result;
}

bool IsEndOfLine(char c)
{
	bool result = (c == '\n') || (c == '\r');
	return result;
}

bool IsLetter(char c)
{
	bool result = false;

	if ((c >= 'A') && (c <= 'Z')) result = true;
	if ((c >= 'a') && (c <= 'z')) result = true;

	return result;
}

bool IsNumeric(char c)
{
	bool result = false;

	if ((c >= '0') && (c <= '9')) result = true;

	return result;
}

void IgnoreCommentsAndWhiteSpace(Tokenizer& tokenizer)
{
	if (IsWhiteSpace(tokenizer.location[0])) tokenizer.location++;
	
	// Look for // and /* symbols for comments
	if ((tokenizer.location[0] == '/') && (tokenizer.location[1] == '/'))
	{
		while (!IsEndOfLine(tokenizer.location[0]))
		{
			tokenizer.location++;
		}
		tokenizer.location++;
	}
	else if ((tokenizer.location[0] == '/') && (tokenizer.location[1] == '*'))
	{
		while (((tokenizer.location[0] == '*') && (tokenizer.location[1] == '/')))
		{
			tokenizer.location++;
		}
		tokenizer.location += 2;
	}
}

Token GetToken(Tokenizer& tokenizer)
{
	Token token = {};
	token.length = 0;

	IgnoreCommentsAndWhiteSpace(tokenizer);

	switch (tokenizer.location[0])
	{
	case '\0': 
	{
		token.type = TokenType_EOF;
		char value[] = "EndSymbol";
		strcopy(token.contents, value);
	} break;
	case '(': token.type = TokenType_LPAREN; break;
	case ')': token.type = TokenType_RPAREN; break;
	case '{': token.type = TokenType_LBRACE; break;
	case '}': token.type = TokenType_RBRACE; break;
	case '[': token.type = TokenType_LBRACK; break;
	case ']': token.type = TokenType_RBRACK; break;

	case '+': token.type = TokenType_PLUS; break;
	case '-': token.type = TokenType_MINUS; break;
	case '*': token.type = TokenType_TIMES; break;
	case '/': token.type = TokenType_SLASH; break;

	case ';': token.type = TokenType_SEMICOLON; break;
	case ',': token.type = TokenType_COMMA; break;
	case '.': token.type = TokenType_PERIOD; break;

	case '=': token.type = TokenType_EQL; break;
	case '>': token.type = TokenType_GTR; break;
	case '<': token.type = TokenType_LSS; break;
	case '!': token.type = TokenType_NOT; break;

	case '"' : 
	{
		// Handle Quoatations
	}
	// Hande single quotes

	default:
	{
		if (IsLetter(tokenizer.location[0]))
		{
			char buffer[256];
			char* index = buffer;
			token.type = TokenType_LETTER;

			while (IsLetter(tokenizer.location[0]))
			{
				index[0] = tokenizer.location[0];
				tokenizer.location++;
				index++;
				token.length++;
			}
			index[0] = '\0';
			//token.contents = buffer;
			strcopy(token.contents, buffer);
		}
		else if (IsNumeric(tokenizer.location[0]))
		{
			char buffer[256];
			char* index = buffer;
			token.type = TokenType_DIGIT;

			while (IsNumeric(tokenizer.location[0]))
			{
				index[0] = tokenizer.location[0];
				tokenizer.location++;
				index++;
				token.length++;
			}
			index[0] = '\0';
			//token.contents = buffer;
			strcopy(token.contents, buffer);
		}
	} break;
	}

	return token;

}

int main()
{
	bool running = true;

	while (running)
	{
		char input[256];
		GetInput(input);
		//char input[256] = "Hello World";
		
		if (input)
		{
			const int MAX_TOKEN = 50;
			Token tokens[MAX_TOKEN];

			Tokenizer tokenizer = {};
			tokenizer.location = input;
			bool lexing = true;

			while (lexing)
			{
				Token token = GetToken(tokenizer);

				tokens[tokenizer.count] = token;
				tokenizer.count++;
				
				if (tokenizer.count == MAX_TOKEN)
				{
					printf("reached maximum token capacity");
					lexing = false;
				}
				if (token.type == TokenType_EOF) lexing = false;
			}

			for (int i = 0; i < tokenizer.count; i++)
			{
				printf("Token %i: %s \n", i, tokens[i].contents);
			}

		}
		else
		{
			printf("no input string recieved");
		}
	}
}