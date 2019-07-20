/*
A unival tree (which stands for "universal value") is a tree where all nodes under it have the same value.

Given the root to a binary tree, count the number of unival subtrees.

For example, the following tree has 5 unival subtrees:

   0
  / \
 1   0
    / \
   1   0
  / \
 1   1
*/

#include <cassert>

using namespace std;

struct Node {
public:
  Node(int val, Node *left = nullptr, Node *right = nullptr)
      : left{left}, right{right}, val{val} {}

  int val;
  Node *left;
  Node *right;
};

auto internal_count(Node *node, int value, int &counter) -> bool {
  if (node == nullptr) {
    return true;
  }

  /*
   If node->left and node->right are nullptr, the node is a leaf node, and
   they are always considered unival trees:
           0
            \
             0 <- This node is a leaf node, it's always considered an unival tree.
  */
  if (node->left == nullptr && node->right == nullptr) {
    counter++;
    return node->val == value;
  }

  /*
   If node->left is null it means that node->right is not (and viceversa, if both were null
   the previous check would have handled the case). If one of the nodes is null, the function
   will return true (this is the first check in the function). This is to handle the following case:
           0
          / \
         1   0 <- when processing this node, equal_right and equal_left should be true, even though there is no node on the right
            /
           0
  */

  bool equal_left = internal_count(node->left, node->val, counter);
  bool equal_right = internal_count(node->right, node->val, counter);

  /*
   If both all our left nodes and all our right nodes have the same value as us, the increment the counter
   as we form an unival tree
            0
          /   \
         1 |   0   |  <- If all the left and right children have the same value as the node we are processing, then the tree formed by
           |  / \  |    the children plus the node that we are processing form an unival tree, therefore we increment the counter
           | 0   0 | 
  */
  if (equal_left == true && equal_right == true) {
    counter++;
  }

  // Return true if our children and the current node form an unival tree, false otherwise
  return node->val == value && equal_left && equal_right;
}

auto count_unival(Node *root) -> int {
  int count = 0;
  internal_count(root, root->val, count);

  return count;
}

int main() {
  {
/*
   0
*/
    auto root = Node(0);
    assert(count_unival(&root) == 1);
  }
  {
/*
     0
    / 
   1  
  / \
 1   1
*/
    auto root = Node(0, new Node(1, new Node(1), new Node(1)));
    assert(count_unival(&root) == 3);
  }
  {
/*
   0
  / \
 1   0
    / \
   1   0
  / \
 1   1
*/
    auto root = Node(0, new Node(1), new Node(0, new Node(1, new Node(1), new Node(1)), new Node(0)));
    assert(count_unival(&root) == 5);
  }
  {
/*
     0
    / 
   0
  / \
 0   0
*/
    auto root = Node(0, new Node(0, new Node(0), new Node(0)));
    assert(count_unival(&root) == 4);
  }

  return 0;
}
