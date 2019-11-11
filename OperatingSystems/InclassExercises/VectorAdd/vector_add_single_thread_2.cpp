// File: vector_add_single_thread.cpp
// compile command: g++ -std=c++17 -pthread -o vector_add_single_thread vector_add_single_thread.cpp
// Author: Jacob Francis
// Created: May 2nd, 2019

#include <thread>
#include <iostream>
#include <vector>
#include <chrono> 
#include <algorithm>
using namespace std::chrono;
using namespace std;


void initWith (float num, vector<double>& a) {
  for (int i = 0; i < a.size(); ++i) {
    a[i] = num;
  }
}

void addVectors (vector<double>& result, vector<double>& a, vector<double>& b, 
                         int begin_index, int end_index) {
  for (int i = begin_index; i <= end_index; i++) {
    result[i] = a[i] + b[i];
  }   
}

void checkElements (double target, const vector<double>& a) {
  for(int i = 0; i < a.size(); i++)
  {
    if (a[i] != target)
    {
      cout << "FAIL: vector[" << i << "] = " << a[i] << " does not equal " << target << endl; 
     	exit(1);
    }
  }
  cout << "SUCCESS! ALL values added correctly" << endl;
}

double sum = 0;

int main() {
  // Number of elements in a vector is a power of 2^(x+1)
  const int N = 2 << 23;
  
  vector<double> a(N);
  vector<double> b(N);
  vector<double> c(N);

  initWith(3, a);
  initWith(4, b);
  initWith(0, c);
  
  
  // Spawn threads
  vector<thread> threads;
  int num_threads = 4;
  
  int thread_elements = N / num_threads;
  
  int start_index[num_threads];
  int end_index[num_threads];
  
  // Store the start and end indexes of threads for parallel execution
  for(int i = 0; i< num_threads; i++){
      start_index[i] = i*thread_elements;
      end_index[i] = start_index[i] + thread_elements - 1;
  }
  
  
  // start timer
  auto start = high_resolution_clock::now();
  
  
  /* ---- original computation w/o threads ----
  addVectors(c, a, b, 0, (c.size() - 1));
  */
  
  // ---- Computation with threads ----
  for(unsigned int i = 0; i<num_threads; i++){
	  threads.emplace_back(addVectors, ref(c), ref(a), ref(b), ref(start_index[i]), ref(end_index[i]));
  }
  for(unsigned int i = 0; i<num_threads; i++)
	  threads[i].join();
  
  // stop timer
  auto stop = high_resolution_clock::now();
  
  // calculate and print addVectors exec time 
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "Ran with " << num_threads << " threads." << endl;
  cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
  
  checkElements(7, c);

  return 0;
}
