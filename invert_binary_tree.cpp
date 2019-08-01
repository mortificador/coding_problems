/*
 You are given the root of a binary tree. Invert the binary tree in place. That is, all left children should become right children, and all right children should become left children.

Example:

      a
    /   \
   b     c
  / \   /
 d   e f


The inverted version of this tree is as follows:

    a
  /   \
 c     b
  \   / \
   f e   d

*/

/*
This problem would be easier if we could just invert the values of the nodes, unfortunately it's not possible, because then we couldn't deal with nullptr nodes.
Because there can be nullptr nodes, the way we need to invert the tree is by updating the left and right children of each node on each level. How should we invert the nodes?
The key is to notice how the children of a node end up after the tree is inverted: the children are swapped, meaning that node->left will point to the previous node->right,
and viceversa. So, all we have to do to invert the tree, is swap the left and right children of every node on each level. 
E.g:

      a                            a                      We can see that every node had it's children swapped: b->left is the previous b->right, and viceversa. The same
    /   \                        /   \                    for c, and for a
   b     c    -----invert---->  c     b           
  / \   /                        \   / \
 d   e f                          f e   d

We can do this the following way: for every level, we get the parent nodes, and we save the nodes on this level in a vector.
Now we just need to point each parent's node left and right to the appropiate nodes.

Let's see for example, the last level of the tree:
parent_nodes : {b,c}
nodes : {d, e, f, nullptr}

We only need to:
 For every parent
   point node->left to nodes[index+1]
   point node->right to nodes[index]
   increase index by two: index += 2;

We can write a recursive function to do this: we pass a vector with the nodes on the current level, and the recursive function will receive the vector and collect all
the children in a new vector, and will call the function again, this time passing the children. Every time the function is called, it receives the parent nodes, and
collects all the children. We don't need to call the function anymore when we find an empty level (all the children nodes are nullptr). At this point we can start
doing the inversion.

E.g
      a
    /   \
   b     c
  / \   /
 d   e f

We call the function with the root node = {a}

We collect all the children:
parent_node = {a}
nodes_level = {b, c}
Call the function to invert the children:
    parent_node = {b, c}
    nodes_level = {d, e, f, nullptr}
    Call the function to invert the children
        parent_node = {d, e, f, nullptr}
        nodes_level = {nullptr, nullptr, nullptr, nullptr}
        All nodes_level are nullptr, so we can finish -> return
    
   Revert the children nodes:
   parent_node = {b, c}
   nodes_level = {d, e, f, nullptr}
   index = 0;
   for each node in parent_node:
     parent_node->left = nodes_level[index+1]
     parent_node->right = nodes_level[index]
     index += 2
   After this: b->left = d, b->right = e, c->left = nullptr, c->right = f
   return


parent_node = {a}
nodes_level = {b, c}
index = 0;
for each node in parent_node:
   parent_node->left = nodes_level[index+1]
   parent_node->right = nodes_level[index]
   index += 2
After this: a->left = c, a->right = b
return

So, after inverting, our tree looks like this:
    a
  /   \
 c     b
  \   / \
   f e   d

Which is the right answer.
*/

#include <algorithm>
#include <cassert>
#include <vector>

using namespace std;

struct Node {
public:
  Node(int val, Node* left = nullptr, Node* right = nullptr)
      : val{val}, left{left}, right{right} {}

  int val;
  Node* left;
  Node* right;
};

void invert_tree(vector<Node*> const& nodes_previous_level) {
  // If we reach the last level (when all children are nullptr), we can start reversing
  bool last_level = true;

  // nodes in current level holds the children of the parents: the first two elements will be the left and right children of the first parent node,
  // the third and fourth elements will be the children of the second parent node, and so on.
  vector<Node*> nodes_current_level;
  for (auto* node : nodes_previous_level) {
    if (node != nullptr) {
      last_level &= (node->right == nullptr);
      last_level &= (node->left == nullptr);
      nodes_current_level.push_back(node->left);
      nodes_current_level.push_back(node->right);
    }
  }
  // If this was the last level with nodes, then we can start reverting the parent nodes
  if (last_level == true) {
    return;
  }

  // Invert the level below
  invert_tree(nodes_current_level);

  int current_index = 0;
  // Do the swapping: parent->left will point to the previous parent->right, and viceversa
  for (auto it = nodes_previous_level.begin(); it != end(nodes_previous_level); it++, current_index += 2) {
    (*it)->left = nodes_current_level[current_index + 1];
    (*it)->right = nodes_current_level[current_index];
  }
}

int main() {
  {
    /*
     __0__
    /     \
   1       2  
*/
    auto* root = new Node(0, new Node(1), new Node(2));
    invert_tree(vector<Node*>{root});
    assert(root->val == 0);
    assert(root->left->val == 2);
    assert(root->right->val == 1);
  }
  {
    /*
     __0__
    /     \
   1       2  
  /  \    / \
 3    4  5   
*/
    auto* root = new Node(0, new Node(1, new Node(3), new Node(4)), new Node(2, new Node(5)));
    invert_tree(vector<Node*>{root});
    assert(root->val == 0);
    assert(root->left->val == 2);
    assert(root->right->val == 1);
    assert(root->left->left == nullptr);
    assert(root->left->right->val == 5);
    assert(root->right->left->val == 4);
    assert(root->right->right->val == 3);
  }
  {
    /*
       __0__
      /     \
     1       2  
    /  \    / \
   3    4  5   6
  / \  /      / \
 7  8 9      10 11
*/
    auto* root = new Node(0, new Node(1, new Node(3, new Node(7), new Node(8)), new Node(4, new Node(9))), new Node(2, new Node(5), new Node(6, new Node(10), new Node(11))));
    invert_tree(vector<Node*>{root});
    assert(root->val == 0);
    assert(root->left->val == 2);
    assert(root->right->val == 1);
    assert(root->left->left->val == 6);
    assert(root->left->right->val == 5);
    assert(root->right->left->val == 4);
    assert(root->right->right->val == 3);
    assert(root->left->left->left->val == 11);
    assert(root->left->left->right->val == 10);
    assert(root->left->right->left == nullptr);
    assert(root->left->right->right == nullptr);
    assert(root->right->left->left == nullptr);
    assert(root->right->left->right->val == 9);
    assert(root->right->right->left->val == 8);
    assert(root->right->right->right->val == 7);
  }
}
