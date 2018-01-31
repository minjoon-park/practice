#include "my_hash.h"


Item::Item() {
  next = NULL;
}


Item::~Item() {
}


HashChain::HashChain() {
  for(int i = 0; i < TABLESIZE; ++i)
    hash_table[i] = NULL;
}


HashChain::~HashChain() {
  for(int i = 0; i < TABLESIZE; ++i) {
    Item* temp = hash_table[i];
    Item* buffer;

    for(int j = 0; temp != NULL; ++j) {
      buffer = temp;
      temp = temp->next;
      delete &(*buffer);
    }
  }
}


void HashChain::Insert(int key_val) {
  Item* new_item = new Item;
  new_item->key = key_val;
  new_item->next = hash_table[HashFtn(key_val)];
  hash_table[HashFtn(key_val)] = new_item;
}


Item* HashChain::Search(int key_val) {
  Item* temp = hash_table[HashFtn(key_val)];
  while(temp != NULL) {
    if(temp->key == key_val) {
      cout << "Item found.\n";
      return temp;
    }
    temp = temp->next;
  }
  cout << "No item found for the given key.\n";
  return NULL;
}


void HashChain::Delete(int key_val) {
  Item* temp = hash_table[HashFtn(key_val)];
  Item* buffer;

  while(temp != NULL) {
    if(temp->key == key_val) {
      if(temp == hash_table[HashFtn(key_val)])
        hash_table[HashFtn(key_val)] = temp->next;
      else
        buffer->next = temp->next;

      delete &(*temp);
      cout << "Item deleted.\n";
      return;
    }
    buffer = temp;
    temp = temp->next;
  }

  cout << "No item found for the given key.\n";
  return;
}


void HashChain::Print() {
  for(int i = 0; i < TABLESIZE; ++i) {
    Item* temp = hash_table[i];

    while(temp != NULL) {
      cout << temp->key << " ";
      temp = temp->next;
    }
    cout << endl;
  }
  cout << endl;
}


ItemOA::ItemOA() {
}


ItemOA::~ItemOA() {
}


HashOA::HashOA() {
  for(int i = 0; i < LargeTABLESIZE; ++i)
    hash_table[i] = NULL;
  ItemOA* DEL = new ItemOA;
  DEL->key = -100;
}


HashOA::~HashOA() {
  for(int i = 0; i < TABLESIZE; ++i)
    delete &(*hash_table[i]);
}


void HashOA::Insert(int key_val) {
  ItemOA* new_item = new ItemOA;
  new_item->key = key_val;

  for(int i = 0; i < LargeTABLESIZE; ++i) {
    ItemOA* temp = hash_table[HashFtnOA(key_val, i)];
    if(temp == NULL || temp == DEL) {
      hash_table[HashFtnOA(key_val, i)] = new_item;
      return;
    }
  }

  cout << "Hash table overflow.\n";
  return;
}


ItemOA* HashOA::Search(int key_val) {
  for(int i = 0; i < LargeTABLESIZE; ++i) {
    ItemOA* temp = hash_table[HashFtnOA(key_val, i)];

    if(temp == NULL) {
      cout << "No item found for the given key.\n";
      return NULL;
    }
    if(temp->key == key_val)
      return temp;
  }

  cout << "No item found for the given key.\n";
  return NULL;
}


void HashOA::Delete(int key_val) {
  for(int i = 0; i < LargeTABLESIZE; ++i) {
    ItemOA* temp = hash_table[HashFtnOA(key_val, i)];

    if(temp == NULL) {
      cout << "No item found for the given key.\n";
      return;
    }
    if(temp->key == key_val) {
      delete &(*temp);
      temp = DEL;
      return;
    }
  }

  cout << "No item found for the given key.\n";
  return;
}


void HashOA::Print() {
  for(int i = 0; i < LargeTABLESIZE; ++i) {
    if(hash_table[i] == NULL) {
      cout << "empty" << endl;
      continue;
    }
    cout << hash_table[i]->key << endl;
  }
  cout << endl;
}
