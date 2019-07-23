/*
 Given a singly-linked list, reverse the list. This can be done iteratively or recursively. Can you get both solutions?
*/

/*
Here I provide the iterative and the recursive solution.
In order to compile this solution, use -std=c++17 (due to the use of structured bindings).
*/

#include <cassert>
#include <iostream>
#include <tuple>

using namespace std;

struct Node {
  Node(int val) : val{val}, next{nullptr} {}

  int val;
  Node* next;
};

Node* create_list(int number_elements) {
  assert(number_elements != 0);

  Node* root = new Node(0);
  Node* prev = root;

  for (int i = 1; i < number_elements; i++) {
    auto* node = new Node(i);
    prev->next = node;
    prev = node;
  }

  return root;
}

// Iterative solution
Node* reverse_list_it(Node* root) {
  assert(root != nullptr);

  if (root->next == nullptr) {
    return root;
  }

  Node* curr = root->next;
  root->next = nullptr;
  Node* prev = root;

  while (curr != nullptr) {
    Node* tmp = curr->next;
    curr->next = prev;
    prev = curr;
    curr = tmp;
  }
  return prev;
}

// Recursive solution.

// Internal helper that does the actual reversing. The second element in the pair returned is the new beginning of the list,
// which is the last element in the non-reversed list
tuple<Node*, Node*> reverse_list_rec_internal(Node* curr) {
  // If this is the last element of the list, it's the new head of the reversed list
  if (curr->next == nullptr) {
    return {curr, curr};
  }

  // Reverse the current element, and continue the unwinding
  auto [next, last] = reverse_list_rec_internal(curr->next);
  next->next = curr;

  return {curr, last};
}

// Reverse function. The heavy lifting is done in the helper function, this one only calls the helper and
// tidy root (which is the last element in the reversed list) to point to nullptr
Node* reverse_list_rec(Node* root) {
  auto [_, new_start] = reverse_list_rec_internal(root);
  root->next = nullptr;

  return new_start;
}

int main() {
  {
    Node* root = create_list(10);
    Node* reversed = reverse_list_it(root);
    for (int i = 0; i < 10; i++) {
      assert(reversed->val == (9 - i));
      reversed = reversed->next;
    }
    assert(reversed == nullptr);
  }
  {
    Node* root = create_list(2);
    Node* reversed = reverse_list_it(root);
    for (int i = 0; i < 2; i++) {
      assert(reversed->val == (1 - i));
      reversed = reversed->next;
    }
    assert(reversed == nullptr);
  }
  {
    Node* root = create_list(1);
    Node* reversed = reverse_list_it(root);
    for (int i = 0; i < 1; i++) {
      assert(reversed->val == (0));
      reversed = reversed->next;
    }
    assert(reversed == nullptr);
  }
  {
    Node* root = create_list(10);
    Node* reversed = reverse_list_rec(root);
    for (int i = 0; i < 10; i++) {
      assert(reversed->val == (9 - i));
      reversed = reversed->next;
    }
    assert(reversed == nullptr);
  }
  {
    Node* root = create_list(2);
    Node* reversed = reverse_list_rec(root);
    for (int i = 0; i < 2; i++) {
      assert(reversed->val == (1 - i));
      reversed = reversed->next;
    }
    assert(reversed == nullptr);
  }
  {
    Node* root = create_list(1);
    Node* reversed = reverse_list_rec(root);
    for (int i = 0; i < 1; i++) {
      assert(reversed->val == (0));
      reversed = reversed->next;
    }
    assert(reversed == nullptr);
  }
}
