/*
Implement a job scheduler which takes in a function f and an integer n, and calls f after n milliseconds.
*/

/*
This question requires to use std::threads. Given that we only need to run a function after n milliseconds, we can have a
function that creates a thread. This thread will sleep for the given time, and then execute the function.
To compile this exercise using g++ we need to add -lpthread to the compile command line.
*/

#include <chrono>
#include <iostream>
#include <thread>

using namespace std;
using namespace std::chrono_literals;

template <typename F>
thread schedule(F f, chrono::seconds delay) {
  thread t([delay, f]() {
    this_thread::sleep_for(chrono::milliseconds(delay));
    f();
  });

  return t;
}

int main() {
  auto t1 = schedule([]() { cout << "First call after 1 second" << endl; }, 1s);
  auto t2 = schedule([]() { cout << "Second call after 2 seconds" << endl; }, 2s);
  auto t3 = schedule([]() { cout << "Third call after 3 seconds" << endl; }, 3s);

  t1.join();
  t2.join();
  t3.join();

  return 0;
}
