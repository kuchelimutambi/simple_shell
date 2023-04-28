#include "sfsh.h"
/**
 * sfsh_launch - find commannd in args and executes
 * @args: pointer to our array of arguments
 * @dirs: pointer to our array of directories
 * @line: string holding path
 * Return: status as int from fork
 */
int sfsh_launch(char **args, char **dirs, char *line)
{
	int status = 1;
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		/* Given full path to run program */
		if (args[0][0] == '/')
		{
			if (execve(args[0], args, NULL) == -1)
				perror("Error launching dir/prog\n");
		}
		/* Given only program name, search path */
		sfsh_search_path(args, dirs);
		free(line);
		free(args);
		free(dirs);
		exit(0);
	}
	else if (pid < 0)
	{
		perror("Fork error in launcher.\n");
		exit(103);
	}
	else
	{
		wait(&status);
	}
	return (status);
}

/**
 * sfsh_search_path - search PATH variables for command
 * @args: pointer to an array of arguments
 * @dirs: pointer to an array of PATH directories
 * Return: nothing
 */
void sfsh_search_path(char **args, char **dirs)
{
	char *command = NULL;
	DIR *p_dir;
	struct dirent *p_file;
	int i;

	for (i = 0; dirs[i] != '\0'; i++)
	{
		p_dir = opendir(dirs[i]);
		while ((p_file = readdir(p_dir)) != NULL)
		{
			if (_strcmp(p_file->d_name, args[0]) == 0)
			{
				command = cmdcat(dirs[i], args[0]);
				if (execve(command, args, NULL) == -1)
					perror("No program in path\n");
			}
		}
			closedir(p_dir);
		}
}
