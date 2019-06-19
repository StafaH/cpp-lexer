#pragma once

enum DataType
{
	TYPENAME_BOOL,
	TYPENAME_UNSIGNED_CHAR,
	TYPENAME_CHAR,
	TYPENAME_SIGNED_CHAR,
	TYPENAME_SHORT_INT,
	TYPENAME_UNSIGNED_CHORT_INT,
	TYPENAME_INT,
	/* ... */
	TYPENAME_POINTER_TO_INT,
	TYPENAME_OTHER
};

//Note: 2 approaches, crate a seperate dictionary for each type, or use a void*
//pointer and cast based on the input. First approach seems safer, second is more flexible,
// but if a user inputs char for a value that is supposed to be int, then problems will occur.

struct TokenDictionaryItem
{
	char *key;
	void *value;
	char *datatype;
};

void ParseChangeExpression(Token *tokens, int token_count, TokenDictionaryItem *dict, int dict_count);

void ParseListExpression(Token *tokens, int token_count, TokenDictionaryItem *dict, int dict_count);

void ParseExpression(Token *tokens, int token_count, TokenDictionaryItem *dict, int dict_count)