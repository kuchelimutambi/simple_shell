#include "sfsh.h"
void check_environ(char *path, char temp[], char **dirs);
/**
 * sfsh_execute - search builtins for function to execute & send to sfsh_launch
 * @args: pointer to list of arguments
 * @line: String holding path
 * Return: 1 if successful
 */
int sfsh_execute(char **args, char *line)
{
	int i = 0, size = 0;
	char temp[150];
	char *path = NULL;
	char **dirs;

	builtin_t builtins[] = {
		{"help", sfsh_help}, {"exit", sfsh_exit},
		{"env", sfsh_env}, {"cd", sfsh_cd},
		{"setenv", sfsh_setenv}, {"unsetenv", sfsh_unsetenv},
		{"", sfsh_error}
	};
	dirs = malloc(sizeof(char *) * BUFFER_SIZE);
	if (!dirs)
	{
		write(1, "Malloc error i execute\n", 24);
		exit(101);
	}
	size = (sizeof(builtins) / (sizeof(char *) * 2));
	if (args[0] == NULL)
	{'
		free(dirs);
		return (1);
	}
	for (i = 0; i < size; i++)
	{
		if (_strcmp(args[0], builtins[i].name) == 0)
		{
			/* Running found matching builtin */
			free(path);
			free(dirs);
			return (builtins[i].function(args));
		}
	}
	/* No matching builtins, search for program in path */
	check_environ(path, temp, dirs);
	free(path);
	/* No builtins found, search path and run matching executable */
	sfsh_launch(args, dirs, line);
	free(dirs);
	return (1);
}
/**
 * check_environ - checks environment variables for command
 * @path: our tokenized string of PATH
 * @temp: a temporary array to hold characters
 * @dirs: pointer to our array of directories
 * Return: Nothing
 */
void check_environ(char *path, char temp[], char **dirs)
{
	int i = 0;

	while (environ[i])
	{
		if (_strncmp("PATH=", environ[i], 5) == 0)
		{
			path = _strcpy(temp, environ[i]);
			break;
		}
		i++;
	}
	path += 5;
	i = 0;
	path = strtok(path, ":");
	while (path != NULL)
	{
		dirs[i] = path;
		i++;
		path = strtok(NULL, ":");
	}
	dirs[i] = NULL;
}
