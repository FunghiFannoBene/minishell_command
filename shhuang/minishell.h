#ifndef MINISHELL_H
#define MINISHELL_H

#include <stddef.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h> //path max
#include <sys/types.h>
#include <sys/wait.h>

/*
echo "Hello"
echo "Hello" | cat
echo "Hello" > file.txt
echo "Hello" >> file.txt
echo "c"'a't is 'good'
cat < file.txt
cat << EOF
Hello
This is a here document.
EOF

*/


typedef struct s_redirect
{
	char *str;
	struct s_redirect *next;
	int		start;
	char	flag;
	int		size;
} t_redirect;


typedef struct s_fix_string
{
	char *command_name;
	char *stringa;
	struct list_redirect* redirect;
	struct s_fix_string *next;
} t_indice;


void next_size(char *s, int *i, t_redirect **command);

#endif
