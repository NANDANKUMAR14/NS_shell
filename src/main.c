/***************************************************************************//**

  @file         main.c

  @author       Nandan

  @date         Monday May 25,2026

  @brief        ns (Nandan's Shell) is a simple shell program written in C. It provides a command-line interface for users to interact with the operating system. The shell supports basic built-in commands such as "cd", "help", and "exit", as well as the ability to execute external programs.

*******************************************************************************/




#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>    
#include<unistd.h>
#include<string.h>

//builtin functions declaration

int ns_cd(char **args);
int ns_help(char **args);
int ns_exit(char **args);
int ns_launch(char **args);


// List of builtin commands, followed by their corresponding functions.
char *builtin_str[] = 
{
    "cd",
    "help",
    "exit"
};  


int (*builtin_func[])(char **) = 
{
    &ns_cd,
    &ns_help,
    &ns_exit
};

int ns_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}


//built in function implementations

int ns_cd(char **args)
{
if(args[1]==NULL)
   fprintf(stderr,"ns: expected argument to \"cd\"\n");
else
{
    if(chdir(args[1])!=0)
   {
         perror("ns");
    }   
}
return 1;
}

//function to print the help menu
int ns_help(char **args)
{
int i;
printf("heyyy, welcome to ns\n");
printf("Author:Nanda\n");
printf("Type program names and arguments, and hit enter.\n");


printf("The following are built in:\n");
for(i=0;i<ns_num_builtins();i++)
{
    printf("  %s\n",builtin_str[i]);
}

printf("Use the man <command> for more information about command\n");
return 1;
}


//exit function to exit the shell
int ns_exit(char **args)
{
    return 0;
}


int ns_execute(char **args)
{
    int i;

    if(args[0]==NULL)
    {
        // An empty command was entered.
        return 1;
    }

    for(i=0;i<ns_num_builtins();i++)
    {
        if(strcmp(args[0],builtin_str[i])==0)
        {
            return (*builtin_func[i])(args);
        }
    }

    return ns_launch(args);
}




//ns launch function to launch a program and wait for it to terminate

int ns_launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0)
    {
        // Child process
        if(execvp(args[0],args)==-1)
        {
            perror("ns");
        }
        exit(EXIT_FAILURE);
    }
    else if(pid < 0)
    {
        // Error forking
        perror("ns");
    }
    else
    {
        // Parent process
        do
        {
            wpid = waitpid(pid,&status,WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}


// it is the function to read a line of input from stdin

char *ns_read_line(void)
{
    char *line = NULL;
    ssize_t bufsize = 0; // have getline allocate a buffer for us
    if(getline(&line, &bufsize, stdin)==-1)
    {
        if(feof(stdin))
        {
            exit(EXIT_SUCCESS);  // We received an EOF
        }
        else
        {
            perror("ns: getline\n");
            exit(EXIT_FAILURE);
        }
    }   
    return line;
}



//it is the function to split the line into tokens (very naively)

#define ns_TOK_BUFSIZE 64
#define ns_TOK_DELIM " \t\r\n\a"
char **ns_split_line(char *line)
{
     int bufsize =ns_TOK_BUFSIZE,position = 0;
     char **tokens = malloc(sizeof(char *) * bufsize);
     char *token;


     if(!tokens)
     {
         fprintf(stderr,"ns: allocation error\n");
         exit(EXIT_FAILURE);
     }


        token = strtok(line,ns_TOK_DELIM);
        while(token!=NULL)
        {
        tokens[position] = token;
        position++;
        
        if(position > bufsize)
        {
         bufsize += ns_TOK_BUFSIZE;
         tokens = realloc(tokens,sizeof(char *) * bufsize);
         if(!tokens)
         {
             fprintf(stderr,"ns: allocation error\n");
             exit(EXIT_FAILURE);
         }
         
        }

        token = strtok(NULL,ns_TOK_DELIM);
        }

        tokens[position] = NULL;
        return tokens;
     
}




// ns_loop func for the main loop of the shell
void ns_loop(void)
{
char *line;
char **args;
int status;

do
{
printf("> ");
line = ns_read_line();
args = ns_split_line(line);    
status = ns_execute(args);

free(line);
free(args);
  
} while (status);




}


//it is the main function of the shell

int main(int argc, char *argv[]) {
    

    ns_loop();

    return EXIT_SUCCESS;
}