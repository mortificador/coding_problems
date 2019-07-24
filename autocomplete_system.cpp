/*
Implement an autocomplete system. That is, given a query string s and a set of all possible query strings, return all strings in the set that have s as a prefix.

For example, given the query string de and the set of strings [dog, deer, deal], return [deer, deal].
*/

/*
I've solved this implementing a similar (but not exactly) data structure to a trie.
It's basically a tree, where each node represents a letter, and contains a map of children. In the map of children, the key is the 
letter that the child is associated to, and the value is the child itself. It allows for fast retrieval of all words that start with a given
prefix.

To compile this code you need to pass: -std=c++17 (use of if init, try_emplace in unordered_map and structured bindings)

E.g.
If we insert the word "cat" in the trie, this is how the data structure would look like:
                       trie
                     /
                  trie_node:
                _letter: "c"
                  /
              trie_node:
             _letter: "a"
               /   
           trie_node:
           _letter: "t"
           _end_of_word: true

If now we add the word cats:
                       trie
                     /
                  trie_node:
                _letter: "c"
                  /
              trie_node:
             _letter: "a"
               /   
          trie_node:
          _letter: "t"
          _end_of_word: true
            /   
       trie_node:
       _letter: "s"
       _end_of_word: true

Finally, if we add the word "bear":

                      __trie__
                     /        \
                  trie_node:   trie_node:
                _letter: "c"   _letter: "b"
                  /               \
              trie_node:        trie_node:
             _letter: "a"       _letter: "e"
               /                    \
          trie_node:             trie_node:
          _letter: "t"           _letter: "a"
          _end_of_word: true         \
            /                     trie_node:
       trie_node:                 _letter: "r" 
       _letter: "s"               _end_of_word: true
       _end_of_word: true


In order to search all the words that start with a given prefix, we just start going through the nodes that correspond to the prefix, and once
we arrive to the node that represents the last letter in the prefix, we now go through all it's children, in a recursive way, and for every node
that we find that has _end_of_word set to true, we add the word to the list of words found.

E.g:
Given the following trie:

                      __trie__
                     /        \
                  trie_node:   trie_node:
                _letter: "c"   _letter: "b"
                  /               \
              trie_node:        trie_node:
             _letter: "a"       _letter: "e"
               /                    \
          trie_node:             trie_node:
          _letter: "t"           _letter: "a"
          _end_of_word: true         \
            /                     trie_node:
       trie_node:                 _letter: "r" 
       _letter: "s"               _end_of_word: true
       _end_of_word: true

And the prefix "cat" we would:
Go through the trie: first get the child that represents the letter "c".

                      __trie__
                     /        \
node letter c->  trie_node:   trie_node:
                _letter: "c"   _letter: "b"
                  /               \
              trie_node:        trie_node:
             _letter: "a"       _letter: "e"
               /                    \
          trie_node:             trie_node:
          _letter: "t"           _letter: "a"
          _end_of_word: true         \
            /                     trie_node:
       trie_node:                 _letter: "r" 
       _letter: "s"               _end_of_word: true
       _end_of_word: true

Now, among the children of that node, find the one that represents the letter "a":

                      __trie__
                     /        \
                  trie_node:   trie_node:
                _letter: "c"   _letter: "b"
                  /               \
node letter a->trie_node:       trie_node:
              _letter: "a"      _letter: "e"
               /                    \
          trie_node:             trie_node:
          _letter: "t"           _letter: "a"
          _end_of_word: true         \
            /                     trie_node:
       trie_node:                 _letter: "r" 
       _letter: "s"               _end_of_word: true
       _end_of_word: true

Now, among the children of that node, find the one that represents the letter "t":
         
                              __trie__
                             /        \
                          trie_node:   trie_node:
                         _letter: "c"   _letter: "b"
                           /               \
                      trie_node:       trie_node:
                     _letter: "a"      _letter: "e"
                        /                    \
node letter t->     trie_node:             trie_node:
                   _letter: "t"           _letter: "a"
                   _end_of_word: true         \
                       /                     trie_node:
                   trie_node:                 _letter: "r" 
                   _letter: "s"               _end_of_word: true
                   _end_of_word: true

Finally, now that we are in the node that represents the last letter in the prefix, we have to return the words represented by all the nodes
under it that represent a word (has _end_of_word set to true). In this case, there are two nodes, that represent the words: "cat" and "cats".
*/

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <cassert>

using namespace std;

struct trie_node {
public:
  trie_node(char letter, bool is_end_of_word) : _letter{letter}, _is_end_of_word{is_end_of_word} {}

  // Add a word to the current trie. word[next_pos] tracks what is the next letter (next node) to add
  void add(string const& word, size_t next_pos) {
    bool is_next_end_of_word = next_pos == (word.size() - 1);
    auto [it, inserted] = _children.try_emplace(word[next_pos], make_unique<trie_node>(word[next_pos], is_next_end_of_word));

    // If we just created the node representing the last letter in the word, then finish
    if (is_next_end_of_word == true) {
      return;
    }

    // Tell the next node to keep adding
    it->second->add(word, next_pos + 1);
  }

  // This method get all the words that start with the prefix passed
  void get_words(string const& prefix, size_t curr_pos, string previous_characters, vector<string>& words_found) {
    previous_characters.push_back(_letter);
    // If the prefix is empty, or we are at the end of the prefix, or past the size of the prefix,
    // then search in every child, and add every word that we find
    if ((prefix.empty() == true) || (curr_pos >= (prefix.size() - 1))) {
      // If all the characters so far correspond to a word, add it to the list of words found
      if (_is_end_of_word == true) {
        words_found.push_back(previous_characters);
      }
      // Search in every child for more words
      for (auto& [key, child] : _children) {
        child->get_words(prefix, curr_pos, previous_characters, words_found);
      }
    }
    // If we are not at the end of the prefix, keep searching in our children
    else {
      auto next_pos = curr_pos + 1;
      char next_letter = prefix[next_pos];
      auto it = _children.find(next_letter);
      // If we don't have a child that contains the next letter of the prefix, then return.
      if (it == end(_children)) {
        return;
      }
      // If we have a child that contains the next letter in the prefix, keep the search in the child
      else {
        it->second->get_words(prefix, next_pos, previous_characters, words_found);
      }
    }
  }

private:
  unordered_map<char, unique_ptr<trie_node>> _children;
  char _letter; // letter representing this node. We could do without this, as our ancestor will have the letter associated with the node in its 
               // _children map, but it's convenient to have it here
  bool _is_end_of_word; // If this node marks the end of a word, this is true.
};

// This data structure is similar to a trie
struct trie {
public:
  trie(vector<string> words) {
    for (const auto& word : words) {
      add(word);
    }
  }

  void add(string const& word) {
    auto [letter_child, inserted] = roots.try_emplace(word[0], make_unique<trie_node>(word[0], word.size() == 1));
    letter_child->second->add(word, 1);
  }

  vector<string> get_words(string const& prefix) {
    vector<string> words_found;
    // prefix empty means: return all the words
    if (prefix.empty() == true) {
      for (auto& [letter, child] : roots) {
        child->get_words(prefix, 0, string{}, words_found);
      }
    }
    // normal search: we start with the root of the prefix
    else if (auto first_letter = prefix[0]; roots.count(first_letter) > 0) {
      roots[first_letter]->get_words(prefix, 0, string{}, words_found);
    }

    return words_found;
  }

private:
  unordered_map<char, unique_ptr<trie_node>> roots;
};

int main() {
  {
    trie t{{"dog", "deer", "deal"}};
    auto words = t.get_words("de");
    assert(words.size() == 2);
    assert(find(begin(words), end(words), string("deal")) != end(words));
    assert(find(begin(words), end(words), string("deer")) != end(words));
  }
  {
    trie t{{"beer", "deer", "deal", "bear", "bearer", "cat"}};
    auto words = t.get_words("be");
    assert(words.size() == 3);
    assert(find(begin(words), end(words), string("bear")) != end(words));
    assert(find(begin(words), end(words), string("bearer")) != end(words));
    assert(find(begin(words), end(words), string("beer")) != end(words));

    words = t.get_words("d");
    assert(words.size() == 2);
    assert(find(begin(words), end(words), string("deer")) != end(words));
    assert(find(begin(words), end(words), string("deal")) != end(words));
    
    words = t.get_words("beer");
    assert(words.size() == 1);
    assert(find(begin(words), end(words), string("beer")) != end(words));
  }
  {
    trie t{{"beer", "deer", "cat"}};
    auto words = t.get_words("");
    assert(words.size() == 3);
    assert(find(begin(words), end(words), string("beer")) != end(words));
    assert(find(begin(words), end(words), string("deer")) != end(words));
    assert(find(begin(words), end(words), string("cat")) != end(words));
  }
}
