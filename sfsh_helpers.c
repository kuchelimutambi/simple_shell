#include "sfsh.h"

/**
 * signal_handler   - Catches and handles signals
 *
 * @sig_num:          Signal trigger
 */

void signal_handler(int sig_num)
{
	/* sig_num is 2 with ctrl + c*/

	if (sig_num)
	{
		write(1, "^C\n", 3);
	}
}
