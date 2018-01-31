// A solution for "Indexing" problem by Minjoon Park

/*
I do not use B-tree or any other standard DB index data structure. My idea is that

1. We prepare a container, T, for (name, id)-pairs, and lexicographically sort them
according to names. Here "id" means the index of (name, score)-pair of the original dataset,
i.e., its location within the dataset.

2. If "name" has '_" in it, separate substring after '_' and insert it into T with the
same "id". This is to allow "_query".

3. Once a query, Q, is given, binary-search T for its appearance. If it doesn't show up,
find the appearance of the string closest but lexicographically larger than it.

4. Increase the last character of Q by 1, and binary-search T for its appearance.
If it doesn't show up, find the appearance of the string closest but lexicographically
smaller than it.

5. All the strings between the two strings found in 3 and 4 are the answer candidate.
Put them in a priority_queue one by one, while popping out the one with the smallest score
to keep the size of the queue <= 10.

6. Complexity is
  building-index: ~ n log n (sorting)
  searching: ~ log n (binary search)
                + k (picking up 10 highest score names, if there are more than 10 candidates)

This is a solution rather specialized to the given problem setting, such as only O(1M)
data entries, only two data columns, no restrictions on efficiency of inserting/deleting an
entry, etc.
*/


#ifndef DB_H
#define DB_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <set>

using namespace std;

struct v_Node {   // struct for an entry of the Index
  string name;
  vector<int> ids;    // "id" of (name, score)-pair within the dataset.
  // Even if the dataset doesn't have duplicating elements, there can be more than one "id"s
  // for the same "name", because we separately add substrings obtained from names with '_'.

  v_Node(string v_name, vector<int>& v_ids) {
    name = v_name;
    ids = v_ids;
  }
};

class DBI {
 public:
  DBI(string file_name);

 private:
  string db_file_name;
  vector<v_Node> idx;   // The Index.

  void FillIdx();   // Build the Index.
};

class DBS {
 public:
  DBS(string db_file, string idx_file);

  void Search(string query);    // Search "query" and print out 10 highest score matches.

 private:
  string db_file_name, idx_file_name;
  vector< pair<string, int> > db;   // Stores the original dataset.
  vector<v_Node> idx;
  int v_len;    // (size - 1) of the Index

  void LoadIdx();   // Load dataset and Index onto memory.
  int BiSearch(string query, int s, int e, char l_or_u);    // Binary-search Index for "query".
};


#endif // DB_H
