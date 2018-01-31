#include "solutions.h"



int Min(int i, int j) {
  return (i < j ? i : j);
}

int Min(int a, int b, int c) {
  int answer;
  if (a <= b && a <= c)
    answer = a;
  else if (b <= a && b <= c)
    answer = b;
  else if (c <= a && c <= b)
    answer = c;

  return answer;
}

int Max(int i, int j) {
  return (i > j ? i : j);
}


long long P15::Answer(int row, int column) {
  if (row == 0 || column == 0)
    return 1;
  else
    return Answer(row - 1, column) + Answer(row, column - 1);
}

long long P15::AnswerDP(int row, int column) {
  long long paths[row + 1][column + 1];

  for (int i = 0; i <= row; ++i)
    paths[i][0] = 1;
  for (int i = 0; i <= column; ++i)
    paths[0][i] = 1;

  for (int i = 1; i <= row; ++i)
    for (int j = 1; j <= column; ++j)
      paths[i][j] = paths[i - 1][j] + paths[i][j - 1];

  return paths[row][column];
}


int P54::Answer() {
  int one_win = 0;
  enum card_ranking {none, one_pair, two_pairs, three_of_a_kind, straight, flush, full_house,
                       four_of_a_kind, straight_flush, royal_flush};

  ifstream poker_file("C:\\Users\\Minjoon\\Dropbox\\works\\ProjectEuler\\poker.txt", ios_base::in);

  string ln_buffer, word_buffer, suit;
  int nums[5], one_highest = 0, two_highest = 0,
               one_highest_n = 0, two_highest_n = 0,
               one_highest_nn = 0, two_highest_nn = 0,
               one_highest_nnn = 0, two_highest_nnn = 0,
               one_highest_nnnn = 0, two_highest_nnnn = 0;
  card_ranking one, two;

  while (getline(poker_file, ln_buffer)) {
    stringstream ln_ss(ln_buffer);

    int cnt = 0;
    while (ln_ss >> word_buffer) {
      if (word_buffer[0] == 'T')
        nums[cnt % 5] = 10;
      else if (word_buffer[0] == 'J')
        nums[cnt % 5] = 11;
      else if (word_buffer[0] == 'Q')
        nums[cnt % 5] = 12;
      else if (word_buffer[0] == 'K')
        nums[cnt % 5] = 13;
      else if (word_buffer[0] == 'A')
        nums[cnt % 5] = 14;
      else
        nums[cnt % 5] = word_buffer[0] - '0';

      suit += word_buffer[1];

      ++cnt;

      if (cnt % 5 == 0) {
        sort(nums, nums + 5);

        if (suit == "DDDDD" || suit == "CCCCC" || suit == "HHHHH" || suit == "SSSSS") {
          if (nums[4] - nums[0] == 4) {
            if (nums[0] == 10) {
              if (cnt == 5)
                one = royal_flush;
              else
                two = royal_flush;
            }
            else {
              if (cnt == 5)
                one = straight_flush;
              else
                two = straight_flush;
            }
          }
          else {
            if (cnt == 5)
              one = flush;
            else
              two = flush;
          }

          if (cnt == 5) {
            one_highest = nums[4];
            one_highest_n = nums[3];
            one_highest_nn = nums[2];
            one_highest_nnn = nums[1];
            one_highest_nnnn = nums[0];
          }
          else {
            two_highest = nums[4];
            two_highest_n = nums[3];
            two_highest_nn = nums[2];
            two_highest_nnn = nums[1];
            two_highest_nnnn = nums[0];
          }
        }
        else {
          int inspect[4];

          for (int i = 0; i < 4; ++i)
            inspect[i] = nums[i + 1] - nums[i];

for (int i = 0; i < 4; ++i)
  cout << inspect[i] << ' ';
cout << endl;


          if (inspect[0] == 1 && inspect[1] == 1 && inspect[2] == 1 && inspect[3] == 1) {
            if (cnt == 5) {
              one = straight;
              one_highest = nums[4];
            }
            else {
              two = straight;
              two_highest = nums[4];
            }
          }
          else if (inspect[0] == 0 && inspect[1] == 0 && inspect[2] == 0) {
            if (cnt == 5) {
              one = four_of_a_kind;
              one_highest = nums[0];
            }
            else {
              two = four_of_a_kind;
              two_highest = nums[0];
            }
          }
          else if (inspect[1] == 0 && inspect[2] == 0 && inspect[3] == 0) {
            if (cnt == 5) {
              one = four_of_a_kind;
              one_highest = nums[1];
            }
            else {
              two = four_of_a_kind;
              two_highest = nums[1];
            }
          }
          else if (inspect[0] == 0 && inspect[1] == 0 && inspect[3] == 0) {
            if (cnt == 5) {
              one = full_house;
              one_highest = nums[0];
            }
            else {
              two = full_house;
              two_highest = nums[0];
            }
          }
          else if (inspect[0] == 0 && inspect[2] == 0 && inspect[3] == 0) {
            if (cnt == 5) {
              one = full_house;
              one_highest = nums[4];
            }
            else {
              two = full_house;
              two_highest = nums[4];
            }
          }
          else if (inspect[0] == 0 && inspect[2] == 0) {
            if (cnt == 5) {
              one = two_pairs;
              one_highest = nums[3];
              one_highest_n = nums[1];
              one_highest_nn = nums[4];
            }
            else {
              two = two_pairs;
              two_highest = nums[3];
              two_highest_n = nums[1];
              two_highest_nn = nums[4];
            }
          }
          else if (inspect[0] == 0 && inspect[3] == 0) {
            if (cnt == 5) {
              one = two_pairs;
              one_highest = nums[4];
              one_highest_n = nums[1];
              one_highest_nn = nums[2];
            }
            else {
              two = two_pairs;
              two_highest = nums[4];
              two_highest_n = nums[1];
              two_highest_nn = nums[2];
            }
          }
          else if (inspect[1] == 0 && inspect[3] == 0) {
            if (cnt == 5) {
              one = two_pairs;
              one_highest = nums[4];
              one_highest_n = nums[2];
              one_highest_nn = nums[0];
            }
            else {
              two = two_pairs;
              two_highest = nums[4];
              two_highest_n = nums[2];
              two_highest_nn = nums[0];
            }
          }
          else if (inspect[0] == 0) {
            if (cnt == 5) {
              one = one_pair;
              one_highest = nums[1];
              one_highest_n = nums[4];
              one_highest_nn = nums[3];
              one_highest_nnn = nums[2];
            }
            else {
              two = one_pair;
              two_highest = nums[1];
              two_highest_n = nums[4];
              two_highest_nn = nums[3];
              two_highest_nnn = nums[2];
            }
          }
          else if (inspect[1] == 0) {
            if (cnt == 5) {
              one = one_pair;
              one_highest = nums[2];
              one_highest_n = nums[4];
              one_highest_nn = nums[3];
              one_highest_nnn = nums[0];
            }
            else {
              two = one_pair;
              two_highest = nums[2];
              two_highest_n = nums[4];
              two_highest_nn = nums[3];
              two_highest_nnn = nums[0];
            }
          }
          else if (inspect[2] == 0) {
            if (cnt == 5) {
              one = one_pair;
              one_highest = nums[3];
              one_highest_n = nums[4];
              one_highest_nn = nums[1];
              one_highest_nnn = nums[0];
            }
            else {
              two = one_pair;
              two_highest = nums[3];
              two_highest_n = nums[4];
              two_highest_nn = nums[1];
              two_highest_nnn = nums[0];
            }
          }
          else if (inspect[3] == 0) {
            if (cnt == 5) {
              one = one_pair;
              one_highest = nums[4];
              one_highest_n = nums[2];
              one_highest_nn = nums[1];
              one_highest_nnn = nums[0];
            }
            else {
              two = one_pair;
              two_highest = nums[4];
              two_highest_n = nums[2];
              two_highest_nn = nums[1];
              two_highest_nnn = nums[0];
            }
          }
          else {
            if (cnt == 5) {
              one = none;
              one_highest = nums[4];
              one_highest_n = nums[3];
              one_highest_nn = nums[2];
              one_highest_nnn = nums[1];
              one_highest_nnnn = nums[0];
            }
            else {
              two = none;
              two_highest = nums[4];
              two_highest_n = nums[3];
              two_highest_nn = nums[2];
              two_highest_nnn = nums[1];
              two_highest_nnnn = nums[0];
            }
          }
        }

        suit = "";
      }
    }

    cout << "one:" << one << ' ' << one_highest << '\t' << "two:" << two << ' ' << two_highest << endl;

    if (one > two) {
      cout << "check\n";
      ++one_win;
    }
    else if (one == two && one_highest > two_highest) {
      cout << "check\n";
      ++one_win;
    }
    else if (one == two && one_highest == two_highest && one_highest_n > two_highest_n) {
      cout << "check\n";
      ++one_win;
    }
    else if (one == two && one_highest == two_highest && one_highest_n == two_highest_n
                && one_highest_nn > two_highest_nn) {
      cout << "check\n";
      ++one_win;
    }
    else if (one == two && one_highest == two_highest && one_highest_n == two_highest_n
                && one_highest_nn == two_highest_nn && one_highest_nnn > two_highest_nnn) {
      cout << "check\n";
      ++one_win;
    }
    else if (one == two && one_highest == two_highest && one_highest_n == two_highest_n
                && one_highest_nn == two_highest_nn && one_highest_nnn == two_highest_nnn
                && one_highest_nnnn > two_highest_nnnn) {
      cout << "check\n";
      ++one_win;
    }
  }

  poker_file.close();

  return one_win;
}


int P67::Answer() {
  ifstream input_file("C:\\Users\\Minjoon\\Dropbox\\works\\ProjectEuler\\triangle.txt", ios_base::in);

  int input_array[5050], sum_array[5050], cnt = 0;
  string number;

  while (input_file >> number) {
    input_array[cnt] = stoi(number);
    sum_array[cnt] = stoi(number);
    ++cnt;
  }

  for (int i = 1; i < 100; ++i) {
    sum_array[i * (i + 1) / 2] += sum_array[i * (i - 1) / 2];
    sum_array[i * (i + 1) / 2 + i] += sum_array[i * (i - 1) / 2 + i - 1];
  }

  for (int i = 2; i < 100; ++i)
    for (int j = 1; j < i; ++j)
      sum_array[i * (i + 1) / 2 + j] += max(sum_array[i * (i - 1) / 2 + j - 1], sum_array[i * (i - 1) / 2 + j]);

  input_file.close();

  int max_sum = 0;
  for (int i = 0; i < 100; ++i)
    if (sum_array[4950 + i] > max_sum)
      max_sum = sum_array[4950 + i];

  return max_sum;
}

/*int P67::Answer() {
  ifstream input_file("C:\\Users\\Minjoon\\Dropbox\\works\\ProjectEuler\\t.txt", ios_base::in);

  int input_array[120], sum_array[120], cnt = 0;
  string number;

  while (input_file >> number) {
    input_array[cnt] = stoi(number);
    sum_array[cnt] = stoi(number);
    ++cnt;
  }

  for (int i = 1; i < 15; ++i) {
    sum_array[i * (i + 1) / 2] += sum_array[i * (i - 1) / 2];
    sum_array[i * (i + 1) / 2 + i] += sum_array[i * (i - 1) / 2 + i - 1];
  }

  for (int i = 2; i < 15; ++i)
    for (int j = 1; j < i; ++j)
      sum_array[i * (i + 1) / 2 + j] += max(sum_array[i * (i - 1) / 2 + j - 1], sum_array[i * (i - 1) / 2 + j]);

  input_file.close();

  int max_sum = 0;
  for (int i = 0; i < 15; ++i)
    if (sum_array[105 + i] > max_sum)
      max_sum = sum_array[105 + i];

  return max_sum;
}*/

int P81::Answer() {
  ifstream matrix_file("C:\\Users\\Minjoon\\Dropbox\\works\\ProjectEuler\\matrix.txt", ios_base::in);
  int matrix[80][80];
  string ln_buffer;

  int row = 0;
  while (matrix_file >> ln_buffer) {
    stringstream single_row(ln_buffer);
    string entry;
    char digit;

    int column = 0;

    while (single_row >> digit) {
      if (digit != ',')
        entry += digit;
      else {
        matrix[row][column] = stoi(entry);
        entry = "";
        ++column;
      }
    }

    matrix[row][column] = stoi(entry);

    ++row;
  }

  matrix_file.close();

  int sum_matrix[80][80];
  memcpy(sum_matrix, matrix, sizeof(sum_matrix));

  for (int i = 78; i >= 0; --i)
    sum_matrix[79][i] += sum_matrix[79][i + 1];
  for (int i = 78; i >= 0; --i)
    sum_matrix[i][79] += sum_matrix[i + 1][79];

  for (int i = 78; i >= 0; --i)
    for (int j = 78; j >= 0; --j)
      sum_matrix[i][j] += Min(sum_matrix[i + 1][j], sum_matrix[i][j + 1]);

  return sum_matrix[0][0];
}


long long P443::GCD(long long a, long long b) {
  long long lesser, larger, gcd = 1;

  if (a > b) {
    larger = a, lesser = b;
  }
  else {
    larger = b, lesser = a;
  }

  for (long long i = 2; i <= lesser; ++i)
    if (larger % i == 0 && lesser % i == 0)
      gcd = i;

  return gcd;
}

long long P443::Answer(long long input) {
  if (input == 4)
    return 13;
  else
    return Answer(input - 1) + GCD(input, Answer(input - 1));
}


/*double P262::Answer() {
  double topography[1601][1601], init_h = height(200, 200), fin_h = height(1400, 1400);

  for (int i = 0; i < 1601; ++i)
    for (int j = 0; j < 1601; ++j)
      topography[i][j] = height(i, j);


}
*/


long long P230::Fibonacci(int n) {
  if (n == 1)
    return size_A;
  else if (n == 2)
    return size_B;
  else
    return Fibonacci(n - 1) + Fibonacci(n - 2);
}

char P230::DAB(long long input) {
  string A = "a", B = "b", C;
  int cur_cnt = 0;

  if (size_A >= input)
    return A_str[input - 1];
  else if (size_B >= input)
    return B_str[input - 1];
  else {
    C = B;
    B = A + B;
    A = C;
    cur_cnt = size_A + size_B;
  }

  for (int i = 3; Fibonacci(i) < input; ++i) {
    C = B;
    B = A + B;
    A = C;
  }

  A = "", C = "";

  cout << B.size() << endl;

  long long str_size = 0, str_size_prev;
  char ans_char;

  for (int i = 0; i < B.size(); ++i) {
    str_size += (B[i] == 'a' ? size_A : size_B);

    if (str_size > input) {
      ans_char = B[i];
      str_size = input - str_size_prev;
      break;
    }

    str_size_prev = str_size;
  }

  return ans_char == 'a' ? A_str[str_size - 1] : B_str[str_size - 1];
}

void P230::Answer() {
  A_str = "1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679",
  B_str = "8214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196";
  size_A = A_str.size(), size_B = B_str.size();

  char ans[18];

  for (int i = 0; i < 18; ++i) {
    long long input = 127 + 19 * i;
    for (int j = 0; j < i; ++j)
      input *= 7;

    ans[17 - i] = DAB(input);
  }

  cout << ans << endl;
}


long long P243::Answer() {
  for (long long i = 2; 1; ++i) {
    if (i % 10000 == 0)
      cout << i << endl;

    long long no_of_resilient_numerator = 1;

    for (long long j = 2; j < i; ++j) {
      bool resilient = true;

      if (i % j == 0)
        continue;
      else
        for (long long k = 2; k * k <= j; ++k)
          if (i % k == 0 && j % k == 0) {
            resilient = false;
            continue;
          }

      if (resilient == true)
        ++no_of_resilient_numerator;
    }

    if ((double)no_of_resilient_numerator / (i - 1) < 15499 / 94744.)
      return i;
  }
}


int MinStepsToOne::Answer(int val) {
  if (val == 0) {
    cout << "invalid input value\n";
    return -1;
  }
  else if (val == 1)
    return 0;
  else if (val == 2)
    return 1;
  else if (val == 3)
    return 1;
  else {
    int result[val];
    result[0] = 0;
    result[1] = 1;
    result[2] = 1;

    int t_1, t_2;
    for (int i = 4; i <= val; ++i) {
      if (i % 3 == 0)
        t_1 = 1 + result[i / 3 - 1];
      else
        t_1 = val;

      if (i % 2 == 0)
        t_2 = 1 + result[i / 2 - 1];
      else
        t_2 = val;

      result[i - 1] = Min(t_1, t_2, 1 + result[i - 2]);
    }

    return result[val - 1];
  }
}





N_Body::N_Body(double t_intv_val, int sim_duration_val) {
  t_intv = t_intv_val;
  sim_duration = sim_duration_val;
}

void N_Body::LeapFrog() {
  ifstream init_cond_file("C:\\Users\\Minjoon\\Dropbox\\works\\ProjectEuler\\init_cond.txt", ios_base::in);
  string buffer;

  init_cond_file >> buffer;
  num_ptls = stoi(buffer);
  double (*data)[7] = new double[num_ptls][7];

  init_cond_file >> buffer;
  sph_size = stod(buffer);

  for (int i = 0; i < num_ptls; ++i)
    for (int j = 0; j < 5; ++j) {
      init_cond_file >> buffer;
      data[i][j] = stod(buffer);
    }

  init_cond_file.close();

  ofstream traj_file("C:\\Users\\Minjoon\\Dropbox\\works\\ProjectEuler\\traj.txt", ios_base::out);

  traj_file << num_ptls << endl << sph_size << endl;

  for (int i = 0; i < num_ptls; ++i)
    traj_file << data[i][0] << ' ' << data[i][1] << "  ";
  traj_file << endl;

  for (int cnt = 0; cnt < sim_duration; ++cnt) {
    if (cnt % 500 == 0)
      cout << cnt << endl;

    bool out_of_bounds = false;

    for (int i = 0; i < num_ptls; ++i) {
      data[i][5] = 0;
      data[i][6] = 0;

      for (int j = 0; j < num_ptls; ++j)
        if (i != j) {
          double dist = sqrt(pow(data[i][0] - data[j][0], 2) + pow(data[i][1] - data[j][1], 2));
          data[i][5] += 6.67e-11 * data[j][4] / pow(dist, 3) * (data[j][0] - data[i][0]);
          data[i][6] += 6.67e-11 * data[j][4] / pow(dist, 3) * (data[j][1] - data[i][1]);
        }
    }

    for (int i = 0; i < num_ptls; ++i) {
      data[i][0] += data[i][2] * t_intv;
      data[i][1] += data[i][3] * t_intv;
      data[i][2] += data[i][5] * t_intv;
      data[i][3] += data[i][6] * t_intv;

      if (abs(data[i][0]) >= sph_size || abs(data[i][1]) >= sph_size)
        out_of_bounds = true;

      traj_file << data[i][0] << ' ' << data[i][1] << "  ";
    }

    if (out_of_bounds == true)
      break;

    traj_file << endl;
  }

  delete[] data;

  traj_file.close();
}





Genome::Genome(string seq_A_given, string seq_B_given) {
  seq_A = seq_A_given, seq_B = seq_B_given;
}

int Genome::GSA() {
  int len_A = seq_A.size(), len_B = seq_B.size();
  map<pair<int, int>, int> answer;

  enum movement {as_is, add_gap_A, add_gap_B};
  map<pair<int, int>, movement> gaps;

  answer[make_pair(len_A - 1, len_B - 1)] = (seq_A.back() == seq_B.back() ? 0 : 1);
  gaps[make_pair(len_A - 1, len_B - 1)] = as_is;

  if (len_A == 1 && len_B == 1)
    return answer[make_pair(0, 0)];

  for (int i = len_A - 2; i >= 0; --i) {
    bool match = false;

    for (int j = i; j < len_A; ++j)
      if (seq_A[j] == seq_B.back()) {
        match = true;
        break;
      }

    answer[make_pair(i, len_B - 1)] = (match == true ? 2 * (len_A - i - 1) : 1 + 2 * (len_A - i - 1));
    gaps[make_pair(i, len_B - 1)] = add_gap_B;
  }

  for (int i = len_B - 2; i >= 0; --i) {
    bool match = false;

    for (int j = i; j < len_B; ++j)
      if (seq_B[j] == seq_A.back()) {
        match = true;
        break;
      }

    answer[make_pair(len_A - 1, i)] = (match == true ? 2 * (len_B - i - 1) : 1 + 2 * (len_B - i - 1));
    gaps[make_pair(len_A - 1, i)] = add_gap_A;
  }

  for (int i = len_A - 2; i >= 0; --i)
    for (int j = len_B - 2; j >= 0; --j) {
      int AB = (seq_A[i] == seq_B[j] ? answer[make_pair(i + 1, j + 1)] : 1 + answer[make_pair(i + 1, j + 1)]);
      int Ag = 2 + answer[make_pair(i, j + 1)];
      int gB = 2 + answer[make_pair(i + 1, j)];

      if (AB <= Ag && AB <= gB) {
        answer[make_pair(i, j)] = AB;
        gaps[make_pair(i, j)] = as_is;
      }
      else if (Ag <= AB && Ag <= gB) {
        answer[make_pair(i, j)] = Ag;
        gaps[make_pair(i, j)] = add_gap_A;
      }
      else if (gB <= AB && gB <= Ag) {
        answer[make_pair(i, j)] = gB;
        gaps[make_pair(i, j)] = add_gap_B;
      }
    }

  int i = 0, j = 0, extra_i = 0, extra_j = 0;

  while (i < len_A || j < len_B) {
    if (gaps[make_pair(i, j)] == as_is) {
      ++i, ++j;
    }
    else if (gaps[make_pair(i, j)] == add_gap_A) {
      ++j;
      seq_A.insert(i + extra_i, " ");
      ++extra_i;
    }
    else if (gaps[make_pair(i, j)] == add_gap_B) {
      ++i;
      seq_B.insert(j + extra_j, " ");
      ++extra_j;
    }
  }

  cout << seq_A << endl;
  cout << seq_B << endl;

  return answer[make_pair(0, 0)];
}





Disk::Disk(int id_val, int file_size) {
  id = id_val;
  items.push_back(file_size);
  remaining = 1000000 - file_size;
}

bool MyComp(int i, int j) {
  return (i > j);
}

void BinPacking::PackBins() {
  ifstream input_file("C:\\Users\\Minjoon\\Dropbox\\works\\ProjectEuler\\call_file.txt", ios_base::in);

  vector<int> file_sizes;
  string buffer;

  while (input_file >> buffer)
    file_sizes.push_back(stoi(buffer));

  input_file.close();

  sort(file_sizes.begin(), file_sizes.end(), MyComp);

  int id = 0, sum = 0;
  for (int i = 0; i < file_sizes.size(); ++i) {
    int cur_file_size = file_sizes[i];
    sum += cur_file_size;

    if (!bins.empty() && bins.top().remaining >= cur_file_size) {
      Disk d_temp = bins.top();
      bins.pop();
      d_temp.items.push_back(cur_file_size);
      d_temp.remaining -= cur_file_size;
      bins.push(d_temp);
    }
    else {
      Disk d_temp(id, cur_file_size);
      bins.push(d_temp);
      ++id;
    }
  }

  cout << "Sum of all files = " << sum / 1000000.0 << "GB\n";
  cout << "# of disks used = " << bins.size() << endl;
  while (!bins.empty()) {
    cout << bins.top().id << '\t' << bins.top().remaining << ':' << "  ";
    for (int i = 0; i < bins.top().items.size(); ++i)
      cout << bins.top().items[i] << ' ';
    cout << endl;
    bins.pop();
  }
}





Vertex::Vertex(string actor_name) {
  name = actor_name;
  color = 'W';
  parent = NULL;
  dist = 1000000;
}

KevinBacon::~KevinBacon() {
  for (map<string, Vertex*>::iterator it = actor_name_map.begin(); it != actor_name_map.end(); ++it)
    delete it->second;
}

void KevinBacon::BuildAdjListAndVertices() {
  ifstream input_file("C:\\Users\\Minjoon\\Dropbox\\works\\ProjectEuler\\input-all.txt", ios_base::in);

  string ln_buffer, title, name;

  while (getline(input_file, ln_buffer)) {
    size_t pos = ln_buffer.find('/', 0), pos_prev;
    title = ln_buffer.substr(0, pos);
    pos_prev = pos;

    set<string> names;

    while ((pos = ln_buffer.find('/', pos_prev + 1)) != string::npos) {
      name = ln_buffer.substr(pos_prev + 1, pos - pos_prev - 1);
      pos_prev = pos;

      names.insert(name);
    }

    names.insert(ln_buffer.substr(pos_prev + 1, ln_buffer.size() - pos_prev - 1));

    for (set<string>::iterator it_out = names.begin(); it_out != names.end(); ++it_out) {
      if (adj_list.count(*it_out)) {
        for (set<string>::iterator it_in = names.begin(); it_in != names.end(); ++it_in)
          if (it_in != it_out) {
            if (adj_list[*it_out].count(*it_in))
              adj_list[*it_out][*it_in].insert(title);
            else {
              set<string> temp_set;
              temp_set.insert(title);
              adj_list[*it_out].insert(make_pair(*it_in, temp_set));
            }
          }
      }
      else {
        map< string, set<string> > temp_map;
        set<string> temp_set;
        temp_set.insert(title);

        for (set<string>::iterator it_in = names.begin(); it_in != names.end(); ++it_in)
          if (it_in != it_out)
            temp_map.insert(make_pair(*it_in, temp_set));

        adj_list.insert(make_pair(*it_out, temp_map));
      }
    }
  }

  input_file.close();

  for (map< string, map< string, set<string> > >::iterator it = adj_list.begin();
       it != adj_list.end(); ++it) {
    Vertex* new_actor = new Vertex(it->first);
    actor_name_map.insert(make_pair(it->first, new_actor));
  }
}

void KevinBacon::BuildBFT(string root_name) {
  actor_name_map[root_name]->color = 'G';
  actor_name_map[root_name]->dist = 0;

  queue<string> grays;

  grays.push(root_name);

  while (!grays.empty()) {
    for (map< string, set<string> >::iterator it = adj_list[grays.front()].begin();
         it != adj_list[grays.front()].end(); ++it) {
      if (actor_name_map[it->first]->color == 'W') {
        actor_name_map[it->first]->color = 'G';
        actor_name_map[it->first]->dist = actor_name_map[grays.front()]->dist + 1;
        actor_name_map[it->first]->parent = actor_name_map[grays.front()];
        grays.push(it->first);
      }
    }
    actor_name_map[grays.front()]->color = 'B';
    grays.pop();
  }

  map<int, int> dist_distribution;

  for (map<string, Vertex*>::iterator it = actor_name_map.begin(); it != actor_name_map.end(); ++it) {
    if (dist_distribution.count(it->second->dist))
      ++dist_distribution[it->second->dist];
    else
      dist_distribution.insert(make_pair(it->second->dist, 1));
  }

  cout << "Kevin Bacon # distribution for " << root_name << endl;
  for (map<int, int>::iterator it = dist_distribution.begin(); it != dist_distribution.end(); ++it) {
    if (it->first == 1000000)
      cout << "inf" << '\t' << it->second << endl;
    else
      cout << it->first << '\t' << it->second << endl;
  }

}

void KevinBacon::BFS(string actor_name) {
  cout << actor_name;
  if (actor_name_map[actor_name]->dist == 1000000)
    cout << "has no connection to the actor/actress in question" << endl;
  else
    cout << " has a Bacon # of " << actor_name_map[actor_name]->dist << endl;

  string name = actor_name;

  while (actor_name_map[name]->parent != NULL) {
    cout << name << " was in ";
    for (set<string>::iterator it = adj_list[name][actor_name_map[name]->parent->name].begin();
         it != adj_list[name][actor_name_map[name]->parent->name].end(); ++it) {
      cout << '\"' + *it + '\"';
      if (it != --(adj_list[name][actor_name_map[name]->parent->name].end()))
        cout << ',';
      cout << ' ';
    }
    cout << "with " << actor_name_map[name]->parent->name << endl;

    name = actor_name_map[name]->parent->name;
  }
}





City::City(int i) {
  id = i;
  parent = NULL;
  d_upper_bnd = 1e12;
}

Routing::~Routing() {
  for (int i = 0; i < num_of_cities; ++i)
    delete id_city_map[i];
}

void Routing::BuildAdjListAndVertices() {
  ifstream data_file("C:\\Users\\Minjoon\\Dropbox\\works\\ProjectEuler\\usa.txt", ios_base::in);

  data_file >> num_of_cities;
  data_file >> num_of_roads;

  int id, x, y;
  for (int i = 0; i < num_of_cities; ++i) {
    data_file >> id;
    data_file >> x;
    data_file >> y;

    if (id != i)
      cout << "?\n";

    city_loc.insert(make_pair(id, make_pair(x, y)));
  }

  int u, v, err_cnt = 0;
  for (int i = 0; i < num_of_roads; ++i) {
    data_file >> u;
    data_file >> v;

    if (adj_list.count(u)) {
      if (adj_list[u].count(v)) {
        ++err_cnt;
        continue;
      }
      adj_list[u].insert(make_pair( v, sqrt( pow(city_loc[u].first - city_loc[v].first, 2)
                                               + pow(city_loc[u].second- city_loc[v].second, 2) ) ) );
    }
    else {
      map<int, double> temp_map;
      temp_map.insert(make_pair( v, sqrt( pow(city_loc[u].first - city_loc[v].first, 2)
                                            + pow(city_loc[u].second- city_loc[v].second, 2) ) ) );
      adj_list.insert(make_pair(u, temp_map));
    }

    if (adj_list.count(v)) {
      adj_list[v].insert(make_pair( u, sqrt( pow(city_loc[u].first - city_loc[v].first, 2)
                                               + pow(city_loc[u].second- city_loc[v].second, 2) ) ) );
    }
    else {
      map<int, double> temp_map;
      temp_map.insert(make_pair( u, sqrt( pow(city_loc[u].first - city_loc[v].first, 2)
                                            + pow(city_loc[u].second- city_loc[v].second, 2) ) ) );
      adj_list.insert(make_pair(v, temp_map));
    }
  }

  data_file >> source;
  data_file >> sink;

  data_file.close();

  if (adj_list.size() != num_of_cities) {
    cout << "# of cities mismatch\n";
    return;
  }

  int num_of_edges = 0;
  for (int i = 0; i < num_of_cities; ++i)
    num_of_edges += adj_list[i].size();
  if (num_of_edges != 2 * num_of_roads)
    cout << "# of roads mismatch: there're " << err_cnt << " duplicate entries for edges.\n";

  for (int i = 0; i < num_of_cities; ++i) {
    City* new_city = new City(i);
    id_city_map.insert(make_pair(i, new_city));
  }
}

bool Routing::Relax(int i, int j) {
  if (id_city_map[j]->d_upper_bnd > id_city_map[i]->d_upper_bnd + adj_list[i][j]) {
    id_city_map[j]->d_upper_bnd = id_city_map[i]->d_upper_bnd + adj_list[i][j];
    id_city_map[j]->parent = id_city_map[i];

    return true;
  }
  else
    return false;
}

void Routing::Dijkstra(int source_city) {
  id_city_map[source_city]->d_upper_bnd = 0;

  set<City*, CityComp> to_do;
  for (int i = 0; i < num_of_cities; ++i)
    to_do.insert(id_city_map[i]);

  for (int i = 0; i < num_of_cities; ++i) {
    set<City*>::iterator s_it = to_do.begin();
    int s = (*s_it)->id;
    to_do.erase(s_it);

    for (map<int, double>::iterator it = adj_list[s].begin(); it != adj_list[s].end(); ++it)
      if (Relax(s, it->first)) {
        to_do.erase(id_city_map[it->first]);
        to_do.insert(id_city_map[it->first]);
      }
  }
}

void Routing::PrintRoute(int i) {
  if (id_city_map[i]->parent == NULL)
    cout << i << endl;
  else {
    PrintRoute(id_city_map[i]->parent->id);
    cout << i << endl;
  }
}

void Routing::GetRoute(int i) {
  cout << id_city_map[i]->d_upper_bnd << endl;
  PrintRoute(i);
}





bool AthComp(pair<int, int> a, pair<int, int> b) {
  return (a.second > b.second);
}

Amazon2::Amazon2() {
  athletes.push_back(make_pair(3, 4));
  athletes.push_back(make_pair(2, 2));
  athletes.push_back(make_pair(7, 6));
  athletes.push_back(make_pair(4, 5));

  height = 0, tot_mass = 0, max_height = 0;

  sort(athletes.begin(), athletes.end(), AthComp);
}

void Amazon2::Tower(vector< pair<int, int> >::iterator it) {
  ++height;
  int mass = it->first, strength = it->second;
  tot_mass += mass;
  athletes.erase(it);

  vector< pair<int, int> >::iterator ath_it = athletes.begin();

  if (ath_it->second < tot_mass) {
    if (height >= max_height)
      max_height = height;
  }
  else
    while (ath_it->second >= tot_mass) {
      Tower(ath_it);
      ++ath_it;
    }

  --height;
  tot_mass -= mass;
  athletes.push_back(make_pair(mass, strength));
  sort(athletes.begin(), athletes.end(), AthComp);
}









