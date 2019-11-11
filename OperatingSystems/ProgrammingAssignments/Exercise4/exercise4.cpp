/*
 * Author: 	Jacob K. Francis
 * Version: 1.1
 * Date:	04/04/2019
 * 
 * This code generates a personalized system shell where internal commands 
 * can be programmed to perform a specified task. Below is a list of
 * the internal commands and their descriptions:
 * 
 * >> clr
 * clear the screen using the system function clear: system("clear").
 * 
 * >> dir <directory>
 * list the current directory contents (ls -al <directory>) 
 * 
 * >> cd <directory>
 * Change the current default directory to <directory>. If the <directory> 
 * argument is not present, report the current directory.
 * 
 * >> environ
 * list all the environment strings from within a program
 * 
 * >> quit
 * quit from the program with a zero return value.
 * 
 * If the command does not match any of the commands listed above, it will
 * be passed to the system to be dealt with by the OS.
 * 
 */
 
 // Include statements
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;			// namespace declaration

/********************************************************************

  void syserrmsg(char * msg1, char * msg2);

  print an error message (or two) on stderr followed by system error
  message.

  if msg2 is NULL only msg1 and system message is printed
  if msg1 is NULL only the system message is printed
 *******************************************************************/

void syserrmsg(char * msg1, char * msg2)
{
    if (msg1)
        fprintf(stderr,"ERROR - %s ", msg1);
    if (msg2)
        fprintf(stderr,"%s ", msg2);
    fprintf(stderr,"; ");
    perror(NULL);
    return;
}

/***********************************************************************

  void execute(char ** args, int foreground);

  fork and exec the program and command line arguments in args
  if foreground flag is TRUE, wait until pgm completes before
    returning

***********************************************************************/

void execute(char ** args, int foreground)
{
    int status;
    pid_t child_pid;

// fork

    switch (child_pid = fork()) {

// something wrong with fork

        case -1:
            syserrmsg((char*)"fork",NULL);
            break;

// execution in child process

        case 0:
            execvp(args[0], args);
            syserrmsg((char*)"exec failed -",args[0]);
            exit(EXIT_FAILURE);
    }

// continued execution in parent process

    if (foreground) waitpid(child_pid, &status, WUNTRACED);
}


void parse(string line){
	const char *args[1024] = {NULL};
	istringstream strm(line);
	string x[1024];
	for(int i = 0; strm >> x[i] && i < 1024; i++){
		args[i] = x[i].c_str();
	}

	execute((char **)args, 1);

}


// Main loop
int main (int argc, char *argv[]) {
  
  system("setterm -foreground green -background black");	// Setting background and text color of shell
  
  string line;
  char cwd[256];
  getcwd(cwd,sizeof(cwd));	// Stores starting directory for shell output
  cout << cwd << "=> ";		// print prompt to read a line
  
  while (getline(cin, line)) {				// read a line from std input
    string first_command, second_command;	// parse the first and second command using stringstream	
    istringstream strm;
    strm.str(line);							// set the string stream to read from line
    strm >> first_command;					// read the first command 
    
    
    // check for internal/external commands
    
    // Directory command
    if (first_command == "dir") {
      line.replace (0, 3, "ls -al");	// replace dir with ls -al 
      cout << line << endl;
	  parse(line);			// system call to output command into shell
    }
    
    // Clear screen command
    else if(first_command == "clr"){
		line = "clear";		// system call to output command into shell
		parse(line);
	}
	
	// Prints environmental variables to shell
    else if(first_command == "environ"){
		extern char **environ;
		
		// Every environmental variable is printed into the shell
		for(int i = 0; environ[i] != NULL; i++){
			cout << environ[i] << endl;
		}
	}
	
	// Changes the working directory of shell 
    else if(first_command == "cd"){
		strm >> second_command;			// read the second command 
		
		if(second_command != ""){
			const char* cmd = second_command.c_str();
			if(chdir(cmd) != -1) {
				getcwd(cwd,sizeof(cwd));
				setenv("PWD", cwd, 1);
			}
			
			else cout << cmd << endl;
			
		}
		else{
			cout << "Current Directory: " << cwd << "\t" <<  endl;
		}
	}
	
    // Quit command
    else if (first_command == "quit")
	break;
	
	// If none of the specified command, uses the standard ANSI C system
	//		function to execute the line through the default system shell
    else {
		parse(line);		// system call to output command into shell
	}
      
    // print prompt before reading next line
    cout << cwd << "=> ";
  }
}




