#include "sfsh.h"
/**
 * main    - Entry point for the Super Friendship Shell
 * @argc:    Argument counter
 * @argv:    pointer to array of strings
 * Return:   Always return 0
 */
int main(int argc, char *argv[])
{
	signal(SIGINT, signal_handler);
	sfsh_loop(argc, argv);

	return (0);
}
