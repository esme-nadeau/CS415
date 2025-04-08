/*1. main.c must be able to start in both of the following modes:
    a. File mode:
        i. File mode is called by using the -f flag and passing it a filename
        (e.g. pseudo-shell -f <filename>)
        ii. The shell only accepts filenames in file mode.
        iii. All input is taken from the file whose name is specified by <filename>
        and all output is written to the file “output.txt”. There should be no
        console output in this mode. Place all output in the file.
    b. Interactive mode:
        i. If no command line argument is given the project will start in interactive
        mode.
        ii. The program accepts input from the console and writes output from the
        console.
2. The main function must have the following input parameters: argc and argv
    a. argc contains the number of input parameters for the main function.
    b. argv contains a 2D array of strings that correspond to the parameters input into
    the program on execution. (e.g. the filename)
3. You must use dynamic memory allocation via malloc() and free().
    a. Hint: it might be helpful to do these in a function.
    b. Valgrind must not show any memory leak for your program.
4. Your solution must use a loop that reads commands on a line-by-line basis using getline()
from the command line or a file.
5. Your solution must be able to parse the input string command into tokens using the
provided header file to identify the commands and their arguments.
    a. The string_parser.h will be provided.
    6. Your solution must be able to call the corresponding function for Unix command
execution.
a. We will provide a header file (see below).*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function declarations

typedef struct
{
    char** command_list;
    int num_token;
}command_line;

int findMode(int argc, char** argv);
void fileMode(int argc, char** argv);
void interactiveMode(int argc, char** argv);
int count_token (char* buf, const char* delim);
command_line str_filler(char* buf, const char* delim);

// Determine if file mode or interactive mode
int findMode(int argc, char** argv) {
    if (argc == 1) { // interactive mode
        return 0;
    } else {
        if (strcmp(argv[1], "-f") == 0) {
            return 1; // file mode
        } else {
            fprintf(stderr, "please provide a valid flag!\n");
            exit(EXIT_FAILURE);
        }
    }
}

// Main entry point
int main(int argc, char** argv) {
    int filemode = findMode(argc, argv);

    if (filemode) {
        fileMode(argc, argv);
    } else {
        interactiveMode(argc, argv);
    }

    return 0;
}

// Stub for file mode (to be implemented later)
void fileMode(int argc, char** argv) {
    printf("File mode not yet implemented.\n");
}

// Interactive shell loop
void interactiveMode(int argc, char** argv) {
    int in_terminal = 1;
    char input_buf[256];  // buffer for input lines

    while (in_terminal) {
        printf(">>> ");
        if (fgets(input_buf, sizeof(input_buf), stdin) == NULL) {
            printf("\nEnd of input.\n");
            break;
        }

        // Remove trailing newline
        input_buf[strcspn(input_buf, "\n")] = '\0';

        // Handle exit command
        if (strcmp(input_buf, "exit") == 0) {
            in_terminal = 0;
        }

        printf("You entered: %s\n", input_buf);
        char* delim = " ";
        command_line filler = str_filler(input_buf, delim);
        
        printf("This is the first token of the string you entered: %s\n", *filler.command_list);
        printf("And you entered this many tokens: %d\n", filler.num_token);
    }
}

int count_token(char* buf, const char* delim)
{

	if (buf == NULL) {
        return 0;
    }

	char *token; char *ptr;
	int count = 0;

	char *copy_buf = (char *)malloc(sizeof(char) * strlen(buf) + 1);
	strcpy(copy_buf, buf);
	copy_buf[strlen(buf)] = '\0';

	token = strtok_r(copy_buf, delim, &ptr);

	while (token != NULL) {
		count++;
		token = strtok_r(NULL, delim, &ptr);
	}

	return count;
}

command_line str_filler(char* buf, const char* delim)
{

	command_line res;
	res.command_list = NULL;
	res.num_token = 0;

	if (buf == NULL) {
        return res;
    }

	// #2.	count the number of tokens with count_token function, set num_token. 
	res.num_token = count_token(buf, delim);

	// #3. malloc memory for token array inside command_line variable based on the number of tokens.
	res.command_list = malloc((res.num_token + 1) * sizeof(char*));
    if (res.command_list == NULL) {
        res.num_token = 0;
        return res;
    }

	char* token; char* ptr; int i = 0;

	char *copy_buf = (char *)malloc(sizeof(char) * strlen(buf) + 1);
	strcpy(copy_buf, buf);
	copy_buf[strlen(buf)] = '\0';

	// #4.	use function strtok_r to find out the tokens 
	token = strtok_r(copy_buf, delim, &ptr);

	// #5. malloc each index of the array with the length of tokens, fill command_list array with tokens, and fill last spot with NULL.
	while (token != NULL && i < res.num_token) {
        size_t len = strlen(token);
        if (len > 0 && token[len - 1] == '\n') {
            token[len - 1] = '\0';
            len--;
        }
		// #5. malloc each index of the array with the length of tokens
		res.command_list[i] = malloc((len + 1) * sizeof(char));
		if (res.command_list[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(res.command_list[j]);
            }
            free(res.command_list);
            res.command_list = NULL;
            res.num_token = 0;
            return res;
		}
		
		// fill command_list array with tokens
		strcpy(res.command_list[i], token);
        if (strlen(token) > 0) {
			i++;
		}
        token = strtok_r(NULL, delim, &ptr);
	}

	// fill last spot with NULL.
    res.command_list[i] = NULL;
    return res;
}