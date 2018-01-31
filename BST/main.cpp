#include "BST.h"

int main() {
  BST stock15;

  fstream data_file("stock15.txt", ios_base::in);

  stock15.Build(data_file);
  data_file.close();

  Node* found_node = stock15.Search(24);
  if (found_node != NULL) {
    cout << found_node->key << endl;
  }
  else
    cout << "No node with the given key can be found.\n";

  cout << stock15.Min(stock15.GetRoot())->key << endl;

  found_node = stock15.Search(27.125);
  if (found_node != NULL) {
    cout << "Successor of 27.125 is " << stock15.Successor(found_node)->key << endl;
  }
  else
    cout << "No node with the given key can be found.\n";

 // cout << endl;

 // stock15.PostOrderPrint(stock15.GetRoot(),"");

  cout << endl << endl;

  stock15.InOrderTreeWalkPrint(stock15.GetRoot(), "\0");

  cout << endl << endl;


  stock15.Delete(27.125);
  cout << "Node with 27.125 is deleted." << endl;

  cout << endl << endl;

  stock15.InOrderTreeWalkPrint(stock15.GetRoot(), "\0");

  string s = "1000";
  cout << stoi(s, NULL, 2) << endl;

  return 0;
}
