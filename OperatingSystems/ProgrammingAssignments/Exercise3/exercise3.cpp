/*
 * Author: 	Jacob K. Francis
 * Version: 1.0
 * Date:	02/26/2019
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
 */
 
 // Include statements
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>

using namespace std;			// namespace declaration

// Main loop
int main (int argc, char *argv[]) {
  
  string line;
  system("setterm -foreground green -background black");	// Setting background and text color of shell
  
  string directory = getenv("PWD");	// Stores starting directory for shell output
  cout << directory << "=> ";		// print prompt to read a line
  
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
      system(line.c_str());		// system call to output command into shell
    }
    
    // Clear screen command
    else if(first_command == "clr"){
		system("clear");		// system call to output command into shell
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
			setenv("PWD", cmd, 1);
			chdir(cmd);
			cout << cmd << endl;
			
		}
		else{
			cout << "Current Directory: " << directory << "\t" <<  endl;
		}
	}
	
    // Quit command
    else if (first_command == "quit")
	break;
	
	// If none of the specified command, uses the standard ANSI C system
	//		function to execute the line through the default system shell
    else {
		system(line.c_str());	// system call to output command into shell
	}
      
    // print prompt before reading next line
    directory = getenv("PWD");	// Grabs the PWD and replaces it in the shell prompt
    cout << directory << "=> ";
  }
}
