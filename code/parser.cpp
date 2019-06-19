#include "parser.h"

#include "lexer.h"
#include "util.h"
#include "stdio.h"

void ParseChangeExpression(Token *tokens, int token_count, TokenDictionaryItem *dict, int dict_count)
{
	for (int i = 0; i < dict_count; i++)
	{
		bool key_found = strcompare(tokens[2].contents, dict[i].key);

		if (key_found)
		{
			if (!strcompare(tokens[1].contents, dict[i].datatype))
			{
				printf("the variable types do not match\n");
				return;
			}
			if (strcompare(tokens[1].contents, "int"))
			{
				*(int *)dict[i].value = strtoint(tokens[3].contents);
				printf("value of %s changed to %d\n", dict[i].key, *(int *)dict[i].value);
				return;
			}
			else if (strcompare(tokens[1].contents, "char"))
			{
				*(char *)dict[i].value = *tokens[3].contents;
				printf("value of %s changed to %c\n", dict[i].key, *(char*)dict[i].value);
				return;
			}
			if (strcompare(tokens[1].contents, "string"))
			{
				strcopy((char *)dict[i].value, tokens[3].contents);
				printf("value of %s changed to %s\n", dict[i].key, (char *)dict[i].value);
				return;
			}
		}
	}
	printf("could not find the variable: %s\n", tokens[2].contents);
}

void ParseListExpression(Token *tokens, int token_count, TokenDictionaryItem *dict, int dict_count)
{
	for (int i = 0; i < dict_count; i++)
	{
		if (strcompare(dict[i].datatype, "int"))
		{
			printf("%s : %d\n", dict[i].key, *(int *)dict[i].value);
		}
		else if (strcompare(dict[i].datatype, "char"))
		{
			printf("%s : %c\n", dict[i].key, *(char *)dict[i].value);
		}
		else if (strcompare(dict[i].datatype, "string"))
		{
			printf("%s : %s\n", dict[i].key, (char *)dict[i].value);
		}
		else
		{
			printf("%s : type not supported\n", dict[i].key);
		}
	}
}

void ParseExpression(Token *tokens, int token_count, TokenDictionaryItem *dict, int dict_count)
{
	switch (tokens[0].type)
	{
	case TokenType_IDENTIFIER:
	{
		if (strcompare(tokens[0].contents, "change"))
		{
			if (token_count < 4)
			{
				printf("The command should take the structure of:\n\n");
				printf("\tchange typename variablename value\n");
				printf("\te.g change int variable1 7\n");
			}

			ParseChangeExpression(tokens, token_count, dict, dict_count);
			return;
		}
		else if (strcompare(tokens[0].contents, "list"))
		{
			if (token_count < 3)
			{
				printf("The command should take the structure of:\n\n");
				printf("\tlist variable OR list all\n");
				return;
			}
			if (strcompare(tokens[1].contents, "all"))
			{
				ParseListExpression(tokens, token_count, dict, dict_count);
				return;
			}
			else
			{
				for (int i = 0; i < dict_count; i++)
				{
					if (strcompare(tokens[1].contents, dict[i].key))
					{
						ParseListExpression(tokens, token_count, &dict[i], 1);
						return;
					}
				}
				printf("could not find the variable : %s\n", tokens[1].contents);
			}
		}
		else
		{
			printf("The command does not exist\n");
		}
	}
	break;

	default:
	{
		printf("Unable to parse the token\n");
	}
	break;
	}
}