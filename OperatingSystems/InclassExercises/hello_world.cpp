// 0Compile command: g++ -pthread -std=c++17 -o hello_world hello_world.cpp

// "Give it my funk" - Ziad Youssfi 11:05AM 4/26/19
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

void hello() {
	cout << "Hello Concurrent World\n";
}

void initVec(vector<int>& v) {
	for(unsigned int i = 0; i<v.size(); i++)
	v[i] = i;
}

void totVec(vector<int>& v, int& total) {
	for(unsigned int i = 0; i<v.size(); i++)
	total += v[i];
}

int main() {
	/*
	// Launch a separate thread to execute the function hello()
	thread t(hello);
	
	// Wait for the new thread to finish
	t.join();
	*/
	int total_main = 0;
	int total_second = 0;
	vector<int> v1(10);
	
	initVec(v1);
	
	thread t(totVec, ref(v1), ref(total_second));
	
	totVec(v1, total_main);
	
	// Launch a second thread to do totVec
	t.join();
	
	cout << "total_main: \t" << total_main << endl 
		 << "total_second: \t" << total_second << endl;
	
	return 0;
	
}
