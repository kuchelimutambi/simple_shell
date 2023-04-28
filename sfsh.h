#ifndef SFSH_H_
#define SFSH_H_

extern char **environ;
#define BUFFER_SIZE 64

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <dirent.h>

/**
 * struct builtin - data type for our list of builtins
 * @name: name of command
 * @function: the function pointer of associated name
 */
typedef struct builtin
{
	char *name;
	int (*function)(char **);
} builtin_t;


/* Core functions */
void sfsh_loop(int argc, char *argv[]);
char *sfsh_getline(void);
char **sfsh_tokenize(char *line);
int sfsh_execute(char **args, char *line);
int sfsh_launch(char **args, char **dirs, char *line);
void sfsh_search_path(char **args, char **dirs);

/* built-ins */
int sfsh_help(char **args);
int sfsh_exit(char **args);
int sfsh_error(char **args);
int sfsh_env(char **args);
int sfsh_setenv(char **args);
int sfsh_unsetenv(char **args);
int sfsh_cd(char **args);

/* helpers */
int _strcmp(char *s1, char *s2);
int _strncmp(char *s1, char *s2, int length);
char *_strcpy(char *dest, char *src);
char *cmdcat(char *dir, char *file);
int _strlen(char *str);
char *get_path(char *dest);
void signal_handler(int sig_num);


#endif /* end of SFSH_H_ header guard */
