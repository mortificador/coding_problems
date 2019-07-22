/*
Implement a job scheduler which takes in a function f and an integer n, and calls f after n milliseconds.
*/

/*
This question requires to use std::threads. Given that we only need to run a function after n milliseconds, our scheduler can be class with
only one method: this method creates a thread that will sleep for the given time, and then execute the function. We save the thread in an internal
vector, and in the dtor of the scheduler, we wait for all threads to finish. It's important to make the scheduler non-copyable: if it wasn't and
it was copied, we would end up calling join() on the same thread multiple times, and that would throw an exception (in a dtor!).
To compile this exercise using g++ we need to add -lpthread to the compile command line.
*/

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;
using namespace std::chrono_literals;

class scheduler {
public:
  scheduler() = default;
  scheduler(const scheduler&) = delete;
  scheduler& operator=(const scheduler&) = delete;

  template <typename F>
  void schedule(F f, chrono::seconds delay) {
    thread t([delay, f]() {
      this_thread::sleep_for(chrono::milliseconds(delay));
      f();
    });

    _threads.emplace_back(move(t));
  }

  ~scheduler() {
    for (auto& t : _threads) {
      t.join();
    }
  }

private:
  vector<thread> _threads;
};

int main() {
  scheduler sc;
  sc.schedule([]() { cout << "First call after 1 second" << endl; }, 1s);
  sc.schedule([]() { cout << "Second call after 2 seconds" << endl; }, 2s);
  sc.schedule([]() { cout << "Third call after 3 seconds" << endl; }, 3s);

  return 0;
}
