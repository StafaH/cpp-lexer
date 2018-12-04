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

TODO: too verbose?
New ideas:
output x 
x = 5
ls listname - just ls for default?

MAIN TODO LIST: 
-	Implement type storage, check if types match behind the scenes
to avoid user typing out types
-	Undo History/Redo History
*/

//NOTE: Another implementation might be to use pointers into the original string,
// this will eliminate the overhead of copying + storing, as long as we hold the original
// in memory, otherwise the tokens become invalid 


int main()
{
	bool running = true;

	// Modifiable Values
	int third_int = 9;
	char my_char = 'c';
	char my_str[] = "Test String";
	int another_int = 7;
	int my_int = 5;
	
	//TODO: abstract this, how do we add new variables, and do cool stuff with
	// the variables already in there other than change/list
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