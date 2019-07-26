/*
There exists a staircase with N steps, and you can climb up either 1 or 2 steps at a time. Given N, write a function that returns the number of unique ways you can climb the staircase. The order of the steps matters.

For example, if N is 4, then there are 5 unique ways:

    1, 1, 1, 1
    2, 1, 1
    1, 2, 1
    1, 1, 2
    2, 2

What if, instead of being able to climb 1 or 2 steps at a time, you could climb any number from a set of positive integers X? For example, if X = {1, 3, 5}, you could climb 1, 3, or 5 steps at a time.
*/

/*
Let's first solve it for the case of climbing up either 1 or 2 steps.
We can use recursion to solve the problem. Let's say we have 4 steps to climb up, what can we do? well, we can either:
  we can climb up 1 step, and now we need to count the number of ways of climbing up N -1 -> 3 steps
  we can climb up 2 steps, and now we need to count the number of ways of climbing up N - 2 -> 1 step
  
But now, we have the problem of counting how many ways we have to climb up 3 steps, and 1 step. We can use the same formula:
For 3 steps:
  we can climb up 1 step, and now we need to count the number of ways of climbing up 3 -1 -> 2 steps
  we can climb up 2 steps, and now we need to count the number of ways of climbing up 2 - 2 -> 0 steps
For 1 step:
  we can climb up 1 step, and now we need to count the number of ways of climbing up 1 - 1 -> 0 steps
  we can climb up 2 steps, and now we need to count the number of ways of climbing up 1 - 2 -> -1 step

Two important things happened: when we find that there are 0 steps left, that means that this is a valid way to climb up the stairs, so we can return 1,
meaning that this is a valid way to climb up the stairs.
When we find that there are less than 0 steps left, it means that this is not a valid way, as we climbed past the limit. We can return 0 in this case,
meaning that it's not a valid way to climb up the stairs.

We can add some memoization, so we don't need to calculate the same number several times: for example, once we have found how many ways there are to climb up
2 steps, we can save it in a map, so the next time we need to calculate the number of ways to climb up two steps, we don't need to calculate it.

The solution for a given set of possible steps is very similar: now, instead of calculating the number of ways  climb up N -1 and N - 2 steps, we need to
calculate the number of ways to climb up N - a1, N - a2, N - a3, ... steps. 
E.g
For 3 steps, and the set of possible steps is {1, 3, 5}:
  we can climb up 1 step, and now we need to count the number of ways of climbing up 3 - 1 -> 2 steps
  we can climb up 3 steps, and now we need to count the number of ways of climbing up 3 - 3 -> 0 steps
  we can climb up 5 steps, and now we need to count the number of ways of climbing up 3 - 5 -> -2 steps

And now repeat the same, but for 2 steps.
*/

#include <unordered_map>
#include <vector>
#include <cassert>

using namespace std;

int count_ways_to_climb(int stair_steps,
                  vector<int> const& possible_steps,
                  unordered_map<int, int>& ways_seen) {
  int n_ways = 0;
  
  // We have found a valid way to climb up, as there are exactly 0 steps left.
  if (stair_steps == 0) {
    return 1;
  }

  // This way is not valid, as we end up climbing past the stair
  if (stair_steps < 0) {
    return 0;
  }

  // Count all the possible ways to go forward from this point
  if (ways_seen.count(stair_steps) == 0) {
    for (auto& steps : possible_steps) {
      n_ways += count_ways_to_climb(stair_steps - steps, possible_steps, ways_seen);
    }
    ways_seen[stair_steps] = n_ways;
  }

  return ways_seen[stair_steps];
}

int main() {
  {
    unordered_map<int, int> m;
    vector<int> v{1, 2};
    assert(5 == count_ways_to_climb(4, v, m));
  }
  {
    unordered_map<int, int> m;
    vector<int> v{1, 2};
    assert(3 == count_ways_to_climb(3, v, m));
  }
  {
    unordered_map<int, int> m;
    vector<int> v{1, 3, 5};
    assert(5 == count_ways_to_climb(5, v, m));
  }
}
