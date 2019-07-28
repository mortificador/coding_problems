/*
 You are given an array of integers in an arbitrary order. Return whether or not it is possible to make the array non-decreasing by modifying at most 1 element to any value.

We define an array is non-decreasing if array[i] <= array[i + 1] holds for every i (1 <= i < n).

Example:

[13, 4, 7] should return true, since we can modify 13 to any value 4 or less, to make it non-decreasing.
[13, 4, 1] however, should return false, since there is no way to modify just one element to make the array non-decreasing.

Can you find a solution in O(n) time?
*/

/*
To solve this problem we need to keep track of two values: the current difference between v[i] and v[i-1], and the difference between v[i-1] and v[i-2]. Why is so?
When we iterate the array and we see a number, v[i], that is smaller than v[i-1], we have two options:
  We change v[i] so it's the same value as v[i-1] (which is the minimum value that v[i] needs to have to keep the array non-decreasing)
  We change v[i-1] to make it smaller than v[i].

How do we decided which option to take?
The second option is better: if we modify v[i-1], we surely won't introduce any problem between v[i] and v[i+1]. If we change v[i], we might make it larger than v[i+1],
and therefore we would have introduced a problem.
The second option, however, presents a problem: if we change v[i-1] we need to change it to a value so that v[i-1] is still equal or larger than v[i-2], so we don't
break the non-decreasing rule. In order to do this, we need to keep track of the difference between v[i-1] and v[i-2]: we need to decrease v[i-1] by 
the difference between v[i-1] and v[i-2] (as that would make v[i-1] = v[i-2]). We can only do this is the difference between v[i] and v[i-1] is smaller or equal than
the difference between v[i-1] and v[i-2] (the reason is the following: we need to decrease v[i-1] by, at least, the difference between v[i] and v[i-1], so v[i] = v[i-1].
However, at the same time, we can't decrease v[i-1] more than the diff between v[i-1] and v[i-2], because if we did, v[i-1] would be smaller than v[i-2])
So, if we can, we should go with the second option, otherwise, take the first option.
If we make a change to the array, we store that we made the change, and keep going. If we have to make another change, we first change if we already made a change: if
we already made change, we return false, as we can't make the array non-decreasing making only one change to the array.

E.g:
[2, 3, 10, 2, 8, 10]

We start iterating from index = 1:
i=1
prev_diff = INT_MAX
diff = v[i] - v[i-1] = 1
diff >= 0 // v[i] >= v[i-1], so we continue
prev_diff = diff = 1

i=2
prev_diff = 1
diff = v[i] - v[i-1] = 7
diff >= 0 // we continue
prev_diff = diff = 7

i=3
prev_diff = 7
diff = v[i] - v[i-1] = 2 - 10 = -8
diff < 0
  // We can't take the second option: we would need to decrease v[i-1] by at least 8 units, but the max. we can decrease it by is prev_diff, which is 7
  // because prev_diff is what would make v[i-1] = v[i-2]. We need to change v[i] to have the value of v[i-1]
v[i] = v[i-1] = 10
already_modified = true
prev_diff = v[i] - v[i-1] = 0

The array now is: [2, 3, 10, 10, 8, 10]

i=4
prev_diff = 0
already_modified = true
diff = v[i] - v[i-1] = 8 - 10 = -2
diff < 0
// We need to modify the array again, but we already did it once. Return false
  


Let's see another example
[2, 3, 10, 4, 8, 10]

We start iterating from index = 1:
i=1
prev_diff = INT_MAX
diff = v[i] - v[i-1] = 1
diff >= 0 // v[i] >= v[i-1], so we continue
prev_diff = diff = 1

i=2
prev_diff = 1
diff = v[i] - v[i-1] = 7
diff >= 0 // we continue
prev_diff = diff = 7

i=3
prev_diff = 7
diff = v[i] - v[i-1] = 4 - 10 = -6
diff < 0
  // We can take the second option: we would need to decrease v[i-1] by at least 4 units, and the max. we can decrease it by is prev_diff, which is 7
v[i-1] = v[i-1] - abs(prev_diff) = 10 - 6 = 4
already_modified = true
prev_diff = v[i] - v[i-1] = 10 - 6 = 4

The array now is:
[2, 3, 4, 4, 8, 10]

i=4
prev_diff = 4
already_modified = true
diff = v[i] - v[i-1] = 8 - 4 = 4
diff >= 0 // We can continue
prev_diff = 4

i=5
prev_diff = 4
already_modified = true
diff = v[i] - v[i-1] = 10 - 4 = 4
diff >= 0 // We can continue

// End of array. return true
*/

#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>

using namespace std;

bool can_make_non_decreasing(vector<int> &v) {
  // We set the first previous diff to max. This diff will only be used in the case that the second number in the array is smaller than the first one.
  int prev_diff = numeric_limits<int>::max();  // prev_diff holds the diff between v[i-2] and v[i-1]
  bool already_modified = false;
  for (int i = 1; i < v.size(); i++) {
    int diff = (v[i] - v[i - 1]);
    // if v[i] is smaller than v[i-1]
    if (diff < 0) {
      // If we already made a modification to the array, return
      if (already_modified == true) {
        return false;
      }
      // if diff between v[i] and v[i-1] (diff) is larger than the diff between v[i-1] and v[i-2] (prev_diff), we can't change v[i-1]: we would need to
      // decrease it by at least diff, but because diff is larger than prev_diff, this would mean that v[i-2] would become larger than v[i-1].
      if (abs(diff) > prev_diff) {
        v[i] = v[i - 1];
      }
      // if we have enough margin to change v[i-1], we change it to the minimum value that guarantees that it's a non-decreasing array
      else {
        v[i - 1] = (v[i - 1] - abs(prev_diff));
      }
      already_modified = true;
    }
    prev_diff = (v[i] - v[i - 1]);
  }
  return true;
}

int main() {
  {
    vector<int> v{13, 4, 7};
    assert(can_make_non_decreasing(v) == true);
  }
  {
    vector<int> v{13, 4, 1};
    assert(can_make_non_decreasing(v) == false);
  }
  {
    vector<int> v{4, 2, 3};
    assert(can_make_non_decreasing(v) == true);
  }
  {
    vector<int> v{1, 2, 3, 10, 2, 8, 9, 10};
    assert(can_make_non_decreasing(v) == false);
  }
  {
    vector<int> v{1, 2, 3, 10, 4, 8, 9, 10};
    assert(can_make_non_decreasing(v) == true);
  }
}
