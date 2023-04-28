#include "sfsh.h"
/**
 * _strcmp - compare two strings
 * @s1: the first string
 * @s2: the second string
 * Return: 0 if the same, positive/negative value otherwise
 */
int _strcmp(char *s1, char *s2)
{
	int s1d, s2d;

	do {
		s1d = *s1++;
		s2d = *s2++;
		if (s1d == '\0')
			return (s1d - s2d);
	} while (s1d == s2d);

	return (s1d - s2d);
}
/**
 * _strncmp    - Checks if @s1 matches @s2 up to @length amount
 *
 * @s1:          String to be checked against @s2
 * @s2:          String used to check @s1
 * @length:      How many characters to check
 *
 * Return:       (0)                       @s1 is a match against @s2
 *               (Any number other than 0) @s1 is not a match against @s2
 */
int _strncmp(char *s1, char *s2, int length)
{
	while (length-- && *s1++ && *s2++ && (*s1 == *s2))
		;
	return (length == 0 ? 0 : *s1 - *s2);
}
/**
 * _strcpy     - Makes a copy of @src string into @dest string
 *
 * @dest:        String to be returned
 * @src:         String to be copied
 *
 * Return:       @dest
 */
char *_strcpy(char *dest, char *src)
{
	int i;

	for (i = 0; src[i] != '\0' && (dest[i] = src[i]); i++)
		;
	dest[i] = '\0';

	return (dest);
}
/**
 * cmdcat     - Concatenates @dir and @file into executable format
 *
 * @dir:         Directory where @file resides
 * @file:        The program to be executed
 *
 * Return:       Concatenated string of @dir + / + @file
 */
char *cmdcat(char *dir, char *file)
{
	int dir_length = 0, file_length = 0;

	while (dir[++dir_length])
		;

	dir[dir_length++] = '/';

	while (file[file_length])
	{
		dir[dir_length++] = file[file_length++];
	}
	dir[dir_length] = '\0';

	return (dir);
}

/**
 * _strlen - returns the length of a string
 * @str: our string to measure
 * Return: length of string
 */
int _strlen(char *str)
{
	int i;

	for (i = 0; str[i]; i++)
		;
	return (i);
}
