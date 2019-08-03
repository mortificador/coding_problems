/*
Each day, you can either buy one share of WOT, sell any number of shares of WOT that you own, or not make any transaction at all. What is the maximum 
profit you can obtain with an optimum trading strategy?

For example, if you know that prices for the next two days are prices = [1,2], you should buy one share day one, and sell it day two for a profit of 1. 
If they are instead prices = [3,2], no profit can be made so you don't buy or sell stock those days. 
*/

/*
The first thing is to come up with the right strategy to buy and sell. We want to buy a share if we know that, in the future, we will be able to sell
it at a larger price. But that's not all: we need to sell it at the point where we make the most profit. The point where we make the most profit
is the largest value in the array of prices after the buy price. So, every time we see a price, in order to know if we must buy it and how much profit we can make,
we need to know what is the largest value in the array after this price.
For this, we could, for every price, find the maximum value in the array after that price. We can do this in a efficient way, without having to find every time
in the array the next maximum price.
For this, we can create a secondary array, max_after: this array contains, at every index i, the maximum price in the array prices after i.

E.g.
prices = [1, 2, 100, 5, 9]
max_after would be = [100, 100, 9, 9, 0] (the last element is always zero, as you would never buy the last day, because you could never sell it)

Once we have the array max_after, we can calculate the profit the following way:
For every price at index i, we look in max_after[i+1]
  max_after[i+1] is the largest price in prices[i] that we can find after the element i, so, if max_after[i+1] is larger than prices[i], the maximum
  profit that we can make is (max_after[i+1] - prices[i]). If prices[i] is larger, then no profit can be made (we should not buy).

How can we build max_after? We can iterate through prices in reverse order (starting from the end), and saving in max_after the max. value seen so far:

E.g:
prices = [1, 2, 100, 5, 9]
We start from the end, and save in in max_after[i] the max seen so far:
i = 4
max_so_far = 9
max_after[4] = 9

i = 3
max_so_far = 9
max_after[3] = 9

i = 2
max_so_far = 100
max_after[2] = 100

i = 1
max_so_far = 100
max_after[1] = 100

i = 0
max_so_far = 100
max_after[0] = 100

Finally: max_after = [100, 100, 100, 9, 9]

So, the first step in our algorithm will be to build the array max_after, and then apply the algorithm that we explained before.

E.g.
prices = [1, 2, 100, 5, 9]

We first build max_after:
max_after =[100, 100, 100, 9, 9]

profit = 0
For every i:
i = 0
prices[0] = 1
max_after[1] = 100
profit += max_after[1] - prices[0] = 99

i = 1
prices[1] = 2
max_after[2] = 100
profit += max_after[2] - prices[2] = 99 + 98 = 197

i = 2
prices[2] = 100
max_after[3] = 9
prices[2] is larger than max_after[3], so we don't do anything (profit would be negative)

i = 3
prices[3] = 5
max_after[4] = 9
profits += prices[3] - max_after[4] = 197 + 4 = 201

We finish (we will never buy the last element, so we don't consider it) and return the profits: 201.
*/

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

uint64_t max_profit(vector<int> const& prices) {
  // max_after is an array that, at every index, it saves the maximum value
  // in prices that you can find past that index
  vector<int> max_after;
  max_after.resize(prices.size());
  int max_price_so_far = prices.back();
  for (int i = prices.size() - 1; i >= 0; i--) {
    if (prices[i] >= max_price_so_far) {
      max_price_so_far = prices[i];
    }
    max_after[i] = max_price_so_far;
  }

  uint64_t profit = 0;
  // If, for a given prices, max_after[i+1] is larger (meaning that, at some point in prices,
  // we will find a price larger than the current prices[i]) then add the profit (simulate that we
  // buy the stock now and sell it when we arrive to the price in max_after[i+1]
  for (int i = 0; i < prices.size() - 1; i++) {
    if (prices[i] < max_after[i + 1]) {
      profit += (max_after[i + 1] - prices[i]);
    }
  }

  return profit;
}

int main() {
  {
    vector<int> v{1, 2, 100};
    assert(max_profit(v) == 197);
  }
  {
    vector<int> v{1, 2, 100, 5, 9};
    assert(max_profit(v) == 201);
  }
  {
    vector<int> v{5, 3, 2};
    assert(max_profit(v) == 0);
  }
  {
    vector<int> v{5, 4, 3, 4, 5};
    assert(max_profit(v) == 4);
  }
}
