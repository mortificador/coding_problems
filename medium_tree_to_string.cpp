/*
Given the root to a binary tree, implement serialize(root), which serializes the tree into a string, and deserialize(s), which deserializes the string back into the tree.

For example, given the following Node class

class Node:
    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

The following test should pass:

node = Node('root', Node('left', Node('left.left')), Node('right'))
assert deserialize(serialize(node)).left.left.val == 'left.left'

*/

/*
This is question is about working with trees. In order to serialize and deserialize, we need to make some choices about how to represent a node as a string.
My choice is the following: the value of the node will be between '|'. This means that '|' is a forbidden character, meaning that it can't not be part
of the value of the node.
Then I represent the left node with the characters &l and the right node with &r. I represent null values as @.
We serialize/deserialize the tree in a depth-first fashion.

E.g:
the following tree:

               ¹"root"               
              /       \
          ²"left"    ⁹"right"
           /    \      /    \
          /      \ ¹⁰null  ¹¹null
         /        \
  ³"left.left" ⁶"left.right"
      /   \         /    \
  ⁴null  ⁵null   ⁷null  ⁸null

would be serialized to:  |root|&l|left|&l|left.left|&l@&r@&r|left.right|&l@&r@&r|right|&l@&r@
                            |  |        |           |   |  |             |  |  |        |  |
                            ¹  |        |           |   |  |             |  |  |        |  |
                               ²        |           |   |  ⁶             |  |  ⁹        |  |
                                        ³           |   |                ⁷  ⁸          ¹⁰  ¹¹  
                                                    ⁴   ⁵
*/

#include <cassert>
#include <iostream>
#include <string>

using namespace std;

struct Node {
public:
  Node() : left(nullptr), right(nullptr) {}

  Node(string const& val, Node* left = nullptr, Node* right = nullptr) {
    this->left = left;
    this->right = right;
    this->val = val;
  }

  string val;
  Node* left;
  Node* right;
};

void internal_serialize(Node* node, string& ser) {
  if (node == nullptr) {
    ser += "@";
    return;
  }

  // '|' is a forbidden character
  assert(node->val.find("|") == std::string::npos);

  // Wrap the value between '|' characters
  ser += "|";
  ser += node->val;
  ser += "|";

  // Add identifier for left node and serialize it
  ser += "&l";
  internal_serialize(node->left, ser);
  // Add identifier for right node and serialize it
  ser += "&r";
  internal_serialize(node->right, ser);
}

auto serialize(Node* root) -> string {
  string result;
  internal_serialize(root, result);

  return result;
}

auto internal_deserialize(string const& s, size_t& pos) -> Node* {
  // If it's a null node, move pos and return null
  if (s[pos] == '@') {
    pos += 1;
    return nullptr;
  }

  // If we are processing a branch, move pos forward and continue
  if (s[pos] == '&') {
    pos += 2;
    return internal_deserialize(s, pos);
  }

  // At this point we have a value to decode
  auto end_pos = s.find('|', pos + 1);
  Node* node = new Node();
  node->val = s.substr(pos + 1, end_pos - pos - 1);
  pos = end_pos + 1;

  if (pos < s.size()) {
    node->left = internal_deserialize(s, pos);
  }
  if (pos < s.size()) {
    node->right = internal_deserialize(s, pos);
  }

  return node;
}

auto deserialize(string const& s) -> Node* {
  size_t pos = 0;
  return internal_deserialize(s, pos);
}

// Helper function to check if two trees are equal
auto are_equal(Node* root1, Node* root2) -> bool {
  if (root1 == nullptr && root2 == nullptr) {
    return true;
  }

  if (root1 == nullptr || root2 == nullptr) {
    return false;
  }

  return root1->val == root2->val &&
         are_equal(root1->left, root2->left) &&
         are_equal(root1->right, root2->right);
}

int main() {
  {
    auto root = Node("root", nullptr, new Node("right"));
    assert(are_equal(&root, deserialize(serialize(deserialize(serialize(&root))))));
  }
  {
    auto root = Node("root", new Node("left", new Node("left.left"), new Node("left.right")), new Node("right"));
    assert(are_equal(&root, deserialize(serialize(deserialize(serialize(&root))))));
  }
  {
    auto root = Node("root", new Node("left", new Node("left.left"), new Node("left.right", nullptr, new Node("left.right.right"))), new Node("right"));
    assert(are_equal(&root, deserialize(serialize(deserialize(serialize(&root))))));
  }

  return 0;
}
