/*
Given a list of integers, write a function that returns the largest sum of non-adjacent numbers. Numbers can be 0 or negative.

For example, [2, 4, 6, 2, 5] should return 13, since we pick 2, 6, and 5. [5, 1, 1, 5] should return 10, since we pick 5 and 5.

Follow-up: Can you do this in O(N) time and constant space?
*/

/*
The key to solve this problem is to solve the following question: how can I calculate the maximum sum at the ith element?
In the ith element, the maximum sum will be one of the following (let's call the input array arr):
1  the maximum sum at the element ith - 2 (because the element ith - 1 cannot appear in the sum, as it's adjacent) + arr[i]
2  the maximum sum at the element ith - 2. This will be the case if arr[i] is negative.
3  the maximum sum at the element ith - 1 (we can't include arr[i], as we can't sum adjacent elements).

This means that we need then two track two numbers: the maximum sum so far, including the ith element (which corresponds to 1 and 2), and the maximum so far, 
not including the current element (which corresponds with 3).
Now that we know what we need, we have to be careful on how we update those numbers:
When we process a new element, the maximum possible sum, including the new element, is either: 
    the maximum possible sum that excluded the element before it plus the new element (case 1, this is the max sum at element (ith - 2) + arr[i])
    the maximum possible sum that excluded the element before it (for example, if the new element is negative, this will be the case) (case 2)
Also, when we process a new element, the maximum possible sum, excluding the new element, is either:
    the previous maximum sum that included the previous element
    the previous maximum sum that excluded the previous element

We initialise the maximum possible sum, including the new element, to the first element of the array, and the maximum sum excluding the new element,
to zero. Initializing maximum sum excluding the new element to zero means that, in the case that all the elements in the array are negative, we will return 0.

We return the largest number: either the maximum sum so far, including the last element, or the maximum so far, excluding the last element.

E.g.
input = [5, 1, 1, 5]
We call the maximum, including the element, maxInclude, and the maximum excluding the element, maxExcluding.
First, we initialise maxInclude to input[0], and maxExclude to zero.
maxIncluding = 5
maxExcluding = 0

We start processing from the second element:
element = 1
previousMaxIncluding = maxIncluding = 5
maxIncluding = max(maxExcluding + element, previousMaxIncluding) = max(0 + 1, 5) = 5
maxExcluding = max(maxExcluding, previousMaxIncluding) = max(0, 5) = 5

Move on:
element = 1
previousMaxIncluding = maxIncluding = 5
maxIncluding = max(maxExcluding + element, previousMaxIncluding) = max(5 + 1, 5) = 6
maxExcluding = max(maxExcluding, previousMaxIncluding) = max(5, 5) = 5

Move on:
element = 5
previousMaxIncluding = maxIncluding = 6
maxIncluding = max(maxExcluding + element, previousMaxIncluding) = max(5 + 5, 5) = 10
maxExcluding = max(maxExcluding, previousMaxIncluding) = max(5, 6) = 6

We've finished processing, so we return the max:
return max(maxExcluding, maxIncluding) = max(6, 10) = 10


*/

#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;

int max_sum(vector<int> v) {
  if (v.size() < 3) {
    return *max_element(begin(v), end(v));
  }

  int maxIncluding = v[0];
  int maxExcluding = 0;

  for (int i = 1; i < v.size(); i++) {
    int prevMaxIncluding = maxIncluding;
    maxIncluding = max(maxExcluding + v[i], maxIncluding);
    maxExcluding = max(prevMaxIncluding, maxExcluding);
  }

  return max(maxIncluding, maxExcluding);
}

int main() {
  {
    vector<int> v{2, 4, 6, 2, 5};
    assert(max_sum(v) == 13);
  }
  {
    vector<int> v{5, 1, 1, 5};
    assert(max_sum(v) == 10);
  }
  {
    vector<int> v{-5, -1, -1, -5, -1};
    assert(max_sum(v) == -1);
  }
  {
    vector<int> v{5, -1, 1, -5, 9};
    assert(max_sum(v) == 15);
  }
  {
    vector<int> v{1, 14, 5, -9, 1};
    assert(max_sum(v) == 15);
  }
  {
    vector<int> v{1, 4, 5, -8, -10, 1, 10};
    assert(max_sum(v) == 16);
  }
  {
    vector<int> v{4, 4, 10, 8, -10, 1, 10};
    assert(max_sum(v) == 24);
  }
  {
    vector<int> v{3, 5, -7, 8, 10};
    assert(max_sum(v) == 15);
  }
}
