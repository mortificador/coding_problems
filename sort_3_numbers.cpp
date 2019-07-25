/*
 Given a list of numbers with only 3 unique numbers (1, 2, 3), sort the list in O(n) time.

Example 1:

Input: [3, 3, 2, 1, 3, 2, 1]
Output: [1, 1, 2, 2, 3, 3, 3]

Challenge: Try sorting the list using constant space.
*/

/*
We solve this in four different steps:
 In the first step, we go through the array and find the three unique numbers. At this point, they are not sorted, we only want to have the three unique 
 numbers
 In the second step we sort the unique numbers, so lowest will contain the lowest number, medium the medium number, and largest the highest number
 In the third step we go through the array, and count how many times each of the numbers appear. We call this: lowest_times and medium_times (we dont need
 largest_times)
 Finally, we go through the array once more: we know how many times the smallest element appears in the array, so we set the first lowest_times elements
 in the array to the lowest value, then we set the next medium_times elements to the medium value, and finally we set the rest of the elements in the array
 to largest

E.g:
 [3,3,2,1,3,1,1]

 1- find the unique elements: lowest = 3, medium = 2, largest = 1
 2- sort them: lowest = 1, medium = 2, largest = 3
 3- count the number of times that each of the values appear: lowest_times = 3, medium_times = 1
 4- Set the first lowest_times elements of the array to lowest, then the next medium_times elements of the array to medium, and the rest to large:
    [1, 1, 1,   2,     3, 3, 3]
     |_____|   |_|     |_____|
     lowest   medium    largest
     
*/

#include <vector>
#include <cassert>

using namespace std;

void sort_3(vector<int>& v) {
  int lowest{v[0]}, medium{v[0]}, largest{v[0]};

  // Go through the array and find the three different numbers.
  for (auto& n : v) {
    if (n != lowest) {
      if (medium == lowest) {
        medium = n;
      } else if (medium != n) {
        largest = n;
      }
    }
  }
  
  // Now we have the unique numbers, but they are not sorted. We sort them here
  if (lowest > medium) {
    swap(lowest, medium);
  }
  if (lowest > largest) {
    swap(lowest, largest);
  }
  if (medium > largest) {
    swap(medium, largest);
  }

  // Go through the array and count the number of times that we've seen each number.
  int lowest_times{0}, medium_times{0};
  for (auto& n : v) {
    if (n == lowest) {
      lowest_times++;
    } else if (n == medium) {
      medium_times++;
    }
  }

  // Finally, change the elements and put them in the right order. First, the lowest, then the medium and finally the largest
  for (auto& n : v) {
    if (lowest_times != 0) {
      n = lowest;
      lowest_times--;
    } else if (medium_times != 0) {
      n = medium;
      medium_times--;
    } else {
      n = largest;
    }
  }
}

int main() {
  {
    vector<int> v{1, 2, 2, 1, 1, 3, 2, 3};
    vector<int> expected{1, 1, 1, 2, 2, 2, 3, 3};
    sort_3(v);
    assert(v == expected);
  }
  {
    vector<int> v{3, 3, 2, 1, 3, 2, 1};
    vector<int> expected{1, 1, 2, 2, 3, 3, 3};
    sort_3(v);
    assert(v == expected);
  }
  {
    vector<int> v{2, 5, 1};
    vector<int> expected{1, 2, 5};
    sort_3(v);
    assert(v == expected);
  }
  {
    vector<int> v{1, 2, 4};
    vector<int> expected{1, 2, 4};
    sort_3(v);
    assert(v == expected);
  }
  {
    vector<int> v{4, 2, 1};
    vector<int> expected{1, 2, 4};
    sort_3(v);
    assert(v == expected);
  }
}
