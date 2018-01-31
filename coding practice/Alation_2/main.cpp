// A solution for "Indexing" problem by Minjoon Park, ver.2.1

#include "RadixTree.h"

using namespace std;

/*/ Driver for building Index. Run this main() first to build Index file, "tree.txt"
int main() {
  rTree test;
  test.ImportDB("db_s.txt");
  test.BuildTree();

  test.WriteTree("tree.txt");

  return 0;
}
*/

// Driver for searching for, adding, removing an DB entry
int main() {
  rTree test;
  test.ImportDB("db_s.txt");
  test.ReadTree("tree.txt");

  string query;
  int sel, id, score;

  cout << "\nEnter 0 to insert an entry to DB. Input format: string name, int score.\n";
  cout << "Enter 1 to search the Index. Input format: string query\n";
  cout << "Enter 2 to remove an entry. Input format: string name, int id, int score \n";
  cout << "Enter 9 to quit\n\n";

  while (cin >> sel) {
    if (sel == 0) {
      cin >> query >> score;
      test.Insert(query, score);
      cout << endl;
    }
    else if (sel == 1) {
      cin >> query;
      test.wordsStartingWith(query);
      cout << endl;
    }
    else if (sel == 2) {
      cin >> query >> id >> score;
      dbEntry entry(query, id, score);
      test.Remove(entry);
      cout << endl;
    }
    else
      break;
  }

  return 0;
}

