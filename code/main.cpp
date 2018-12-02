#include <stdio.h>

#include "util.cpp"
#include "lexer.cpp"
#include "parser.cpp"

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
	char my_str[256] = "Test String";
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

		const int MAX_TOKEN = 50;
		Token tokens[MAX_TOKEN];
		int token_count = LexInput(input, tokens, MAX_TOKEN);

		// Debug code for lexer, lists all tokens
		/*
		for (int i = 0; i < token_count; i++)
		{
			printf("Token %i: %s \n", i, tokens[i].contents);
		}
		*/

		ParseExpression(tokens, token_count, token_dict, ArrayCount(token_dict));
	}
}