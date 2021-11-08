#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>

int main()
{
    // used to store two ends of the pipelines
	int fd1[2];
	int fd2[2];

	char input_str[100];
	pid_t p;

    // check fails
	if (pipe(fd1) == -1)
	{
		fprintf(stderr, "Pipe Failed" );
		return 1;
	}
	if (pipe(fd2)==-1)
	{
		fprintf(stderr, "Pipe Failed" );
		return 1;
	}

    // get input
	scanf("%s", input_str);

    // create child with fork()
	p = fork();

    // check fork fail
	if (p < 0)
	{
		fprintf(stderr, "fork Failed" );
		return 1;
	}

	// parent - writes
	else if (p > 0)
	{
		char string[100];

        // close reading end of the first pipe
		close(fd1[0]);

        // write input and close writing end of the first pipe
		write(fd1[1], input_str, strlen(input_str)+1);
		close(fd1[1]);

		// wait for child to return response
		wait(NULL);

        // close writing end of second pipe
		close(fd2[1]);

		// Read output from child, print and close the reading part of second pipe
		read(fd2[0], string, 100);
		printf("%s\n", string);
		close(fd2[0]);
	}

	// child - reads
	else
	{   
        // close writing end of first pipe
		close(fd1[1]);

		// read input from parent
		char string[100];
		read(fd1[0], string, 100);        

        // process string
        int i;
        for (i = 0; i < strlen(string); i++) {
            if (string[i] >= 'a' && string[i] <= 'z')
                string[i] = toupper(string[i]);
            else
                string[i] = tolower(string[i]);
        }        

        // mark ending for string
        string[i + 1] = '\0';

		// close both reading ends
		close(fd1[0]);
		close(fd2[0]);

		// write to parent and close the writing end
		write(fd2[1], string, strlen(string)+1);
		close(fd2[1]);

		exit(0);
	}
}
