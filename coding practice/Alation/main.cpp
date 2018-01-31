// A solution for "Indexing" problem by Minjoon Park

#include <iostream>

#include "DB.h"

using namespace std;

// Driver for building index. Run this main() first to build Index file, "idx.txt"
int main() {
  DBI test_i("db_s.txt");

  return 0;
}


// Driver for querying
int main() {
  DBS test_s("db_s.txt", "idx.txt");

  string query;
  cout << "Enter your query. Type \"###\" to quit.\n";

  while (cin >> query) {
    if (query == "###")
      break;

    test_s.Search(query);

    cout << endl;
  }

  return 0;
}

