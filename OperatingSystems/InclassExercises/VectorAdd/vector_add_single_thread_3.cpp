// File: vector_add_single_thread.cpp
// compile command: g++ -std=c++17 -pthread -o vector_add_single_thread_3 vector_add_single_thread_3.cpp
// Author: Jacob Francis
// Created: May 2nd, 2019


// Include Statements
#include <thread>
#include <iostream>
#include <vector>
#include <chrono> 
#include <algorithm>
#include <mutex>
#include <condition_variable>
using namespace std::chrono;
using namespace std;

// Global Variable Declarations
double global_sum = 0;
int thread_num = 0;
double average = 0;
int max_num_threads = thread::hardware_concurrency();
mutex mtx;
mutex mtx2;
condition_variable cv;



void initWith (float num, vector<double>& a) {
  for (int i = 0; i < a.size(); ++i) {
    a[i] = num;
  }
}

void addVectors (vector<double>& result, vector<double>& a, vector<double>& b, 
                         int begin_index, int end_index) {

  double sum = 0;
  for (int i = begin_index; i <= end_index; i++) {
    result[i] = a[i] + b[i];
    sum += result[i];
  }   
  double avg = sum / result.size();
  for (int i = begin_index; i <= end_index; i++) {
    result[i] += avg;
  }
}

void multiThreadSum (vector<double>& result, vector<double>& a, vector<double>& b, 
                         int begin_index, int end_index) {
							 
  double local_sum  = 0;	// The sum variable for each individual thread
  
  // Stores the addition of the a and b vectors and sums the individual members and stores it in local_sum
  for (int i = begin_index; i <= end_index; i++) {
    result[i] = a[i] + b[i];
    local_sum += result[i];
  }
  
  // Critical section of thread: adds the thread local_sum to global_sum
  {
     lock_guard<mutex> lck(mtx);
     global_sum += local_sum;
     thread_num++;
  }
  
  // Last thread computes average and notifies other threads when it is done executing
  if(thread_num == max_num_threads) {
		average = global_sum/result.size();
		cv.notify_all();
  } 
  
  // Threads that finish before the last thread wait until it is done with computation
  else {
	  unique_lock<mutex> lck_l1(mtx2);
	  while(thread_num != max_num_threads) cv.wait(lck_l1);
  }

  // Adds average value to result vector
  for (int i = begin_index; i <= end_index; i++) {
    result[i] += average;
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


int main() {
  // Number of elements in a vector is a power of 2^(x+1)
  const int N = 2 << 23;
  
  vector<double> a(N);
  vector<double> b(N);
  vector<double> c(N);	// single thread vector
  vector<double> d(N);	// multithread vector

  initWith(3, a);
  initWith(4, b);
  initWith(0, c);
  initWith(0, d);
  
  /*************************************
   * 
   * SINGLE THREAD CODE
   * 
   *************************************/
  /* ---- original computation w/o threads ----*/
  int num_threads = 1;
  auto start = high_resolution_clock::now();	// start timer
  
  addVectors(c, a, b, 0, (c.size() - 1));
  
  auto stop = high_resolution_clock::now();		// stop timer
  
  auto singleThreadDuration = duration_cast<microseconds>(stop - start);
  cout << "Ran with " << num_threads << " threads." << endl;
  cout << "Time taken by function: " << singleThreadDuration.count() << " microseconds" << endl;
  
  checkElements(14, c);
  
  /*************************************
   * 
   * MULTITHREAD CODE
   * 
   *************************************/
  initWith(3, a);
  initWith(4, b);
  initWith(0, c);
  
  // Spawn threads
  vector<thread> threads;
  num_threads = 4;
  
  int thread_elements = N / num_threads;
  
  int start_index[num_threads];
  int end_index[num_threads];
  
  // Store the start and end indexes of threads for parallel execution
  for(int i = 0; i< num_threads; i++){
      start_index[i] = i*thread_elements;
      end_index[i] = start_index[i] + thread_elements - 1;
  }
  
  
  // start timer
  auto start2 = high_resolution_clock::now();
  
  // ---- Computation with threads ----
  for(unsigned int i = 0; i<num_threads; i++){
	  threads.emplace_back(multiThreadSum, ref(d), ref(a), ref(b), ref(start_index[i]), ref(end_index[i]));
  }
  for(auto& entry:threads){
	  entry.join();  
  }
  
  // stop timer
  auto stop2 = high_resolution_clock::now();
  
  // calculate and print addVectors exec time 
  auto multiThreadDuration = duration_cast<microseconds>(stop2 - start2);
  cout << "Ran with " << num_threads << " threads." << endl;
  cout << "Time taken by function: " << multiThreadDuration.count() << " microseconds" << endl;
  cout << "Total Speedup: " << (double)singleThreadDuration.count()/multiThreadDuration.count() << endl;
  
  checkElements(14, d);

  return 0;
}
