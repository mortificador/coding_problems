/*
Imagine you are building a compiler. Before running any code, the compiler must check that the parentheses in the program are balanced. 
Every opening bracket must have a corresponding closing bracket. We can approximate this using strings.

Given a string containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.
An input string is valid if:
- Open brackets are closed by the same type of brackets.
- Open brackets are closed in the correct order.
- Note that an empty string is also considered valid.
*/

/*
The key to solve this problem is to understand how to check that an expression is balanced.
An expression is balanced if, when we see an closing bracket, the previous opening bracket that we saw corresponds to the same type of bracket.
For example, if we see a closing square bracket, ], we need to check that the previous opening bracket was an opening square bracket, [. If that's
the case, the expression is so far balanced, if it's not, then the expression is not balanced.
Let's see an example: 
[([])] <- this expression is balanced, because every time we see a closing bracket, the previous opening bracket that we saw
          corresponds to the same type of bracket.
[(]) <- this expression is not balanced: when we find the first closing bracket is a square bracket, ], but the previous opening bracket was
        a parentheses, ).

So, the algorithm will be the following:
  for every character in the string:
    if it's an opening bracket, we save it in a data structure of our choice and continue
    if it's a closing bracket, we retrieve the last opening bracket from our data structure, and then:
       if the last opening bracket is of the same type, we remove the opening bracket from our data structure, and continue
       if the last opening bracket is not of the same type, we stop and return false
  When we finish iterating over all the elements, we return true if our data structure is empty, and false otherwise. This is to 
  return false in cases like this: ([])(

Now, the choice that remains is which data structure to use. In the algorithm we will be saving the opening characters that we see in this data
structure, and when we retrieve them, we need to retrieve the last one that was added, and so on (this is, we need a LIFO data structure, so
it will give us the last element that was inserted, and then the previous to that one, ans so on). A stack is perfect for this.

E.g:
expr: ([])
stack: empty
We start iterating through expr:
 ( is an opening bracket, so we push it in the stack. Stack now contains: (
 [ is an opening bracket, so we push it in the stack. Stack now contains: ([
 ] is a closing bracket, so we take the last element from the stack and check if the brackets are the same type:
     the last element in the stack is: ], which is the same type as ], so we remove the element from the stack and continue. Stack now contains: (
 ) is a closing bracket, so we take the last element from the stack and check if the brackets are the same type:
     the last element in the stack is: ), which is the same type as ), so we remove the element from the stack and continue. Stack is empty now
 There are no more elements, we return is the stack is empty. The stack is empty, so we return true
*/

#include <cassert>
#include <stack>
#include <string>

using namespace std;

auto is_balanced(string expr) -> bool {
  stack<char> opened;
  for (auto character : expr) {
    if (character == '{' || character == '(' || character == '[') {
      opened.push(character);
    }
    // The problem states that there will be only brackets in the string, so if the character is not an opening bracket,
    // it must be a closing bracket
    else {
      auto last_opening_bracket = opened.top();
      if ((last_opening_bracket == '{' && character == '}') ||
          (last_opening_bracket == '(' && character == ')') ||
          (last_opening_bracket == '[' && character == ']')) {
        opened.pop();

      } else {
        return false;
      }
    }
  }
  return opened.empty() == true;
}

int main() {
  assert(is_balanced("[]") == true);
  assert(is_balanced("([])") == true);
  {
    string s = "{[{(({}))}]}";
    assert(is_balanced(s) == true);
  }
  assert(is_balanced("") == true);
  assert(is_balanced("([)]") == false);
  {
    string s = "{(])}";
    assert(is_balanced(s) == false);
  }
  {
    string s = "{[}]";
    assert(is_balanced(s) == false);
  }
  assert(is_balanced("[()][") == false);

  return 0;
}
