/*
Implement a class for a stack that supports all the regular functions (push, pop) and an additional function of max() which returns the 
maximum element in the stack (return None if the stack is empty). Each method should run in constant time.
*/

/*
The way to solve this problem is to keep an internal stack where we save not only the element, but also the max element seen so far. We can achieve this by
declaring a new type that contains both the element inserted, and the max element seen so far, and create an internal stack of that type. We call this type internal_data.
Then, when we push, we do the following:
  If the stack is empty, push a new internal_data where max_element is the element
  If the stack is not empty:
    Get the top of the stack (that gives you the latest internal_data pushed)
    If the max_element corresponding to the top of the stack is larger than the new element
       Push a new internal_data to the stack, where max_element is max_element from the top of the stack
    Else
       Push a new internal_data to the stack, where max_element is the new element

The max() method will do just the following:
  Get the internal_data object that it's at the top of the stack
  Return max_element from the read internal_data object from the top of the stack

To pop, we just pop from the internal stack.

E.g.
Create an max_stack<int>
  internal_stack: {}
Push 1:
  Stack is empty, we push a new internal data where max_element is the new element: { element: 1, max_element: 1}
Push 2:
  Stack is not empty: read the top element
  top element is {element: 1, max_element: 1}
  top element's max_element is smaller than the new element to push, so we insert new element: { element: 2, max_element: 2}
Push 0:
  Stack is not empty: read the top element
  top element is {element: 1, max_element: 2}
  top element's max_element is larger than the new element to push, so we insert new element: { element: 0, max_element: 2}

At this point, our internal stack looks like: [{element: 1, max_element: 1}, {element: 2, max_element: 2}, {element: 0, max_element: 2}]

Max()
  Read top element, and return max_element -> top element is {element:0, max_element: 2} so we return 2
Pop()
  Pop from the internal stack, now our stack looks like: [{element: 1, max_element: 1}, {element: 2, max_element: 2}]
Max()
  Read top element, and return max_element -> top element is {element:2, max_element: 2} so we return 2
Pop()
  Pop from the internal stack, now our stack looks like: [{element: 1, max_element: 1}]
Max()
  Read top element, and return max_element -> top element is {element:1, max_element: 1} so we return 1
*/

#include <cassert>
#include <stack>

using namespace std;

template <typename T>
class max_stack {
private:
  // Our internal stack will save elements of type internal_data. For every element, we also save the max element seen so far
  struct internal_data {
    internal_data(T const& new_element, T const& max_element)
        : element{new_element}, max_element{max_element} {}
    T element;
    T max_element;
  };

public:
  void push(T const& element) {
    // If the size is zero, just insert the element.
    if (s.size() == 0) {
      s.push(internal_data{element, element});
    }

    auto& last_element = s.top();
    if (element > last_element.max_element) {
      s.push(internal_data{element, element});
    } else {
      s.push(internal_data{element, last_element.max_element});
    }
  }

  void pop() {
    s.pop();
  }

  // We will throw if the stack is empty (the problem states to return None if the stack is empty, but that
  // only applies to python)
  T& max() {
    return s.top().max_element;
  }

private:
  stack<internal_data> s;
};

int main() {
  {
    max_stack<int> ms;
    ms.push(1);
    ms.push(2);
    ms.push(3);
    assert(ms.max() == 3);
    ms.pop();
    assert(ms.max() == 2);
    ms.pop();
    assert(ms.max() == 1);
  }
  {
    max_stack<int> ms;
    ms.push(3);
    ms.push(2);
    ms.push(1);
    assert(ms.max() == 3);
    ms.pop();
    assert(ms.max() == 3);
    ms.pop();
    assert(ms.max() == 3);
  }
  {
    max_stack<int> ms;
    ms.push(3);
    ms.push(2);
    ms.push(1);
    ms.push(4);
    assert(ms.max() == 4);
    ms.pop();
    assert(ms.max() == 3);
    ms.pop();
    assert(ms.max() == 3);
    ms.pop();
    assert(ms.max() == 3);
  }
}
