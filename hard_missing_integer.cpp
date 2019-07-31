/*
Given an array of integers, find the first missing positive integer in linear time and constant space. In other words, find the lowest positive integer that does not exist in the array. The array can contain duplicates and negative numbers as well.

For example, the input [3, 4, -1, 1] should give 2. The input [1, 2, 0] should give 3.
[3, 4, -2, 1]

You can modify the input array in-place.
*/

/*
The key to solve this problem in linear time and constant space is to realize that we can use the index of the array to save information.
We need to somehow mark when we see an element (if we see the number 3, somehow we need to mark that we have seen it). To accomplish this, we will mark
the presence of an element x by changing the value at the index x to a special value, -1. So, to mark that 1 has been seen, we will mark v[1] (or,
because in this problem the last non-positive integer that could be missing is equal to the size of the array, we would mark v[0])
This is not enough, as -1 could appear in the original array. To solve this problem, we first process the array, changing
 the value of all non-positive integers (including 0) to another special value, -2.
After processing the array to change the non-positive integers, and then processing the array again to mark all the seen values, we can find
the first missing integer by searching for the first -2 in the array, and the index of that element is the missing integer.

The full algorithm would be the following (keep in mind that based on the problem, the first missing integer possible is 1, so,
to mark that 1 is missing, we will mark the index 0, to mark that 2 is missing, we will mark the index 1, and so on):
 1 Process the original array, changing all values less-or-equal than zero to -2

 2 Process the modified array:
 3  for index i starting at 0:
 4    if the value of the element at index i is, input[i] > 0:
       Save the index pointed by the current element: idx = v[i] - 1
       swap(v[idx], v[i]). By doing this we make sure that we will process the value at index v[v[i] - 1]
       mark v[idx] as seen, setting the value to -1: v[idx] = -1
 5   else
 6     increase i, i++

 7 Find the first -2 in the array, and return the index (plus 1). That's the missing integer.

E.g.:
[2, 1, 0]

We first process the array and change the elements <= 0 to the special value -2:
[2, 1, -2]

Now we start processing: 
 index = 0
 value (at index) = 2
 
 We swap v[value - 1] with v[index], v[1] with v[0]. This is to make sure that we will still process whatever number is at v[1] 
 [1, 2, -2]
 We mark v[value-1] as seen:
 [1, -1, -2]
 
 We keep processing: we can't increase the index, as the new value at index 0 is not negative:
 index = 0
 value = 1
 Again, we swap v[value - 1] with v[index]. In this case it stays the same, as we swap v[0] with v[0]
 [-1, -1, -2]
 Mark v[value - 1] as seen:
 [-1, -1, -2]
  
 Now we increase the index, until we find a positive integer or the array ends.
 There are no more positive integers.
 
 Now we find the first -2, and return the index (plus 1). In this case the first -2 is at the index 2, so we return 3.
*/

#include <algorithm>
#include <cassert>
#include <vector>

using namespace std;

int find_missing(vector<int> v) {
  // Substitute all numbers less than 0 by the special value -2
  transform(begin(v), end(v), begin(v), [](auto &v) { v = v <= 0 ? -2 : v; return v; });

  for (int i = 0; i < v.size();) {
    if (v[i] > 0) {
      auto idx = v[i] - 1;
      swap(v[idx], v[i]);
      v[idx] = -1;
    } else {
      ++i;
    }
  }

  auto missing = find(begin(v), end(v), -2);
  return distance(begin(v), missing) + 1;
}

int main() {
  {
    vector<int> v{3, 4, -1, 1};
    assert(find_missing(v) == 2);
  }
  {
    vector<int> v{1, 2, 0};
    assert(find_missing(v) == 3);
  }
  {
    vector<int> v{-1, -1, -1};
    assert(find_missing(v) == 1);
  }
  {
    vector<int> v{0, 8, 2, 2, 4, 7, 5, -2};
    assert(find_missing(v) == 1);
  }
}
