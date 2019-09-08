/*
Given an array of time intervals (start, end) for classroom lectures (possibly overlapping), find the minimum number of rooms required.

For example, given [(30, 75), (0, 50), (60, 150)], you should return 2.
*/

/*
To compile use --std=c++17
*/
#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

struct time_slot {
  int start;
  int end;
};
int rooms(vector<time_slot> slots) {
  map<int, int> rooms_needed_at;
  for (auto &slot : slots) {
    rooms_needed_at[slot.start]++;
    rooms_needed_at[slot.end]--;
  }

  int max_rooms_needed = 0;
  int current_rooms_needed = 0;
  for (auto [_, rooms_needed] : rooms_needed_at) {
    current_rooms_needed += rooms_needed;
    max_rooms_needed = max(max_rooms_needed, current_rooms_needed);
  }

  return max_rooms_needed;
}

int main() {
  {
    vector<time_slot> slots{{30, 75}, {0, 50}, {60, 150}};
    assert(rooms(slots) == 2);
  }
  {
    vector<time_slot> slots{{0, 50}, {0, 50}, {60, 150}};
    assert(rooms(slots) == 2);
  }
  {
    vector<time_slot> slots{{30, 75}, {10, 75}, {75, 150}};
    assert(rooms(slots) == 2);
  }
  {
    vector<time_slot> slots{{30, 75}, {75, 80}, {80, 150}};
    assert(rooms(slots) == 1);
  }

  return 0;
}
