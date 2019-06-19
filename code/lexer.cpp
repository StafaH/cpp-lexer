#include "lexer.h"

#include "util.h"
#include "stdio.h"

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

void DeleteTokenContents(TokenArray token_array)
{
	for (int i = 0; i < token_array.count; i++)
	{
		delete[] token_array.tokens[i].contents;
	}
}

void DeleteTokens(TokenArray token_array)
{
	DeleteTokenContents(token_array);
	delete[] token_array.tokens;
	token_array = {};
}

void InitializeTokenArray(TokenArray& token_array, unsigned int size)
{
	token_array.tokens = new Token[size];
	token_array.capacity = size;
}
void ResizeTokenArray(TokenArray& token_array, unsigned int size)
{
	DeleteTokens(token_array);
	token_array.tokens = new Token[size];
	token_array.capacity = size;
}

TokenArray LexInput(char *input)
{
	Tokenizer tokenizer = {};
	tokenizer.location = input;
	bool lexing = true;
	
	TokenArray token_array = {};
	InitializeTokenArray(token_array, 10);
	
	while (lexing)
	{
		token_array.tokens[tokenizer.count] = GetToken(tokenizer);
		tokenizer.count++;

		if (tokenizer.count == token_array.capacity)
		{
			ResizeTokenArray(token_array, token_array.capacity * 2);
		}

		if (token_array.tokens[tokenizer.count - 1].type == TokenType_EOF)
		{
			lexing = false;
		}
	}

	token_array.count = tokenizer.count;
	return token_array;
}

char* TokenTypeToString(TokenType type)
{
	switch (type)
	{
		case TokenType_IDENTIFIER:
			return "Identifier";
		case TokenType_DIGIT:
		case TokenType_FLOAT:
		case TokenType_STRING:
		case TokenType_BOOL:
			return "Literal";
		case TokenType_PLUS:
		case TokenType_MINUS:
		case TokenType_TIMES:
		case TokenType_SLASH:
		case TokenType_PERIOD:
		case TokenType_EQL:
		case TokenType_NOT:
		case TokenType_LSS:
		case TokenType_GTR:
		case TokenType_LEQ:
		case TokenType_GEQ:
			return "Operator";
		case TokenType_LPAREN:
		case TokenType_RPAREN:
		case TokenType_LBRACK:
		case TokenType_RBRACK:
		case TokenType_LBRACE:
		case TokenType_RBRACE:
		case TokenType_SEMICOLON:
		case TokenType_COMMA:
		case TokenType_BECOMES:
			return "Seperator";
		case TokenType_IF:
		case TokenType_WHILE:
		case TokenType_FOR:
		case TokenType_RETURN:
		case TokenType_DO:
		case TokenType_NEW:
		case TokenType_DELETE:
		case TokenType_NULL:
			return "Keyword";
		case TokenType_EOF:
			return "EndOfStream";
		case TokenType_UNKNOWN:
		default: return "unknown";
	}
}

void DebugPrintTokenArray(TokenArray token_array)
{
	//TODO: Print token type enum
	for (int i = 0; i < token_array.count; i++)
	{
		printf("Token %i: %s - %s \n", i,TokenTypeToString(token_array.tokens[i].type),
				token_array.tokens[i].contents);
	}
}