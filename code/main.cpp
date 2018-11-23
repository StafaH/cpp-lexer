#include <stdio.h>

/*
Simple lexer to tokenize an input
string. Useful to solve variety of 
problems.
*/
int strlen(char* str)
{
	int result = 0;
	if (!str) return result;
	while (*str != '\0')
	{ str++; result++; }
	return result;
}

bool strcompare(char *first, char *second)
{
	bool result;

	//NOTE: if they start the same but one is shorter, this still returns true
	// so we should check their size before returning true;
	for (int i = 0; first[i] != '\0' || second[i] != '\0'; i++)
	{
		if (first[i] != second[i])
		{
			return false;
		}
	}
	if (strlen(first) != strlen(second)) return false;

	return true;
}

char* strcopy(char* destination, char* source)
{
	//Note: very basic implementation, does not check for overflow
	// as the rest of the app is limited to char[256]
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

	//TODO: single quotes are skipped for now
	//Note: should quotes be a single token, or each quote symbol is a token,
	// and every token inbetween can be interpreted as a quote
	case '"':
	{
		token.type = TokenType_STRING;
		char buffer[256];
		char *index = buffer;

		//We need to skip the " and start copying from the next char
		tokenizer.location++;

		while (tokenizer.location[0] != '"' && token.length < 255)
		{
			index[0] = tokenizer.location[0];
			tokenizer.location++;
			index++;
			token.length++;

			//Note: how do we properly handle unclosed quotation?
			//if we reached the end of the input then we know for sure something went wrong. 
			if (tokenizer.location[0] == '\0')
			{
				//For now we can just print an error and skip copying our values
				printf("you forgot to close the quotation mark\n");
				token.type = TokenType_UNKNOWN;
				break;
			}
		}

		// We also skip the end " 
		tokenizer.location++; 
		index[0] = '\0';
		strcopy(token.contents, buffer);
	}break;

	default:
	{
		if (IsLetter(tokenizer.location[0]))
		{
			char buffer[256];
			char* index = buffer;
			token.type = TokenType_IDENTIFIER;

			while (IsLetter(tokenizer.location[0]) || IsNumeric(tokenizer.location[0]) || tokenizer.location[0] == '_')
			{
				*index = *tokenizer.location;
				tokenizer.location++;
				index++;
				token.length++;

				if (token.length == 255) break;
			}
			*index = '\0';
			
			// Once we have an identifier, we can check if it matches a keyword
			if (strcompare(buffer, "for"))
			{
				token.type = TokenType_FOR;
				break;
			}
			if (strcompare(buffer, "while"))
			{
				token.type = TokenType_WHILE;
				break;
			}
			if (strcompare(buffer, "return"))
			{
				token.type = TokenType_RETURN;
				break;
			}
			if (strcompare(buffer, "do"))
			{
				token.type = TokenType_DO;
				break;
			}
			if (strcompare(buffer, "new"))
			{
				token.type = TokenType_NEW;
				break;
			}
			if (strcompare(buffer, "delete"))
			{
				token.type = TokenType_DELETE;
				break;
			}
			if (strcompare(buffer, "null"))
			{
				token.type = TokenType_NULL;
				break;
			}
			// We save the copy to the end to avoid copying for keywords
			strcopy(token.contents, buffer);

			// For boolean values we can change our token type
			
			if (strcompare(buffer, "true") || strcompare(buffer, "false"))
			{
				token.type = TokenType_BOOL;
			}
		}
		else if (IsNumeric(tokenizer.location[0]))
		{
			//TODO: should we eat all leading 0's in a numerical value 
			//		other than the first 0 for a float?
			char buffer[256];
			char* index = buffer;
			token.type = TokenType_DIGIT;

			while (IsNumeric(tokenizer.location[0]) && token.length < 255)
			{
				index[0] = tokenizer.location[0];
				tokenizer.location++;
				index++;
				token.length++;

				//Look forward 1 charachter for the . operator for floats
				if (tokenizer.location[0] == '.')
				{
					token.type = TokenType_FLOAT;
					index[0] = tokenizer.location[0];
					tokenizer.location++;
					index++;
					token.length++;
				}
				//Note: we should probably check for multiple '.', because that's not a real float
			}
			index[0] = '\0';
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