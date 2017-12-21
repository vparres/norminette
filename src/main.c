/*
** EPITECH PROJECT, 2017
** main
** File description:
** Main function for the project
*/

#include "functions.h"
#include "my.h"
#include "global2.h"
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>

int     find_type(mode_t mode)
{
	switch ((int)(mode & S_IFMT)) {
	case S_IFSOCK:
		return ('s');
	case S_IFLNK:
		return ('l');
	case S_IFREG:
		return ('-');
	case S_IFBLK:
		return ('b');
	case S_IFDIR:
		return ('d');
	case S_IFCHR:
		return ('c');
	case S_IFIFO:
		return ('p');
	}
	return ('?');
}

int     is_dir(char *path)
{
	struct stat	info;

	if (lstat(path, &info) >= 0 && find_type(info.st_mode) == 'd')
		return (1);
	return (0);
}

char	**parse_args(int argc, char **args)
{
	char	**dirs;
	int	got_an_arg = 0;
	int	pos = 0;

	if (argc != 1) {
		dirs = malloc(sizeof(*dirs) * argc);
		for (int i = 0; i < argc; i++)
			dirs[i] = 0;
	} else {
		dirs = malloc(sizeof(*dirs) * 2);
		for (int i = 0; i < 2; i++)
			dirs[i] = 0;
	}
	for (int i = 1; i < argc; i++)
		if (args[i][0] != '-') {
			dirs[pos++] = args[i];
			got_an_arg = 1;
		}		
	if (!got_an_arg) {
		dirs[0] = ".";
		dirs[1] = 0;
	} else
		dirs[argc - 1] = 0;
	return (dirs);
}

void	set_sigaction(void)
{
	struct sigaction	action;

	memset(&action, '\0', sizeof(action));
        action.sa_sigaction = &catch_sig;
	action.sa_flags = SA_SIGINFO;
	sigaction(2, &action, 0);
	sigaction(4, &action, 0);
	sigaction(5, &action, 0);
	sigaction(6, &action, 0);
	sigaction(11, &action, 0);
	sigaction(15, &action, 0);
}

int	main(int argc, char **args)
{
	char	**dirs = 0;

	set_sigaction();
	flags = get_flags(argc, args);
	dirs = parse_args(argc, args);
	for (int i = 0; i < 40; i++)
		mistakes[i] = 0;
	for (int i = 0; dirs[i] != 0; i++) {
		if (flags.d)
			printf("Entering %s\n", dirs[i]);
		if (is_dir(dirs[i]))
			scan_folder(dirs[i], &flags, mistakes);
		else
			scan_file(dirs[i], &flags, mistakes);
	}
        free(dirs);
	if (flags.d)
		printf("Displaying result\n");
	display_result(mistakes, &flags);
	return (0);
}

int	*get_mistakes()
{
	return (mistakes);
}

flag	*get_flags_var()
{
	return (&flags);
}
