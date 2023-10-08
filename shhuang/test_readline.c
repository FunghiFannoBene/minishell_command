#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>


int size_readline(char *input)
{
	int i=0;
	int count = 0;
	while(input[i])
	{
		if(input[i+1]== '\\' && input[i] == '\\' )
		{
			count++;
			i+=2;
		}
		else if(input[i] == '\\')
			i++;
		else
		{
			i++;
			count++;
		}
	}
	return(count);
}

char* adapt_readline(char *input, char* nuovo_input)
{
	int i=0;
	int count = 0;
	while(input[i])
	{
		if(input[i+1] == '\\' && input[i] == '\\' )
		{
			nuovo_input[count] = input[i]; 
			count++;
			i+=2;
		}
		else if(input[i] == '\\')
			i++;
		else
		{
			nuovo_input[count] = input[i];
			i++;
			count++;
		}
	}
	nuovo_input[count] = '\0';
	return(nuovo_input);
}

int main() {
    char *input = readline("Stringa: ");
 
    printf("Hai inserito: %s\n", input);


	char* nuovo_input = calloc((size_readline(input)+1), sizeof(char));
	nuovo_input = adapt_readline(input, nuovo_input);

	printf("è diventato: %s\n", nuovo_input);
	free(input);
}


//conserva tutti i /