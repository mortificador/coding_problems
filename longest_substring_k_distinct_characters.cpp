/*
Given an integer k and a string s, find the length of the longest substring that contains at most k distinct characters.

For example, given s = "abcba" and k = 2, the longest substring with k distinct characters is "bcb".
*/

/*
To solve this problem we can use a counter: we start iterating through the string, and we save in a map how many times we have seen the current character so far. This
map has as many keys as the number of different characters in the current substring. We keep the length of the current substring in a variable.
Once we have seen more than k characters, we know that the current substring contains too many different characters, so we start eliminating characters, starting from the
beginning of the string. Every time we eliminate one character, we decrease the variable that keeps track of the length of the current substring.
Every time we eliminate a character, we decrease the counter for that character by one. If the count reaches zero, we eliminate the character from the map.
Once we have eliminated enough characters so the map contains now k characters, we continue, and if at some point we again have more than k characters in the counter, 
we keep eliminating them, starting from we left last time.

E.g.
input = abcba
k = 2
counter = {}
eliminate_index = 0
max_length = 0
curr_length = 0

Iterate through the string
letter = a
counter = {a:1}
curr_length = 1
max_length = max(max_length, curr_length) = 1

letter = b
counter = {a:1, b:1}
curr_length = 2
max_length = max(max_length, curr_length) = 2


letter = c
counter = {a:1, b:1, c:1}
curr_length = 3

counter contains more than k elements, so we start eliminating:
  eliminate_index = 0
  counter[input[eliminate_index]]--
  counter: {a:0, b:1, c:1}
  curr_length--
  eliminate_index++
 
  Eliminate all the elements with count zero from the map
  counter: {b:1, c:1}
 
  Now counter contains k elements we can continue
max_length = max(max_length, curr_length) = 2


letter = b
counter = {b:2, c:1}
curr_length = 3
max_length = max(max_length, curr_length) = 3

letter = a
counter = {a:1, b:2, c:1}
curr_length = 4
counter contains more than k elements, so we start eliminating:
  eliminate_index = 1
  counter[input[eliminate_index]]-- 
  counter: {a:1, b:1, c:1}
  curr_length--
  eliminate_index++

  Eliminate all the elements with count zero from the map
  counter: {a:1, b:1, c:1}

  Counter still contains more than k elements, repeat
  eliminate_index = 2
  counter[input[eliminate_index]]--
  counter: {a:1, b:1, c:0}
  curr_length--
  eliminate_index++

  Eliminate all the elements with count zero from the map
  counter: {a:1, b:1}

  Now counter contains k elements, we can continue
  max_length = max(max_length, curr_length) = 3

We have finished iterating through the string, return max_length, which is 3.
*/

#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

int find_longest(string const& s, int k) {
  unordered_map<char, int> counter;
  int left_idx = 0;
  int longest = 0;
  int curr_length = 0;
  for (int i = 0; i < s.size(); i++) {
    // Increase the number of times that we have seen this letter
    counter[s[i]]++;
    curr_length++;
    // If we have seen more than k different characters in the current substring
    // we start eliminating characters from the left side of the string, until
    // we have seen at most k different characters
    while (counter.size() > k) {
      counter[s[left_idx]]--;
      // If, by eliminating the character, we have seen the character zero times,
      // we eliminate it from the map
      if (counter[s[left_idx]] <= 0) {
        counter.erase(s[left_idx]);
      }
      left_idx++;
      curr_length--;
    }
    longest = max(curr_length, longest);
  }

  return longest;
}

int main() {
  assert(find_longest("abcba", 2) == 3);
  assert(find_longest("awcbaakaaaulaa", 3) == 7);
  assert(find_longest("wwwwxwvvttttttyyyyyyyytttttaaawwwwt", 2) == 19);
  assert(find_longest("aaabbb", 2) == 6);
}


