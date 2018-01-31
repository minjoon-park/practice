/**
With $50k seed money, holding at most 1000 turns, quit at 10k turns, invest in
10 items with 5% margin: 11.861%
10 items with 10% margin: 14.51%
10 items with 15% margin: 14.51%

14 items with 10% margin: 17.083

15 items with 5% margin: 16.311%
15 items with 8% margin: 8.366
15 items with 9% margin: 6.34
15 items with 10% margin: 20.952%
15 items with 10.5% margin: 16.609
15 items with 11% margin: 16.609
15 items with 15% margin: 16.609%

16 items with 10% margin: 9.944

20 items with 5% margin: 11.693%
20 items with 10% margin: 9.448%
20 items with 15% margin: 13.442%
20 items with 20% margin: 13.442%

quit at 15k turns
15 items with 10% margin: 19%

*/


#include <iostream>

#include "DayTrading.h"

using namespace std;

int main() {
  DayTrading trial;
  cout << trial.Invest(50000.0, 15, .10, 1000, 15000);

/*
  Item test;
  test.SetFstream("C:\\Users\\Minjoon\\Documents\\Dropbox\\works\\stock_data_agnostic\\(40).csv");
  test.SetInitPrice(40);
  cout << test.init_price << endl;
  cout << test.ReadPrice() << endl;
  cout << test.ReadPrice() << endl;
  cout << test.ReadPrice() << endl;
*/
  return 0;
}
