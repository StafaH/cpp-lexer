#include "lexer.h"

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
		char* str = new char[10];
		strcopy(str, "EndSymbol");
		token.contents = str;
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
	//NOTE: should everything in the quotes be a single token, 
	// or each quote symbol is a token,
	// and every token inbetween can be interpreted as a quote
	case '"':
	{
		//We need to skip the " and start copying from the next char
		tokenizer.location++;
		
		token.type = TokenType_STRING;
		char* start_loc = tokenizer.location;
		
		while (tokenizer.location[0] != '"')
		{
			tokenizer.location++;
			token.length++;

			//Note: how do we properly handle unclosed quotation?
			// if we reached the end of the input then we know for sure something went wrong. 
			if (tokenizer.location[0] == '\0')
			{
				//For now we can just print an error and skip copying our values
				printf("you forgot to close the quotation mark\n");
				token.type = TokenType_UNKNOWN;
				break;
			}
		}
		
		token.contents = new char[token.length + 1];
		int iterator = 0;
		while (start_loc != tokenizer.location)
		{
			token.contents[iterator] = *start_loc;
			start_loc++;
			iterator++;
		}
		token.contents[token.length] = '\0';
		
		// We also skip the end " 
		tokenizer.location++; 
	}break;

	default:
	{
		if (IsLetter(tokenizer.location[0]))
		{
			// We put a pointer at the beginning, then iterate to the end
			// of the identifier, and then copy everything inbetween into 
			// our token - much more efficient then copying 1 lettr at a time
			char* start_loc = tokenizer.location;
			token.type = TokenType_IDENTIFIER;

			while (IsLetter(tokenizer.location[0]) || IsNumeric(tokenizer.location[0]) || tokenizer.location[0] == '_')
			{
				tokenizer.location++;
				token.length++;
				//TODO: should we limit ourselves?
				if (token.length == 255) break;
			}
			token.contents = new char[token.length + 1];
			int iterator = 0;
			while (start_loc != tokenizer.location)
			{
				token.contents[iterator] = *start_loc;
				start_loc++;
				iterator++;
			}
			token.contents[token.length] = '\0';
			
			// Once we have an identifier, we can check if it matches a keyword
			if (strcompare(token.contents, "for"))
			{
				token.type = TokenType_FOR;
				break;
			}
			if (strcompare(token.contents, "while"))
			{
				token.type = TokenType_WHILE;
				break;
			}
			if (strcompare(token.contents, "return"))
			{
				token.type = TokenType_RETURN;
				break;
			}
			if (strcompare(token.contents, "do"))
			{
				token.type = TokenType_DO;
				break;
			}
			if (strcompare(token.contents, "new"))
			{
				token.type = TokenType_NEW;
				break;
			}
			if (strcompare(token.contents, "delete"))
			{
				token.type = TokenType_DELETE;
				break;
			}
			if (strcompare(token.contents, "null"))
			{
				token.type = TokenType_NULL;
				break;
			}
			//TODO: before we avoided copies, now we copy early
			// We save the copy to the end to avoid copying for keywords
			//strcopy(token.contents, buffer);

			// For boolean values we can change our token type
			if (strcompare(token.contents, "true") || 
				strcompare(token.contents, "false"))
			{
				token.type = TokenType_BOOL;
			}
		}
		else if (IsNumeric(tokenizer.location[0]))
		{
			//TODO: should we eat all leading 0's in a numerical value 
			// other than the first 0 for a float?
			char* start_loc = tokenizer.location;
			token.type = TokenType_DIGIT;

			while (IsNumeric(tokenizer.location[0]))
			{
				tokenizer.location++;
				token.length++;
				//TODO: should we limit ourselves, avoid overflowing integers
				// by accident?

				//Look forward 1 charachter for the . operator for floats
				if (tokenizer.location[0] == '.')
				{
					token.type = TokenType_FLOAT;
					tokenizer.location++;
					token.length++;
				}
				//TODO: we should probably check for multiple '.', because that's not a real float
			}
			token.contents = new char[token.length + 1];
			int iterator = 0;
			while (start_loc != tokenizer.location)
			{
				token.contents[iterator] = *start_loc;
				start_loc++;
				iterator++;
			}
			token.contents[token.length] = '\0';
		}
	} break;
	}

	return token;

}

TokenArray LexInput(char *input)
{
	//TODO: should we join TokenArray and Tokenizer, 
	// or keep them seperate? 
	Tokenizer tokenizer = {};
	tokenizer.location = input;
	bool lexing = true;
	
	TokenArray token_array = {};
	//TODO: default initialize this?
	token_array.capacity = 10;
	token_array.tokens = new Token[token_array.capacity];
	
	while (lexing)
	{
		Token new_token = GetToken(tokenizer);

		token_array.tokens[tokenizer.count] = new_token;
		tokenizer.count++;

		if (tokenizer.count == token_array.count)
		{
			//TODO: Resize the array with a new heap allocation when
			// we reach max capacity. Is there a way we can predict?
			printf("reached maximum token capacity");
			lexing = false;
		}
		if (new_token.type == TokenType_EOF)
			lexing = false;
	}

	token_array.count = tokenizer.count;
	return token_array;
}