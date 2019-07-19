/*
An XOR linked list is a more memory efficient doubly linked list. Instead of each node holding next and prev fields, it holds a field named both, which is an XOR of the next node and the previous node. Implement an XOR linked list; it has an add(element) which adds the element to the end, and a get(index) which returns the node at index.

If using a language that has no pointers (such as Python), you can assume you have access to get_pointer and dereference_pointer functions that converts between nodes and memory addresses.
*/

/*
This question is a bit misleading: you cannot implement a fully doubly linked-list if each element only holds the XOR of the next and prev nodes.
You can't, for example, given one node, and only one node, get the previous or next node, unless that given node is the first node in the list.
You can, however, implement a doubly linked-list in the sense that the list can be traversed from first node to last or from last node to first.

To be able to come up with a solution, we need to remember the following property of the XOR operation:
A ^ B = C
C ^ A = A ^ C = B
C ^ B = B ^ C = A

It's also handy to remember that:
A ^ A = 0

In this case, C would be 'both', and A and B would be 'prev' and 'next': so, by having prev and both, we can get next, and by having next and both, we can
get prev. This means that we can forward traverse a list if we have the first pointer of the list, and we can backwards traverse a list if we have the
last element of the list.
E.g.:
the first element has the address:  10101110
the second element has the address: 11001000
the third element has the address:  10011101

Then:
first->both = prev ^ next = third ^ second = 11001000 ^ 10011101 = 01010101
second->both = prev ^ next = first ^ third = 10101110 ^ 10011101 = 00110011
third->both = prev ^ next = second ^ first = 11001000 ^ 10101110 = 01100110

If we want to traverse the list, all we need is the address of the last element and the first element, and from there we traverse:
second = first->both ^ last   = 01010101 ^ 10011101 = 11001000  <- this is, effectively, the address of second
third  = second->both ^ first = 00110011 ^ 10101110 = 10011101  <- address of third
...

If we wanted to traverse it backwards, we would do it the same way, but changing prev by next
second = third->both  ^ first = 01100110 ^ 10101110 = 11001000  <- this is, effectively, the address of second
first  = second->both ^ third = 00110011 ^ 10011101 = 10101110  <- address of first
...

Now that we now that we need to keep a pointer to the first and last element, and how to retrieve the prev and next node, given either the next or prev node,
we can desing the logic of inserting an element at the end of the list.
We need to consider three different cases when inserting a new node at the end of the list:
 If we are inserting the first element:  create the node
                                         point first to new_node
 If we are inserting the second element: create the node, 
                                         update first->both to point to it
                                         update last to point to it too
 If we are inserting any other element:  create the new node 
                                         get the previous_to_last node, 
                                         update last->both to be previous_to_last ^ new_node, 
                                         set new_node->both to be first ^ last
                                         update last to point to new_node

Why do we need to consider three different cases? The first case is clear: first would be null, so we need a special case where we set first
for the first time. 
The second case is because, when there are only two elements in the list, the next element to first is new_node, and the
previous element of first is also new_node, which means that first->both should be: first->both = new_node ^ new_node. However, if you A ^ A
the result is zero, so first->both would point to null, and the information of where the second node is would be lost. That's why we need to consider
this special case.
Finally, the third case is the common case.
*/

#include <cstdint>
#include <cassert>

using namespace std;

class Node {
public:
  Node(int val) : val{val}, both{0} {}

  int val;
  uint64_t both;

  Node *get_other(Node *n) {
    return reinterpret_cast<Node *>(both ^ reinterpret_cast<uint64_t>(n));
  }
};

/*
This class maintains a pointer to the first element of the list and the last element of it.
*/
class XORList {
public:
  XORList() : first{nullptr}, last{nullptr} {}

  void add(int val) {
    // First element in the list
    if (first == nullptr) {
      first = new Node(val);

      // First only points to itself
      first->both = reinterpret_cast<uint64_t>(first) ^ reinterpret_cast<uint64_t>(first);

      last = first;

      return;

    } else {
      auto *node = new Node(val);

      // If first is the only element in the list:
      // we make first->both point to the new node,
      // and we update last to point to the new node. 
      // This will be the state of the list after this code:
      // first->both points to the new node
      // node->both points to null
      // last becomes node
      if (first == last) {
        first->both = reinterpret_cast<uint64_t>(node);
        node->both = reinterpret_cast<uint64_t>(first) ^ reinterpret_cast<uint64_t>(first);

        last = node;

        return;
      }

      // If we have two or more elements in the list

      // Get the previous node to the last node
      auto *prev_to_last = last->get_other(first);
      // last->both now is prev_to_last ^ node, because node is now its next element
      last->both = reinterpret_cast<uint64_t>(prev_to_last) ^ reinterpret_cast<uint64_t>(node);

      // insert the new node: its previous node is the old last, and the next node is first
      node->both = reinterpret_cast<uint64_t>(last) ^ reinterpret_cast<uint64_t>(first);

      // Update last
      last = node;
    }
  }

  auto get(int index) -> Node * {
    if (index == 0) {
      return first;
    }
    if (index == 1) {
      return reinterpret_cast<Node *>(first->both);
    }

    auto *node = reinterpret_cast<Node *>(first->both);
    auto *prev = first;
    for (; index > 1; index--) {
      auto *tmp = node;
      node = node->get_other(prev);
      prev = tmp;
    }
    return node;
  }

private:
  Node *first;
  Node *last;
};

int main() {
  {
    auto ll = XORList();
    ll.add(1);
    assert(ll.get(0)->val == 1);
  }
  {
    auto ll = XORList();
    ll.add(1);
    ll.add(2);
    assert(ll.get(0)->val == 1);
    assert(ll.get(1)->val == 2);
  }
  {
    auto ll = XORList();
    for (int i = 0; i < 1000; i++) {
      ll.add(i);
    }
    for (int i = 0; i < 1000; i++) {
      assert(ll.get(i)->val == i);
    }
  }
  return 0;
}
