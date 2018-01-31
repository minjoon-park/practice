#include "my_hash.h"
#include <fstream>
#include <string>

using namespace std;

int main() {
  HashOA data;
  fstream data_file("stock15.txt", ios_base::in);

  int buffer;
  while(data_file >> buffer) {
    data.Insert(buffer);
  }
  data_file.close();

  data.Print();

 // data.Delete(2);

//  data.Print();

  return 0;
}
