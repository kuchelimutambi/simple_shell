sfsh : main.c sfsh_builtins0.c sfsh_builtins1.c sfsh_loop.c sfsh_execute.c sfsh_helpers.c sfsh_string_helpers.c sfsh_getline.c sfsh_launch.c sfsh_tokenize.c
	gcc -Wall -Werror -Wextra -pedantic main.c sfsh_builtins0.c sfsh_builtins1.c sfsh_loop.c sfsh_execute.c sfsh_helpers.c sfsh_string_helpers.c sfsh_getline.c sfsh_launch.c sfsh_tokenize.c -o sfsh
