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

typedef enum e_pnode_type
{
	Null,
	Program_Call,
	Pipe,
	Redirect_input,
	Redirect_input_heredoc,
	Redirect_output,
	Redirect_output_append
}	t_ntype;

typedef struct s_pipeline_tree_node {
	enum e_pnode_type			type;
	char						**args;
	int							input_fd;
	int							output_fd;
	struct s_pipeline_tree_node	*input[2];
	struct s_pipeline_tree_node	*output;
	struct s_pipeline_tree_node *next;
}	t_pnode;

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
