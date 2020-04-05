
/*
 Given a list of numbers, find if there exists a pythagorean triplet in that list. A pythagorean triplet is 3 variables a, b, c where a^2 + b^2 = c^2
*/

#include <cassert>
#include <unordered_map>
#include <vector>

using namespace std;

bool find_pythagorean_triplets(vector<int> const &values) {
  unordered_map<int, int> value_times;

  for (auto v : values) {
    value_times[v * v]++;
  }

  for (int i = 0; i < values.size(); i++) {
    int a2 = values[i] * values[i];
    value_times[a2]--;
    for (int j = i + 1; j < values.size(); j++) {
      int b2 = values[j] * values[j];
      value_times[b2]--;
      int c2 = a2 + b2;
      auto c2_present_it = value_times.find(c2);
      if (c2_present_it != end(value_times) && c2_present_it->second > 0) {
        return true;
      }
      value_times[b2]++;
    }
  }

  return false;
}

int main() {
  {
    std::vector<int> v{3, 5, 12, 5, 13};
    assert(find_pythagorean_triplets(v));
  }
  {
    std::vector<int> v{3, 1, 4, 6, 5};
    assert(find_pythagorean_triplets(v));
  }
  {
    std::vector<int> v{10, 4, 6, 12, 5};
    assert(find_pythagorean_triplets(v) == false);
  }

  return 0;
}
