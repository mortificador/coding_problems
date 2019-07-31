/*
Suppose we represent our file system by a string in the following manner:

The string "dir\n\tsubdir1\n\tsubdir2\n\t\tfile.ext" represents:

dir
    subdir1
    subdir2
        file.ext

The directory dir contains an empty sub-directory subdir1 and a sub-directory subdir2 containing a file file.ext.

The string "dir\n\tsubdir1\n\t\tfile1.ext\n\t\tsubsubdir1\n\tsubdir2\n\t\tsubsubdir2\n\t\t\tfile2.ext" represents:

dir
    subdir1
        file1.ext
        subsubdir1
    subdir2
        subsubdir2
            file2.ext

The directory dir contains two sub-directories subdir1 and subdir2. subdir1 contains a file file1.ext and an empty second-level sub-directory subsubdir1. subdir2 contains a second-level sub-directory subsubdir2 containing a file file2.ext.

We are interested in finding the longest (number of characters) absolute path to a file within our file system. For example, in the second example above, the longest absolute path is "dir/subdir2/subsubdir2/file2.ext", and its length is 32 (not including the double quotes).

Given a string representing the file system in the above format, return the length of the longest absolute path to a file in the abstracted file system. If there is no file in the system, return 0.

Note:

The name of a file contains at least a period and an extension.

The name of a directory or sub-directory will not contain a period.
*/

/*
By reading at the description of the problem and looking at the examples, we notice that they give the path in a depth-first fashion.
E.g:

dir
    subdir1
    subdir2
      file.ext

The string that represents this filesystem is: "dir\n\tsubdir1\n\tsubdir2\n\t\tfile.ext":
start with dir, and go through every subfolder
   subdir1 is a subfolder, go through every subfolder
      subdir1 does not have any other subfolders
   subdir2 is a subfolder, go through every subfolder
      file.ext is a file inside subdir2

As we said, it's built following a depth-first approach. We know that file.ext is inside subdir2 because the last folder that we inspected was subdir2.

In order to count the longest absolute path, we can do the following:
Keep track of the current absolute path, as a vector or a stack of subdirs (e.g.: for the path dir/subdir1/subsubdir2 the current absolute path would contain:
{dir, subdir1, subsubdir2})
Read the root, and add it to curr_abs_path. Update also the current length of the current absolute path.
 While we haven't read the full string:
  Read the next depth (we call depth to the number of \n and \t). Read the name corresponding to that depth (e.g: "dir\n\tsubdir1", if already read "dir", then
  the next depth would be 2, and the name would be "subdir1").
  If the depth is larger or equal than the number of elements in the current absolute path, then:
     start deleting the last elements in the current absolute path
     until the depth is smaller than the number of elements in the current absolute path. Let's see why:

        "dir\n\tsubdir1\n\tsubdir2\n\t\tsubsubdir3\n\t\t\tfile.ext", let's say that we just read "subdir2", then: 
        depth = 2
        current_abs_path = {dir, subdir1}
        name = subdir2

        As we can see, if the depth is equals to the size of current_abs_path, this means that the current name (subdir2 in this case) is at the same level as the 
        last element in current_abs_path (subdir1 in this example), and because the string follows a depth-first approach, it means that there's nothing 
         else on subdir1. So, we need to pop subdir1 out of current_abs_path.

  After the previous step, now current_abs_path contains the parent absolute path of the name that we just read. Now have to do either:
    if the name that we just read is a file, then update the largest_path seen, taking into account the number of '/' that there would be in the path (the number
    of '/' is equal to the size of current_abs_path).
  
    if the name that we just read is a subdirectory, then add it to current_abs_path

  Continue the loop
 Finally, return longest_path

E.g:

s = "dir\n\tsubdir1\n\tsubdir2\n\t\t\tfile.ext",
which corresponds to:
dir
    subdir1
    subdir2
      file.ext

We first read the root and add it to curr_abs_path
curr_abs_path = {dir}
curr_length = 3
longest_path = 3

Now start the loop:
Read next depth -> depth = 2
Read next name  -> name  = subdir1
Pop out elements of curr_abs_path until there are less elements than depth
curr_abs_path = {dir} (nothing to pop out)
name (subdir1) is not a file name, so we add it to curr_bas_path and update curr_length
curr_abs_path = {dir, subdir1}
curr_length = 10

Next iteration of the loop
Read next depth -> depth = 2
Read next name  -> name  = subdir2
Pop out elements of curr_abs_path until there are less elements than depth
curr_abs_path = {dir} (we pop'd out subdir1)
name (subdir2) is not a file name, so we add it to curr_bas_path and update curr_length
curr_abs_path = {dir, subdir2}
curr_length = 10

Next iteration of the loop
Read next depth -> depth = 3
Read next name  -> name  = file.ext
Pop out elements of curr_abs_path until there are less elements than depth
curr_abs_path = {dir, subdir2} (nothing to pop out)
name (file.ext) is a file name, so we update max_length (we add curr_abs_path.size() to take into account the '/' in the path):
max_length = max(max_length, curr_length + name.size() + curr_abs_path.size()) = 20

The loop finishes: max_length = 20

In order to compile this exercise you need to add the following flag to the compiler: -std=c++17
*/

#include <algorithm>
#include <string>
#include <tuple>
#include <vector>
#include <cassert>

using namespace std;

// This function returns the name of the next directory or filename from the string s, starting at index start_index.
// It also returns the index of s where the name finishes
tuple<string, size_t> read_name(string const& s, size_t start_index) {
  string name;
  while (start_index < s.size() && s[start_index] != '\n' && s[start_index] != '\t') {
    name.push_back(s[start_index]);
    start_index++;
  }

  return tie(name, start_index);
}

// This function reads the depth of the next directory or filename, where the depth is the sum of the number of characters \n and \t read
// It returns the depth and also the index where the next name starts.
tuple<int, size_t> read_depth(string const& s, size_t start_index) {
  int depth = 0;
  while (s[start_index] == '\n' || s[start_index] == '\t') {
    depth++;
    start_index++;
  }

  return tie(depth, start_index);
}

bool is_file(string const& s) {
  return (find(begin(s), end(s), '.') != end(s));
}

int longest_absolute_path(string s) {
  // We assume that there will be at least one directory (the root directory)
  vector<string> current_path; // the current_path that we are processing.
  size_t longest_path = 0;     // the current longest absolute path for a file that we have seen
  size_t current_length = 0;   // the length of the current absolute path
  size_t current_index = 0;    // current index in the string s

  // Read the root path, and update the current_index
  auto [root_path, end_index] = read_name(s, 0);
  current_index = end_index;
  current_length = root_path.size();
  current_path.emplace_back(move(root_path));

  while (current_index < s.size()) {
    // current_index points to to the start of a depth, read the depth corresponding to the next name that we will read
    auto [depth, name_start_index] = read_depth(s, current_index);
    // name_start_index points to the start of the name corresponding to the depth that we just read
    // read the name
    auto [name, name_end_index] = read_name(s, name_start_index);

    // If the name that we just read is is not a subdirectory of the current path, then we need to pop out
    // the subdirectories in the current path until we are at the parent folder. We update the current length too
    while (current_path.size() >= depth) {
      string& last_dir = current_path.back();
      current_length -= last_dir.size();
      current_path.pop_back();
    }

    // If the last name that we read corresponds to a name, update (if necessary) the longest path,
    // taking into account the number of '/' in the absolute path
    if (is_file(name) == true) {
      // We need to take into account the number of '/' that will appear in a path
      size_t number_of_slashes = current_path.size();
      longest_path = max(current_length + name.size() + number_of_slashes, longest_path);
    } else {
      // If it was a folder, update the current length, and update the current path so that it includes the folder
      current_length += name.size();
      current_path.emplace_back(move(name));
    }
    current_index = name_end_index;
  }

  return longest_path;
}

int main() {
  {
    string filesystem = "dir\n\tsubdir1\n\tsubdir2\n\t\tfile.ext";
    assert(longest_absolute_path(filesystem) == 20);
  }
  {
    string filesystem = "dir\n\tsubdir1\n\t\tfile1.ext\n\t\tsubsubdir1\n\tsubdir2\n\t\tsubsubdir2\n\t\t\tfile2.ext";
    assert(longest_absolute_path(filesystem) == 32);
  }
  return 0;
}
