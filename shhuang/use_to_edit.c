int	end_check(char *s, int *i, t_redirect **command)
{
	int end;
	if (s[*i] == (*command)->flag)
	{
		(*i)++;
		end = *i;
		(*command)->str = substring(s, end, (*command)->size);
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
		end = *i;
		(*command)->str = substring(s, end, (*command)->size);
		if(s[*i] == '\0')
		{
			(*command)->next = NULL;
			return(-2);
		}
		else
			next_size(s, i, command);
		return(-1);
	}
	return 0;
}
