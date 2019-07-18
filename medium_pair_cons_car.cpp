/*
cons(a, b) constructs a pair, and car(pair) and cdr(pair) returns the first and last element of that pair. For example, car(cons(3, 4)) returns 3, and cdr(cons(3, 4)) returns 4.

Given this implementation of cons:

def cons(a, b):
    def pair(f):
        return f(a, b)
    return pair

Implement car and cdr.
*/

/*
The key to solve this problem is to notice that cons returns a function. That function takes another function as argument, and calls it with the values
of the pair as arguments.
We just need to create a couple of functions (one for car and other for cdr) that take another function as argument, and calls it, 
passing as argument another function that takes two integers, and return either the first (car) or the second (cdr) argument.
*/

#include <cassert>

using namespace std;

template <typename T>
auto cons(T a, T b) {
  return [a, b](auto f) -> T { return f(a, b); };
}

template <typename F>
auto car(F f) {
  auto first = [](auto a, auto b) { return a; };

  return f(first);
}

template <typename F>
auto cdr(F f) {
  auto second = [](auto a, auto b) { return b; };

  return f(second);
}

int main() {
  assert(car(cons(3, 4)) == 3);
  assert(cdr(cons(3, 4)) == 4);

  return 0;
}
