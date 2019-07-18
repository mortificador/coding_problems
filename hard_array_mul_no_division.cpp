/*
Given an array of integers, return a new array such that each element at index i of the new array is the product of all the numbers in the original array except the one at i.

For example, if our input was [1, 2, 3, 4, 5], the expected output would be [120, 60, 40, 30, 24]. If our input was [3, 2, 1], the expected output would be [2, 3, 6].

Follow-up: what if you can't use division?
*/


/*
For the solution without using division we need to understand that, one of the ways to get the multiplication of all the numbers in the array
except for the one at the element i (we are going to call this operation product_arr), is to multiply the result of multiplying all the elements 
to the left of i by all the elements to the righ of i.
With this insight, what we can do is to create two arrays: one, called mul_left, will hold the multiplication of all elements, 
starting from the left, until the index i, and the other array, mul_right, will hold the multiplication of all numbers, 
starting from the right, until the index size - i - 1.
This means that, given an input "arr":
  mul_left[i] = arr[0] * arr[1] * ... * arr[i-1]
  mul_right[i] = arr[size() - 1] * arr[size() - 2] * ... * arr[size() - i - 1]
And we can get the value of the product of all the numbers in arr except the one at element i by doing: 
  mul_left[i-1] * mul_right[i+1], which, if we expand, is: (arr[0] * .. * arr[i-1]) * (arr[size() - i] * ... * arr[size() - 1] 
There are two special cases: 
  When i = 0 (the first element), the expected value is: arr[1] * arr[2] * ... * arr[size() - 1]. This is exactly mul_right[1]
  When i = size() - 1 (the last element), the expected value is: arr[0] * arr[1] * ... * arr[size() - 2]. This is exactly mul_left[size() - 2]

E.g.:
[1, 2, 3]

First let's create mul_left:
mul_left[0] = arr[0] = 1
mul_left[1] = arr[0] * arr[1] = mul_left[0] * arr[1] = 1 * 2 = 2
mul_left[2] = arr[0] * arr[1] * arr[2] = mul_left[1] * arr[2] = 2 * 3 = 6

Now with mul_right:
size() = 3
mul_right[size() - 1] = mul_right[3 - 1] = mul_right[2] = arr[2] = arr[2] = 3
mul_right[size() - 2] = mul_right[3 - 2] = mul_right[1] = arr[2] * arr[1] = mul_right[2] * arr[1] = 3 * 2 = 6
mul_right[size() - 3] = mul_right[3 - 3] = mul_right[0] = arr[2] * arr[1] * arr[0] = mul_right[1] * arr[0] = 6 * 1 = 6

So, finally, we can populate the result vector:
First special case, i = 0
result[0] = arr[1] * arr[2]  = mul_right[1] = 6

Non special case, use general expression: mul_left[i - 1] * mul_right[i + 1]
result[i] = mul_left[i-1] * mul_right[i+1] => i equals to 1 =>  mul_left[0] * mul_right[2] = arr[0] * arr[2] = 1 * 3 = 3

Second special case, i = size()-1
result[size() - 1] = arr[0] * arr[1] = mul_left[size() - 2] = mul_left[1] = 2

*/

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

using namespace std;

template <template <typename...> typename container, typename t>
void print(container<t> v) {
  std::copy(std::cbegin(v), std::cend(v), std::ostream_iterator<t>(std::cout, " "));
}

auto product_arr(vector<int> input) -> vector<int> {
  if (input.size() < 3) {
    return input;
  }

  const auto size = input.size();

  vector<int> mul_right;
  vector<int> mul_left;

  mul_left.resize(size);
  mul_left.front() = input.front();

  mul_right.resize(size);
  mul_right.back() = input.back();

  partial_sum(std::begin(input), std::end(input), std::begin(mul_left), multiplies<int>());
  partial_sum(std::rbegin(input), std::rend(input), std::rbegin(mul_right), multiplies<int>());

  vector<int> result;
  result.reserve(size);

  // Special case 1
  result.push_back(mul_right[1]);
  // General case
  for (int i = 1; i < size - 1; i++) {
    result.push_back(mul_left[i - 1] * mul_right[i + 1]);
  }
  // Special case 2
  result.push_back(mul_left[size - 2]);

  return result;
}

int main() {
  {
    vector<int> r{120, 60, 40, 30, 24};
    assert(product_arr(vector<int>{1, 2, 3, 4, 5}) == r);
  }
  {
    vector<int> r{2, 3, 6};
    assert(product_arr(vector<int>{3, 2, 1}) == r);
  }
  {
    vector<int> r{4, 5};
    assert(product_arr(vector<int>{4, 5}) == r);
  }
  return 0;
}


