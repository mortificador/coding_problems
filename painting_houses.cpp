/*
A builder is looking to build a row of N houses that can be of K different colors. He has a goal of minimizing cost while ensuring that no two neighboring houses are of the same color.

Given an N by K matrix where the nth row and kth column represents the cost to build the nth house with kth color, return the minimum cost which achieves this goal.
*/

#include <cassert>
#include <limits>
#include <unordered_map>
#include <vector>

using namespace std;

// Build a size_t from a short and an int, to be able to use the {short, int} as a key in an unordered_map
inline size_t key(short i, int j) { return (size_t)i << 32 | (size_t)j; }

// house_color_prices: array with the price of painting the house i of each color. e.g. house_color_prices[2][0] is the price
// of painting house 2 of color 0.
size_t calc_min_cost(size_t prev_cost,
                     int prev_house_index,
                     short prev_color,
                     vector<vector<size_t>> const &house_color_prices,
                     unordered_map<size_t, size_t> &house_color_costs) {
  auto n_houses = house_color_prices.size();

  int curr_house_index = prev_house_index + 1;
  if (curr_house_index == n_houses) {
    return prev_cost;
  }

  // All houses should have the same number of possible colors
  auto n_colors = house_color_prices[0].size();

  auto min_cost = numeric_limits<size_t>::max();

  for (int i = 0; i < n_colors; i++) {
    if (i != prev_color) {
      if (house_color_costs.count(key(i, curr_house_index)) == 0) {
        auto cost = calc_min_cost(house_color_prices[curr_house_index][i], curr_house_index, i, house_color_prices, house_color_costs);
        min_cost = min(cost, min_cost);
        house_color_costs[key(i, curr_house_index)] = cost;
      } else {
        min_cost = min(min_cost, house_color_costs[key(i, curr_house_index)]);
      }
    }
  }

  return prev_cost + min_cost;
}

size_t calc_all_min_costs(vector<vector<size_t>> const &house_color_prices) {
  // key: {color (short), house_index (int)}
  unordered_map<size_t, size_t> house_color_costs;
  return calc_min_cost(0, -1, -1, house_color_prices, house_color_costs);
}

int main() {
  {
    // 3 houses, 3 colors
    auto house_color_prices = vector<vector<size_t>>{{668, 126, 216}, {307, 524, 464}, {430, 655, 958}};
    assert(calc_all_min_costs(house_color_prices) == 1020);
  }

  {
    // 8 houses, 3 colors
    auto house_color_prices = vector<vector<size_t>>{{918, 497, 710}, {268, 529, 226}, {447, 288, 411}, {489, 189, 776}, {688, 70, 617}, {401, 656, 858}, {958, 467, 665}, {545, 746, 545}};
    assert(calc_all_min_costs(house_color_prices) == 2983);
  }
  return 0;
}
