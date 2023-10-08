#include "./minishell.h"

// int checkflag(char *c)
// {
// 	if(c == '"')
// 	{
// 		return(1);
// 	}
// 	else if(c == '\'')
// 	{
// 		return(2);
// 	}
// 	else
// 		return(3);
// }

int odd_virgolette(char *s)
{
    int i = 0;
    int count_single;
    count_single = 0;
    int count_double;
    count_double = 0;
    while(s[i])
    {
        if(s[i] == '\\' && s[i+1] == '\\' || (s[i] == '\\' && (s[i+1] == '\'' || s[i+1] == '"')))
            i+=2;
        else if(s[i] == '\'')
        {
            i++;
            count_single++;
            while(s[i] && s[i] != '\'')
            {
                i++;
                if(s[i] == '\'' && s[i-1] == '\\')
                  i++;
            }
            if(s[i] == '\0')
              break;
            else
              count_single++;
            i++;
        }
        else if(s[i] == '"')
        {
            i++;
            count_double++;
            while(s[i] && s[i] != '"')
            {
                i++;
                if(s[i] == '"' && s[i-1] == '\\')
                  i++;
            }
            if(s[i] == '\0')
              break;
            else
              count_double++;
            i++;
        }
        else
            i++;
    }
    if((count_double % 2) == 0 && (count_single % 2) == 0)
        return(1);
    return(0);
    //Trova se la stringa contiene dispari ' o " senza contare quelle precedute da // (ma non precedute da /// 3 o +) UN casino
}


int check_flag(int flag)
{
	if(flag == 0)
	{
		return(1);
	}
	else
	{
		return(0);
	}
}

int calculate_string_size(char *s, int i, t_redirect *list)
{
    char flag = 0;
    int count = 0;

    if(s[i] == ' ')
    {
		while(s[i] == ' ')
		i++;
    }
    else
      return(-1);
    while(s[i])
    {
        if(s[i] == '\'')
            flag = '\'';
        else if(s[i] == '"')
            flag = '"';
        if(flag != 0)
            i++;
        if(flag == s[i])
        {
          i++;
          flag = 0;
          continue;
        }
        while(s[i])
        {
			if((flag == 0 || flag == '"' )&& s[i] == '\\' && s[i+1] == '\\' )
			{
				i+=2;
				count++;
				continue;
			}
			if(flag == 0 && s[i] == '\\' && (s[i+1] == '\'' || s[i+1] == '"'))
			{
				count++;
				i+=2;
				continue;
			}
			if(s[i] == '\\' && s[i+1] == '"')
            {
                count++;
                i+=2;
                continue;
            }
            if(s[i] == flag)
            {
                i++;
				if(s[i] == '\0')
                  return count;
                flag = 0; //ho trovato la chiusura ma la stringa potrebbe continuare ancora
                break;
            }
            else if(flag == 0 && s[i] == '|')
            {
                i++;
                return(count); //ritorno conta se trova un simbolo significativo non incluso tra virgolette
            }
            if((flag == 0 || flag == '"') && s[i] == '$')
                count += 0; //Non funzionale finche non c'è env; Si attiva solo se $ è tra le "$" o senza nulla.
            if(flag == 0 && s[i] == ' ') //se non sono dentro le virgolette e trovo uno spazio skippo tutto. e continuo senza aggiungere i++;
            {
                while(s[i] && s[i] == ' ')
                    i++;
				if(s[i] == '\0')
					return(count);
                count++;
                break;
            }
			if(flag == 0 && (s[i] == '\'' || s[i] == '"'))
              break;
            count++;    //count arg size;
            i++;
        }
        
    }
    return(count);
}

void insert_string(char*s, char **str, int* x)
{
	int i = *x;
    char flag = 0;
    int count = 0;
    if(s[i] == ' ')
    {
    	while(s[i] == ' ')
      		i++;
    }
     while(s[i])
    {
        if(s[i] == '\'')
            flag = '\'';
        else if(s[i] == '"') 
        {
            flag = '"';
        }
        if(flag != 0)
            i++;
        if(flag == s[i])
        {
          i++;
          flag = 0;
          continue;
        }
        while(s[i])
        {
            if((flag == 0 || flag == '"') && s[i] == '\\' && s[i+1] == '\\' )
            {
                (*str)[count] = s[i];
                i+=2;
                count++;
                continue;
            }
            if(flag == 0 && s[i] == '\\' && (s[i+1] == '\'' || s[i+1] == '"'))
            {
                (*str)[count] = s[i+1];
                count++;
                i+=2;
                continue;
            }
            if(s[i] == '\\' && s[i+1] == '"')
            {
                (*str)[count] = s[i+1];
                count++;
                i+=2;
                continue;
            }
            if(s[i] == flag)
            {
                i++;
                if(s[i] == '\0')
                {
					*x = i;
                  (*str)[count] = '\0';
                  return;
                }
                flag = 0; 
                break;
            }
            else if(flag == 0 && s[i] == '|')
            {
                i++;
				*x = i;
                (*str)[count] = '\0';
                return; 
            }
            if((flag == 0 || flag == '"') && s[i] == '$')
                count += 0; //funzione riprendi da lista il valore; scorri data
            if(flag == 0 && s[i] == ' ') 
            {
                (*str)[count] = s[i];
                while(s[i] && s[i] == ' ')
                    i++;
                count++;
                break;
            }
            if(flag == 0 && (s[i] == '\'' || s[i] == '"'))
              break;
            (*str)[count] = s[i];
            count++;
            i++;
        }
    }
	*x = i;
	printf("ritorno\n");
    (*str)[count] = '\0';
}

void search_command(char *input, int *i, t_redirect* command)
{
    int x = 0;
    int start;

    while (input[*i] && input[*i] == ' ')
        (*i)++;
    start = *i;
    while (input[start + x] && input[start + x] != ' ')
        x++;
    command->str = malloc(sizeof(char) * (x + 1));
    if (!command->str)
        return;
    x = 0;
    while (input[*i] && input[*i] != ' ')
        command->str[x++] = input[(*i)++];
    command->str[x] = '\0';
}


t_indice* create_commands_list(char *input)
{
    t_redirect *command = NULL;
    t_indice *head = NULL;
    t_indice *prev = NULL;
    int index = 0;

    while (input[index]) {
        command = malloc(sizeof(t_indice));
        if (!command) {
            return NULL;
        }
        search_command(input, &index, command);
        command->next = NULL;

        int size = calculate_string_size(input, index);
        if (size == -1) 
		{
			//freelist
            printf("\n\nLista non creata: err -1;\n\n");
            free(input);
            return NULL;
        }
        if (!head)
            head = command;
        else if (prev)
            prev->next = command;
        prev = command;
        command->stringa = calloc(size + 1, sizeof(char));
        insert_string(input, &command->stringa, &index);
		if(input[index] == '\0')
		{
			prev->next = NULL;
			free(input);
			return(head);
		}
    }
    return head;
}

int main() {
	while(1)
	{

		//stringhe non funzionanti : 
		//echo "c"i'a\\\'o\'\'\'\'
		//output: cia\\\o''''        Se inizia con slash la virgoletta non ha significato!!
		//per testare quello di sopra: char input[50] = "echo \"c\"i'a\\\\\\'o\\'\\'\\'\\'";
		//echo "c"i'a\\\'o  
		//output == cia\\\o slash non deve avere significato se dentro virgolette.
		//echo 'It'\''s a "mixed" bag: '\''$var'\''!'
		//output: It's a "mixed" bag: '$var'!
		//ERRORI ANCORA da fix
		//echo "This is a \"nested\" quote."
		//echo "This is \"complex\" with some 'mixed' quotes. 'And within these quotes we escape.'" "Then we add \"more\" complexity." 'Backslashes: \\\\ and then \\\\\\ more.'
		//echo "Hello, \"world\"!" 'It'\''s a beautiful day.' "Isn't it?" 'Look at these backslashes: \\\\' "More \\\\"

		//Fondere readline
		// PROBLEMA!!!!!! echo "chat\" ciao \" mondo 22\\"    TERMINALE: chat" ciao " mondo 22\    PROGRAMMA: chat ciao  mondo 22\
		

		//FONDERE RIMOZIONE \ con CREA OUTPUT UGUALE A ECHO ETC.. //readline va tenuto
		char *input = readline("Stringa: ");
		printf("\nReadline =%s\n", input);
		// char **args = calloc(sizeof(char*) , 3);

		// args[0] = strdup("cat");
		// if (!args[0]) 
		// {
    	// 	perror("Memory allocation failed");
    	// 	exit(1);
		// }
		// args[2] = NULL;
		
		// char* nuovo_input = calloc((size_readline(input)+1), sizeof(char));
		// nuovo_input = adapt_readline(input, nuovo_input);
		// free(input);

		
		// execute();



		t_indice *command;
		command = create_commands_list(input);
		int x= 0;
		while(command != NULL)
		{
			x++;
			printf("Lista: Comando %d: %s Content: %s\n",x, command->str, command->stringa);
			command=command->next;
		}






		//esegue tutti i comandi da sinistra a destra

		//shhuang@LAPTOP-2QEICO5A:~$ echo adssd  >fruits.txt  "asbcb"
		//shhuang@LAPTOP-2QEICO5A:~$ cat fruits.txt

// 		shhuang@LAPTOP-2QEICO5A:~$ echo "a>file.txt"
// a>file.txt
// shhuang@LAPTOP-2QEICO5A:~$ echo 'a>file.txt'
// a>file.txt

		// args[1] = s;
		// if(strncmp("cat", input, 3) == 0)//cat impostare i a 3
		// 	execute(args); 
		// free(input);
	}	
}


// int main()
// {
//   char *s = calloc( sizeof(char) , 19 );
//   char x[50] = "echo \"He's a \"good\" guy.\"";
//     insert_string(x, &s);
//      printf("%s", s);
// }

// char *editstring(char *s, t_list*env)
// {
// 	int i = 4; // command size
// 	int flag;
// 	char *string;
// 	if(s[i] == '\0') //nel caso ci sia echo
// 	{
// 		string = calloc(1, sizeof(char));
// 		return(string);
// 	}
// 	if(s[i] != ' ') //nel caso ci sia echoabc
// 	{
// 		perror("echo not valid.\n");
// 		return(NULL);
// 	}
// 	while(s[i] && s[i] == ' ')
// 		i++;
// 	if(!s[i]) //nel caso di solo spazi
// 	{
// 		string = calloc(1, sizeof(char));
// 		return(string);
// 	}

// 	//END CHECKS

// 	//if " o ' priority
// 	calculate_string_size(s+i);

// 	while(s[i])
// 	{
// 		if(flag == 0)
// 		{
			
// 			checkflag(s[i]);
// 		}
// 		else if(flag == 1)
// 		{
// 			i += print_till(checkclosing(s+i,'"'));
// 			while(s[i] && s[i] == ' ')
// 				i++;
// 			flag = 0;
// 		}
// 		else if(flag == 2)
// 		{
// 			i += print_till(checkclosing(s+i, '\''));
// 			while(s[i] && s[i] == ' ')
// 				i++;
// 			flag = 0;
// 		}

// 	}
	
// }

// void echo(char *s)
// {
	
// }


// int main(int argc, char **argv, char**env)
// {
// 	int i=0;

// }