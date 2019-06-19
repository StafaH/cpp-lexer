#include <stdio.h>

#include "util.h"
#include "lexer.h"
#include "parser.h"

/*
Simple parser that lexes input from the
console, parses it, and performs action to modify or retrieve
data at runtime.

e.g. 
change x = 5	-> variable x assigned value of 5
list variables	-> list all variables that can be changed
*/


int main()
{
	bool running = true;

	// Modifiable Values
	int third_int = 9;
	char my_char = 'c';
	char my_str[] = "Test String";
	int another_int = 7;
	int my_int = 5;
	
	TokenDictionaryItem token_dict[] =
		{
			{VarToString(my_int), &my_int, "int"},
			{VarToString(another_int), &another_int, "int"},
			{VarToString(third_int), &third_int, "int"},
			{VarToString(my_char), &my_char, "char"},
			{VarToString(my_str), &my_str, "string"}};

	while (running)
	{
		char input[256];
		GetInput(input);

		if (!input)
			printf("no input string recieved\n");

		TokenArray token_array = LexInput(input);		
		ParseExpression(token_array.tokens, token_array.count, token_dict, ArrayCount(token_dict));
		DebugPrintTokenArray(token_array);
		DeleteTokens(token_array);
	}
}