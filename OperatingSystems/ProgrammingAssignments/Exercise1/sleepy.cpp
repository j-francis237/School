/*
 * Author: 	Jacob K. Francis
 * Version: 1.0
 * Date:	01/26/2019
 * 
 * This code receives a loop count from the command line:
 * 		
 * 		sleepy n
 * 
 * where n is the number of seconds for which the program should run. In each
 * loop, the loop count and the process ID are printed so that the particular 
 * process can be identified.
 * 
 * Example command line input:	nice ./sleepy 10	// 10 loops
 * 
 * 
 */
// Include statements
#include <iostream>
#include <unistd.h>
#include <cstdlib>

using namespace std;		// namespace declaration


// Main loop
int main(int argc, char** argv) {

	cout << "Seconds\t\tProcess ID" << endl;
	int seconds = atoi(argv[1]);

	for (int j = 0; j < seconds; j++) {
		sleep(1);
		int process_id = getpid();
		cout << j+1 << "\t\t" << process_id << endl;
	}
	
    return 0; 
}

