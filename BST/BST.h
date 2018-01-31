#ifndef BST_H
#define BST_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>


using namespace std;

class Node {
 public:
  Node();
  //Node(const Node& source);
  ~Node();

  float key;
  Node* left;
  Node* right;
  Node* parent;
    // satellite data
  //friend class BST;
};

class BST {
  public:
    BST();
    ~BST();

    void InOrderTreeWalk(Node* p_start);
    void InOrderTreeDelete(Node* p_start);
    Node* GetRoot();
    Node* Search(float search_key);
    Node* Min(Node* p_node);
    Node* Max(Node* p_node);
    Node* Successor(Node* p_node);
    Node* Predecessor(Node* p_node);
    void Insert(float key_val);
    void Build(fstream& data_file);
    void Delete(float key_val);
    void PostOrderPrint(Node* p, string indent);
    void InOrderTreeWalkPrint(Node* p_start, string path);
    int Depth();

    void Insert2(float key_val);
    void Build2(fstream& data_file);
    void Delete2(float key_val);

  private:
    Node* p_root;
    multimap< int, pair<float, string> > elmts;
};

#endif // BST_H
