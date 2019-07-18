/*
Given a list of numbers and a number k, return whether any two numbers from the list add up to k.

For example, given [10, 15, 3, 7] and k of 17, return true since 10 + 7 is 17.
*/

/*
Given the number k, what we need to do for every element 'i' is check if the number k - i is present in the array.
We can do that with a set: for every element 'i', we check if the number k - i is present in the set. If it's not,
we add the element 'i' to the set, and repeat the process with the next element of the array. If it's present, we finish.

E.g.:
k = 17
[10, 15, 3, 7]

The set is empty when we start:
set = {}

Now we take the first element in the array, which is 10:
set = {}
element = 10
is k - element => 17 - 10 => 7 in the set? is not, so we add 10 to the set and continue

set = {10}
element = 15
is k - element => 17 - 15 => 2 in the set? is not, so we add 15 to the set and continue

set = {10, 15}
element = 3
is k - element => 17 - 3 => 14 in the set? is not, so we add 3 to the set and continue

set = {10, 15, 3}
element = 7
is k - element => 17 - 7 => 10 in the set? it is, so we finish
*/

#include <cassert>
#include <unordered_set>
#include <vector>

using namespace std;

auto add_up_to(vector<int> v, int k) -> bool {
  unordered_set<int> s;
  for (auto element : v) {
    if (s.count(k - element) > 0) {
      return true;
    }
    s.insert(element);
  }
  return false;
}

int main() {
  assert(add_up_to(vector<int>{10, 15, 3, 7}, 17) == true);
  assert(add_up_to(vector<int>{10, 15, 3, 7}, 16) == false);
  assert(add_up_to(vector<int>{-1, 9, 7, -3, 4}, -4) == true);

  return 0;
}
