/*
Given an integer k and a binary search tree, find the floor (less than or equal to) of k, and the ceiling (larger than or equal to) of k. If either does not exist, then print them as None.
*/

/*
The approach to solve this problem is the following: we walk through the tree like if we are searching for k. Everytime that we see a node which value is not k,
we update either floor (if the current node is closer to k than the current floor) or ceiling (if the current node is closer to k than the current ceiling), and
keep searching for k like we do in a binary tree (i.e. if the node is larger than k, then we keep searching on the left node, and if the node is smaller than k,
we keep searching in the right node). We stop if we either find a node which value is exactly k, or if we reach a leaf.

To show this think about the following tree:
              _ 8_
             /    \
            4      12
           / \    /  \
          2   6   10 14

How could we find the floor and ceiling of k=9? the only way is walk through the tree searching for k, until we find either a leaf or k. At that point, 
we are sure that we have seen the smallest number that is closer to k, and the highest number that is closer to k.
The approach is the following:
 we process a node:
                    if the value of the node is less than k, and the difference between the value of the node and k is smaller than the difference between
                    the current floor and k, we update floor with the current node, and keep searching on the right side of the node.

                    if the value of the node is larger than k, and the difference between the value of the node and k is smaller than the difference between
                    the current ceiling and k, we update ceiling with the current node, and keep searching on the left side of the node.

                    if the value of the node is equal to k, we update floor and ceiling with the current node and finish.

                    if the current node is null, we finish.

E.g:
              _ 8_
             /    \
            4      12
           / \    /  \
          2   6   10 14
floor = nullptr
ceiling = nullptr
node = 8
k = 9

We start:
node->val (8) is smaller than k, and floor is nullptr, so we update floor and keep searching on the right side of the tree

node = 12
floor = 8
ceiling = nullptr
node->val is larger than k, and ceiling is nullptr, so we update ceiling and keep searching on the left side of the tree

node = 10
floor = 8
ceiling = 12
node->val is larger than k, and node->val is closer to k than the current ceiling, so we update ceiling and keep searching on the left side of the tree

node = nullptr
floor = 8
ceiling = 10
node is nullptr, so we finish

So, the result is: floor = 8, ceiling = 10, which is the right answer.
*/


#include <cassert>
#include <stdlib.h>

using namespace std;

struct Node {
public:
  Node(int val, Node *left = nullptr, Node *right = nullptr) : val{val}, left{left}, right{right} {}

  int val;
  Node *left;
  Node *right;
};

struct value_found {
  bool was_found = false;

  int val;
};

void find_ceiling_floor(Node *node, int k, int *&floor, int *&ceiling) {
  if (node == nullptr) {
    return;
  }

  if (node->val == k) {
    floor = &(node->val);
    ceiling = &(node->val);
    return;
  }

  int diff = abs(node->val - k);

  if (node->val > k) {
    if (ceiling == nullptr) {
      ceiling = &(node->val);
    } else if (diff < (*ceiling - k)) {
      ceiling = &(node->val);
    }
    return find_ceiling_floor(node->left, k, floor, ceiling);
  }

  if (node->val < k) {
    if (floor == nullptr) {
      floor = &(node->val);
    } else if (diff < (k - *floor)) {
      floor = &(node->val);
    }
    return find_ceiling_floor(node->right, k, floor, ceiling);
  }
}


int main() {
  auto *root = new Node(8);
  root->left = new Node(4);
  root->right = new Node(12);

  root->left->left = new Node(2);
  root->left->right = new Node(6);

  root->right->left = new Node(10);
  root->right->right = new Node(14);

  {
    int *floor = nullptr;
    int *ceiling = nullptr;
    find_ceiling_floor(root, 6, floor, ceiling);
    assert(floor != nullptr);
    assert(ceiling != nullptr);
    assert(*floor == 6);
    assert(*ceiling == 6);
  }
  {
    int *floor = nullptr;
    int *ceiling = nullptr;
    find_ceiling_floor(root, 8, floor, ceiling);
    assert(floor != nullptr);
    assert(ceiling != nullptr);
    assert(*floor == 8);
    assert(*ceiling == 8);
  }
  {
    int *floor = nullptr;
    int *ceiling = nullptr;
    find_ceiling_floor(root, 3, floor, ceiling);
    assert(floor != nullptr);
    assert(ceiling != nullptr);
    assert(*floor == 2);
    assert(*ceiling == 4);
  }
  {
    int *floor = nullptr;
    int *ceiling = nullptr;
    find_ceiling_floor(root, 15, floor, ceiling);
    assert(floor != nullptr);
    assert(ceiling == nullptr);
    assert(*floor == 14);
  }
  {
    int *floor = nullptr;
    int *ceiling = nullptr;
    find_ceiling_floor(root, 1, floor, ceiling);
    assert(floor == nullptr);
    assert(ceiling != nullptr);
    assert(*ceiling == 2);
  }
  {
    int *floor = nullptr;
    int *ceiling = nullptr;
    find_ceiling_floor(root, 9, floor, ceiling);
    assert(floor != nullptr);
    assert(ceiling != nullptr);
    assert(*floor == 8);
  }

  return 0;
}
