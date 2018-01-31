#ifndef MY_HASH_H
#define MY_HASH_H

#include <iostream>
#define TABLESIZE 10
#define LargeTABLESIZE 20

using namespace std;

class Item {
 public:
  Item();
  virtual ~Item();

 private:
  int key;
  Item* next;
  // satellite data
  friend class HashChain;
};


class HashChain {
 public:
  HashChain();
  virtual ~HashChain();
  void Insert(int key_val);
  Item* Search(int key_val);
  void Delete(int key_val);
  void Print();

 private:
  int HashFtn(int key_val) {
    return key_val % TABLESIZE;
  }

  Item* hash_table[TABLESIZE];
};


class ItemOA {
 public:
  ItemOA();
  virtual ~ItemOA();

 private:
  int key;
  // satellite data
  friend class HashOA;
};


class HashOA {
 public:
  HashOA();
  virtual ~HashOA();
  void Insert(int key_val);
  ItemOA* Search(int key_val);
  void Delete(int key_val);
  void Print();

 private:
  int HashFtnOA(int key_val, int i) {
    return (key_val % LargeTABLESIZE + i * (1 + key_val % (LargeTABLESIZE - 1))) % LargeTABLESIZE;
  }

  ItemOA* hash_table[LargeTABLESIZE];
  ItemOA* DEL;
};


#endif // MY_HASH_H
