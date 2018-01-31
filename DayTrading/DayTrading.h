#ifndef DAYTRADING_H
#define DAYTRADING_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class Item {
 public:
  Item();
  virtual ~Item();
  void SetFstream(const string& file_name);
  void SetInitPrice(long time);
  float ReadPrice();

  float init_price;
  int how_long_so_far;
  int item_no;
  int bought_shares;
  fstream bought_item;

 private:
};

class DayTrading {
 public:
  DayTrading();
  virtual ~DayTrading();
  float Invest(float money, int items, float margin, int hold, long term);

 private:
};

#endif // DAYTRADING_H
