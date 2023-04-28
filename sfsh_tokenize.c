#include "sfsh.h"
#define DELIM " \t\r\n"

/**
 * sfsh_tokenize        - Split @line into tokens and store in an array
 *
 * @line:                 User input stored as a string
 *
 * Return:                pointer to the string
 */
char **sfsh_tokenize(char *line)
{
	char **tokens;
	char *token = NULL;
	int i = 0;

	tokens = malloc(sizeof(char *) * BUFFER_SIZE);
	if (tokens == NULL)
	{
		write(1, "Failure to allocate\n", 20);
		return (NULL);
	}
	/* Case handling if no user input */
	if (line == NULL)
	{
		tokens[0] = "";
		return (tokens);
	}
	/* Actual user input. Process into array of tokens */
	token = strtok(line, DELIM);
	while (token != NULL)
	{
		tokens[i++] = token;
		token = strtok(NULL, DELIM);
	}
	tokens[i] = NULL;
	return (tokens);
}
