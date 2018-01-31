/*/ City Block Flyover
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  string ln_buffer, num;

  while (getline(in, ln_buffer)) {
    vector<int> st, ave;        // Container for streets and avenues
    bool is_st = true;
    for (char c : ln_buffer) {
      if (c >= '0' && c <= '9')
        num += c;               // Store numbers in a string
      else if (c == ',' || c == ')') {   // Upon receiving ',' or ')',
        if (is_st)                       // convert the string into int and push it into container,
          st.push_back(stoi(num));
        else
          ave.push_back(stoi(num));
        num = "";                        // and initialize the string.
      }

      if (c == ')')             // Into which container to put the value
        is_st = false;          // is determined by whether ')" is met.
    }

// Let's denote the streets by 'x' and the avenues by 'y'.
// Then the chopper flight trajectory is described by
//     (x[i], max_ave / max_st * x[i]).
// To avoid handling floating point numbers, let's rescale streets and avenues,
// in such a way that max_st = max_ave.
    int st_f = st.back(), ave_f = ave.back();
    for (int i = 0; i < st.size(); ++i)
      st[i] *= ave_f;
    for (int i = 0; i < ave.size(); ++i)
      ave[i] *= st_f;

// On a column bounded by 2 adjacent streets (x[i - 1] and x[i]),
// the number of city blocks that a chopper flies over is
//     min {j; y[j] >= x[i]} - max {j; y[j] <= x[i - 1]}.
    if (st.size() == 1)
      cout << ave.size() - 1 << endl;
    else if (ave.size() == 1)
      cout << st.size() - 1 << endl;
    else {
// The chopper starts at (0, 0), so that at x[0], max = 0, min = 1.
      int under = 0, over = 1, answer = 0;

      for (int i = 1; i < st.size(); ++i) {
        while (ave[over] < st[i])   // At each x[i], find min[i],
          ++over;

        answer += (over - under);   // and obtain min[i] - max[i - 1].

// If flight path crosses an intersection, the next iteration shouldn't
// consider the block bounded above by min[i], as block borders don't count by themselves.
        if (ave[over] == st[i])
          ++over;

        under = over - 1;           // Update max[i].
      }

      cout << answer << endl;       // Complexity: O(N + M). No extra memory cost.
    }
  }

  in.close();

  return 0;
}
*/


/*/ Grid Walk
#include <iostream>
#include <vector>
#include <set>

using namespace std;

class GridWalk {
 public:
// A GridWalk object instantiates with the coordinates,
//     (start_x_val, start_y_val),
// of the starting point and the constraint,
//     sum of digits of abs(x-coordinates) and abs(y-coordinates) <= bound_val.
  GridWalk(int bound_val, int start_x_val, int start_y_val) {
    bound = bound_val;
    start_x = start_x_val;
    start_y = start_y_val;
  }
// AccessiblePoints() counts the number of points accessible
//   from the starting point.
  int AccessiblePoints();

 private:
  int bound, start_x, start_y;
// mature stores points whose immediate neighbors are fully discovered:
//   it needs to be a set, because we need to look up elements in it.
// active stores points that are newly discovered and not yet mature.
  vector< pair<int, int> > active;
  set< pair<int, int> > mature;
// InsertPoint(x, y), checks if the point (x, y) satisfies
//   the constraints, and if so, put it in active.
  void InsertPoint(int x_val, int y_val);
};

int GridWalk::AccessiblePoints() {
  pair<int, int> origin = make_pair(start_x, start_y);
  active.push_back(origin);

// Pull out a point, X, stored at the end of active.
//   (Choose the last element, so that active does not rearrange
//    after its elimination.)
// Find which of the 4 points reached by moving up, down, left and right by 1 from X
//   are legal and not mature.
// Put those found into active.
// Put X into mature.
// Repeat till active gets empty.
// Return the size of mature set.
  while (!active.empty()) {
    int x = active.back().first, y = active.back().second;
    mature.insert(active.back());
    active.pop_back();

    InsertPoint(x + 1, y);
    InsertPoint(x - 1, y);
    InsertPoint(x, y + 1);
    InsertPoint(x, y - 1);
  }

  return mature.size();
}

void GridWalk::InsertPoint(int x_val, int y_val) {
  int x = (x_val >= 0 ? x_val : -x_val),
      y = (y_val >= 0 ? y_val : -y_val),
      sum = 0;

// Obtain the sum of digits of abs(x_val) and abs(y_val).
  while (x > 0) {
    sum += x - (x / 10) * 10;
    x /= 10;
  }
  while (y > 0) {
    sum += y - (y / 10) * 10;
    y /= 10;
  }

// Check if (x, y) is legal and immature.
  if (sum <= bound && !mature.count(make_pair(x_val, y_val))) {
    pair<int, int> point = make_pair(x_val, y_val);
    active.push_back(point);
  }
}


int main() {
  GridWalk test(19, 0, 0);
  cout << test.AccessiblePoints() << endl;
// Complexity: Let's say there are n elements in the mature set at a given moment.
//   Pull out a point from the active set. It gives at most 4 active set candidates,
//   and it takes at most 4 * log n to check if they are mature.
//   Then the size of the mature set grows by 1.
//   Since each point will be discovered at most 4 times,
//   the number of points whose look-up time is O(log n) is constant, and therefore
//     complexity ~ sum_{n=1}^N log n ~ N log N,
//   where N is the number of all the points legally accessible from the starting point.

  return 0;
}
*/


/*/ Double Squares
#include <iostream>
#include <fstream>
#include <math.h>


using namespace std;

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  int N;
  in >> N;

  for (int i = 0; i < N; ++i) {
    long long X;
    in >> X;
    long long a = 0, b = (long long)(sqrt(X));
    long answer = 0;

    while (2 * a * a <= X) {
      if (a * a + b * b == X) {
        ++answer;
        ++a;
      }
      else {
        a = (long long)(sqrt(X - b * b));
        if (a * a + b * b != X)
          ++a;
      }

      b = (long long)(sqrt(X - a * a));
    }

    cout << answer << endl;
  }

  return 0;
}
*/


/*/ Remove Characters
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  string ln_buffer;

  while (getline(in, ln_buffer)) {
    size_t loc = ln_buffer.find(',') + 2;
    string phrase = ln_buffer.substr(0, loc - 2);
    string to_erase = ln_buffer.substr(loc, ln_buffer.size() - loc);

    for (int i = 0; i < phrase.size(); ) {
      if (to_erase.find(phrase[i]) != string::npos)
        phrase.erase(i, 1);
      else
        ++i;
    }

    cout << phrase << endl;
  }

  return 0;
}
*/


/*/ Prime Numbers
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  long long n;

  while (in >> n) {
    vector<long long> primes;
    primes.push_back(2);

    for (long long i = 3; i <= n; i += 2) {
      bool is_prime = true;
      for (long j = 0; j < primes.size() && primes[j] * primes[j] <= i; ++j) {
        if (!(i % primes[j])) {
          is_prime = false;
          break;
        }
      }
      if (is_prime)
        primes.push_back(i);
    }

    for (long i = 0; i < primes.size() - 1; ++i)
      cout << primes[i] << ',';
    cout << primes.back() << endl;
  }

  return 0;
}
*/


/*/ Number Pairs
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  string ln_buffer, num;

  while (getline(in, ln_buffer)) {
    vector<int> int_list;
    int target, len = ln_buffer.size();

    for (int i = 0; i < len; ++i) {
      char c = ln_buffer[i];
      if (c >= '0' && c <= '9')
        num += c;
      else if (c == ',' || c == ';') {
        int_list.push_back(stoi(num));
        num = "";
      }
    }
    target = stoi(num);
    num = "";

    string answer;
    int i_rev = int_list.size() - 1;
    for (int i = 0; i < i_rev; ++i) {
      while (i < i_rev && int_list[i] + int_list[i_rev] >= target) {
        if (int_list[i] + int_list[i_rev] == target) {
          answer += to_string(int_list[i]) + ',' + to_string(int_list[i_rev]) + ';';
          --i_rev;
          break;
        }
        --i_rev;
      }
    }

    if (answer != "") {
      answer.erase(answer.size() - 1, 1);
      cout << answer << endl;
    }
    else
      cout << "NULL\n";
  }

  return 0;
}
*/


/*/ Longest Common Subsequence
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  string ln_buffer;

  while (getline(in, ln_buffer)) {
    string s_a, s_b;
    int lb_i = 0;
    while (ln_buffer[lb_i] != ';') {
      s_a += ln_buffer[lb_i];
      ++lb_i;
    }
    ++lb_i;
    while (lb_i < ln_buffer.size()) {
      s_b += ln_buffer[lb_i];
      ++lb_i;
    }

    int l_a = s_a.size(), l_b = s_b.size();
    int l_LCS[l_a + 1][l_b + 1];
    char path[l_a + 1][l_b + 1];
    for (int i = 0; i <= l_a; ++i)
      l_LCS[i][0] = 0;
    for (int i = 1; i <= l_b; ++i)
      l_LCS[0][i] = 0;

    for (int i = 1; i <= l_a; ++i)
      for (int j = 1; j <= l_b; ++j) {
        if (s_a[i - 1] == s_b[j - 1]) {
          l_LCS[i][j] = l_LCS[i - 1][j - 1] + 1;
          path[i][j] = 'd';
        }
        else {
          if (l_LCS[i - 1][j] >= l_LCS[i][j - 1]) {
            l_LCS[i][j] = l_LCS[i - 1][j];
            path[i][j] = 'u';
          }
          else {
            l_LCS[i][j] = l_LCS[i][j - 1];
            path[i][j] = 'l';
          }
        }
      }

    int x = l_a, y = l_b;
    string answer;
    while (x != 0 && y != 0) {
      if (path[x][y] == 'd') {
        answer += s_a[x - 1];
        --x;
        --y;
      }
      else if (path[x][y] == 'u') {
        --x;
      }
      else if (path[x][y] == 'l') {
        --y;
      }
    }
    for (int i = answer.size() - 1; i >= 0; --i)
      cout << answer[i];
    cout << endl;
  }

  return 0;
}
*/


/*/ Happy Numbers
#include <iostream>
#include <fstream>
#include <set>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    int N;

    while (in >> N) {
        if (N == 1) {
            cout << 1 << endl;
            continue;
        }
        else {
            int n = N;
            set<int> prev;

            while (1) {
                prev.insert(n);

                int sum = 0;
                while (n > 0) {
                    sum += (n % 10) * (n % 10);
                    n /= 10;
                }

                if (sum == 1) {
                    cout << 1 << endl;
                    break;
                }
                else if (prev.count(sum)) {
                    cout << 0 << endl;
                    break;
                }
                else
                    n = sum;
            }
        }
    }

    in.close();
    return 0;
}
*/


/*/ Set Intersection
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer, num;

    while (getline(in, ln_buffer)) {
        vector<int> a, b;

        int i = 0;
        while (ln_buffer[i] != ';') {
            if (ln_buffer[i] == ',') {
                a.push_back(stoi(num));
                num = "";
            }
            else
                num += ln_buffer[i];
            ++i;
        }
        a.push_back(stoi(num));
        num = "";
        ++i;
        while (i < ln_buffer.size()) {
            if (ln_buffer[i] == ',') {
                b.push_back(stoi(num));
                num = "";
            }
            else
                num += ln_buffer[i];
            ++i;
        }
        b.push_back(stoi(num));
        num = "";

        int j = 0, k = 0;
        vector<int> answer;
        while (j < a.size() && k < b.size()) {
            if (a[j] == b[k]) {
                answer.push_back(a[j]);
                ++j;
                ++k;
            }
            else if (a[j] > b[k])
                ++k;
            else
                ++j;
        }

        int l = answer.size();
        if (!l)
          cout << endl;
        else {
          for (int j = 0; j < l - 1; ++j)
            cout << answer[j] << ',';
          cout << answer.back() << endl;
        }
    }

    in.close();
    return 0;
}
*/


/*/ Trailing String
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;

    while (getline(in, ln_buffer)) {
        int c = ln_buffer.find(',');
        string a = ln_buffer.substr(0, c), b = ln_buffer.substr(c + 1, ln_buffer.size() - 1 - c);

        bool included = true;
        for (int i = a.size() - b.size(); i < a.size(); ++i)
            if (a[i] != b[i - a.size() + b.size()]) {
                included = false;
                break;
            }

        cout << included << endl;
    }

    in.close();
    return 0;
}
*/


/*/ Pascal's Triangle
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    int N;

    while (in >> N) {
        vector< vector<int> > PT;

        for (int i = 0; i < N; ++i) {
            vector<int> row;
            for (int j = 0; j <= i; ++j) {
                if (j == 0 || j == i)
                    row.push_back(1);
                else
                    row.push_back(PT[i - 1][j - 1] + PT[i - 1][j]);
            }
            PT.push_back(row);
        }

        for (int i = 0; i < N; ++i)
            for (int j = 0; j <= i; ++j)
                cout << PT[i][j] << ' ';
        cout << endl;
    }

    in.close();
    return 0;
}
*/


/*/ Car Race
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdio.h>

using namespace std;

bool LTComp(pair<int, double> a, pair<int, double> b) {
  return (a.second < b.second);
}

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;
    getline(in, ln_buffer);
    stringstream ss(ln_buffer);
    double num;
    vector<double> track;
    track.push_back(180);
    while (ss >> num)
      track.push_back(num);

    vector< pair<int, double> > lap_times;
    while (in >> num) {
      int id = num;
      in >> num;
      double max_speed = num;
      in >> num;
      double acc_t = num;
      in >> num;
      double dec_t = num;

      double lap_time_cur = 0;
      for (int i = 1; i < track.size() - 1; i += 2) {
        lap_time_cur += track[i] / max_speed * 3600
          + (track[i - 1] * track[i - 1] * acc_t + track[i + 1] * track[i + 1] * dec_t) / 180 / 360;
      }

      lap_times.push_back(make_pair(id, lap_time_cur));
    }

    sort(lap_times.begin(), lap_times.end(), LTComp);

    for (int i = 0; i < lap_times.size(); ++i)
      printf("%d %.2f\n", lap_times[i].first, lap_times[i].second);

    in.close();
    return 0;
}
*/


/*/ Data Recovery
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;

    while (getline(in, ln_buffer)) {
        size_t sc = ln_buffer.find(';');

        stringstream words(ln_buffer.substr(0, sc)),
                     nums(ln_buffer.substr(sc + 1, ln_buffer.size() - 1 - sc));

        map<int, string> sentence;
        string word;
        int pos;
        while (words >> word) {
            nums >> pos;
            sentence.insert(make_pair(pos, word));
        }
        words >> word;
        int tot_words = sentence.size() + 1;

        map<int, string>::iterator it = sentence.begin();
        for (int pos = 1; pos <= tot_words; ++pos) {
            if (pos == it->first) {
                cout << it->second << ' ';
                ++it;
            }
            else
                cout << word << ' ';
        }

        cout << endl;
    }

    in.close();
    return 0;
}
*/


/*/ Reverse and Add
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool IsPal(long n) {
  string num = to_string(n);
  int len = num.size();
  bool is_pal = true;
  for (int i = 0; i < len / 2; ++i)
    if (num[i] != num[len - 1 - i]) {
      is_pal = false;
      break;
    }

  return is_pal;
}

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    long num;

    while (in >> num) {
      int turns = 0;
      while (!IsPal(num)) {
        ++turns;
        string rev_num = to_string(num);
        int len = rev_num.size();
        for (int i = 0; i < len / 2; ++i) {
          char c = rev_num[len - 1 - i];
          rev_num[len - 1 - i] = rev_num[i];
          rev_num[i] = c;
        }
        num += stol(rev_num);
      }
      cout << turns << ' ' << num << endl;
    }

    in.close();
    return 0;
}
*/


/*/ Detecting Cycles
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;

    while (getline(in, ln_buffer)) {
      int len = ln_buffer.size();
      int blank_1 = ln_buffer.rfind(' '), blank_2 = ln_buffer.rfind(' ', blank_1 - 1);

      while (blank_2 != string::npos) {
        string cand = ln_buffer.substr(blank_1 + 1, len - 1 - blank_1);

        if (cand == ln_buffer.substr(blank_2 + 1, blank_1 - blank_2 - 1)) {
          cout << cand << endl;
          break;
        }
        else {
          blank_1 = ln_buffer.rfind(' ', blank_1 - 1);
          for (int j = 0; j < 2; ++j)
            blank_2 = ln_buffer.rfind(' ', blank_2 - 1);
        }
      }
    }

    in.close();
    return 0;
}
*/


/*/ Number of Ones
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    int num;

    while (in >> num) {
      int cnt = 0;
      while (num > 0) {
        int remainder = num % 2;
        if (remainder)
          ++cnt;
        num = (num - remainder) / 2;
      }
      cout << cnt << endl;
    }

    in.close();
    return 0;
}
*/


/*/ Sum of Integers
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;

    while (getline(in, ln_buffer)) {
      int len = ln_buffer.size();
      vector<int> ar;
      string num;
      for (int i = 0; i < len; ++i) {
        char c = ln_buffer[i];

        if (c == ',') {
          ar.push_back(stoi(num));
          num = "";
        }
        else
          num += c;
      }
      ar.push_back(stoi(num));

      int cnt = ar.size();
      int sums[cnt][cnt];

      int max = -30000;
      for (int i = 0; i < cnt; ++i) {
        sums[i][0] = ar[i];
        if (sums[i][0] > max)
          max = sums[i][0];
      }

      for (int i = 0; i < cnt - 1; ++i)
        for (int j = 1; j < cnt - i; ++j) {
          sums[i][j] = sums[i][j - 1] + ar[i + j];
          if (sums[i][j] > max)
            max = sums[i][j];
        }

      cout << max << endl;
    }

    in.close();
    return 0;
}
*/


/*/ Pangrams
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;

    while (getline(in, ln_buffer)) {
      int len = ln_buffer.size();
      int answer[26] = {0};
      for (int i = 0; i < len; ++i) {
        char c = ln_buffer[i];
        if (c >= 'A' && c <= 'Z')
          ++answer[c - 'A'];
        if (c >= 'a' && c <= 'z')
          ++answer[c - 'a'];
      }

      bool is_pan = true;
      for (int i = 0; i < 26; ++i)
        if (!answer[i]) {
          cout << (char)(i + 'a');
          is_pan = false;
        }
      if (is_pan)
        cout << "NULL";
      cout << endl;
    }

    in.close();
    return 0;
}
*/


/*/ Parentheses
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;

    while (getline(in, ln_buffer)) {
      int len = ln_buffer.size();
      vector<int> parentheses;

      bool sth_wrong = false;
      for (int i = 0; i < len; ++i) {
        char c = ln_buffer[i];

        if (c == '(')
          parentheses.push_back(1);
        else if (c == '{')
          parentheses.push_back(2);
        else if (c == '[')
          parentheses.push_back(3);
        else if (c == ')') {
          if (parentheses.empty() || parentheses.back() != 1) {
            sth_wrong = true;
            break;
          }
          else
            parentheses.pop_back();
        }
        else if (c == '}') {
          if (parentheses.empty() || parentheses.back() != 2) {
            sth_wrong = true;
            break;
          }
          else
            parentheses.pop_back();
        }
        else if (c == ']') {
          if (parentheses.empty() || parentheses.back() != 3) {
            sth_wrong = true;
            break;
          }
          else
            parentheses.pop_back();
        }
      }

      cout << (parentheses.empty() && !sth_wrong ? "True\n" : "False\n");
    }

    in.close();
    return 0;
}
*/


/*/ Array Absurdity
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;

    while (getline(in, ln_buffer)) {
      int len = ln_buffer.size(), s_c = ln_buffer.find(';');
      int N = stoi(ln_buffer.substr(0, s_c));
      int cnt[N - 1];
      for (int i = 0; i < N - 1; ++i)
        cnt[i] = 0;

      string num;
      for (int i = s_c + 1; i <= len; ++i) {
        char c = ln_buffer[i];
        if (c == ',' || i == len) {
          int x = stoi(num);
          ++cnt[x];
          if (cnt[x] == 2) {
            cout << x << endl;
            break;
          }
          num = "";
        }
        else
          num += c;
      }
    }

    in.close();
    return 0;
}
*/


/*/ Chain Inspection
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer, origin, dest;

    while (getline(in, ln_buffer)) {
        int len = ln_buffer.size(), start = 0, dash, sc;
        map<string, string> route;

        while (start < len) {
            dash = ln_buffer.find('-', start);
            origin = ln_buffer.substr(start, dash - start);

            sc = ln_buffer.find(';', start);
            if (sc == string::npos)
                sc = len;
            dest = ln_buffer.substr(dash + 1, sc - dash - 1);

            route.insert(make_pair(origin, dest));

            start = sc + 1;
        }

        origin = "BEGIN";

        for (int i = 0; i < route.size(); ++i) {
            if (route.count(origin))
                origin = route[origin];
            else {
                origin = "BAD END";
                break;
            }
        }

        cout << (origin == "END" ? "GOOD" : "BAD") << endl;
    }

    in.close();
    return 0;
}
*/


/*/ Overlapping Rectangles
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;

    while (getline(in, ln_buffer)) {
        int len = ln_buffer.size(), cds[8];
        string coord;

        for (int i = 0, j = 0; i < len; ++i) {
            char c = ln_buffer[i];

            if (c == ',') {
                cds[j] = stoi(coord);
                coord = "";
                ++j;
            }
            else
                coord += c;
        }
        cds[7] = stoi(coord);

        if (cds[0] <= cds[6] && cds[2] >= cds[4] && cds[1] >= cds[7] && cds[3] <= cds[5])
            cout << "True\n";
        else
            cout << "False\n";
    }

    in.close();
    return 0;
}
*/


/*/ Cash Register
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer, coord;
    int denom[12] = {1, 5, 10, 25, 50, 100, 200, 500, 1000, 2000, 5000, 10000};
    string note[12] = {"PENNY", "NICKEL", "DIME", "QUARTER", "HALF DOLLAR",
                       "ONE", "TWO", "FIVE", "TEN", "TWENTY", "FIFTY", "ONE HUNDRED"};

    while (getline(in, ln_buffer)) {
        int sc = ln_buffer.find(';'), len = ln_buffer.size();
        int ch = 100 * stod(ln_buffer.substr(sc + 1, len - sc - 1))
                    - 100 * stod(ln_buffer.substr(0, sc));

        if (ch == 0) {
            cout << "ZERO\n";
            continue;
        }
        else if (ch < 0) {
            cout << "ERROR\n";
            continue;
        }
        else {
            for (int i = 11; i >= 0; --i) {
                int n = ch / denom[i];
                if (n) {
                    cout << note[i];
                    for (int j = 1; j < n; ++j)
                        cout << ',' << note[i];
                    ch -= n * denom[i];
                    if (ch)
                        cout << ',';
                    else
                        break;
                }
            }
            cout << endl;
        }
    }

    in.close();
    return 0;
}
*/


/*/ Reverse Groups
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer, num;
    int n;

    while (getline(in, ln_buffer)) {
        int len = ln_buffer.size();
        vector<int> ar;
        for (int i = 0; i < len; ++i) {
            char c = ln_buffer[i];
            if (c == ',' || c == ';') {
                ar.push_back(stoi(num));
                num = "";
            }
            else
                num += c;
        }
        n = stoi(num), len = ar.size();
        num = "";

        for (int i = 0; i < len / n; ++i)
            for (int j = 0; j < n / 2; ++j) {
                int temp = ar[(i + 1) * n - 1 - j];
                ar[(i + 1) * n - 1 - j] = ar[i * n + j];
                ar[i * n + j] = temp;
            }

        cout << ar[0];
        for (int i = 1; i < len; ++i)
            cout << ',' << ar[i];
        cout << endl;
    }

    in.close();
    return 0;
}
*/


/*/ String Rotation
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;

    while (getline(in, ln_buffer)) {
        int comma = ln_buffer.find(',');
        string f = ln_buffer.substr(0, comma), s = ln_buffer.substr(comma + 1, ln_buffer.size() - 1 - comma);
        int len = f.size();

        if (len != s.size()) {
            cout << "False\n";
            continue;
        }

        int match_start = s.find(f[0], 0), start = match_start;
        if (match_start == string::npos) {
            cout << "False\n";
            continue;
        }

        bool cycle = false, match;
        while (match_start != string::npos && !cycle) {
            match = true;
            for (int i = 1, j = match_start + 1; i < len; ++i, ++j) {
                if (j >= len)
                    j -= len;
                if (f[i] != s[j]) {
                    match_start = s.find(f[0], j);
                    if (match_start == start)
                        cycle = true;
                    match = false;
                    break;
                }
            }
            if (match)
                break;
        }

        if (match)
            cout << "True\n";
        else
            cout << "False\n";
    }

    in.close();
    return 0;
}
*/


/*/ Jolly Jumpers
#include <iostream>
#include <fstream>
#include <string>
#include <set>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    int n, num_1, num_2;

    while (in >> n) {
        in >> num_1;
        if (n == 1) {
            cout << "Jolly\n";
            continue;
        }

        set<int> diff;

        for (int i = 1; i < n; ++i) {
            in >> num_2;
            int x = (num_2 > num_1 ? num_2 - num_1 : num_1 - num_2);
            diff.insert(x);
            num_1 = num_2;
        }

        if (diff.size() == n - 1 && *(diff.begin()) == 1 && *prev(diff.end()) == n - 1)
            cout << "Jolly\n";
        else
            cout << "Not jolly\n";
    }

    in.close();
    return 0;
}
*/


/*/ Minimum Coins
#include <iostream>
#include <fstream>
#include <set>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    int n;

    while (in >> n) {
      int answer = 0;

      answer += n / 5;
      n -= (n / 5) * 5;

      answer += n / 3;
      n -= (n / 3) * 3;

      cout << answer + n << endl;
    }

    in.close();
    return 0;
}
*/


/*/ Flavius Josephus
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;

    while (getline(in, ln_buffer)) {
      int comma = ln_buffer.find(',');
      int n = stoi(ln_buffer.substr(0, comma)),
          m = stoi(ln_buffer.substr(comma + 1, ln_buffer.size() - 1 - comma));

      vector<int> kill_list(n, 0);

      for (int i = m - 1, j = 0, k = 0; k != n; ++i) {
        if (i >= n)
          i -= n;

        if (!kill_list[i]) {
          if (!(j % m)) {
            cout << i << ' ';
            kill_list[i] = 1;
            ++k;
          }
          ++j;
        }
      }
      cout << endl;
    }

    in.close();
    return 0;
}
*/


/*/ Find a Square
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  string ln_buffer;

  while (getline(in, ln_buffer)) {
    int v[8];
    int comma_1 = ln_buffer.find(','),
        comma_2 = ln_buffer.find(',', comma_1 + 1),
        comma_3 = ln_buffer.find(',', comma_2 + 1),
        comma_4 = ln_buffer.find(',', comma_3 + 1),
        comma_5 = ln_buffer.find(',', comma_4 + 1),
        comma_6 = ln_buffer.find(',', comma_5 + 1),
        comma_7 = ln_buffer.find(',', comma_6 + 1);

    v[0] = stoi(ln_buffer.substr(1, comma_1 - 1)),
    v[1] = stoi(ln_buffer.substr(comma_1 + 1, comma_2 - comma_1 - 2)),
    v[2] = stoi(ln_buffer.substr(comma_2 + 3, comma_3 - comma_2 - 3)),
    v[3] = stoi(ln_buffer.substr(comma_3 + 1, comma_4 - comma_3 - 2)),
    v[4] = stoi(ln_buffer.substr(comma_4 + 3, comma_5 - comma_4 - 3)),
    v[5] = stoi(ln_buffer.substr(comma_5 + 1, comma_6 - comma_5 - 2)),
    v[6] = stoi(ln_buffer.substr(comma_6 + 3, comma_7 - comma_6 - 3)),
    v[7] = stoi(ln_buffer.substr(comma_7 + 1, ln_buffer.size() - comma_7 - 2));

    int dist[3];
    dist[0] = pow(v[0] - v[2], 2) + pow(v[1] - v[3], 2),
    dist[1] = pow(v[0] - v[4], 2) + pow(v[1] - v[5], 2),
    dist[2] = pow(v[0] - v[6], 2) + pow(v[1] - v[7], 2);

    int diag, nb_1, nb_2;
    if (dist[0] > dist[1] && dist[1] == dist[2])
      diag = 1, nb_1 = 2, nb_2 = 3;
    else if (dist[1] > dist[0] && dist[0] == dist[2])
      diag = 2, nb_1 = 1, nb_2 = 3;
    else if (dist[2] > dist[0] && dist[0] == dist[1])
      diag = 3, nb_1 = 1, nb_2 = 2;
    else {
      cout << "false\n";
      continue;
    }

    if ((v[2 * nb_1] - v[0]) * (v[2 * nb_2] - v[0])
            + (v[2 * nb_1 + 1] - v[1]) * (v[2 * nb_2 + 1] - v[1]) ||
        (v[2 * nb_1] - v[2 * diag]) * (v[2 * nb_2] - v[2 * diag])
            + (v[2 * nb_1 + 1] - v[2 * diag + 1]) * (v[2 * nb_2 + 1] - v[2 * diag + 1]))
      cout << "false\n";
    else
      cout << "true\n";
  }

  in.close();
  return 0;
}
*/


/*/ A Pile of Bricks
#include <iostream>
#include <fstream>
#include <string>
#include <set>

using namespace std;

void Helper(int& a, int& b) {
  if (a > b) {
    int t = b;
    b = a;
    a = t;
  }
}

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  string ln_buffer;

  while (getline(in, ln_buffer)) {
    set<int> answer;

    int comma_1 = ln_buffer.find(','),
        c_b_1 = ln_buffer.find(']', comma_1 + 1),
        comma_2 = ln_buffer.find(',', c_b_1 + 1),
        c_b_2 = ln_buffer.find(']', comma_2 + 1),
        p_o = ln_buffer.find('(', comma_2 + 1),
        p_c = ln_buffer.find(')', p_o + 1);

    int h_x_1 = stoi(ln_buffer.substr(1, comma_1 - 1)),
        h_y_1 = stoi(ln_buffer.substr(comma_1 + 1, c_b_1 - comma_1 - 1)),
        h_x_2 = stoi(ln_buffer.substr(c_b_1 + 3, comma_2 - c_b_1 - 3)),
        h_y_2 = stoi(ln_buffer.substr(comma_2 + 1, c_b_2 - comma_2 - 1));

    int h_d_x = (h_x_2 > h_x_1 ? h_x_2 - h_x_1 : h_x_1 - h_x_2),
        h_d_y = (h_y_2 > h_y_1 ? h_y_2 - h_y_1 : h_y_1 - h_y_2);

    Helper(h_d_x, h_d_y);

    while (p_o != string::npos) {
      string brick = ln_buffer.substr(p_o + 1, p_c - p_o - 1);
      int sp_1 = brick.find(' ', 0),
          c_1 = brick.find(',', sp_1 + 1),
          c_2 = brick.find(',', c_1 + 1),
          sp_2 = brick.find(' ', c_2 + 1),
          c_3 = brick.find(',', sp_2 + 1),
          c_4 = brick.find(',', c_3 + 1);

      int id = stoi(brick.substr(0, sp_1)),
          b_x_1 = stoi(brick.substr(sp_1 + 2, c_1 - sp_1 - 2)),
          b_y_1 = stoi(brick.substr(c_1 + 1, c_2 - c_1 - 1)),
          b_z_1 = stoi(brick.substr(c_2 + 1, sp_2 - c_2 - 2)),
          b_x_2 = stoi(brick.substr(sp_2 + 2, c_3 - sp_2 - 2)),
          b_y_2 = stoi(brick.substr(c_3 + 1, c_4 - c_3 - 1)),
          b_z_2 = stoi(brick.substr(c_4 + 1, brick.size() - c_4 - 2));

      int b_d_x = (b_x_2 > b_x_1 ? b_x_2 - b_x_1 : b_x_1 - b_x_2),
          b_d_y = (b_y_2 > b_y_1 ? b_y_2 - b_y_1 : b_y_1 - b_y_2),
          b_d_z = (b_z_2 > b_z_1 ? b_z_2 - b_z_1 : b_z_1 - b_z_2);

      Helper(b_d_x, b_d_y);
      Helper(b_d_y, b_d_z);
      Helper(b_d_x, b_d_y);

      if (b_d_x <= h_d_x && b_d_y <= h_d_y)
        answer.insert(id);

      p_o = ln_buffer.find('(', p_c + 1),
      p_c = ln_buffer.find(')', p_o + 1);
    }

    if (answer.empty())
      cout << "-\n";
    else {
      for (set<int>::iterator it = answer.begin(); it != prev(answer.end()); ++it)
        cout << *it << ',';
      cout << *(prev(answer.end())) << endl;
    }
  }

  in.close();
  return 0;
}
*/


/*/ Sudoku
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <set>

using namespace std;

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  string ln_buffer;

  while (getline(in, ln_buffer)) {
    int N = ln_buffer[0] - '0', table[N][N], n = sqrt(N);

    for (int i = 2, j = 0; i < ln_buffer.size(); i += 2, ++j)
      table[j / N][j % N] = ln_buffer[i] - '0';

    bool wrong = false;

    for (int i = 0; i < N; ++i) {
      set<int> temp;

      for (int j = 0; j < N; ++j)
        temp.insert(table[i][j]);

      if (temp.size() != N) {
        wrong = true;
        break;
      }

      temp.clear();

      for (int j = 0; j < N; ++j)
        temp.insert(table[j][i]);

      if (temp.size() != N) {
        wrong = true;
        break;
      }

      temp.clear();

      for (int j = 0; j < n; ++j)
        for (int k = 0; k < n; ++k)
          temp.insert(table[(i / n) * n + j][(i % n) * n + k]);

      if (temp.size() != N) {
        wrong = true;
        break;
      }
    }

    if (wrong)
      cout << "False\n";
    else
      cout << "True\n";
  }

  in.close();
  return 0;
}
*/


/*/ Color Code Converter
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>

using namespace std;

double Abs(double x) {
  return (x >= 0 ? x : -x);
}

double Mod_2(double x) {
  int q = x / 2;
  return x - 2 * q;
}

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;

    while (getline(in, ln_buffer)) {
      char c = ln_buffer[0];
      int len = ln_buffer.size(), R, G, B;

      if (c == '(') {
        int c_1 = ln_buffer.find(','), c_2 = ln_buffer.find(',', c_1 + 1), c_3 = ln_buffer.find(',', c_2 + 1);
        double C = stod(ln_buffer.substr(1, c_1 - 1)),
               M = stod(ln_buffer.substr(c_1 + 1, c_2 - c_1 - 1)),
               Y = stod(ln_buffer.substr(c_2 + 1, c_3 - c_2 - 1)),
               K = stod(ln_buffer.substr(c_3 + 1, len - c_3 - 2));

        R = round(255 * (1 - C) * (1 - K)),
        G = round(255 * (1 - M) * (1 - K)),
        B = round(255 * (1 - Y) * (1 - K));
      }
      else if (c == '#') {
        stringstream ss;
        ss << ln_buffer.substr(1, 2);
        ss >> hex >> R;
        ss.str("");
        ss.clear();
        ss << ln_buffer.substr(3, 2);
        ss >> hex >> G;
        ss.str("");
        ss.clear();
        ss << ln_buffer.substr(5, 2);
        ss >> hex >> B;
      }
      else if (ln_buffer[2] == 'L') {
        int c_1 = ln_buffer.find(','), c_2 = ln_buffer.find(',', c_1 + 1);
        double H = stod(ln_buffer.substr(4, c_1 - 4)) / 60.,
               S = stod(ln_buffer.substr(c_1 + 1, c_2 - c_1 - 1)) / 100.,
               L = stod(ln_buffer.substr(c_2 + 1, len - c_2 - 2)) / 100.,
               C = (1 - Abs(2 * L - 1)) * S,
               X = C * (1 - Abs(Mod_2(H) - 1)),
               m = L - C / 2,
               R_p, G_p, B_p;

        if (0 <= H && H < 1)
          R_p = C, G_p = X, B_p = 0;
        else if (1 <= H && H < 2)
          R_p = X, G_p = C, B_p = 0;
        else if (2 <= H && H < 3)
          R_p = 0, G_p = C, B_p = X;
        else if (3 <= H && H < 4)
          R_p = 0, G_p = X, B_p = C;
        else if (4 <= H && H < 5)
          R_p = X, G_p = 0, B_p = C;
        else
          R_p = C, G_p = 0, B_p = X;

        R = round(255 * (R_p + m)),
        G = round(255 * (G_p + m)),
        B = round(255 * (B_p + m));
      }
      else {
        int c_1 = ln_buffer.find(','), c_2 = ln_buffer.find(',', c_1 + 1);
        double H = stod(ln_buffer.substr(4, c_1 - 4)) / 60.,
               S = stod(ln_buffer.substr(c_1 + 1, c_2 - c_1 - 1)) / 100.,
               V = stoi(ln_buffer.substr(c_2 + 1, len - c_2 - 2)) / 100.,
               C = V * S,
               X = C * (1 - Abs(Mod_2(H) - 1)),
               m = V - C,
               R_p, G_p, B_p;

        if (0 <= H && H < 1)
          R_p = C, G_p = X, B_p = 0;
        else if (1 <= H && H < 2)
          R_p = X, G_p = C, B_p = 0;
        else if (2 <= H && H < 3)
          R_p = 0, G_p = C, B_p = X;
        else if (3 <= H && H < 4)
          R_p = 0, G_p = X, B_p = C;
        else if (4 <= H && H < 5)
          R_p = X, G_p = 0, B_p = C;
        else
          R_p = C, G_p = 0, B_p = X;

        R = round(255 * (R_p + m)),
        G = round(255 * (G_p + m)),
        B = round(255 * (B_p + m));
      }

      cout << "RGB(" << R << ',' << G << ',' << B << ')' << endl;
    }

    in.close();
    return 0;
}
*/


/*/ Counting Primes
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;
    vector<long> primes;
    primes.push_back(2);

    while (getline(in, ln_buffer)) {
      int c = ln_buffer.find(','), len = ln_buffer.size();
      long N = stoi(ln_buffer.substr(0, c)),
           M = stoi(ln_buffer.substr(c + 1, len - c - 1)),
           s = primes.back() + 1;

      for (long i = s; i <= M; i = (s % 2 ? i + 2 : i + 1)) {
        bool is_prime = true;

        for (int j = 0; j < primes.size() && primes[j] * primes[j] <= i; ++j)
          if (i % primes[j] == 0) {
            is_prime = false;
            break;
          }

        if (is_prime)
          primes.push_back(i);
      }

      int answer = 0;
      for (int i = 0; i < primes.size(); ++i)
        if (N <= primes[i] && primes[i] <= M)
          ++answer;

      cout << answer << endl;
    }

    in.close();
    return 0;
}
*/


/*/ Decode Numbers
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int Count(int* ar, int s, int n) {
  if (s >= n - 1)
    return 1;

  int answer;
  if (*(ar + s) == 0 || *(ar + s) > 2 || (*(ar + s) == 2 && *(ar + s + 1) > 6))
    answer = Count(ar, s + 1, n);
  else
    answer = Count(ar, s + 1, n) + Count(ar, s + 2, n);

  return answer;
}

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;


    while (getline(in, ln_buffer)) {
      int n = ln_buffer.size(), ar[n];

      for (int i = 0; i < n; ++i)
        ar[i] = ln_buffer[i] - '0';

      cout << Count(ar, 0, n) << endl;
    }

    in.close();
    return 0;
}
*/


/*/ How to print out all the binary numbers smaller than some number
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;

    while (getline(in, ln_buffer)) {
      int sp = ln_buffer.find(' ');
      string n = ln_buffer.substr(0, sp);
      int n_len = n.size();
      char n_st[n_len];
      for (int i = 0; i < n_len; ++i)
        n_st[i] = {'A'};

      vector<int> ones;
      for (int i = 0; i < n_len; ++i)
        if (n[i] == '1')
          ones.push_back(i);

      string ch = ln_buffer.substr(sp + 1, ln_buffer.size() - sp - 1),
             ch_sh;
      ch_sh += ch[0];
      for (int i = 1; i < ch.size(); ++i)
        if (ch[i] != ch[i - 1])
          ch_sh += ch[i];

      if (ch_sh == "A") {
        cout << "Yes\n";
        continue;
      }

      for (int i = 1; i < pow(2, ones.size()); ++i) {
        for (int j = 0; j < ones.size(); ++j) {
          if (n_st[ones[j]] == 'A') {
            n_st[ones[j]] = 'B';
            break;
          }
          else
            n_st[ones[j]] = 'A';
        }
      }
    }

    in.close();
    return 0;
}
*/

/*/ Word Chain
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class WordChain {
 public:
  WordChain(string ln_buffer);

 private:
  map< char, vector<string> > head_list;
  void ChainLen();
  int Len(char c, int i);

};

WordChain::WordChain(string ln_buffer) {
  int len = ln_buffer.size();
  string word;

  int c_1 = 0;
  for (int c_2 = ln_buffer.find(','); c_1 < len; c_2 = ln_buffer.find(',', c_1)) {
    if (c_2 == string::npos)
      c_2 = len;

    word = ln_buffer.substr(c_1, c_2 - c_1);

    if (head_list.count(word[0]))
      head_list[word[0]].push_back(word);
    else {
      vector<string> temp;
      temp.push_back(word);
      head_list.insert(make_pair(word[0], temp));
    }

    c_1 = c_2 + 1;
  }

  ChainLen();
}

void WordChain::ChainLen() {
  int max = 0;
  for (map< char, vector<string> >::iterator it = head_list.begin(); it != head_list.end(); ++it)
    for (int i = 0; i < (it->second).size(); ++i) {
      int len = Len(it->first, i);
      if (len > max)
        max = len;
    }

  if (max == 1)
    cout <<"None";
  else
    cout << max;
  cout << endl;
}

int WordChain::Len(char c, int i) {
  string word = head_list[c][i];
  char e = word.back();

  if (e == '0')
    return 0;
  if (!head_list.count(e))
    return 1;
  else {
    head_list[c][i] = "000";
    int local_max = 0;
    for (int j = 0; j < head_list[e].size(); ++j) {
      int sublen = Len(e, j);
      if (sublen > local_max)
        local_max = sublen;
    }
    head_list[c][i] = word;
    return local_max + 1;
  }
}

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer, word;

    while (getline(in, ln_buffer))
      WordChain cases(ln_buffer);

    in.close();
    return 0;
}
*/


/*/ Pass Triangle - greedy, wrong
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    vector< vector<int> > triangle;

    string ln_buffer;
    while (getline(in, ln_buffer)) {
        stringstream ss(ln_buffer);
        int val;
        vector<int> temp;
        while (ss >> val)
            temp.push_back(val);
        triangle.push_back(temp);
    }

    int N = triangle.size();
    int sums[N];
    for (int i = 0; i < N; ++i)
        sums[i] = triangle[N - 1][i];

    for (int i = 0; i < N - 1; ++i) {
        sums[0] += triangle[i][0];
        sums[N - 1] += triangle[i][i];
    }

    for (int i = 1; i < N - 1; ++i) {
        int pos = i;
        for (int j = N - 2; j > 0; --j) {
            if (pos == j + 1)
              --pos;
            else if (pos)
                pos = triangle[j][pos - 1] > triangle[j][pos] ? pos - 1 : pos;

            sums[i] += triangle[j][pos];
        }
        sums[i] += triangle[0][0];
    }

    int max = 0;
    for (int i = 0; i < N; ++i)
        if (max < sums[i])
            max = sums[i];

    cout << max;

    in.close();
    return 0;
}
*/

/*/ - DP
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

int Max(int i, int j) {
  return (i > j ? i : j);
}

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    vector< vector<int> > triangle;

    string ln_buffer;
    while (getline(in, ln_buffer)) {
        stringstream ss(ln_buffer);
        int val;
        vector<int> temp;
        while (ss >> val)
            temp.push_back(val);
        triangle.push_back(temp);
    }
    int N = triangle.size();

    vector <vector<int> > sums = triangle;

    for (int i = 1; i < N; ++i) {
        sums[i][0] += sums[i - 1][0];
        sums[i][i] += sums[i - 1][i - 1];
    }

    for (int i = 2; i < N; ++i)
        for (int j = 1; j < i; ++j)
            sums[i][j] += Max(sums[i - 1][j - 1], sums[i - 1][j]);

    int max = 0;
    for (int i = 0; i < N; ++i)
        if (max < sums[N - 1][i])
            max = sums[N - 1][i];

    cout << max;

    in.close();
    return 0;
}
*/


/*/ Work Experience
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <math.h>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;
    double mon, bg_time, fin_time;

    while (getline(in, ln_buffer)) {
        int len = ln_buffer.size();
        map<double, double> time_line;
        string temp;

        for (int i = 0; i <= len; ++i) {
            char c = ln_buffer[i];
            if (c == ' ') {
              if (ln_buffer[i - 1] != ';') {
                if (temp == "Jan")
                    mon = 0;
                else if (temp == "Feb")
                    mon = 1. / 12;
                else if (temp == "Mar")
                    mon = 2. / 12;
                else if (temp == "Apr")
                    mon = 3. / 12;
                else if (temp == "May")
                    mon = 4. / 12;
                else if (temp == "Jun")
                    mon = 5. / 12;
                else if (temp == "Jul")
                    mon = 6. / 12;
                else if (temp == "Aug")
                    mon = 7. / 12;
                else if (temp == "Sep")
                    mon = 8. / 12;
                else if (temp == "Oct")
                    mon = 9. / 12;
                else if (temp == "Nov")
                    mon = 10. / 12;
                else if (temp == "Dec")
                    mon = 11. / 12;
                temp = "";
              }
              else
                continue;
            }
            else if (c == '-') {
                bg_time = stoi(temp) + mon;
                temp = "";
            }
            else if (c == ';' || i == len) {
                fin_time = stoi(temp) + mon;
                temp = "";

                if (time_line.count(bg_time) && time_line[bg_time] < fin_time)
                    time_line[bg_time] = fin_time;
                else
                  time_line.insert(make_pair(bg_time, fin_time));
            }
            else
              temp += c;
        }

        for (map<double, double>::iterator it = time_line.begin(); it != --(time_line.end()); ++it)
          for (map<double, double>::iterator it_i = next(it); it_i->first <= it->second && it_i != time_line.end(); ) {
            if (it_i->second > it->second)
              it->second = it_i->second;
            it_i = time_line.erase(it_i);
          }

        double answer = 0;
        for (map<double, double>::iterator it = time_line.begin(); it != time_line.end(); ++it)
          answer += (it->second - it->first + 1. / 12);

        int offed = (int)answer;
        if (answer - offed < .99)
          cout << offed << endl;
        else
          cout << offed + 1 << endl;
    }

    in.close();
    return 0;
}
*/


/*/ Da Vynci - in-line
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  string ln_buffer;

  while (getline(in, ln_buffer)) {
    string frag;
    vector<string> frags;   // container for page fragments

    for (int i = 0; i < ln_buffer.size(); ++i) {    // loop to fill up frags
      char c = ln_buffer[i];
      if (c == ';') {
        frags.push_back(frag);
        frag = "";
      }
      else
        frag += c;
    }
    frags.push_back(frag);

    while (frags.size() > 1) {      // repeat till frags has only 1 element,
                                    // which is the reconstructed page
      int f = 0, s = 0,     // locations of maximally overlapping fragments in frags
          max_ol = 0;       // max overlap length
      string combined;      // new fragment obtained
                            // by combining two maximally overlapping fragments

      for (int i = 0; i < frags.size(); ++i) {      // run through frags to find
        for (int j = 0; j < frags.size(); ++j) {    // the 2 fragments with maximal overlap
          if (i != j) {
            string f_str = frags[i], s_str = frags[j];
            int f_s_len = f_str.size(), s_s_len = s_str.size();
            char f_f_c = f_str[0];
            int m_f = s_str.find(f_f_c);

            if (m_f == string::npos)    // the 1st character of the 1st fragment
              continue;                 // should appear somewhere in the 2nd fragment

            int ol = 0;     // overlap length for the 2 fragments under consideration
            for (int m = m_f; m < s_s_len; ) {
              if (m - m_f > f_s_len - 1) {      // the case where the 1st fragment
                ol = f_s_len;                   // fully matches (part of) the 2nd fragment
                break;
              }

              if (f_str[m - m_f] == s_str[m]) {
                ++ol;
                ++m;
              }
              else {    // if a mismatch occurs before the end of the 2nd fragment
                ol -= (m - m_f);    // is reaches, reset ol and find
                m_f = s_str.find(f_f_c, m);     // the new 'start-matching-point'
                if (m_f == string::npos)
                  break;
                m = m_f;
              }
            }

            if (ol > max_ol) {      // if the current overlap length is the largest so far,
              f = i, s = j, max_ol = ol;    // record the locations of the 2 fragments
              if (m_f + f_s_len > s_s_len)  // and make the new fragment by combining them
                combined = s_str.substr(0, m_f) + f_str;
              else      // the case where the 1st fragment falls within the 2nd fragment
                combined = s_str;
            }
          }
        }
      }

      frags[f] = combined;      // replace one of such found fragments with the combined
      frags.erase(frags.begin() + s);   // fragment and remove the other
    }

    cout << frags[0] << endl;
  }

  in.close();
  return 0;
}
*/

/*/ - class
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class DaVyncy {
 public:
  DaVyncy(string ln_buffer);

 private:
  vector<string> frags;
  void Combine();
};

DaVyncy::DaVyncy(string ln_buffer) {
  string frag;

  for (int i = 0; i < ln_buffer.size(); ++i) {
    char c = ln_buffer[i];
    if (c == ';') {
      frags.push_back(frag);
      frag = "";
    }
    else
      frag += c;
  }
  frags.push_back(frag);

  Combine();
}

void DaVyncy::Combine() {
  while (frags.size() > 1) {
    int f = 0, b = 0, max_ol = 0;
    string combined;

    for (int i = 0; i < frags.size(); ++i) {
      for (int j = 0; j < frags.size(); ++j) {
        if (i != j) {
          string f_str = frags[i], b_str = frags[j];
          int f_s_len = f_str.size(), b_s_len = b_str.size();
          char f_f_c = f_str[0];
          int m_f = b_str.find(f_f_c);

          if (m_f == string::npos)
            continue;

          int ol = 0;
          for (int m = m_f; m < b_s_len; ) {
            if (m - m_f > f_s_len - 1) {
              ol = f_s_len;
              break;
            }

            if (f_str[m - m_f] == b_str[m]) {
              ++ol;
              ++m;
            }
            else {
              ol -= (m - m_f);
              m_f = b_str.find(f_f_c, m);
              if (m_f == string::npos)
                break;
              m = m_f;
            }
          }

          if (ol > max_ol) {
            f = i, b = j, max_ol = ol;
            combined = b_str.substr(0, m_f) + f_str;
            if (m_f + f_s_len < b_s_len)
              combined += b_str.substr(m_f + f_s_len, b_s_len - m_f - f_s_len);
          }
        }
      }
    }

    frags[f] = combined;
    frags.erase(frags.begin() + b);
  }

  cout << frags[0];
}

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  string ln_buffer;

  while (getline(in, ln_buffer)) {
    DaVyncy test(ln_buffer);
    if (in.peek() != EOF)
      cout << endl;
  }

  in.close();
  return 0;
}
*/


/*/ Bay Bridges
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <stdio.h>

using namespace std;

struct Point {  // either end of a bridge
  double x, y;

  Point() {}
  Point(double x_val, double y_val) {
    x = x_val, y = y_val;
  }
};

struct Coordis {
  Point b, e;   // b is where the bridge starts and has smaller x-coordinate.
  double s;     // e is where the bridge ends.
  bool vert;    // s is the slope of the bridge in the x-y plane.
                // vert tells if the bridge is vertical in the x-y plane.
  Coordis(double x_1, double y_1, double x_2, double y_2) {
    Point p(x_1, y_1), q(x_2, y_2);

    if (x_1 != x_2) {
      vert = false;
      b = (x_1 < x_2 ? p : q);
      e = (x_1 > x_2 ? p : q);
      s = (e.y - b.y) / (e.x - b.x);
    }
    else {
      vert = true;
      b = (y_1 < y_2 ? p : q);
      e = (y_1 > y_2 ? p : q);
    }
  }
};


int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  string ln_buffer;
  map<int, Coordis> bridges;        // contains the details of the bridges.
  map< int, set<int> > crossings;   // contains info on bridge crossing.

  while (getline(in, ln_buffer)) {
    int colon = ln_buffer.find(':'),
        c_1 = ln_buffer.find(',', colon + 1),
        c_2 = ln_buffer.find(',', c_1 + 1),
        c_3 = ln_buffer.find(',', c_2 + 1),
        len = ln_buffer.size();

    int id = stoi(ln_buffer.substr(0, colon));
    double x_1 = stod(ln_buffer.substr(colon + 4, c_1 - colon - 4)),
           y_1 = stod(ln_buffer.substr(c_1 + 1, c_2 - c_1 - 2)),
           x_2 = stod(ln_buffer.substr(c_2 + 3, c_3 - c_2 - 3)),
           y_2 = stod(ln_buffer.substr(c_3 + 1, len - c_3 - 3));

    Coordis new_brg(x_1, y_1, x_2, y_2);
    set<int> crossings_cur;

    for (map<int, Coordis>::iterator it = bridges.begin(); it != bridges.end(); ++it) {
      Coordis brg_cmprd = it->second;
      bool cross = false;   // tells if the new bridge crosses the bridge under inspection

      if (new_brg.vert) {   // the case when the new bridge is vertical
        if (brg_cmprd.vert) {
          if (new_brg.b.x == brg_cmprd.b.x
                && new_brg.b.y <= brg_cmprd.e.y
                && new_brg.e.y >= brg_cmprd.b.y)
            cross = true;
        }
        else if (brg_cmprd.b.x <= new_brg.b.x && new_brg.b.x <= brg_cmprd.e.x) {
          double y_c = brg_cmprd.s * (new_brg.b.x - brg_cmprd.b.x) + brg_cmprd.b.y;
          if (new_brg.b.y <= y_c && y_c <= new_brg.e.y)
            cross = true;
        }
      }
      else {    // the case when the new bridge is not vertical
        if (brg_cmprd.vert) {
          if (new_brg.b.x <= brg_cmprd.b.x && brg_cmprd.b.x <= new_brg.e.x) {
            double y_c = new_brg.s * (brg_cmprd.b.x - new_brg.b.x) + new_brg.b.y;
            if (brg_cmprd.b.y <= y_c && y_c <= brg_cmprd.e.y)
              cross = true;
          }
        }
        else if (new_brg.s == brg_cmprd.s) {
          if (new_brg.b.x <= brg_cmprd.b.x) {
            if (brg_cmprd.b.x <= new_brg.e.x
                  && brg_cmprd.b.y ==
                        new_brg.s * (brg_cmprd.b.x - new_brg.b.x) + new_brg.b.y)
              cross = true;
          }
          else {
            if (new_brg.b.x <= brg_cmprd.e.x
                  && new_brg.b.y ==
                        brg_cmprd.s * (new_brg.b.x - brg_cmprd.b.x) + brg_cmprd.b.y)
              cross = true;
          }
        }
        else {
          double x_c = (new_brg.s * new_brg.b.x - brg_cmprd.s * brg_cmprd.b.x
                            - new_brg.b.y + brg_cmprd.b.y)
                         / (new_brg.s - brg_cmprd.s);
          if (new_brg.b.x <= x_c && x_c <= new_brg.e.x
                && brg_cmprd.b.x <= x_c && x_c <= brg_cmprd.e.x)
            cross = true;
        }
      }

      if (cross) {
        crossings[it->first].insert(id);
        crossings_cur.insert(it->first);
      }
    }

    crossings.insert(make_pair(id, crossings_cur));
    bridges.insert(make_pair(id, new_brg));
  }

  while (1) {   // Find such a bridge, i, that # of bridges crossing it is largest.
    // crossings[i] gives the set of bridges, {i_0, i_2, ... i_k}, that cross i.
    // For each j (0 <= j <= k), remove i from crossings[i_j].
    // Then remove i from crossings.
    // Repeat till crossings[n] is empty for all n.
    int max_crossing = 0;
    map< int, set<int> >::iterator m_it;
    for (map< int, set<int> >::iterator it = crossings.begin();
            it != crossings.end(); ++it) {
      if ((it->second).size() > max_crossing) {
        max_crossing = (it->second).size();
        m_it = it;
      }
    }

    if (max_crossing) {
      for (set<int>::iterator it = (m_it->second).begin();
             it != (m_it->second).end(); ++it)
        crossings[*it].erase(m_it->first);

      crossings.erase(m_it);
    }
    else
      break;
  }

  for (map< int, set<int> >::iterator it = crossings.begin();
          it != crossings.end(); ++it)
    cout << it->first << endl;

  in.close();
  return 0;
}
*/


/*/ String Permutations
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

void Permute(vector<char>& s, int i, int& cnt, int f) {
  if (i == s.size()) {
    ++cnt;
    for (int j = 0; j < s.size(); ++j)
      cout << s[j];
    if (cnt != f)
      cout << ',';
    return;
  }

  for (int j = i; j < s.size(); ++j) {
    vector<char> temp(s.begin() + i, s.end());
    char c = s[i];
    s[i] = s[j];
    s[j] = c;
    sort(s.begin() + i + 1, s.end());
    Permute(s, i + 1, cnt, f);
    for (int k = i; k < s.size(); ++k)
      s[k] = temp[k - i];
  }
}

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;

    while (getline(in, ln_buffer)) {
      vector<char> s;
      int f = 1;
      for (int i = 0; i < ln_buffer.size(); ++i)
        f *= (i + 1);

      for (int i = 0; i < ln_buffer.size(); ++i)
        s.push_back(ln_buffer[i]);
      sort(s.begin(), s.end());

      int cnt = 0;
      Permute(s, 0, cnt, f);
      cout << endl;
    }

    in.close();
    return 0;
}
*/


/*/ The Ministry of Truth
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  string ln_buffer, word;

  while (getline(in, ln_buffer)) {
    int colon = ln_buffer.find(';');
    stringstream ss_o(ln_buffer.substr(0, colon)),
                 ss_t(ln_buffer.substr(colon + 1, ln_buffer.size() - colon - 1));
    vector<string> orig, targ;

    while (ss_o >> word)
      orig.push_back(word);
    while (ss_t >> word)
      targ.push_back(word);

    int len_o = orig.size(), len_t = targ.size(), i_t = 0;

    for (int i_o = 0; i_o < len_o; ++i_o) {
      int len_o_s = orig[i_o].size();

      if (i_t == len_t) {
        string fill_(len_o_s, '_');
        orig[i_o] = fill_;
        continue;
      }

      string s_o = orig[i_o], s_t = targ[i_t];

      int len_t_s = s_t.size(), i_t_s = 0;

      for (int i_o_s = 0; i_o_s < len_o_s; ++i_o_s) {
        if (s_o[i_o_s] != s_t[i_t_s] || i_t_s == len_t_s)
          s_o[i_o_s] = '_';
        else
          ++i_t_s;
      }

      if (i_t_s == len_t_s) {
        orig[i_o] = s_o;
        ++i_t;
      }
      else {
        string fill_(len_o_s, '_');
        orig[i_o] = fill_;
      }
    }

    if (i_t == len_t) {
      for (int i = 0; i < len_o; ++i)
        cout << orig[i] << ' ';
      cout << endl;
    }
    else
      cout << "I cannot fix history\n";
  }

  in.close();
  return 0;
}
*/


/*/ String Searching
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  string ln_buffer;

  while (getline(in, ln_buffer)) {
    int comma = ln_buffer.find(','), len = ln_buffer.size(), t_i = comma + 1;

    for (int o_i = 0; o_i < comma; ++o_i) {
      if (t_i == len)
        break;

      if (ln_buffer[o_i] == '\\')
        continue;
      if (ln_buffer[t_i] == '\\')
        ++t_i;

      if (ln_buffer[t_i] == '*' && ln_buffer[t_i - 1] != '\\') {
        if (t_i == len - 1) {
          ++t_i;
          break;
        }

        o_i = ln_buffer.find(ln_buffer[t_i + 1], o_i);
        t_i += 2;
      }
      else if (ln_buffer[o_i] == ln_buffer[t_i])
        ++t_i;
    }

    cout << (t_i == len ? "true\n" : "false\n");
  }

  in.close();
  return 0;
}
*/


/*/ String List
#include <iostream>
#include <fstream>
#include <string>
#include <set>

using namespace std;

void Prt(string S, string C, int N) {
  if (C.size() == N) {
    cout << C;

    if (C != string(N, S.back()))
      cout << ',';
    else
      cout << endl;

    return;
  }

  for (int i = 0; i < S.size(); ++i)
    Prt(S, C + S[i], N);
}

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  string ln_buffer;

  while (getline(in, ln_buffer)) {
    int comma = ln_buffer.find(','), N = stoi(ln_buffer.substr(0, comma)), len = ln_buffer.size();
    set<char> letters;

    for (int i = comma + 1; i < len; ++i) {
      char c = ln_buffer[i];
      letters.insert(c);
    }

    string S;
    for (set<char>::iterator it = letters.begin(); it != letters.end(); ++it)
      S += *it;

    for (int i = 0; i < S.size(); ++i)
      Prt(S, string(1, S[i]), N);
  }

  in.close();
  return 0;
}
*/


/*/ Word Search
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int row = 3, col = 4;

bool Found(vector< vector<char> >& grid, int i, int j, vector< vector<int> >& used, string word, int n) {
  if (n == word.size() - 1)
    return true;

  if (i + 1 < row && !used[i + 1][j] && grid[i + 1][j] == word[n + 1]) {
    used[i + 1][j] = 1;
    if (Found(grid, i + 1, j, used, word, n + 1))
      return true;
    else
      used[i + 1][j] = 0;
  }
  if (i - 1 >= 0 && !used[i - 1][j] && grid[i - 1][j] == word[n + 1]) {
    used[i - 1][j] = 1;
    if (Found(grid, i - 1, j, used, word, n + 1))
      return true;
    else
      used[i - 1][j] = 0;
  }
  if (j + 1 < col && !used[i][j + 1] && grid[i][j + 1] == word[n + 1]) {
    used[i][j + 1] = 1;
    if (Found(grid, i, j + 1, used, word, n + 1))
      return true;
    else
      used[i][j + 1] = 0;
  }
  if (j - 1 >= 0 && !used[i][j - 1] && grid[i][j - 1] == word[n + 1]) {
    used[i][j - 1] = 1;
    if (Found(grid, i, j - 1, used, word, n + 1))
      return true;
    else
      used[i][j - 1] = 0;
  }

  return false;
}

int main(int argc, char* argv[]) {
  vector< vector<char> > grid;
  char s[] = {'A', 'B', 'C', 'E', 'S', 'F', 'C', 'S', 'A', 'D', 'E', 'E'};
  for (int i = 0; i < row; ++i) {
    vector<char> temp;
    for (int j = 0; j < col; ++j)
      temp.push_back(s[col * i + j]);
    grid.push_back(temp);
  }

  ifstream in(argv[1], ios_base::in);
  string ln_buffer;

  while (getline(in, ln_buffer)) {
    bool found = false;

    for (int i = 0; i < row; ++i) {
      for (int j = 0; j < col; ++j) {
        if (grid[i][j] == ln_buffer[0]) {
          vector< vector<int> > used;
          for (int k = 0; k < row; ++k) {
            vector<int> temp(4, 0);
            used.push_back(temp);
          }

          used[i][j] = 1;

          if (Found(grid, i, j, used, ln_buffer, 0)) {
            found = true;
            break;
          }
        }
      }
      if (found)
        break;
    }

    cout << (found ? "True\n" : "False\n");
  }

  in.close();
  return 0;
}
*/


/*/ Distinct Subsequences
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int Find(string& seq, string& subseq, int s, int i) {
  int s_n = seq.find(subseq[i], s), answer = 0;

  while (s_n < seq.size() && s_n != string::npos) {
    if (i == subseq.size() - 1)
      answer += 1;
    else
      answer += Find(seq, subseq, s_n + 1, i + 1);

    s_n = seq.find(subseq[i], s_n + 1);
  }

  return answer;
}

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  string ln_buffer;

  while (getline(in, ln_buffer)) {
    int comma = ln_buffer.find(','), len = ln_buffer.size();
    string seq = ln_buffer.substr(0, comma), subseq = ln_buffer.substr(comma + 1, len - comma - 1);

    cout << Find(seq, subseq, 0, 0) << endl;
  }

  in.close();
  return 0;
}
*/


/*/ HackerRank Substring diff
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int Mijk(string P, string Q, int s_len, int S, int i, int j) {
  int f = 0, L = 0, mis_match = 0;

  for (int s = 0; s < s_len - max(i, j) - L; ++s) {
    for (int k = f; k < s_len - max(i, j); ++k) {
      if (P[i + k] != Q[j + k])
        ++mis_match;

      if (mis_match > S) {
        L = max(L, k - s);
        f = k + 1;

        while (P[i + s] == Q[j + s])
          ++s;

        --mis_match;
        break;
      }

      if (k == s_len - max(i, j) - 1)
        L = max(L, k - s + 1);
    }
  }

  return L;
}

int main() {
  int T, S;
  cin >> T;
  string P, Q;
  int answers[T];

  for (int c = 0; c < T; ++c) {
    cin >> S >> P >> Q;
    int s_len = P.size(), L_max = 0;

    for (int i = 0; i < s_len; ++i) {
      int m = max(Mijk(P, Q, s_len, S, i, 0), Mijk(P, Q, s_len, S, 0, i));
      L_max = max(m, L_max);
    }

    answers[c] = L_max;
  }

  for (int i = 0; i < T; ++i)
    cout << answers[i] << endl;

  return 0;
}
*/

/*/ N^3
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main() {
  ifstream in("1.txt", ios_base::in);
  int T, S;
  in >> T;
  string P, Q;
  int answers[T];

  for (int i = 0; i < T; ++i) {
    in >> S >> P >> Q;
    int l = P.size();

    vector< vector<int> > match;

    for (int j = 0; j < l; ++j) {
      vector<int> row;
      for (int k = 0; k < l; ++k) {
        if (P[j] == Q[k])
          row.push_back(0);
        else
          row.push_back(1);
      }
      match.push_back(row);
    }

    int l_m = 0;
    for (int j = 0; j < l; ++j) {
      for (int k = 0; k < l - j; ++k) {
        int u_t_mm = 0, l_t_mm = 0;
        for (int m = k; m < l - j; ++m) {
          if (match[m][j + m])
            ++u_t_mm;
          if (match[j + m][m])
            ++l_t_mm;
          if (u_t_mm > S && l_t_mm > S)
            break;
          if (l_m < m - k + 1)
            l_m = m - k + 1;
        }
      }
    }

    answers[i] = l_m;
  }

  for (int i = 0; i < T; ++i)
    cout << answers[i] << endl;

  in.close();
  return 0;
}
*/


/*/ Arktos, Longest Increasing Subsequence
#include <iostream>
#include <string>

using namespace std;

string LIS(string in) {
  int len = in.size();
  string lis[len];    // Let LIS memorize the longest increasing subsequences (lis) of substrings of in.
                      // Here, lis[i] is lis of in's substring of last (len - i) characters.
  lis[len - 1] = in.back();

  for (int i = len - 2; i >= 0; --i) {
    int L = 0;
    string answer;

    for (int j = i + 1; j < len; ++j) {   // For given i, check for all lis[j], j > i, which are already calculated.
      string loc_answer;

      if (in[i] < lis[j][0])          // If the char in[i] can be legitimately joined to lis[j], do it.
        loc_answer = in[i] + lis[j];
      else
        loc_answer = lis[j];

      if (loc_answer.size() > L) {    // If the resulting string is longer than the current longest one,
        L = loc_answer.size();        // update it.
        answer = loc_answer;
      }
      else if (loc_answer.size() == L && loc_answer[0] > answer[0])   // If there are many lis's of the same length
        answer = loc_answer;                     // choose the one whose 1st character is lexicographically largest.
    }

    lis[i] = answer;
  }

  return lis[0];
}

int main() {
  string word;

  while (cin >> word) {
    if (word == "quit")
      break;

    cout << LIS(word) << endl;
  }

  return 0;
}
*/


/*/ Minimum Path Sum
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  int n;
  string ln_buffer;

  while (in >> n) {
    int mat[n][n];

    for (int i = 0; i < n; ++i) {
      in >> ln_buffer;
      int len = ln_buffer.size(), idx = 0;
      string num;
      for (int j = 0; j < len; ++j) {
        char c = ln_buffer[j];
        if (c == ',') {
          mat[i][idx] = stoi(num);
          ++idx;
          num = "";
        }
        else
          num += c;
      }
      mat[i][n - 1] = stoi(num);
    }

    for (int i = n - 2; i >= 0; --i)
      mat[i][n - 1] += mat[i + 1][n - 1];
    for (int j = n - 2; j >= 0; --j)
      mat[n - 1][j] += mat[n - 1][j + 1];

    for (int i = n - 2; i >= 0; --i)
      for (int j = n - 2; j >= 0; --j)
        mat[i][j] += min(mat[i + 1][j], mat[i][j + 1]);

    cout << mat[0][0] << endl;
  }

  in.close();
  return 0;
}
*/


/*/ Digit Statistics
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  long long a, n, l_d;

  while (in >> a >> n) {
    int last_digits[9] = {0};
    last_digits[0] = a;

    int n_th = a, cycle_size = 0;

    while (cycle_size < n) {
      ++cycle_size;
      n_th *= a;
      l_d = n_th % 10;

      bool full = false;
      for (int i = 0; i < 9; ++i)
        if (last_digits[i] == l_d) {
          full = true;
          break;
        }
      if (full)
        break;
      else
        last_digits[cycle_size] = l_d;
    }

    long long digit_cnt[10] = {0};
    for (int i = 0; i < cycle_size; ++i)
      digit_cnt[last_digits[i]] = n / cycle_size;

    for (int i = 0; i < n % cycle_size; ++i)
      ++digit_cnt[last_digits[i]];

    for (int i = 0; i < 9; ++i)
      cout << i << ": " << digit_cnt[i] << ", ";
    cout << "9: " << digit_cnt[9] << endl;
  }

  in.close();
  return 0;
}
*/


/*/ Robot Movements
// Prepare a matrix, "ok", of the same size as the grid,
// and let it denote if the corresponding cell is already taken.
// Then build a recursive function, "CntPaths", whose arguments are "ok"
// and the coordinates, X, of a cell on the grid. It checks each of
// 4 neighboring points of X if it is OK to move there, and if so,
// marks this point as taken, and calls itself with this point.
// After the recursive call returns, unmark this point,
// so that the other iterations can use it.
// When it reaches the bottom-right corner, it returns 1, telling that
// it has found a path.

#include <iostream>
#include <vector>

using namespace std;

int row = 4, col = 4;

int CntPaths(int i, int j, vector< vector<int> >& ok) {
  if (i == row - 1 && j == col - 1)
    return 1;

  int answer = 0;
  if (j + 1 < col && ok[i][j + 1]) {
    ok[i][j + 1] = 0;
    answer += CntPaths(i, j + 1, ok);
    ok[i][j + 1] = 1;
  }
  if (j - 1 >= 0 && ok[i][j - 1]) {
    ok[i][j - 1] = 0;
    answer += CntPaths(i, j - 1, ok);
    ok[i][j - 1] = 1;
  }
  if (i + 1 < row && ok[i + 1][j]) {
    ok[i + 1][j] = 0;
    answer += CntPaths(i + 1, j, ok);
    ok[i + 1][j] = 1;
  }
  if (i - 1 >= 0 && ok[i - 1][j]) {
    ok[i - 1][j] = 0;
    answer += CntPaths(i - 1, j, ok);
    ok[i - 1][j] = 1;
  }

  return answer;
}

int main() {
  vector< vector<int> > ok;
  for (int i = 0; i < row; ++i) {
    vector<int> temp(col, 1);
    ok.push_back(temp);
  }

  ok[0][0] = 0;
  cout << CntPaths(0, 0, ok);

  return 0;
}
*/


/*/ Lowest Common Ancestor
#include <iostream>
#include <fstream>
#include <set>

using namespace std;

struct Node {
  int val;
  Node* p;
  Node* l;
  Node* r;
  bool is_p;
};

class BST {
 public:
  BST() {
    root = NULL;
  }
  ~BST() {
    DeleteAll(root);
  }

  void Insert(int v);
  Node* Search(int v);

 private:
  Node* root;

  void DeleteAll(Node* node);
};

void BST::Insert(int v) {
  Node* p_n = NULL;
  Node* n = root;

  while (n) {
    p_n = n;

    if (v <= n->val)
      n = n->l;
    else
      n = n->r;
  }

  Node* nn = new Node;
  nn->val = v;
  nn->p = p_n;
  nn->l = NULL;
  nn->r = NULL;
  nn->is_p = false;

  if (!root)
    root = nn;
  else {
    if (v <= p_n->val)
      p_n->l = nn;
    else
      p_n->r = nn;
  }
}

Node* BST::Search(int v) {
  Node* n = root;

  while (n->val != v) {
    if (v <= n->val)
      n = n->l;
    else
      n = n->r;
  }

  return n;
}

void BST::DeleteAll(Node* node) {
  if (!node->l && !node->r)
    delete node;
  else {
    if (node->l)
      DeleteAll(node->l);
    if (node->r)
      DeleteAll(node->r);
  }
}


int main(int argc, char* argv[]) {
  int vals[] = {30, 8, 52, 3, 20, 10, 29};
  BST sample;
  for (int i = 0; i < 7; ++i)
    sample.Insert(vals[i]);

  ifstream in(argv[1], ios_base::in);
  int a, b;

  while (in >> a >> b) {
    Node* n_a = sample.Search(a);
    Node* n_b = sample.Search(b);

    set<Node*> path_a;

    while (n_a) {
      path_a.insert(n_a);
      n_a = n_a->p;
    }

    while (!path_a.count(n_b))
      n_b = n_b->p;

    cout << n_b->val << endl;
  }

  in.close();
  return 0;
}
*/


/*/ Levenshtein Distance, hash_set

// The strategy is
//   1. Build the adjacency list of words in the word list, where an edge
//      is formed between 2 words if their Levenshtein distance is less than 2.
//   2. Run Breadth-First-Search with each test word at the root,
//      and return the size of the breadth-first-tree.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

class LevDist {
 public:
  LevDist(string filename) {
    PrepareInputs(filename);
    BFS();
  }

 private:
  vector<string> test_words;    // Container for test words
  map< int, unordered_set<string> > bins;
    // Divide words in the word list according to their lengths.
    // Hash map could have been used, but that would make the words-comparison
    // process slightly more complicated.
    // Also, since the number of "buckets" is O(10), the time gain isn't much,
    // compared to using an ordered map.
  unordered_map< string, unordered_set<string> > adj_list;    // Adjacency list

  void PrepareInputs(string filename);
  void Comp_0(int bin_num, unordered_set<string> strns);
    // Compare words of the same length.
  void Comp_1(int bin_num_l, unordered_set<string> strns_s, unordered_set<string> strns_l);
    // Compare words of length-difference 1.

  void BFS();   // Breadth-first-search
};

void LevDist::PrepareInputs(string filename) {
  ifstream in(filename, ios_base::in);
  string ln_buffer;

  while (getline(in, ln_buffer)) {    // Get test words.
    if (ln_buffer == "END OF INPUT")
      break;

    test_words.push_back(ln_buffer);
  }

  adj_list.reserve(10000);
    // In order to avoid rehashing, let the complier know how many words
    // it's gonna handle, so that it can prepare appropriate number of "buckets".

  while (getline(in, ln_buffer)) {
    int len = ln_buffer.size();

    if (bins.count(len))              // Fill up bins.
      bins[len].insert(ln_buffer);
    else {
      unordered_set<string> bin_set;
      bin_set.reserve(1000);          // Avoid rehashing.
      bin_set.insert(ln_buffer);
      bins.insert(make_pair(len, bin_set));
    }

    unordered_set<string> alist_set;    // Prepare adj_list.
    adj_list.insert(make_pair(ln_buffer, alist_set));
  }

  in.close();

  for (map< int, unordered_set<string> >::iterator it = bins.begin(); it != bins.end(); ++it) {
    int bin_num = it->first;

    Comp_0(bin_num, it->second);
        // Compare words of the same length, and fill up adj_list.

    if (it != --(bins.end()) && next(it)->first - bin_num == 1)
        // If the length-difference between 2 words
        // is 2 or more, their Lev. distance is
        // at least 2, so that there's no need to consider them.
      Comp_1(bin_num + 1, it->second, next(it)->second);
  }
}

void LevDist::Comp_0(int bin_num, unordered_set<string> strns) {
  for (unordered_set<string>::iterator it = strns.begin(); it != strns.end(); ++it)
    // Check if a word obtained by a single character replacement is present
    // in the given bin and if so, update adj_list.
    // This works faster than the brute-force compare-every-possible-pair method,
    // because characters vary only from 'a' to 'z'. If the full 256 characters were allowed
    // I'd provide a brute-force comparison method too and choose between the 2 methods
    // depending on the number of words in the given bin.
    for (int i = 0; i < bin_num; ++i) {
      char o = (*it)[i];

      for (int j = 0; j < 26; ++j) {
        string swapped = *it;
        char r = 'a' + j;

        if (r != o) {
          swapped = swapped.replace(i, 1, 1, r);

          if (strns.count(swapped)) {
            adj_list[*it].insert(swapped);
            adj_list[swapped].insert(*it);
          }
        }
      }
    }
}

void LevDist::Comp_1(int bin_num_l, unordered_set<string> strns_s,
                                    unordered_set<string> strns_l) {
  for (unordered_set<string>::iterator it = strns_l.begin(); it != strns_l.end(); ++it) {
    // Remove one character from the longer word and see if it's
    // present in the bin of shorter words.
    for (int i = 0; i < bin_num_l; ++i) {
      string removed = *it;
      removed = removed.erase(i, 1);

      if (strns_s.count(removed)) {
        adj_list[*it].insert(removed);
        adj_list[removed].insert(*it);
      }
    }
  }
}

void LevDist::BFS() {     // Textbook implementation of BFS
  int n_t = test_words.size();

  for (int i = 0; i < n_t; ++i) {
    string root = test_words[i];
    int popularity = 0;   // Size of the tree, i.e., the number of friends of the root word

    unordered_map<string, char> colors;
    colors.reserve(10000);

    for (unordered_map< string, unordered_set<string> >::iterator it = adj_list.begin();
         it != adj_list.end(); ++it)
      colors.insert(make_pair(it->first, 'w'));

    colors[root] = 'g';

    queue<string> grays;
    grays.push(root);

    while (!grays.empty()) {
      string parent = grays.front();
      grays.pop();

      for (unordered_set<string>::iterator it = adj_list[parent].begin();
           it != adj_list[parent].end(); ++it) {
        string child = *it;

        if (colors[child] == 'w') {
          colors[child] = 'g';
          grays.push(child);
        }
      }

      colors[parent] = 'b';
      ++popularity;
    }

    cout << popularity << endl;
  }
}

int main(int argc, char* argv[]) {
  //LevDist test(argv[1]);
  LevDist test("3.txt");

  return 0;
}
*/

/*/ Selectable same-size-string-comp methods
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <queue>

using namespace std;

class LevenshteinDist {
 public:
  LevenshteinDist(string filename) {
    PrepareInputs(filename);
    BFS();
  }

 private:
  vector<string> test_words;
  map< int, unordered_set<string> > bins;
  map< string, unordered_set<string> > adj_list;

  void PrepareInputs(string filename);
  void Comp_0(int bin_num, unordered_set<string> strns);
  void Comp_1(int bin_num_l, unordered_set<string> strns_s, unordered_set<string> strns_l);
  bool AreFriends_0(string a, string b);

  void BFS();
};

void LevenshteinDist::PrepareInputs(string filename) {
  ifstream in(filename, ios_base::in);
  string ln_buffer;

  while (getline(in, ln_buffer)) {
    if (ln_buffer == "END OF INPUT")
      break;

    test_words.push_back(ln_buffer);
  }

  while (getline(in, ln_buffer)) {
    int len = ln_buffer.size();

    if (bins.count(len))
      bins[len].insert(ln_buffer);
    else {
      unordered_set<string> bin_set;
      bin_set.insert(ln_buffer);
      bins.insert(make_pair(len, bin_set));
    }

    unordered_set<string> alist_set;
    adj_list.insert(make_pair(ln_buffer, alist_set));
  }

  in.close();

  for (map< int, unordered_set<string> >::iterator it = bins.begin(); it != bins.end(); ++it) {
    int bin_num = it->first;
    unordered_set<string> strns = it->second;
    int bin_size = strns.size();

    if (bin_size > 25)
      Comp_0(bin_num, strns);
    else {
      for (unordered_set<string>::iterator i_o = strns.begin(); i_o != strns.end(); ++i_o)
        for (unordered_set<string>::iterator i_i = next(i_o); i_i != strns.end(); ++i_i)
          if (AreFriends_0(*i_o, *i_i)) {
            adj_list[*i_o].insert(*i_i);
            adj_list[*i_i].insert(*i_o);
          }
    }

    if (it != --(bins.end()) && next(it)->first - bin_num == 1) {
      unordered_set<string> strns_l = next(it)->second;

      Comp_1(bin_num + 1, strns, strns_l);
    }
  }
}

void LevenshteinDist::Comp_0(int bin_num, unordered_set<string> strns) {
  for (unordered_set<string>::iterator it = strns.begin(); it != strns.end(); ++it) {
    for (int i = 0; i < bin_num; ++i) {
      char o = (*it)[i];

      for (int j = 0; j < 26; ++j) {
        string swapped = *it;
        char r = 'a' + j;

        if (r != o) {
          swapped = swapped.replace(i, 1, 1, r);

          if (strns.count(swapped)) {
            adj_list[*it].insert(swapped);
            adj_list[swapped].insert(*it);
          }
        }
      }
    }
  }
}

void LevenshteinDist::Comp_1(int bin_num_l, unordered_set<string> strns_s,
                                            unordered_set<string> strns_l) {
  for (unordered_set<string>::iterator it = strns_l.begin(); it != strns_l.end(); ++it) {
    for (int i = 0; i < bin_num_l; ++i) {
      string removed = *it;
      removed = removed.erase(i, 1);

      if (strns_s.count(removed)) {
        adj_list[*it].insert(removed);
        adj_list[removed].insert(*it);
      }
    }
  }
}

bool LevenshteinDist::AreFriends_0(string a, string b) {
  int len = a.size(), diff = 0;

  for (int i = 0; i < len; ++i) {
    if (a[i] != b[i])
      ++diff;
    if (diff > 1)
      return false;
  }

  return true;
}

void LevenshteinDist::BFS() {
  int n_t = test_words.size();

  for (int i = 0; i < n_t; ++i) {
    string root = test_words[i];
    int popularity = 0;

    map<string, char> colors;
    for (map< string, unordered_set<string> >::iterator it = adj_list.begin();
         it != adj_list.end(); ++it)
      colors.insert(make_pair(it->first, 'w'));

    colors[root] = 'g';

    queue<string> grays;
    grays.push(root);

    while (!grays.empty()) {
      string parent = grays.front();
      grays.pop();

      unordered_set<string> children = adj_list[parent];

      for (unordered_set<string>::iterator it = children.begin(); it != children.end(); ++it) {
        string child = *it;

        if (colors[child] == 'w') {
          colors[child] = 'g';
          grays.push(child);
        }
      }

      colors[parent] = 'b';
      ++popularity;
    }

    cout << popularity << endl;
  }
}

int main(int argc, char* argv[]) {
  LevenshteinDist test(argv[1]);

  return 0;
}
*/









/*/ Sequence Transformation
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;

    while (getline(in, ln_buffer)) {
      int sp = ln_buffer.find(' ');
      string n = ln_buffer.substr(0, sp), n_sh;
      n_sh += (n[0] == '1' ? '1' : 'A');
      for (int i = 1; i < n.size(); ++i) {
        if (n[i] == '1')
          n_sh += n[i];
        else if (n[i] != n[i - 1])
          n_sh += 'A';
      }
      int n_len = n_sh.size();

      string ch = ln_buffer.substr(sp + 1, ln_buffer.size() - sp - 1), ch_sh;
      ch_sh += ch[0];
      for (int i = 1; i < ch.size(); ++i)
        if (ch[i] != ch[i - 1])
          ch_sh += ch[i];
      int c_len = ch_sh.size();

      if (ch.size() < n.size()) {
        cout << "No\n";
        continue;
      }

      bool match = true;
      int i = 0, j = 0;
      while (i < c_len && j < n_len) {
        if (ch_sh[i] == 'A') {
          if (n_sh[j] == '1')
            n_sh[j] = 'A';
          if (n_sh[j + 1] == 'A')
            ++j;
        }
        else {
          if (n_sh[j] == '1')
            n_sh[j] = 'B';
          else {
            match = false;
            break;
          }
        }

        ++i, ++j;
      }

      if (match) {
        if (i != c_len)
          match = false;
        else if (j != n_len && ch_sh.back() == 'B')
          for (int k = j; k < n_len; ++k)
            if (n_sh[k] != '1') {
              match = false;
              break;
            }
      }

      cout << (match ? "Yes\n" : "No\n");
    }

    in.close();
    return 0;
}
*/


/*/ Balanced Smileys
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in(argv[1], ios_base::in);
    string ln_buffer;

    while (getline(in, ln_buffer)) {
      if (ln_buffer == "") {
        cout << "YES\n";
        continue;
      }

      int len = ln_buffer.size();

      vector<char> parentheses;
      bool imbalanced = false;
      for (int i = 0; i < len; ++i) {
        char c = ln_buffer[i];

        if (!((c >= 'a' && c <= 'z') || a == ' ' || a == ':')) {
          imbalanced = true;
          break;
        }

        if (c == '(') {
          if (i == 0)
            parentheses.push_back('o');
          else if (ln_buffer[i - 1] == ':')
            parentheses.push_back('f');
          else
            parentheses.push_back('o');
        }
        else if (c == ')') {
          if (i == 0) {
            imbalanced = true;
            break;
          }
          else if (ln_buffer[i - 1] == ':')
            parentheses.push_back('s');
          else {
            if (parentheses.empty()) {
              imbalanced = true;
              break;
            }
            else if (parentheses.back() == 'o')
              parentheses.pop_back();
            else
              parentheses.push_back('t');
          }
        }
      }

      if (imbalanced)
        cout << "NO\n";
      else {

      }
    }

    in.close();
    return 0;
}
*/


/*/ HackerRank Play game
#include <iostream>
#include <vector>

using namespace std;

int main() {
    int T, N, v;
    cin >> T;
    for (int i = 0; i < T; ++i) {
        cin >> N;
        vector<int> bricks;
        for (int j = 0; j < N; ++j) {
            cin >> v;
            bricks.push_back(v);
        }

        int taken = 0, f_score = 0, s_score = 0;

        while (taken < N) {
            int u = 3;
            f_score += (bricks[taken] + bricks[taken + 1] + bricks[taken + 2]);
            taken += u;
            while (u > 1) {
                if (f_score < s_score + bricks[taken] ||
                        f_score < s_score + bricks[taken] + bricks[taken + 1] ||
                        f_score < s_score + bricks[taken] + bricks[taken + 1] + bricks[taken + 2]) {
                    --u;
                    f_score -= bricks[taken + u];
                }
                else {
                    taken += u;
                    break;
                }
            }

            u = 3;
            s_score += (bricks[taken] + bricks[taken + 1] + bricks[taken + 2]);
            taken += u;
            while (u > 1) {
                if (s_score < f_score + bricks[taken] ||
                        s_score < f_score + bricks[taken] + bricks[taken + 1] ||
                        s_score < f_score + bricks[taken] + bricks[taken + 1] + bricks[taken + 2]) {
                    --u;
                    s_score -= bricks[taken + u];
                }
                else {
                    taken += u;
                    break;
                }
            }
        }

        cout << f_score << endl;
    }

    return 0;
}
*/


/*/ HackerRank Game of Thrones II
#include <iostream>
#include <string>
#include <map>

using namespace std;

int M = 1000000007;

unsigned long long Combination(int p, int q) {
    int n_q = (p - q > q ? q : p - q);
    unsigned long long answer = 1;

    for (int i = p; i > p - n_q; --i)
        answer = (answer * i) % M;
    for (int i = n_q; i > 0; --i)
        answer /= i;

    return answer;
}

int main() {
    string s;
    cin >> s;
    int l = s.size(), to_fill = l / 2;
    map<char, int> constituents;

    for (int i = 0; i < l; ++i) {
        char c = s[i];
        if (constituents.count(c))
            ++constituents[c];
        else
            constituents.insert(make_pair(c, 1));
    }

    long long answer = 1;

    for (map<char, int>::iterator it = constituents.begin();
        it != constituents.end(); ++it) {
        int freq = it->second / 2;

        answer = (answer * Combination(to_fill, freq)) % M;
        to_fill -= freq;
    }

    cout << answer << endl;

    return 0;
}
*/


/*/ Climbing Stairs
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  int S;

  while (in >> S) {
    double W[S];
    W[0] = 1, W[1] = 2;

    for (int i = 2; i < S; ++i)
      W[i] = W[i - 1] + W[i - 2];

    cout << W[S - 1] << endl;
  }

  in.close();
  return 0;
}
*/


/*/ Repeated Substring
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
  ifstream in(argv[1], ios_base::in);
  string ln_buffer;

  while (getline(in, ln_buffer)) {
    int len = ln_buffer.size();
    vector<string> candidates;

    for (int i = 0; i < len - 1; ++i) {
      string c;
      c += ln_buffer[i];

      for (int n = i; n < (len + i) / 2; ) {
        if (c != string(c.size(), ' ') && c == ln_buffer.substr(n + 1, c.size())) {
          candidates.push_back(c);
          n += c.size();
          c += c;
        }
        else {
          c += ln_buffer[n + 1];
          ++n;
        }
      }
    }

    if (candidates.empty())
      cout << "NONE\n";
    else {
      int m = 0, l = -1;
      for (int i = 0; i < candidates.size(); ++i) {
        if (candidates[i].size() > m) {
          m = candidates[i].size();
          l = i;
        }
      }
      cout << candidates[l] << endl;
    }
  }

  in.close();
  return 0;
}
*/





// CodeChef: Flushot Lineup
#include <iostream>
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[]) {
  int K, N;
  cin >> K;
  double T, x;

  for (int i = 0; i < K; ++i) {
    cin >> N >> T;
    double ppl[N];

    for (int j = 0; j < N; ++j) {
      cin >> x;
      ppl[j] = x;
    }

    double prev_gap = ppl[0], D = 0;

    for (int j = 1; j < N; ++j) {
      double d = ppl[j] - ppl[j - 1] - T;

      if (d < 0) {
        if (prev_gap >= -d / 2) {
          ppl[j - 1] = ppl[j - 1] + d / 2;
          ppl[j] = ppl[j] - d / 2;
          d = -d / 2;
        }
        else {
          ppl[j - 1] = ppl[j - 1] - prev_gap;
          ppl[j] = ppl[j] - d - prev_gap;
          d = -d - prev_gap;
        }
        prev_gap = 0;
      }
      else {
        prev_gap = d;
        d = 0;
      }

      if (d > D)
        D = d;
    }

    printf("%.4f\n", D);
  }

  return 0;
}

/*
    double offsetted_gaps[N + 1], prev = x;
    offsetted_gaps[0] = x, offsetted_gaps[N] = 10000000;

    for (int j = 1; j < N; ++j) {
      cin >> x;
      offsetted_gaps[j] = x - prev - T;
      prev = x;
    }

    double D = 0, d = 0;
    for (int j = 1; j < N; ++j) {
      if (offsetted_gaps[j] < 0) {
        if (offsetted_gaps[j - 1] + offsetted_gaps[j + 1] >= -offsetted_gaps[j]) {
          if (offsetted_gaps[j - 1] >= -offsetted_gaps[j] / 2 && offsetted_gaps[j + 1] >= -offsetted_gaps[j] / 2) {
            d = -offsetted_gaps[j] / 2;
            offsetted_gaps[j - 1] -= d;
            offsetted_gaps[j + 1] -= d;
          }
          else if (offsetted_gaps[j - 1] > -offsetted_gaps[j] / 2 && offsetted_gaps[j + 1] < -offsetted_gaps[j] / 2) {
            d = -offsetted_gaps[j] - offsetted_gaps[j + 1];
            offsetted_gaps[j - 1] -= d;
            offsetted_gaps[j + 1] = 0;
          }
          else if (offsetted_gaps[j - 1] < -offsetted_gaps[j] / 2 && offsetted_gaps[j + 1] > -offsetted_gaps[j] / 2) {
            d = -offsetted_gaps[j] - offsetted_gaps[j - 1];
            offsetted_gaps[j - 1] = 0;
            offsetted_gaps[j + 1] -= d;
          }
        }
        else {
          d = (offsetted_gaps[j - 1] > (-offsetted_gaps[j] - offsetted_gaps[j - 1]) ?
                   offsetted_gaps[j - 1] : (-offsetted_gaps[j] - offsetted_gaps[j - 1]));
          offsetted_gaps[j - 1] = 0;
          offsetted_gaps[j + 1] -= (-offsetted_gaps[j] - offsetted_gaps[j - 1]);
        }

        offsetted_gaps[j] = 0;

        if (d > D)
          D = d;
      }
    }

    printf("%.4f\n", D);
  }

  return 0;
}
*/






















