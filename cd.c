#include "shell.h"
​
/**
 * c_strcat - concatenates the destination string and the source string,
 * and the result is stored in the destination string("~" in cd)
 * @dest: pointer to the destination array where the content is to be copied
 * @src: string to be copied
 * Return: concatenated string
 */
char *c_strcat(char *dest, char *src)
{
	int len = 0;
	int len2 = 0;
	int total_len = 0;
	int j = 0;
​
	while (dest[len] != '\0')
	{
		len++;
		total_len++;
	}
	while (src[len2] != '\0')
	{
		len2++;
		total_len++;
	}
​
	dest = _realloc(dest, len, sizeof(char) * total_len + 1);
​
	j = 1;
	while (src[j] != '\0')
	{
		dest[len] = src[j];
		len++;
		j++;
	}
	dest[len] = '\0';
​
	return (dest);
}
​
/**
 * c_setenv -  creates or modifies
 * linked list node with given env variable name and value
 * @env: pointer to the head of the linked list
 * @name: pointer to a string representing the name of the
 * environment variable to be set or updated (e.g. "OLDPWD")
 * @dir:  string representing the value to be set for the
 * environment variable (e.g. "/home/vagrant/directory1")
 * Return: 0 on success (e.g. "OLDPWD=/home/vagrant/directory1")
 */
int c_setenv(list_t **env, char *name, char *dir)
{
	int index = 0, j = 0;
	char *cat;
	list_t *holder;
​
	cat = _strdup(name);
	cat = _strcat(cat, "=");
	cat = _strcat(cat, dir);
	index = find_env(*env, name);
​
	holder = *env;
	while (j < index)
	{
		holder = holder->next;
		j++;
	}
	free(holder->var);
	holder->var = _strdup(cat);
	free(cat);
	return (0);
}
​
/**
 * cd_only - changes the current working directory to a specified directory
 * @env: nvironmental variables that need to be updated PATH and OLDPWD
 * @current: updates the shell's environmental variable to reflect the change
 */
void cd_only(list_t *env, char *current)
{
	char *home = NULL;
​
	home = get_env("HOME", env);
	c_setenv(&env, "OLDPWD", current);
	free(current);
	if (access(home, F_OK) == 0)
		chdir(home);
	current = NULL;
	current = getcwd(current, 0);
	c_setenv(&env, "PWD", current);
	free(current);
	free(home);
}
/**
 * cd_execute - implements the behavior of the cd command
 * @env: bring in environmental linked list to update PATH and OLDPWD
 * @current: bring in current working directotry
 * @dir: bring in directory path to change to
 * @str: bring in the 1st argument to write out error
 * @num: bring in the line number to write out error
 * Return: 0 if success 2 if fail
 */
int cd_execute(list_t *env, char *current, char *dir, char *str, int num)
{
	int i = 0;
​
	if (access(dir, F_OK) == 0)
	{
		c_setenv(&env, "OLDPWD", current);
		free(current);
		chdir(dir);
		current = NULL;
		current = getcwd(current, 0);
		c_setenv(&env, "PWD", current);
		free(current);
	}
	else
	{
		cant_cd_to(str, num, env);
		free(current);
		i = 2;
	}
	return (i);
}
​
/**
 * _cd - change directory
 * @str: user's typed in command
 * @env: enviromental linked list to retrieve HOME and OLDPWD paths
 * @num: nth user command, to be used at error message
 * Return: 0 if success 2 if failed
 */
int _cd(char **str, list_t *env, int num)
{
	char *current = NULL, *dir = NULL;
	int exit_stat = 0;
​
	current = getcwd(current, 0);
	if (str[1] != NULL)
	{
		if (str[1][0] == '~')
		{
			dir = get_env("HOME", env);
			dir = c_strcat(dir, str[1]);
		}
		else if (str[1][0] == '-')
		{
			if (str[1][1] == '\0')
				dir = get_env("OLDPWD", env);
		}
		else
		{
			if (str[1][0] != '/')
			{
				dir = getcwd(dir, 0);
				dir = _strcat(dir, "/");
				dir = _strcat(dir, str[1]);
			}
			else
				dir = _strdup(str[1]);
		}
		exit_stat = cd_execute(env, current, dir, str[1], num);
		free(dir);
	}
	else
		cd_only(env, current);
	free_double_ptr(str);
	return (exit_stat);
}
​
​
== == == == == == == == executive.c == == == == == == == =
​
#include "shell.h"
​
/**
 * c_exit - Frees memory allocated for user's
 * command and linked list before exiting
 * @str: User's typed command as a double pointer
 * @env: Linked list of environment variables
 */
void c_exit(char **str, list_t *env)
{
	free_double_ptr(str);
	free_linked_list(env);
	_exit(0);
}
​
/**
 * _execve - Executes a command with arguments using execve
 * @s: The command with arguments as an array of strings
 * @env: The linked list of environment variables
 * @num: The command number in case of error messages
 * Return: 0 on success
 */
int _execve(char **s, list_t *env, int num)
{
	char *holder;
	int status = 0, t = 0;
	pid_t pid;
​
	if (access(s[0], F_OK) == 0)
	{
		holder = s[0];
		t = 1;
	}
	else
		holder = _which(s[0], env);
	if (access(holder, X_OK) != 0)
	{
		not_found(s[0], num, env);
		free_double_ptr(s);
		return (127);
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			if (execve(holder, s, NULL) == -1)
			{
				not_found(s[0], num, env);
				c_exit(s, env);
			}
		}
		else
		{
			wait(&status);
			free_double_ptr(s);
			if (t == 0)
				free(holder);
		}
	}
	return (0);
}
​
​
​
== == == == == == == = whic.c == == == == == ==
#include "shell.h"
​
/**
 * _which - returns full path of command if found in PATH directories
 * @str: command to be searched (e.g. "ls" if user typed "ls -l")
 * @env: pointer to linked list containing environment variables
 * Return: pointer to full path of command if found,
 * otherwise pointer to str (e.g. "/bin/ls" if originally "ls")
 */
char *_which(char *str, list_t *env)
{
	char *pth, *cat = NULL, **toks;
	int i = 0;
​
	pth = get_env("PATH", env);
	toks = c_str_tok(pth, ":");
	free(pth);
​
	i = 0;
	while (toks[i] != NULL)
	{
		if (toks[i][0] == '\0')
			cat = getcwd(cat, 0);
		else
			cat = _strdup(toks[i]);
		cat = _strcat(cat, "/");
		cat = _strcat(cat, str);
		if (access(cat, F_OK) == 0)
		{
			free_double_ptr(toks);
			return (cat);
		}
		free(cat);
		i++;
	}
	free_double_ptr(toks);
	return (str);
}
​
​
== == == == == == == strtok.c == == == == == == == ==
​
#include "shell.h"
​
/**
 * c_t_size - returns number of delimiters in a string
 * @str: user's command typed into shell
 * @delm: delimiter (e.g. " ")
 * Return: number of tokens
 */
int c_t_size(char *str, char delm)
{
	int i = 0, num_delm = 0;
​
	while (str[i] != '\0')
	{
		if (str[i] == delm)
		{
			num_delm++;
		}
		i++;
	}
	return (num_delm);
}
​
​
/**
 * c_str_tok - tokenizes a string even with continuous
 * delimiters and empty strings
 * @str: user's command typed into shell
 * @delm: delimeter
 * Return: an array of tokens
 */
char **c_str_tok(char *str, char *delm)
{
	int buffsize = 0, p = 0, si = 0, i = 0, len = 0, se = 0;
	char **toks = NULL, d_ch;
​
	d_ch = delm[0];

	buffsize = c_t_size(str, d_ch);
	toks = malloc(sizeof(char *) * (buffsize + 2));
	if (toks == NULL)
		return (NULL);
​
	while (str[se] != '\0')
		se++;
	while (si < se)
	{
		len = t_strlen(str, si, d_ch);
		toks[p] = malloc(sizeof(char) * (len + 1));
		if (toks[p] == NULL)
			return (NULL);
		i = 0;
		while ((str[si] != d_ch) &&
		       (str[si] != '\0'))
		{
			toks[p][i] = str[si];
			i++;
			si++;
		}
		toks[p][i] = '\0';
		p++;
		si++;
	}
	toks[p] = NULL;
	return (toks);
}
​
== == == == == == == == = environment_linked_list.c == == == == == == =
​
#include "shell.h"
​
/**
 * env_linked_list - creates a linked list of environment variables
 * @env: array of environment variables
 * Return: pointer to head of linked list
 */
list_t *env_linked_list(char **env)
{
	list_t *head;
	int i = 0;
​
	head = NULL;
	while (env[i] != NULL)
	{
		add_end_node(&head, env[i]);
		i++;
	}
	return (head);
}
​
/**
 * _env - prints the environment variable
 * @str: the user input to the shell, not used in this function
 * @env: pointer to the head of the linked list of environment variables
 * Return: 0 on success
 */
int _env(char **str, list_t *env)
{
	free_double_ptr(str);
	print_list(env);
	return (0);
}
​
​
​
== == == == == == = free_double_pointer.c == == == == ==
​
#include "shell.h"
​
/**
 * free_double_ptr - free malloced arrays
 * @str: array of strings
 */
void free_double_ptr(char **str)
{
	int i = 0;
​
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
}
​
​
== == == == == = get_environment.c == == == == == == == == =
#include "shell.h"
​
/**
 * c_strdup - duplicates a string, excluding the first cs characters
 * @str: the string to be duplicated
 * @cs: the number of characters to be excluded from
 * the beginning of the string
 * Return: a pointer to the duplicated string, or NULL on failure
 */
char *c_strdup(char *str, int cs)
{
	char *duplicate_str;
	int i, len = 0;
​
	if (str == NULL)
		return (NULL);
​
	while (*(str + len))
		len++;
	len++;
​
	duplicate_str = malloc(sizeof(char) * (len - cs));
	if (duplicate_str == NULL)
		return (NULL);
​
	i = 0;
	while (i < (len - cs))
	{
		*(duplicate_str + i) = *(str + cs + i);
		i++;
	}
	return (duplicate_str);
}
​
/**
 * get_env - retrieves the value of an environment variable
 * @str: the name of the environment variable
 * @env: a linked list containing the environment variables
 * Return: a string containing the value of the environment variable, or NULL
 * if the variable does not exist
 */
char *get_env(char *str, list_t *env)
{
	int j = 0, cs = 0;
​
	while (env != NULL)
	{
		j = 0;
		while ((env->var)[j] == str[j])
			j++;
		if (str[j] == '\0' && (env->var)[j] == '=')
			break;
		env = env->next;
	}
​
	while (str[cs] != '\0')
		cs++;
	cs++;
	return (c_strdup(env->var, cs));
}
​
== == == == == == = interger_to_string.c == == == == =
#include "shell.h"
​
/**
 * numlen - calculates the number of digits in an integer
 * @n:  the integer to calculate the number of digits for
 * Return: the number of digits in the integer
 */
int numlen(int n)
{
	int count = 0;
	int num = n;
​
	while (num > 9 || num < -9)
	{
		num /= 10;
		count++;
	}
	return (count);
}
/**
 * int_to_string - converts an integer to a string
 * @number: the integer to be converted
 * Return: a string representation of the integer
 */
​
char *int_to_string(int number)
{
	int digits, tens, i = 0, t = 0, x;
	char *res;
​
	digits = number;
	tens = 1;
​
	if (number < 0)
		t = 1;
	res = malloc(sizeof(char) * (numlen(digits) + 2 + t));
	if (res == NULL)
		return (NULL);
	if (number < 0)
	{
		res[i] = '-';
		i++;
	}
	for (x = 0; digits > 9 || digits < -9; x++)
	{
		digits /= 10;
		tens *= 10;
	}
	for (digits = number; x >= 0; x--)
	{
		if (digits < 0)
		{
			res[i] = (digits / tens) * -1 + '0';
			i++;
		}
		else
		{
			res[i] = (digits / tens) + '0';
			i++;
		}
		digits %= tens;
		tens /= 10;
	}
	res[i] = '\0';
	return (res);
}
​
== == == == == == == linked - lists.c == == == == == == == =
#include "shell.h"
​
/**
 * print_list - prints all the elements of a linked list
 * @h: pointer to the head of the list
 * Return: the number of nodes in the list
 */
size_t print_list(list_t *h)
{
	list_t *c_list = h;
	int count = 0;
	int c = 0;
​
	if (h == NULL)
		return (0);
	while (c_list != NULL)
	{
		if (c_list->var == NULL)
		{
			write(STDOUT_FILENO, "(nil)", 5);
			write(STDOUT_FILENO, "\n", 1);
		}
		else
		{
			c = 0;
			while ((c_list->var)[c] != '\0')
				c++;
			write(STDOUT_FILENO, c_list->var, c);
			write(STDOUT_FILENO, "\n", 1);
		}
		c_list = c_list->next;
		count++;
	}
	return (count);
}
​
/**
 * add_end_node - adds a new node at the end of a linked list
 * @head: pointer to the pointer of the head of the list
 * @str: string to be added as the var of the new node
 * Return: the address of the new element, or NULL if it failed
 */
list_t *add_end_node(list_t **head, char *str)
{
	list_t *new;
	list_t *holder;
​
	if (head == NULL || str == NULL)
		return (NULL);
	new = malloc(sizeof(list_t));
	if (new == NULL)
		return (NULL);
​
	new->var = _strdup(str);
	new->next = NULL;
​
	holder = *head;
	if (holder != NULL)
	{
		while (holder->next != NULL)
		{
			holder = holder->next;
		}
		holder->next = new;
	}
	else
	{
		*head = new;
	}
	return (*head);
}
