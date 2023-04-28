#include "sfsh.h"

/**
 * update_pwd     - Update the OLDPWD entry in env with @cwd
 *
 * @cwd:            The current working directory as a string
 * @prefix:         The entry to search for in the environment
 */

void update_pwd(char *cwd, char *prefix)
{
	int i = 0, j = 0, length;
	char *pwd_path;

	length = _strlen(cwd) + _strlen(prefix);
	pwd_path = malloc(sizeof(char) * length);

	for (i = 0; i < length; i++)
	{
		if (i < _strlen(prefix))
		{
			pwd_path[i] = prefix[i];
		}
		else
		{
			pwd_path[i] = cwd[j];
			j++;
		}
	}
	pwd_path[i] = '\0';
	i = 0;
	length = _strlen(prefix);
	while (environ[i])
	{
		if (_strncmp(prefix, environ[i], length) == 0)
		{
			environ[i] = pwd_path;
			break;
		}
		i++;
	}
}

/**
 * path_match     - Helper to return processed string matching @entry
 *
 * @entry:          Entry in the environment to match for
 *
 * Return:          Entry in environment matching @entry
 */
char *path_match(char *entry)
{
	int i = 0, length = 0;
	char *target;

	length = _strlen(entry);

	while (environ[i])
	{
		if (_strncmp(entry, environ[i], length) == 0)
		{
			target = environ[i];
			break;
		}
		i++;
	}
	target += length;

	return (target);
}

/**
 * sfsh_cd       - Changes current directory to first element of @args
 *
 * @args:          @args[1] will be destination directory to change to
 *
 * Return:         (1) Success
 */
int sfsh_cd(char **args)
{
	int i = 0;
	char *target;
	char cwd[1024];

	while (args[++i])
		;
	if (i < 2)
	{
		target = path_match("HOME=");
		getcwd(cwd, sizeof(cwd));
		update_pwd(cwd, "OLDPWD=");
		if (chdir(target) != 0)
			perror("");
		getcwd(cwd, sizeof(cwd));
		update_pwd(cwd, "PWD=");
	}
	else
	{
		if (args[1][0] == '-')
		{

			target = path_match("OLDPWD=");
			getcwd(cwd, sizeof(cwd));
			update_pwd(cwd, "OLDPWD=");
			if (chdir(target) != 0)
				perror("");
			getcwd(cwd, sizeof(cwd));
			update_pwd(cwd, "PWD=");
		}
		else
		{
			getcwd(cwd, sizeof(cwd));
			update_pwd(cwd, "OLDPWD=");
			if (chdir(args[1]) != 0)
				perror("");
			getcwd(cwd, sizeof(cwd));
			update_pwd(cwd, "PWD=");
		}
	}
	return (1);
}
/**
 * sfsh_setenv   - Sets or updates an entry in the environment
 *
 * @args:          @args[1] is the name of the environment variable
 *                 @args[2] is the new value for @args[1]
 *                 @args[3] None-zero overwrite, zero to not overwrite
 *
 * Return:         (1) Success
 */
int sfsh_setenv(char **args)
{
	int i = 0, length = 0, new_entry = 0;
	char *new_value;

	/* check for proper number of arguments, else return */
	while (args[++i])
		;
	if (i != 4 || args[3] == 0)
	{
		write(1, "Wrong arg count\n", 16);
		return (1);
	}
	/* ensure no '=' for proper syntax processing */
	for (length = 0; args[1][length] != '\0'; length++)
	{
		if (args[1][length] == '=')
		{
			write(1, "Incorrect format\n", 17);
			return (1);
		}
	}
	new_value = malloc(_strlen(args[0]) + _strlen(args[1]) + 2);
	_strcpy(new_value, args[1]);
	_strcpy(&new_value[_strlen(new_value)], "=");
	_strcpy(&new_value[_strlen(new_value)], args[2]);
	i = 0;
	while (environ[i])
	{
		if (_strncmp(args[1], environ[i], length) == 0)
		{
			environ[i] = new_value;
			new_entry = 1;
		}
		i++;
	}
	if (new_entry == 0)
	{
		environ[i++] = new_value;
		environ[i] = NULL;
	}
	return (1);
}
/**
 * sfsh_unsetenv - Deletes an entry in the environment
 *
 * @args:          @args[1] The entry to be deleted from the environment
 *
 * Return:         (1) Success
 */
int sfsh_unsetenv(char **args)
{
	int i = 0, length = 0, j = 0;
	char **new_env;

	/* check for proper number of arguments, else return */
	while (args[++i])
		;
	if (i != 2)
	{
		write(1, "Wrong arg count\n", 16);
		return (1);
	}
	/* ensure no '=' for proper syntax processing */
	for (length = 0; args[1][length] != '\0'; length++)
	{
		if (args[1][length] == '=')
		{
			write(1, "Incorrect format\n", 17);
			return (1);
		}
	}
	i = 0;
	while (environ[++i])
		;
	new_env = malloc(sizeof(char *) * i);
	i = 0;
	length = _strlen(args[1]);
	while (environ[i])
	{
		if (_strncmp(args[1], environ[i], length) == 0)
			j--;
		else
		{
			new_env[j] = environ[i];
		}
		i++;
		j++;
	}

	environ = new_env;
	return (1);
}
