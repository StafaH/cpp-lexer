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

int LexInput(char *input, Token *tokens, int max_tokens)
{
	Tokenizer tokenizer = {};
	tokenizer.location = input;
	bool lexing = true;

	while (lexing)
	{
		Token token = GetToken(tokenizer);

		tokens[tokenizer.count] = token;
		tokenizer.count++;

		if (tokenizer.count == max_tokens)
		{
			printf("reached maximum token capacity");
			lexing = false;
		}
		if (token.type == TokenType_EOF)
			lexing = false;
	}
	return tokenizer.count;
}