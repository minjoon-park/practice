#include <iostream>
#include "inverted_index.h"

using namespace std;

int main() {
  //InvertedIndex sample("C:\\Users\\Minjoon\\Documents\\Dropbox\\works\\sample");

  InvertedIndex sample("C:\\Users\\Minjoon\\Documents\\Dropbox\\works\\content_hillaryclinton_s1992_copypruned");

  sample.BuildInvertedIndex();

// sample.SearchAndPrint();

  while(1)
    sample.VectorSpaceSearch();

  return 0;
}
