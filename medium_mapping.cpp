/*
Given the mapping a = 1, b = 2, ... z = 26, and an encoded message, count the
number of ways it can be decoded. For example, the message '111' would give 3,
since it could be decoded as 'aaa', 'ka', and 'ak'. You can assume that the
messages are decodable. For example, '001' is not allowed.
*/


/*
The max. number that represents a letter is 26, so when we consider possible encodings, we will have to look at two numbers
at the same time maximum (as, for example, 100, is not a possible mapping).
To count the number of possible encodings we look at two numbers at a time, and see if they could be a letter (i.e if the 
number formed by the two digits is less than 26), if they can be a letter, we increase the number of possible encodings.
Then we move the window by one index to the right, and repeat.
The number of possible encodings starts as 1, as the full vector, where each element represents a letter, is a possible encoding.
E.g.:
counter = 1;
The window contain the numbers 1 and 2. 12 represents a letter, so we increase the counter.
 |--| 
[1, 2, 3, 1, 9]

counter = 2

Now we move the window and repeat the process. 23 represents a letter, so we increase the counter;
    |--| 
[1, 2, 3, 1, 9]

counter = 3

We move the window again and repeat. 31 does not represent a letter:
       |--| 
[1, 2, 3, 1, 9]

counter = 3;


And again. 19 represents a letter:
          |--| 
[1, 2, 3, 1, 9]

counter = 4

*/


#include <iostream>
#include <vector>
#include <cassert>

using namespace std;


int count_poss(vector<int> code) {
  int counter = 1;
  for (auto it = std::begin(code); it != std::end(code) - 1; ++it) {
    if (((*it * 10) + *(it + 1)) <= 26) {
      ++counter;
    }
  }
  return counter;
}

int main() {
  assert(count_poss(std::vector<int>{1, 2, 1, 8}) == 4);
  assert(count_poss(std::vector<int>{1,2,3,1,9}) == 4);
  assert(count_poss(std::vector<int>{1,1,1}) == 3);

  return 0;
}
