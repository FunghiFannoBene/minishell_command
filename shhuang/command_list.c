#include "minishell.h"

int    search_command(char *s, int *i, t_redirect **command)
{
    int    x;
    int    start;
    t_redirect *head = NULL;
  
    x = 0;
    *command = malloc(sizeof(t_redirect));
    head = *command;
    if (!*command)
        return -1;
    while (s[*i] && s[*i] == ' ')
        (*i)++;
    start = *i;
    while (s[start + x] && s[start + x] != ' ')
        x++;
    (*command)->str = malloc(sizeof(char) * (x + 1));
    if (!(*command)->str)
        return -1;
    x = 0;
    while (s[*i] && s[*i] != ' ')
        (*command)->str[x++] = s[(*i)++];
    (*command)->str[x] = '\0';
    (*command)->size = x;
    (*command)->flag = 0;
    if(s[*i] == '\0')
        {
            (*command)->next = NULL;
            return(-1);
			// FREE????? chiedere conferma di cosa fare quando echo è da solo. ad esempio o qualsiasi command
        }
    else
        next_size(s, i, &head);
    return(0);
}


int	check_and_skip_space(char *s, int *i)
{
	if (s[*i] == ' ')
	{
		while (s[*i] == ' ')
			(*i)++;
	}
	else
		return (-1);
	return(0);
}

int	assign_flag(char *s, int *i, t_redirect **command)
{
	if (s[*i] == '\'')
		(*command)->flag = '\'';
	else if (s[*i] == '"')
		(*command)->flag = '"';
	if ((*command)->flag != 0)
		(*i)++;
	if ((*command)->flag == s[*i])
	{
		(*i)++;
		(*command)->flag = 0;
		return (-1);
	}
	(*command)->start = *i;
	return (1);
}

int	check_slashes(char *s, int *i, t_redirect **command)
{
	if (((*command)->flag == 0 || (*command)->flag == '"') && s[*i] == '\\'
			&& s[*i + 1] == '\\')
	{
		(*i) += 2;
		(*command)->size++;
		return (-1);
	}
	if ((*command)->flag == 0 && s[*i] == '\\' && (s[*i + 1] == '\'' || s[*i
			+ 1] == '"'))
	{
		(*command)->size++;
		(*i) += 2;
		return (-1);
	}
	if (s[*i] == '\\' && s[*i + 1] == '"')
	{
		(*command)->size++;
		(*i) += 2;
		return (-1);
	}
	return (0);
}

char *substring(const char *str, size_t begin, size_t len) 
{
    if (str == NULL || strlen(str) < begin || strlen(str) < (begin + len)) 
        return NULL; 

    return strndup(str + begin, len);
}

void next_size(char *s, int *i, t_redirect **command)
{
	(*command)->next = malloc(sizeof(t_redirect));
	(*command) = (*command)->next;
	(*command)->flag = 0;
	(*command)->size = 0;
}

int	end_check(char *s, int *i, t_redirect **command)
{
	if (s[*i] == (*command)->flag)
	{
		(*i)++;
		(*command)->str = substring(s, (*command)->start, (*command)->size);
		if(s[*i] == '\0')
		{
			(*command)->next = NULL;
			return(-2);
		}
		else
			next_size(s, i, command);
		return(-1);
	}
	else if ((*command)->flag == 0 && s[*i] == '|')
	{
		(*i)++;
		(*command)->str = substring(s, (*command)->start, (*command)->size);
		check_and_skip_space(s, i);
		(*command)->next = NULL;
		if(s[*i] == '\0')
			return(-3);
		return(-2);
	}
	return 0;
}

void dollar_sign(char *s, int *i, t_redirect **command)
{
	 if(((*command)->flag == 0 || (*command)->flag == '"') && s[*i] == '$')
                (*command)->size += 0; // per ora
}

int flag_zero_space(char *s, int *i, t_redirect **command)
{

	if((*command)->flag == 0 && s[*i] == ' ') //se non sono dentro le virgolette e trovo uno spazio skippo tutto. e continuo senza aggiungere i++;
    {
		(*command)->str = substring(s, (*command)->start, (*command)->size);
        while(s[*i] && s[*i] == ' ')
            i++;
		if(s[*i] == '\0')
		{
			(*command)->next = NULL;
			return(-2);
		}
		next_size(s, i, command);
		return(-1);
	}
	return(1);

}

int	size_of_command(char *s, int *i, t_redirect **command)
{
	t_redirect *head = *command;
	if (check_and_skip_space(s, i) == -1)
		return (-1);
	(*command)->flag = 0;
	(*command)->start = *i;
	(*command)->size = 0;
	while (s[*i])
	{
		check_and_skip_space(s, i);
		if (assign_flag(s, i, command) == -1)
			continue ;
		while (s[*i])
		{
			dollar_sign(s, i, command);
			if (check_slashes(s, i, command) == -1)
				continue;
			if (end_check(s, i, command) == -1)
				break;
			else if(end_check(s, i, command) == -2)
			{
				return (1);
				//pipe trovata esci. o fine stringa
			}
			else if(end_check(s, i, command) == -3)
			{
				return (-1);
				//Errore niente dopo i pipes
			}
			if(flag_zero_space(s, i, command) == -1)
			{
				break;
			}
			else if(flag_zero_space(s, i, command) == -2)
			{
				return(1); //fine stringa
			}
			if((*command)->flag == 0 && (s[*i] == '\'' || s[*i] == '"'))
              break;
            (*command)->size++;    //count arg size;
            (*i)++;
		}
	}
}

void	create_command_list()
{
	t_redirect	*command;
	t_redirect	*head;
	int			i;
	char *s = "abcd c | c 234";
	command = NULL;
	i = 0;
	if(search_command(s, &i, &command) == -1)
	{
		printf("stringa completa, solo comando.");
	}
	else
	{
		head = command;
		size_of_command(s, &i, &command);
		printf("%d, string: %s", command->size, command->str);
	}
}

int	main(void)
{
	create_command_list();
}