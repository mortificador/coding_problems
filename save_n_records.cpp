/*
You run an e-commerce website and want to record the last N order ids in a log. Implement a data structure to accomplish this, with the following API:

    record(order_id): adds the order_id to the log
    get_last(i): gets the ith last element from the log. i is guaranteed to be smaller than or equal to N.
*/

/*
The main characteristic of the data structure is asked is that it wraps around: when we add the N + 1 element, we should evict the oldest element, and insert
the new element in it's place.
There's a data structure that behaves exactly in that way: the circular buffer.
In this case I've implemented a circular buffer that uses an array internally; that means that the size of the buffer must be known at compile time. If we didn't want this,
we could use a vector instead of an array.
*/

#include <array>
#include <cassert>

using namespace std;

template <size_t N>
struct circular_buffer {
public:
  circular_buffer() : next_element{0} {
    static_assert(N > 0, "The size of the buffer must be larger than 0");
  }

  void add(int element) {
    // If the next element is to be added past the end of the elements
    // we add it to the beginning
    if (next_element == N) {
      next_element = 0;
    }
    elements[next_element] = element;
    next_element++;
  }

  // We are assuming that, when this is called, something was already saved in the pos
  // N - idx. Otherwise, we would return the default element that the internal array
  // created there when the array was created.
  // idx must be larger than 0.
  int& get_last(int idx) {
    assert(idx <= N && idx > 0);
    return elements[N - idx];
  }

private:
  // next_elements points to the index where the next element should be inserted to.
  size_t next_element;
  // internal array where we save the elements that are inserted
  array<int, N> elements;
};

int main() {
  {
    circular_buffer<10> records;
    for (int i = 0; i < 10; i++) {
      records.add(i);
    }
    for (int i = 1; i < 11; i++) {
      assert(records.get_last(i) == 10 - i);
    }
  }
  {
    circular_buffer<1> records;
    records.add(1);
    records.add(2);
    records.add(3);
    assert(records.get_last(1) == 3);
  }
  {
    circular_buffer<2> records;
    records.add(1);
    records.add(2);
    records.add(3);
    assert(records.get_last(2) == 3);
    assert(records.get_last(1) == 2);
  }
}
