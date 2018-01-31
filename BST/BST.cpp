#include "BST.h"

Node::Node() {
  parent = NULL;
  left = NULL;
  right = NULL;
}

/*Node::Node(const Node& source) {
  key = source.key;
  parent = source.parent;
  left = source.left;
  right = source.right;
}*/


Node::~Node() {

}


BST::BST() {
  p_root = NULL;
}


void BST::InOrderTreeDelete(Node* p_start) {
  if(p_start != NULL) {
    InOrderTreeDelete(p_start->left);
    //cout << "Node: " << p_start->key << " is deleted." << endl;
    delete p_start;
    InOrderTreeDelete(p_start->right);
  }
}


BST::~BST() {
  InOrderTreeDelete(p_root);
}


void BST::InOrderTreeWalk(Node* p_start) {
  if(p_start != NULL) {
    InOrderTreeWalk(p_start->left);
    cout << p_start->key << '\t';
    InOrderTreeWalk(p_start->right);
  }
}


Node* BST::GetRoot() {
  return p_root;
}


Node* BST::Search(float search_key) {
  Node* p_node = p_root;
  while(p_node != NULL && search_key != p_node->key) {
     if(search_key >= p_node->key)
      p_node = p_node->right;
    else
      p_node = p_node->left;
  }
  return p_node;
}


Node* BST::Min(Node* p_node) {
  while(p_node->left != NULL)
    p_node = p_node->left;
  return p_node;
}


Node* BST::Max(Node* p_node) {
  while(p_node->right != NULL)
    p_node = p_node->right;
  return p_node;
}


Node* BST::Successor(Node* p_node) {
  if(p_node->right != NULL)
    return Min(p_node->right);

  Node* p_node_parent = p_node->parent;
  while(p_node_parent != NULL && p_node_parent->right == p_node) {
    p_node = p_node_parent;
    p_node_parent = p_node_parent->parent;
  }
  return p_node;
}


Node* BST::Predecessor(Node* p_node) {
  if(p_node->left != NULL)
    return Max(p_node->left);

  Node* p_node_parent = p_node->parent;
  while(p_node_parent != NULL && p_node_parent->left == p_node) {
    p_node = p_node_parent;
    p_node_parent = p_node_parent->parent;
  }
  return p_node;
}


void BST::Insert(float key_val) {
  Node* p_node_parent = NULL;
  Node* p_node = p_root;

  while(p_node != NULL) {
    p_node_parent = p_node;
    if(key_val >= p_node->key)
      p_node = p_node->right;
    else
      p_node = p_node->left;
  }

  Node* new_Node = new Node;
  new_Node->key = key_val;
  new_Node->parent = p_node_parent;
  new_Node->left = NULL;
  new_Node->right = NULL;
  if(new_Node->parent == NULL)
    p_root = new_Node;
  else {
    if(p_node_parent->key >= new_Node->key)
      p_node_parent->left = new_Node;
    else
      p_node_parent->right = new_Node;
  }
}


void BST::Build(fstream& data_file) {
  float temp;
  while(data_file >> temp)
    Insert(temp);
}


void BST::Delete(float key_val) {
  Node* del_node = Search(key_val);
  Node* cut_node;
  Node* paste_node;

  if(del_node->left == NULL || del_node->right == NULL)
    cut_node = del_node;
  else
    cut_node = Successor(del_node);

  if(cut_node->left != NULL)
    paste_node = cut_node->left;
  else
    paste_node = cut_node->right;

  if(paste_node != NULL)
    paste_node->parent = cut_node->parent;

  if(cut_node->parent == NULL)
    cut_node = p_root;
  else
    if(cut_node->parent->left == cut_node)
      cut_node->parent->left = paste_node;
    else
      cut_node->parent->right = paste_node;

  if(cut_node != del_node)
    del_node->key = cut_node->key;
}


void BST::PostOrderPrint(Node* p, string indent) {
  if(p != NULL) {
    if(p->left) {
      PostOrderPrint(p->left, indent);
      indent += '\t';
    }
    if(p->right)
      PostOrderPrint(p->right, indent + '\t');
    cout << indent << p->key << endl;
  }
}


void BST::InOrderTreeWalkPrint(Node* p_start, string path) {
  if(p_start != NULL) {
    InOrderTreeWalkPrint(p_start->left, path + "L");
    cout << path << "-> " << p_start->key << endl;
    InOrderTreeWalkPrint(p_start->right, path + "R");
  }
}

int BST::Depth() {
  Node* x = GetRoot();
  int temp = 0, depth = 0;

  while(x != NULL) {
    if(x->left != NULL)


  }

}






void BST::Insert2(float key_val) {
  Node* p_node_parent = NULL;
  Node* p_node = p_root;
  string path = "\0";

  cout << key_val << ": ";
  while(p_node != NULL) {
    p_node_parent = p_node;
    if(key_val >= p_node->key) {
      p_node = p_node->right;
      path += "1";
    }
    else {
      p_node = p_node->left;
      path += "0";
    }
  }
  cout << endl;

  Node* new_Node = new Node;
  new_Node->key = key_val;
  new_Node->parent = p_node_parent;
  new_Node->left = NULL;
  new_Node->right = NULL;
  if(new_Node->parent == NULL)
    p_root = new_Node;
  else {
    if(p_node_parent->key >= new_Node->key)
      p_node_parent->left = new_Node;
    else
      p_node_parent->right = new_Node;
  }

  elmts.insert(make_pair(path.size(), make_pair(key_val, path)));
}


void BST::Build2(fstream& data_file) {
  float temp;
  while(data_file >> temp)
    Insert2(temp);
}


void BST::Delete2(float key_val) {
  Node* del_node = Search(key_val);
  Node* cut_node;
  Node* paste_node;

  if(del_node->left == NULL || del_node->right == NULL)
    cut_node = del_node;
  else
    cut_node = Successor(del_node);

  if(cut_node->left != NULL)
    paste_node = cut_node->left;
  else
    paste_node = cut_node->right;

  if(paste_node != NULL)
    paste_node->parent = cut_node->parent;

  if(cut_node->parent == NULL)
    cut_node = p_root;
  else
    if(cut_node->parent->left == cut_node)
      cut_node->parent->left = paste_node;
    else
      cut_node->parent->right = paste_node;

  if(cut_node != del_node)
    del_node->key = cut_node->key;
}

/*
void BST::PrintTree() {
  stoi(string, 2)

} */
