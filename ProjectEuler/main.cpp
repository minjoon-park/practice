#include "solutions.h"

#include <iostream>
#include <cstdint>
#include <math.h>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <map>
#include <queue>
#include <vector>
#include <algorithm>
#include <set>
#include <stack>



// Amazon 1
char Amazon1(string input) {
  char answer;
  vector<char> progress;

  for (char c : input) {
    bool found = false;
    if (!progress.empty()) {
      for (int i = 0; i < progress.size(); ++i) {
        if (c == progress[i]) {
          progress.erase(progress.begin() + i);
          found = true;
          break;
        }
      }
      if (found == false)
        progress.push_back(c);
    }
    else
      progress.push_back(c);
  }

  return progress[0];
}


int Bin_Search(int target, int* sorted_array, int start, int fin) {
  if (start > fin) {
    cout << "Not found.\n";
    return -1;
  }
  else {
    int mid = (start + fin) / 2;

    if (target == *(sorted_array + mid))
      return mid;
    else if (target > *(sorted_array + mid))
      return Bin_Search(target, sorted_array, mid + 1, fin);
    else
      return Bin_Search(target, sorted_array, start, mid - 1);
  }
}

int Bin_Search_It(int target, int* sorted_array, int array_size) {
  int start = 0, fin = array_size - 1;

  while (start <= fin) {
    int mid = (start + fin) / 2;

    if (target == *(sorted_array + mid))
      return mid;
    else if (target > *(sorted_array + mid))
      start = mid + 1;
    else
      fin = mid - 1;
  }

  cout << "Not found.\n";
  return -1;
}


/* I have an array stockPricesYesterday where:
The indices are the time, as a number of minutes past trade opening time, which was 9:30am local time.
The values are the price of Apple stock at that time, in dollars.
For example, the stock cost $500 at 10:30am, so stockPricesYesterday[60] = 500.
Write an efficient algorithm for computing the best profit I could have made from 1 purchase and 1 sale of 1 Apple stock yesterday. For this problem, we won't allow "shorting"—you must buy before you sell.
*/
void Max_Profit(double* prices, int business_mins) {
  int min_t = 0, buy, sell;
  double max_profit = 0;

  for (int i = 1; i < business_mins; ++i) {
    if (max_profit <= *(prices + i) - *(prices + min_t)) {
      max_profit = *(prices + i) - *(prices + min_t);
      sell = i;
      buy = min_t;
    }
    if (*(prices + min_t) >= *(prices + i))
      min_t = i;
  }

  cout << "Buy at t = " << buy << ", Sell at t = " << sell << ", for profit = " << max_profit << endl;
}


/* You're working with an intern that keeps coming to you with JavaScript code that won't run because the braces, brackets, and parentheses are off. To save you both some time, you decide to write a braces/brackets/parentheses validator.
Let's say:
'(', '{', '[' are called "openers."
')', '}', ']' are called "closers."
Write an efficient function that tells us whether or not an input string's openers and closers are properly nested.
Examples:
"{ [ ] ( ) }" should return true
"{ [ ( ] ) }" should return false
"{ [ }" should return false
*/
    bool Validator(string s) {
        int n = s.size();
        if (n % 2 != 0 || s[0] == '}' || s[0] == ']' || s[0] == ')')
          return false;

        stack<int> parentheses;

        for (int i = 0; i < n; ++i) {
            char paren = s[i];

            if (paren == '{')
                parentheses.push(1);
            else if (paren == '[')
                parentheses.push(2);
            else if (paren == '(')
                parentheses.push(3);
            else if (paren == '}' && !parentheses.empty() && parentheses.top() == 1)
                parentheses.pop();
            else if (paren == ']' && !parentheses.empty() && parentheses.top() == 2)
                parentheses.pop();
            else if (paren == ')' && !parentheses.empty() && parentheses.top() == 3)
                parentheses.pop();
        }

        return parentheses.empty();
    }


/* Longest Substring Without Repeating Characters
Given a string, find the length of the longest substring without repeating characters. For example, the longest substring without repeating letters for "abcabcbb" is "abc", which the length is 3. For "bbbbb" the longest substring is "b", with the length of 1.
*/
int lengthOfLongestSubstring(string s) {
  if (s.size() == 1)
    return 1;

  int start = 0, max_len = 0;

  for (int i = 1; i < s.size(); ++i) {
    bool dup = false;
    int len = i - start + 1;

    for (int j = start; j < i; ++j)
      if (s[i] == s[j]) {
        dup = true;
        start = j + 1;
        break;
      }

    if (dup)
      --len;

    if (len >= max_len)
      max_len = len;
  }

  return max_len;
}



struct ListNode {
      int val;
      ListNode *next;
      ListNode(int x) : val(x), next(NULL) {}
  };




/* ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
        int val = l1->val + l2->val;
        bool up = false;
        if (val > 9) {
            up = true;
            val -= 10;
        }

        ListNode* next_node_1 = l1->next;
        ListNode* next_node_2 = l2->next;
        ListNode* root = new ListNode(val);
        ListNode* moving_node = root;

        int val_1, val_2;
        while (next_node_1 != NULL || next_node_2 != NULL) {
            val_1 = (next_node_1 == NULL ? 0 : next_node_1->val);
            val_2 = (next_node_2 == NULL ? 0 : next_node_2->val);
            val = (up ? val_1 + val_2 + 1 : val_1 + val_2);
            up = false;
            if (val > 9) {
                up = true;
                val -= 10;
            }

            ListNode* new_node = new ListNode(val);
            moving_node->next = new_node;
            moving_node = new_node;

            next_node_1 = next_node_1->next;
            next_node_2 = next_node_2->next;
        }

        return root;
    }


    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
        int val = l1->val + l2->val;

        ListNode* next_node_1 = l1->next;
        ListNode* next_node_2 = l2->next;

        int power = 1;
        while (next_node_1 != NULL) {
            val += next_node_1->val * pow(10, power);
            next_node_1 = next_node_1->next;
            ++power;
        }

        power = 1;
        while (next_node_2 != NULL) {
            val += next_node_2->val * pow(10, power);
            next_node_2 = next_node_2->next;
            ++power;
        }



        return root;
    }
*/


    string convert(string s, int nRows) {
        if (nRows == 1)
            return s;

        vector<char> rearranged[nRows];

        for (int i = 0; i < s.size(); ++i) {
            int remainder = i % (2 * nRows - 2);

            if (remainder >= 0 && remainder < nRows)
                  rearranged[remainder].push_back(s[i]);

            if (remainder >= nRows && remainder < 2 * nRows - 2)
                  rearranged[2 * nRows - 2 - remainder].push_back(s[i]);
        }

        string out;
        for (int i = 0; i < nRows; ++i)
            for (int j = 0; j < rearranged[i].size(); ++j)
                out += rearranged[i][j];

        return out;
    }




    vector<int> twoSum(vector<int> &numbers, int target) {
        int n = numbers.size();
        vector<int> answer;

        for (int i = 0; i < n - 1; ++i)
            for (int j = i + 1; j < n; ++j) {
                if (numbers[i] + numbers[j] == target) {
                    answer.push_back(i);
                    answer.push_back(j);
                }
            }

        return answer;
    }



/*
// Given a linked list, return the node where the cycle begins. If there is no cycle, return null.
    ListNode *detectCycle(ListNode *head) {
        ListNode* next_node = head;

        set<ListNode*> prev_nodes;

        while (next_node) {
            if (prev_nodes.count(next_node))
                return next_node;

            prev_nodes.insert(next_node);
            next_node = next_node->next;
        }

        return NULL;
    }
*/




/*
    bool isConnected(string a, string b) {
        int n = a.size(), m = 0;

        for (int i = 0; i < n; ++i)
            if (a[i] == b[i])
                ++m;

        return (m == n - 1);
    }

    struct Node {
        string word;
        int dist;
        char color;
        Node* parent;
        Node(string a) : word(a), dist(0), color('w'), parent(NULL) {}
    };

    int ladderLength(string start, string end, unordered_set<string> &dict) {
        int n = start.size();


    }
*/




/*    int minimumTotal(vector< vector<int> > &triangle) {
        int n = triangle.size(), min = 1000000;
        vector< vector<int> > sum = triangle;

        for (int i = 1; i < n; ++i) {
            sum[i][0] += sum[i - 1][0];
            sum[i][i] += sum[i - 1][i - 1];
        }

        for (int i = 2; i < n; ++i)
            for (int j = 1; j < i; ++j)
                sum[i][j] += (sum[i - 1][j - 1] <= sum[i - 1][j] ? sum[i - 1][j - 1] : sum[i - 1][j]);

        for (int i = 0; i < n; ++i)
            if (sum[n - 1][i] <= min)
                min = sum[n - 1][i];

        return min;
    }
*/



    int minimumTotal(vector< vector<int> > &triangle) {
        int n = triangle.size(), min = 1000000;
        vector<int> sum = triangle[n - 1];

        for (int i = 0; i < n; ++i) {
            int loc = i;
            for (int j = n - 2; j >= 0; --j) {
                if (loc == 0)
                    sum[i] += triangle[j][0];
                else if (loc == j + 1) {
                    sum[i] += triangle[j][j];
                    --loc;
                }
                else {
                    loc = (triangle[j][loc - 1] < triangle[j][loc] ? loc - 1 : loc);
                    sum[i] += triangle[j][loc];
                }
            }
        }

        for (int i = 0; i < n; ++i)
            if (sum[i] <= min)
                min = sum[i];

        return min;
    }


    int evalRPN(vector<string> &tokens) {
        stack<int> temp;

        for (int i = 0; i < tokens.size(); ++i) {
            if (tokens[i] == "+" || tokens[i] == "-" || tokens[i] == "*" || tokens[i] == "/") {
                if (i < 2) {
                    cout << "input format error\n";
                    return -1;
                }

                int a = temp.top();
                temp.pop();
                int b = temp.top();
                temp.pop();

                if (tokens[i] == "+")
                    temp.push(a + b);
                else if (tokens[i] == "*")
                    temp.push(a * b);
                else if (tokens[i] == "-")
                    temp.push(b - a);
                else if (tokens[i] == "/") {
                    if (a == 0) {
                        cout << "can't divide by 0\n";
                        return -1;
                    }
                    temp.push(b / a);
                }
            }
            else
                temp.push(stoi(tokens[i]));
        }

        if (temp.size() != 1) {
            cout << "input format error\n";
            return -1;
        }

        return temp.top();
    }


/*    void reverseWords(string &s) {
        stringstream ss(s);
        string word = "";
        stack<string> temp;
        while (ss >> word)
            temp.push(word);

        if (temp.empty()) {
            s = "";
            return;
        }

        s = "";

        while (temp.size() > 1) {
            s += temp.top() + ' ';
            temp.pop();
        }

        s += temp.top();
    }
*/

    void reverseWords(string &s) {
        int n = s.size();
        while (s[0] == ' ') {
            s.erase(0, 1);
            --n;
        }
        while (s[n - 1] == ' ') {
            s.erase(n - 1, 1);
            --n;
        }

        string word;
        int pos = 0, end = n;

        for (int i = 0; i < n; ++i) {
            if (s[pos] == ' ') {
                s.erase(pos, 1);
                --end;
                s.erase(0, pos);
                end -= pos;

                if (word != "") {
                    s.insert(end, " ");
                    s.insert(end + 1, word);
                    word = "";
                    pos = 0;
                }
            }
            else {
                word += s[pos];
                ++pos;
            }
        }
    }


    vector< vector<int> > generate(int numRows) {
        vector< vector<int> > result;

        if (numRows == 0)
            return result;

        vector<int> row, prev_row;
        row.push_back(1);
        result.push_back(row);
        prev_row = row;

        for (int i = 1; i < numRows; ++i) {
            row.clear();
            row.push_back(1);
            for (int j = 1; j < i; ++j)
                row.push_back(prev_row[j - 1] + prev_row[j]);
            row.push_back(1);
            result.push_back(row);
            prev_row = row;
        }

        return result;
    }

    vector<int> getRow(int rowIndex) {
        vector<int> row, prev_row;

        row.push_back(1);
        prev_row = row;

        for (int i = 1; i <= rowIndex; ++i) {
            row.clear();
            row.push_back(1);
            for (int j = 1; j < i; ++j)
                row.push_back(prev_row[j - 1] + prev_row[j]);
            row.push_back(1);
            prev_row = row;
        }

        return row;
    }




 struct TreeNode {
     int val;
     TreeNode *left;
     TreeNode *right;
     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 };

    bool hasPathSum(TreeNode *root, int sum) {
        if (root == NULL)
            return false;

        if (root->left == NULL && root->right == NULL)
            return sum - root->val == 0 ? true : false;

        if (root->left)
            if (hasPathSum(root->left, sum - root->val))
                return true;
        if (root->right)
            if (hasPathSum(root->right, sum - root->val))
                return true;

        return false;
    }



class Solution {
public:
    Solution() {
        sum = 0;
    }

    int sumNumbers(TreeNode *root) {
        pathSum(root, "");

        return sum;
    }

private:
    int sum;

    void pathSum(TreeNode* root, string s) {
        if (root->left == NULL && root->right == NULL) {
            sum += stoi(s + to_string(root->val));
            return;
        }

        if (root->left != NULL)
            pathSum(root->left, s + to_string(root->val));
        if (root->right != NULL)
            pathSum(root->right, s + to_string(root->val));
    }
};



    string addBinary(string a, string b) {
        int i_a = 0, i_b = 0, i_s;

        for (int i = 0; i < a.size(); ++i)
            i_a = i_a * 2 + a[i] - '0';
            cout << i_a << endl;
        for (int i = 0; i < b.size(); ++i)
            i_b = i_b * 2 + b[i] - '0';
            cout << i_b << endl;
        i_s = i_a + i_b;

        int remainder = i_s % 2;
        string s = to_string(remainder);

        while(i_s > 2) {
            i_s = (i_s - remainder) / 2;
            remainder = i_s % 2;
            s += to_string(remainder);
        }

        s += "1";

        for (int i = 0; i < s.size() / 2; ++i) {
            char t = s[i];
            s[i] = s[s.size() - 1 - i];
            s[s.size() - 1 - i] = t;
        }

        return s;
    }




    int largestRectangleArea(vector<int> &height) {
        int n = height.size(), max = 0;
        vector< vector<int> > areas;

        for (int i = 0; i < n; ++i) {
            vector<int>* area = new vector<int>;
            area->push_back(height[i]);
            areas.push_back(*area);
            if (areas[i][0] >= max)
                max = areas[i][0];
        }

        for (int i = 0; i < n - 1; ++i)
            for (int j = 1; j < n - i; ++j) {
                areas[i].push_back(areas[i][j - 1] / j <= height[i + j] ? areas[i][j - 1] / j * (j + 1) : height[i + j] * (j + 1));
                if (areas[i][j] >= max)
                    max = areas[i][j];
            }

        return max;
    }




int largestArea(int arr[], int len) {
int area[len]; //initialize it to 0
int n, i, t;
stack<int> St;  //include stack for using this #include<stack>
bool done;

for (i=0; i<len; i++)
{
while (!St.empty())
{
   if(arr[i] <= arr[St.top()])
   {
       St.pop();
   }
   else
       break;
}
if(St.empty())
   t = -1;
else
   t = St.top();
//Calculating Li
area[i] = i - t - 1;
St.push(i);
}

//clearing stack for finding Ri
while (!St.empty())
St.pop();

for (i=len-1; i>=0; i--)
{
while (!St.empty())
{
   if(arr[i] <= arr[St.top()])
   {
       St.pop();
   }
   else
       break;
}
if(St.empty())
   t = len;
else
   t = St.top();
//calculating Ri, after this step area[i] = Li + Ri
area[i] += t - i -1;
St.push(i);
}

int max = 0;
//Calculating Area[i] and find max Area
for (i=0; i<len; i++)
{
area[i] = arr[i] * (area[i] + 1);
if (area[i] > max)
   max = area[i];
}

return max;
}




/*    vector<int> twoSum(vector<int> &numbers, int target) {
        int n = numbers.size(), i = 0, j = n - 1, k;
        vector<int> answer;

        while (i < j) {
            k = numbers[i] + numbers[j];
            if (k > target)
                --j;
            else if (k < target)
                ++i;
            else {
                answer.push_back(i + 1);
                answer.push_back(j + 1);
                break;
            }
        }

        return answer;
    }
*/





/* ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
        if (!l1 && !l2)
            return NULL;

        ListNode* node_1 = l1;
        ListNode* node_2 = l2;
        ListNode* root = NULL;
        bool up = false;
        int val_1, val_2, sum;
        ListNode* next_node = NULL;

        while (node_1 || node_2 || up) {
            val_1 = node_1 ? node_1->val : 0;
            val_2 = node_2 ? node_2->val : 0;
            sum = val_1 + val_2;
            if (up) {
                ++sum;
                up = false;
            }

            if (sum > 9) {
                sum -= 10;
                up = true;
            }

            if (!root) {
                root = new ListNode(sum);
                next_node = root;
            }
            else {
                next_node->next = new ListNode(sum);
                next_node = next_node->next;
            }

            if (node_1)
                node_1 = node_1->next;
            if (node_2)
                node_2 = node_2->next;
        }

        return root;
    }
*/

     ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
       ListNode *res=NULL, *tail=NULL;
       ListNode *L1=l1, *L2=l2;
       int carry=0;
       int sum=0;
       while(L1!=NULL||L2!=NULL||carry!=0)
       {
           int num1=(L1==NULL)? 0 :L1->val;
           int num2=(L2==NULL)? 0 :L2->val;
           sum=num1+num2+carry;
           carry=sum/10;
           sum%=10;

           if(res==NULL)
           {
               res=new ListNode(sum);
               tail=res;;
           }
           else
           {
               tail->next=new ListNode(sum);
               tail=tail->next;

           }

           L1=(L1==NULL)? NULL :L1->next;
           L2=(L2==NULL)? NULL :L2->next;
       }

       return res;

    }





int main() {
/*  //P15 test15;
  //cout << test15.AnswerDP(20, 20) << endl;

  //P67 test67;
  //cout << test67.Answer();

  //P443 test443;
  //cout << test443.Answer(100000) << endl;

  //P262 test262;
  //test262.Answer();

  //P243 test243;
  //cout << test243.Answer();

  //MinStepsToOne test;
  //cout << test.Answer(7) << endl;

  //N_Body test(2500, 60000);
  //test.LeapFrog();

  //BinPacking test;
  //test.PackBins();

  //KevinBacon test;
  //test.BuildAdjListAndVertices();
  //test.BuildBFT("Jolie, Angelina");
  //test.BFS("Cooper, Bradley");
*/

/*  Genome test("GGGAATCACGAGAGCAGACAGATCACACAGGTTTATGGGTTCTACGACGAGTGTTTA",
              "GGGAATCATGAGAGCAGACGATCACACAAGTTTATGGTTTCTATGATGAATGTTTA");
  cout << test.GSA() << endl;
*/

/*  Routing test;
  test.BuildAdjListAndVertices();
  int source = 0;
  test.Dijkstra(source);
  while (1) {
    int dest;
    cin >> dest;
    if (dest == source)
      break;
    test.GetRoute(dest);
  }
*/

  /*Amazon2 test;
  for (vector< pair<int, int> >::iterator it = test.athletes.begin(); it != test.athletes.end(); ++it)
    test.Tower(it);
  cout << test.max_height << endl;*/

  /*double prices[20] = {500, 453, 4778, 76869, 87, 4758, 879, 679, 568, 89,
  346, 58, 600, 400, 67, 8, 8906, 700, 456 ,450};

  Max_Profit(prices, 20);

  string x = "abcdefg";
  InPlaceReverse(x);*/

  /*int myints[] = {};
  vector<int> numbers(myints, myints + sizeof(myints) / sizeof(int) );

  vector<int> answer = twoSum(numbers, 16021);

  cout << answer[0] << '\t' << answer[1];*/

  /*vector<int> a, b, c;
  a.push_back(-1);
  b.push_back(2);
  b.push_back(3);
  c.push_back(1);
  c.push_back(-1);
  c.push_back(-3);
  vector< vector<int> > d;
  d.push_back(a);
  d.push_back(b);
  d.push_back(c);

  cout << minimumTotal(d);*/

  /*string s = "the sky is blue  ";
  reverseWords(s);
  cout << s << 'N';
*/

  /*TreeNode a1(5), b1(4), b2(8), c1(1), c2(3), c3(4), d1(7), d2(2), d3(1);
  a1.left = &b1;
  a1.right = &b2;
  b1.left = &c1;
  b2.left = &c2;
  b2.right = &c3;
  c1.left = &d1;
  c1.right = &d2;
  c3.right = &d3;

  Solution test;
  cout << test.sumNumbers(&a1);
*/

/*  vector<int> height;
  height.push_back(2);
  height.push_back(1);
  height.push_back(5);
  height.push_back(6);
  height.push_back(2);
  height.push_back(3);
  cout << largestRectangleArea(height) << endl;

  int arr[] = {2, 1, 5, 6, 2, 3};
  cout << largestArea(arr, 6) << endl;
*/

    int N;
    cin >> N;
    long long int ar[N], P, Q, diff[N - 1];
    for (int i = 0; i < N; ++i)
        cin >> ar[i];
    cin >> P >> Q;

    sort(ar, ar + N - 1);

    long long int lower, upper;
    if (P < ar[0] && Q < ar[N - 1]) {
        lower = ar[0] - P;
        upper = 0;
    }
    else if (P > ar[0] && Q > ar[N - 1]) {
        lower = 0;
        upper = Q - ar[N - 1];
    }
    else if (P < ar[0] && Q > ar[N - 1]) {
        lower = ar[0] - P;
        upper = Q - ar[N - 1];
    }
    else {
        lower = 0;
        upper = 0;
    }

    if (N == 1) {
        cout << (lower > upper ? P : Q);
        return 0;
    }

    long long int max_diff = 0;
    for (int i = 0; i < N - 1; ++i) {
        diff[i] = ar[i + 1] - ar[i];
        if (diff[i] / 2 > max_diff)
            max_diff = diff[i] / 2;
    }

    set<long long int> answer;
    for (int i = 0; i < N - 1; ++i){
        if (diff[i] / 2 == max_diff)
            answer.insert(ar[i] + max_diff);
    }

    for (set<long long int>::iterator it = answer.begin();
         !(*it >= P && *it <= Q) && it != answer.end(); )
        it = answer.erase(it);
    if (answer.empty())
        max_diff = 0;

    if (lower >= upper && lower >= max_diff)
        cout << P;
    else if (upper > lower && upper > max_diff)
        cout << Q;
    else if (max_diff > lower && max_diff >= upper)
        cout << *(answer.begin());

  return 0;
}











/* I'm making a search engine called MillionGazillion¢â.
I wrote a crawler that visits web pages, stores a few keywords in a database, and follows links to other web pages. I noticed that my crawler was wasting a lot of time visiting the same pages over and over, so I made a hash table visited where I'm storing URLs I've already visited. Now the crawler only visits a URL if it hasn't already been visited.
Thing is, the crawler is running on my old desktop computer in my parents' basement (where I totally don't live anymore), and it keeps running out of memory because visited is getting so huge.
How can I trim down the amount of space taken up by visited?
*/




/* Write a function to reverse an array of characters in place.
"In place" means "without creating a new string in memory."
void InPlaceReverse(string& in) {
  int len = in.size();

  for (int i = 0; i < len / 2; ++i) {
    char x = in[i];
    in[i] = in[len - 1 - i];
    in[len - 1 - i] = x;
  }

  cout << in << endl;
}
*/



/* Implement a queue with 2 stacks.

enqueue(a) {
  while (stack_2 != empty()) {
    x = stack_2.pop();
    stack_1.push(x);
  }

  stack_1.push(a);

  while (stack_1 != empty()) {
    x = stack_1.pop();
    stack_2.push(x);
  }
}

dequeue(a) {
  stack_2.pop();
}
*/



