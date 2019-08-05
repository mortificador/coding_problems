/*
Given two singly linked lists that intersect at some point, find the intersecting node. The lists are non-cyclical.

For example, given A = 3 -> 7 -> 8 -> 10 and B = 99 -> 1 -> 8 -> 10, return the node with value 8.

In this example, assume nodes with the same value are the exact same node objects.

Do this in O(M + N) time (where M and N are the lengths of the lists) and constant space.
*/

#include <cassert>
#include <iostream>

using namespace std;

struct Node {
  Node(int val, Node *next = nullptr)
      : val{val}, next{next} {}
  int val;
  Node *next;
};

int calculate_length(Node *l) {
  int length = 0;
  while (l != nullptr) {
    l = l->next;
    length++;
  }

  return length;
}

Node *find_common_node(Node *l1, Node *l2) {
  int l1_length = calculate_length(l1);
  int l2_length = calculate_length(l2);

  Node *longest_list = l1_length > l2_length ? l1 : l2;
  Node *shortest_list = longest_list == l1 ? l2 : l1;

  int to_skip = abs(l1_length - l2_length);
  // Skip the first elements from the longest list that we know that won't be common
  while (to_skip > 0) {
    longest_list = longest_list->next;
    to_skip--;
  }

  // Look for the common node
  while (longest_list != nullptr) {
    if (longest_list == shortest_list) {
      return longest_list;
    }
    longest_list = longest_list->next;
    shortest_list = shortest_list->next;
  }

  // We didn't find anything
  return nullptr;
}

int main() {
  {
    Node *common = new Node(7, new Node(8, new Node(9)));
    Node *l1 = new Node(0, new Node(1, new Node(2, new Node(3, new Node(4, new Node(5, common))))));
    Node *l2 = new Node(10, new Node(11, common));
    assert(find_common_node(l1, l2) == common);
    assert(find_common_node(l2, l1) == common);
  }
  {
    Node *common = new Node(7, new Node(8, new Node(9)));
    Node *l1 = new Node(0, new Node(1, new Node(2, new Node(3, new Node(4, new Node(5, common))))));
    Node *l2 = common;
    assert(find_common_node(l1, l2) == common);
    assert(find_common_node(l2, l1) == common);
  }
  {
    Node *l1 = new Node(0, new Node(1, new Node(2, new Node(3, new Node(4, new Node(5))))));
    Node *l2 = new Node(10, new Node(11));
    assert(find_common_node(l1, l2) == nullptr);
    assert(find_common_node(l2, l1) == nullptr);
  }
}
