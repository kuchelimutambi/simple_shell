#include "sfsh.h"
/**
 * sfsh_getline - get the command as one large string
 * @void: no args
 * Return: pointer to the string
 */
char *sfsh_getline(void)
{
	char *line = NULL;
	size_t b_size = 0;
	int i = 0, length = 0;
	char cwd[128];

	getcwd(cwd, sizeof(cwd));
	length = _strlen(cwd);

	write(1, "(", 1);
	write(1, cwd, length);
	write(1, ")", 1);
	getline(&line, &b_size, stdin);
	if (_strcmp(line, "\n") == 0)
	{
		/* checks condition when enter is pressed */
		free(line);
		return (NULL);
	}
	while (line[i])
	{
		i++;
	}
	if (i <= 1)
	{
		fflush(stdin);
		line = "";
	}
	return (line);
}
