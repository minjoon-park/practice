#include "DayTrading.h"


Item::Item() {
  how_long_so_far = 0;
}


Item::~Item() {
  bought_item.close();
}


void Item::SetFstream(const string& file_name) {
  bought_item.open(file_name, ios_base::in);
}


void Item::SetInitPrice(long time) {
  string temp_line;
  int line = 0;
  while(getline(bought_item, temp_line)) {
    ++line;
    if(line - 1 == time)
      break;
  }

  string init_price_str;
  int count_pos = 0;
  for(char temp : temp_line) {
    if(temp == ';')
      ++count_pos;
    if(count_pos == 3) {
      if(temp == ';')
        continue;
      if(temp == ',')
        temp = '.';
      init_price_str += temp;
    }
    if(count_pos == 4)
      break;
  }

  init_price = stof(init_price_str);
}


float Item::ReadPrice() {
  string temp_line;
  getline(bought_item, temp_line);

  string price_str;
  int count_pos = 0;
  for(char temp : temp_line) {
    if(temp == ';')
      ++count_pos;
    if(count_pos == 3) {
      if(temp == ';')
        continue;
      if(temp == ',')
        temp = '.';
      price_str += temp;
    }
    if(count_pos == 4)
      break;
  }

  return stof(price_str);
}



DayTrading::DayTrading() {
}


DayTrading::~DayTrading() {
}


float DayTrading::Invest(float money, int no_of_items, float profit_margin,
                         int how_long_to_hold, long running_term) {
  float balance = money;
  long time = 1;

  Item item_stack[no_of_items];
  for (int i = 0; i < no_of_items; ++i) {
    int item_no;
    bool check;
    do {
      check = false;
      item_no = rand() % 52 + 1;
      for (int j = 0; j < i; ++j)
        if (item_stack[j].item_no == item_no)
          check = true;
    } while (check);

    item_stack[i].SetFstream("C:\\Users\\Minjoon\\Documents\\stock_data\\(" + to_string(item_no) + ").csv");
    item_stack[i].SetInitPrice(time);
    item_stack[i].item_no = item_no;
    item_stack[i].bought_shares = (int)(balance / no_of_items / item_stack[i].init_price);
    balance -= item_stack[i].bought_shares * item_stack[i].init_price - 7;
  }

  while (time < running_term) {
    ++time;
    for (int i = 0; i < no_of_items; ++i) {
      ++item_stack[i].how_long_so_far;
      float current_price = item_stack[i].ReadPrice();
      if (item_stack[i].how_long_so_far >= how_long_to_hold
          || current_price >= (1 + profit_margin) * item_stack[i].init_price) {
        balance += current_price * item_stack[i].bought_shares - 7;
        item_stack[i].bought_item.close();

        int item_no;
        bool check;
        do {
          check = false;
          item_no = rand() % 52 + 1;
          for (int j = 0; j < no_of_items; ++j)
            if (item_stack[j].item_no == item_no)
              check = true;
        } while (check);

        item_stack[i].SetFstream("C:\\Users\\Minjoon\\Documents\\stock_data\\(" + to_string(item_no) + ").csv");
        item_stack[i].SetInitPrice(time);
        item_stack[i].item_no = item_no;
        item_stack[i].bought_shares = (int)(balance / item_stack[i].init_price);
        balance -= item_stack[i].bought_shares * item_stack[i].init_price - 7;
      }
    }
  }

  for (int i = 0; i < no_of_items; ++i)
    balance += item_stack[i].bought_shares * item_stack[i].ReadPrice();

  return balance / money;
}
