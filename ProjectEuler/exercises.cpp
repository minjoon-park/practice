/*/ Inverted-index, function style

// Build the list of files
int list_files(const string& dir_name, vector<string>* p_file_list) {
  DIR* dir;
  struct dirent* ent;

  if((dir = opendir(dir_name.c_str())) != NULL) {
    while(((ent = readdir(dir)) != NULL))  {
      if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
        continue;

      string subdir_str;
      subdir_str = subdir_str + dir_name + "\\" + ent->d_name;

      if((opendir(subdir_str.c_str()) != NULL))
        list_files(subdir_str.c_str(), p_file_list);
      else
        p_file_list->push_back(subdir_str);
    }
    closedir(dir);
  }
  else {
    perror ("");
    return -1;
  }

  return 0;
}

// Build the inverted index
void Build_Inverted_Index(const vector<string>& file_list, multimap<string, pair<string, int> >* p_Inverted_index) {
  for(int i = 0; i < file_list.size(); ++i) {
    fstream working_file(file_list[i].c_str(), ios_base::in);

    string temp;
    int loc = 0;

    while(working_file >> temp) {
      ++loc;
      (*p_Inverted_index).insert(make_pair(temp, make_pair(file_list[i], loc)));
    }

    working_file.close();
  }
}

int main() {
  vector<string> file_list;
  multimap<string, pair<string, int> > Inverted_index;

  //list_files("C:\\Users\\Minjoon\\Documents\\Dropbox\\works\\content_hillaryclinton_s1992_copypruned", file_list, num_files);

  list_files("C:\\Users\\Minjoon\\Documents\\Dropbox\\works\\sample", &file_list);

  Build_Inverted_Index(file_list, &Inverted_index);

  string keyword;
  cout << "Enter your keyword:" << endl;
  while(cin >> keyword) {
    int word_count = 0;
    pair<multimap<string, pair<string, int> >::iterator, multimap<string, pair<string, int> >::iterator> it_loc = Inverted_index.equal_range(keyword);

    for(multimap<string, pair<string, int> >::iterator it = it_loc.first; it != it_loc.second; ++it) {
      ++word_count;

      fstream file_found((it->second).first.c_str(), ios_base::in);
      int loc_in_file_found = (it->second).second, loc_in_file = 0;
      const int buffer = 2;
      string temp;

      cout << (it->second).first << " :" << endl << '\t';
      while(file_found >> temp) {
        ++loc_in_file;

        if(loc_in_file_found - buffer <= loc_in_file && loc_in_file <= loc_in_file_found + buffer)
          cout << temp << " ";
      }
      cout << endl;

      file_found.close();
    }

    cout << "There are " << word_count << " appearance(s) of \"" << keyword << "\" in all the files under inspection." << endl;
  }

  return 0;
}

*/

/*/ Single file keyword search and print

int main() {
  fstream myFile("C:\\Users\\Minjoon\\Documents\\Dropbox\\works\\sample\\514.atc", ios_base::in);

  vector<string> word_array;
  vector<int> found;
  const int buffer = 2;
  string temp, search_keyword;
  cin >> search_keyword;

  if(myFile.is_open()) {
    while(myFile >> temp) {
      word_array.push_back(temp);

      if(!search_keyword.compare(temp))
        found.push_back(word_array.size() - 1);
    }
  myFile.close();
  }
  else {
    cout << "File open error." << endl;
    return -1;
  }

  if(found.size() > 0) {
    for(int i = 0; i < buffer; ++i) {
      if(found[i] < buffer)
        for(int j = - found[i]; j <= buffer; ++j)
          cout << word_array[found[i] + j] << " ";
      else
        for(int j = - buffer; j <= buffer; ++j)
          cout << word_array[found[i] + j] << " ";
      cout << endl;
    }

    for(int i = buffer; i < found.size() - buffer; ++i) {
      for(int j = - buffer; j <= buffer; ++j)
        cout << word_array[found[i] + j] << " ";
      cout << endl;
    }

    for(int i = found.size() - buffer; i < found.size(); ++i) {
      if(found[i] > word_array.size() - 1 - buffer)
        for(int j = - buffer; j <= word_array.size() - found[i] - 1; ++j)
          cout << word_array[found[i] + j] << " ";
      else
        for(int j = - buffer; j <= buffer; ++j)
          cout << word_array[found[i] + j] << " ";
      cout << endl;
    }
  }
  else {
    cout << "none found." << endl;
    return -1;
  }

  return 0;
}

*/

/*/ Liftoff

1. Intersection of unsorted lists of strings
Sample args:
["a", "b", "e", "e", "e"]
["c", "a", "a", "e", "e", "d", "foo"]
Sample return value:
=> ["a", "e", "e"]

#include <iostream>
#include <map>

using namespace std;

int main() {
  string list_1[n_1] = { ... }, list_2[n_2] = { ... };

  map<string, int> map_1, map_2;

  for (int i = 0; i < n_1; ++i) {
    if (map_1.count(list_1[i]))
      ++map_1[list_1[i]];
    else
      map_1.insert(make_pair(list_1[i], 1));
  }
  for (int i = 0; i < n_2; ++i) {
    if (map_2.count(list_2[i]))
      ++map_2[list_2[i]];
    else
      map_2.insert(make_pair(list_2[i], 1));
  }

  for (map<string, int>::iterator it = map_1.begin(); it != map_1.end(); ++it) {
    if (map_2.count(it->first)) {
      int m = (it->second > map_2[it->first] ? map_2[it->first] : it->second);
      for (int i = 0; i < m; ++i)
        cout << it->first;
    }
  }

  return 0;
}


2. Write print_diagonals(matrix)
Sample arg:
[[1,2,3],
 [4,5,6],
 [7,8,9],
 [a,b,c]]
Print:
a 7 b 4 8 c 1 5 9 2 6 3


print_diagonal(matrix mat, int n, int m) {
  int i = n - 1, j = 0, k = n - 1;

  while (i >= 0 && j < m) {
    int a = i;
    while (a < n && j < m) {
      cout << mat[a][j] << ' ';
      ++a;
      ++j;
    }
    --i;
    --k;
    if (i < 0) {
      i = 0;
      j = -k;
    }
    else
      j = 0;
  }

}


3.
Sample data:
NYC 10
SF 0.9
London 6

Write a function which randomly returns a city with probability proportional to that city's population.
P(return 'NYC') = 10 / 16.9
*/






/*/ sorting

// heap sort
void max_heapify(vector<int>* p_data_array, int heap_size, int root_index) {
  int largest = 0, l_index = 2 * root_index, r_index = 2 * root_index + 1, temp = 0;

  if(l_index <= heap_size && (*p_data_array)[root_index - 1] < (*p_data_array)[l_index - 1])
    largest = l_index;
  else largest = root_index;
  if(r_index <= heap_size && (*p_data_array)[largest - 1] < (*p_data_array)[r_index - 1])
    largest = r_index;

  if(largest != root_index) {
    temp = (*p_data_array)[root_index - 1];
    (*p_data_array)[root_index - 1] = (*p_data_array)[largest - 1];
    (*p_data_array)[largest - 1] = temp;
    max_heapify(p_data_array, heap_size, largest);
  }
}

void build_max_heap(vector<int>* p_data_array, int array_size) {
  for(int i = array_size / 2; i > 0; --i)
    max_heapify(p_data_array, array_size, i);
}

void heap_sort(vector<int>* p_data_array, int array_size) {
  build_max_heap(p_data_array, array_size);
  int temp = 0;

  for(int i = array_size - 1; i > 0; --i) {
    temp = (*p_data_array)[0];
    (*p_data_array)[0] = (*p_data_array)[i];
    (*p_data_array)[i] = temp;
    max_heapify(p_data_array, i, 1);
  }
}


// quick sort
int partitioning(vector<int>* p_data_array, int start_index, int end_index) {
  int divider = start_index - 1, pivot = (*p_data_array)[end_index];

  for(int i = start_index, temp = 0; i < end_index; ++i) {
    if((*p_data_array)[i] <= pivot) {
      ++divider;
      temp = (*p_data_array)[i];
      (*p_data_array)[i] = (*p_data_array)[divider];
      (*p_data_array)[divider] = temp;
    }
  }
  (*p_data_array)[end_index] = (*p_data_array)[divider + 1];
  (*p_data_array)[divider + 1] = pivot;

  return divider + 1;
}

void quick_sort(vector<int>* p_data_array, int start_index, int end_index) {
  if(start_index < end_index) {
    int d = partitioning(p_data_array, start_index, end_index);

    quick_sort(p_data_array, start_index, d - 1);
    quick_sort(p_data_array, d + 1, end_index);
  }
}


// Insertion Sort
void InsertionSort(int* input, int N) {
  for(int i = 1; i < N; ++i) {
    int temp = input[i];
    int j = i;
    while(j > 0 && temp < input[j - 1]) {
      input[j] = input[j - 1];
      --j;
    }
    input[j] = temp;
  }
}


// Merge Sort
void Merge(int* input, int p, int q, int r) {
  int n_l = q - p + 1;
  int n_r = r - q;

  int array_l[n_l + 1];
  int array_r[n_r + 1];

  for(int i = 0; i < n_l; ++i)
    array_l[i] = input[p + i];
  array_l[n_l] = 10000;

  for(int i = 0; i < n_r; ++i)
    array_r[i] = input[q + 1 + i];
  array_r[n_r] = 10000;

  int i_l = 0, i_r = 0;

  for(int i = p; i <= r; ++i) {
    if(array_l[i_l] > array_r[i_r]) {
      input[i] = array_r[i_r];
      ++i_r;
    }
    else {
      input[i] = array_l[i_l];
      ++i_l;
    }
  }
}

void MergeSortRec(int* input, int p, int q) {
  int r = (p + q) / 2;

  if(p < q) {
    MergeSortRec(input, p, r);
    MergeSortRec(input, r + 1, q);
    Merge(input, p, r, q);
  }
}

void MergeSortIt(int* input, int N) {
  for(int i = 1; i <= N / 2 + 1; i *= 2)
    for(int j = i; j < N; j += 2 * i) {
      int q = (j + i >= N) ? (N - 1) : (j + i - 1);
      Merge(input, j - i, j - 1, q);
    }
}
*/


/*/ permutation

// generic
void swap_ftn(int* p_former, int* p_latter) {
  int temp = *p_former;
  *p_former = *p_latter;
  *p_latter = temp;
}

void permutation(int* p_data_array, int current_pos, int array_size) {
  if(current_pos == array_size - 1) {
    for(int j = 0; j < array_size; ++j)
      cout << *(p_data_array + j) << '\t';
    cout << endl;
  }
  else {
    for(int i = current_pos; i < array_size; ++i) {
      swap_ftn(p_data_array + i, p_data_array + current_pos);
      permutation(p_data_array, current_pos + 1, array_size);
      swap_ftn(p_data_array + current_pos, p_data_array + i);
    }
  }
}

// char
void permute_swap(char* a, int i, int j) {
  char x = a[i];
  a[i] = a[j];
  a[j] = x;
}

void permute(char* a, int N, int x) {
  if(x == N) {
    for(int i = 0; i < N; ++i)
      cout << a[i];
    cout << endl;
    return;
  }

  for(int i = x; i < N; ++i) {
    permute_swap(a, i, x);
    permute(a, N, x + 1);
    permute_swap(a, i, x);
  }
}
*/


/*/ Stack with GetMin
class StackSample {
 public:
  StackSample(int n);
  void Push(int x);
  int Pop();
  void GetMin();
  void PrintStack();

 private:
  int stack_size;
  int* stack_array;
  int* min_array;
  int stack_end_loc, min_end_loc;
};

StackSample::StackSample(int n) {
  stack_size = n;
  stack_end_loc = 0;
  min_end_loc = 0;
  stack_array = new int[n];
  min_array = new int[n];
}

StackSample::~StackSample() {
  delete stack_array;
  delete min_array;
}

void StackSample::Push(int x) {
  if(stack_end_loc == stack_size) {
    cout << "Stack full!" << endl;
    return;
  }
  stack_array[stack_end_loc] = x;
  ++stack_end_loc;
  if(x <= min_array[min_end_loc - 1]) {
    min_array[min_end_loc] = x;
    ++min_end_loc;
  }
}

int StackSample::Pop() {
  if(stack_end_loc == 0) {
    cout << "Stack empty!" << endl;
    return -1;
  }
  --stack_end_loc;
  if(stack_array[stack_end_loc] == min_array[min_end_loc - 1])
    --min_end_loc;
  return stack_array[stack_end_loc];
}

void StackSample::GetMin() {
  cout << min_array[min_end_loc - 1] << endl;
}

void StackSample::PrintStack() {
  if(stack_end_loc == 0) {
    cout << "Stack empty!" << endl;
    return;
  }
  for(int i = 0; i < stack_end_loc; ++i)
    cout << stack_array[i] << " ";
  cout << endl;
}
*/


/*/ minimize the trace by shuffling rows

void swap_ftn(string* p_former, string* p_latter) {
  string temp = *p_former;
  *p_former = *p_latter;
  *p_latter = temp;
}

void obt_trace(int** p_data_array, int current_pos, int array_size, vector<int>* p_output) {
  if(current_pos == array_size - 1) {
    int trace = 0;
    for(int i = 0; i < array_size; ++i)
      trace += *(p_data_array + i * array_size + i);
    (*p_output).push_back(trace);
  }
  else {
    for(int i = current_pos; i < array_size; ++i) {
      swap_ftn(p_data_array + i, p_data_array + current_pos);
      obt_trace(p_data_array, current_pos + 1, array_size, p_output);
      swap_ftn(p_data_array + current_pos, p_data_array + i);
    }
  }
}


int main() {
  const int range = 100, mat_dim = 5;
  int mat[mat_dim][mat_dim] = {{0}};

  for(int i = 0; i < mat_dim; ++i)
    for(int j = 0; j < mat_dim; ++j)
      i_mat[i][j] = rand() % range + 1;

  vector<int> output;

  obt_trace(mat, 0, mat_dim, &output);

  int array_size = output.size();

  quick_sort(&output, 0, array_size - 1);

  for(int i = 0; i < array_size; ++i)
    cout << output[i] << endl;

  return 0;
}

*/


/*/ 1. Multiples of 3 & 5

int main() {
  int sum = 0;

  for(int i = 1; i < 1000; ++i)
    if( ((i % 3) == 0) || ((i % 5) == 0) )
      sum += i;

  cout << sum << endl;

  return 0;
}

*/


/*/ 2. Fibonacci sequence

long Fibonacci(int n) {
  if(n < 3)
    return n;
  else
    return Fibonacci(n - 1) + Fibonacci(n - 2);
}

int main() {
  long sum = 0;

  for(int n = 1; Fibonacci(n) < 4000000; ++n)
    if((Fibonacci(n) % 2) == 0)
      sum += Fibonacci(n);

  cout << sum;

  return 0;
}
*/


/*/ 3-1. Finding primes

int main() {
  long u_bound = 0;
  cin >> u_bound;
  vector<long> primes;
  primes.push_back(2);

  for(long n = 3; n < u_bound; ++n) {
    bool factor_found = false;
    for(int i = 0; i < primes.size() && primes[i] * primes[i] <= n ; ++i) {
      if( n % primes[i] == 0 ) {
        factor_found = true;
        break;
      }
    }
    if (factor_found == false) {
      primes.push_back(n);
    }
  }

  for(int j = 0; j < primes.size(); ++j)
    cout << primes[j] << " ";
  cout << endl;

  return 0;
}

*/


/*/ 3-2. Prime factors

int main() {
  long long n = 0;
  cin >> n;
  vector<long long> factors;

  for (long long i = 2; i * i <= n; ++i) {
      bool factor_found = false;
      int power = 0;
      while(n % i == 0) {
        factor_found = true;
        n /= i;
        ++power;
      }
      if (factor_found == true) {
        cout << "(" << i << ", " << power << ")" << endl;
        factors.push_back(i);
      }
    }

    if (n == 1)
      ;
    else cout << "(" << n << ", " << 1 << ")" << endl;

  return 0;
}

*/


/*/ 4. Palindrome product

string convertInt(long number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int main() {
  long temp = 0, pdn = 0;
  string x;
  int l = 0;

  for(int i = 100; i < 1000; ++i)
    for(int j = 100; j <= i; ++j) {
      x = convertInt(i * j);
      l = x.length();
      for(int k = 0; k < l / 2; ++k) {
        if(x[k] != x[l - 1 - k])
          break;
        else if( k == l / 2 - 1 ) {
          temp = atoi(x.c_str());
          if(temp > pdn)
            pdn = temp;
        }
      }
    }

  cout << pdn << endl;

  return 0;
}

*/


/*/ 5. Smallest multiple

int find_primes(int n, vector<int>* p_primes) {
  (*p_primes).push_back(2);

  for(long i = 3; i <= n; ++i) {
    bool factor_found = false;
    for(int j = 0; j < (*p_primes).size() && (*p_primes)[j] * (*p_primes)[j] <= i ; ++j) {
      if( i % (*p_primes)[j] == 0 ) {
        factor_found = true;
        break;
      }
    }
    if (factor_found == false) {
      (*p_primes).push_back(i);
    }
  }

  return (*p_primes).size();
}

int main() {
  vector<int> primes;
  int n = find_primes(20, &primes);
  long result = 1;

  for (int i = 0; i < n; ++i) {
    int k = 1;
    for (int j = 1; k <= 20 ; ++j)
      k *= primes[i];
    result *= k / primes[i];
  }

  cout << result << endl;

  return 0;
}

*/


/*/ 6. Sum square difference

int main() {
  long result1 = 0, result2 = 0;

  for (int i = 1; i < 101; ++i)
    result1 += i * i;

  for (int i = 1; i < 101; ++i)
    result2 += i;

  cout << result2 * result2 - result1 << endl;

  return 0;
}

*/


/*/ 7. 10001st prime

int main() {
  vector<long> primes;
  primes.push_back(2);

  for(long i = 3; primes.size() < 10001; ++i) {
    bool factor_found = false;
    for(int j = 0; j < primes.size() && primes[j] * primes[j] <= i ; ++j) {
      if( i % primes[j] == 0 ) {
        factor_found = true;
        break;
      }
    }
    if (factor_found == false) {
      primes.push_back(i);
    }
  }

  cout << primes[10000] << endl;

  return 0;
}

*/


/*/ 8. Largest product in a series

int main() {
  fstream myFile("data.txt", ios_base::in);
  vector<int> series;
  int array_size = 0, temp = 0, product = 1, max_product = 0;

  while(myFile >> temp) {
    series.push_back(temp);
    ++array_size;
  }
  myFile.close();

  // cout << series[100] << " " << series[150] << endl;

  for(int i = 0; i < array_size - 4; ++i) {
    for (int j = 0; j < 5; ++j)
      product *= series[i + j];
    if (product >= max_product)
      max_product = product;
  }

  cout << max_product << endl;

  return 0;
}


*/


/*/ 9. Special Pythagorian numbers

int main () {
  int a = 1, b = 2;

  for (int i = 1; i < 1000; ++i) {
    for (int j = i; j < 1000; ++j) {
      int c = 1000 - i - j;
      if ( c * c - i * i - j * j == 0 ) {
        a = i, b = j;
        break;
      }
    }
  }
  cout << a * b * (1000 - a - b) << endl;

  return 0;
}

*/


/*/ 10. Summation of primes

int main() {
  long sum = 0;
  vector<long> primes;
  primes.push_back(2);

  for(long n = 3; n < 2000000; ++n) {
    bool factor_found = false;
    for(int i = 0; i < primes.size() && primes[i] * primes[i] <= n ; ++i) {
      if( n % primes[i] == 0 ) {
        factor_found = true;
        break;
      }
    }
    if (factor_found == false) {
      primes.push_back(n);
    }
  }

  for(int j = 0; j < primes.size(); ++j)
    sum += primes[j];
  cout << sum << endl;

  return 0;
}

*/


/*/ Recursive Activity selector

void RAS (const int *s_time, const int *f_time, int strt, int fin) {
  int m = strt + 1;

  while (m < fin && s_time[m] < f_time[strt])
    m++;

  if (m < fin) {
    cout << m << endl;
    RAS (s_time, f_time, m, fin);
  }
}

int main () {
  int s_time[] = {-100,1,3,0,5,3,5,6,8,8,2,12,100};
  int f_time[] = {0,4,5,6,7,8,9,10,11,12,13,14,200};

  RAS (s_time, f_time, 0, 12);

  return 0;
}

*/


/*/ Greedy Activity selector

int main () {
  int s_time[] = {-100,1,3,0,5,3,5,6,8,8,2,12,100};
  int f_time[] = {0,4,5,6,7,8,9,10,11,12,13,14,200};
  int i = 0;
  vector<int> Max_set;

  for (int m = 1; m < 12; ++m)
    if (s_time[m] >= f_time[i]) {
      cout << m << endl;
      i = m;
    }

  return 0;
}

*/


/*/ Greedy: unit time task scheduling
struct data {
  int job_name;
  int dead_line;
  int penalty;
};

// heap sort, decreasing order
void min_heapify(struct data* p_data_array, int heap_size, int root_index) {
  int smallest = 100, l_index = 2 * root_index, r_index = 2 * root_index + 1;
  struct data temp = {0, 0};

  if(l_index <= heap_size && (p_data_array + (root_index - 1))->penalty > (p_data_array +(l_index - 1))->penalty)
    smallest = l_index;
  else smallest = root_index;
  if(r_index <= heap_size && (p_data_array + (smallest - 1))->penalty > (p_data_array + (r_index - 1))->penalty)
    smallest = r_index;

  if(smallest != root_index) {
    temp = p_data_array[root_index - 1];
    p_data_array[root_index - 1] = p_data_array[smallest - 1];
    p_data_array[smallest - 1] = temp;
    min_heapify(p_data_array, heap_size, smallest);
  }
}

void build_min_heap(struct data* p_data_array, int array_size) {
  for(int i = array_size / 2; i > 0; --i)
    min_heapify(p_data_array, array_size, i);
}

void heap_sort(struct data* p_data_array, int array_size) {
  build_min_heap(p_data_array, array_size);
  struct data temp = {0, 0};

  for(int i = array_size - 1; i > 0; --i) {
    temp = p_data_array[0];
    p_data_array[0] = p_data_array[i];
    p_data_array[i] = temp;
    min_heapify(p_data_array, i, 1);
  }
}

int main() {
  struct data input[ ] = {{1, 4, 70}, {2, 2, 60}, {3, 4, 50}, {4, 3, 40}, {5, 1, 30}, {6, 4, 20}, {7, 6, 10}};

  heap_sort(input, 7);

  for(int i = 0; i < 7; ++i)
    cout << input[i].job_name << " " << input[i].dead_line << " " << input[i].penalty << endl;

  vector<data> indep_set;

  for(int i = 0; i < 7; ++i) {
    indep_set.push_back(*(input + i));
    for(int t = 0; t < 7; ++t) {
      int N_A = 0;
      while(N_A <= indep_set.size() && indep_set[N_A].dead_line <= t)
        ++N_A;
      if(N_A <= t)
        ;
      else {
        indep_set.pop_back();
        break;
      }
    }
  }

  cout << endl;

  int tot_penalty = 0;

  for(int i = 0; i < indep_set.size(); ++i) {
    cout << indep_set[i].job_name << " " << indep_set[i].dead_line << " " << indep_set[i].penalty << endl;
    tot_penalty += indep_set[i].penalty;
  }

  cout << 280 - tot_penalty << endl;

  return 0;
}
*/


/*/ Random walks
int main() {
  const int trial = 10;
  double dist_sum = 0;

  for(int i = 0; i < trial; ++i) {
    int x = 0, y = 0, tot_steps = 100;

    for(int j = 0; j < tot_steps; ++j) {
      int x_incr = rand() % 3 -1;
      int y_incr = rand() % 3 -1;

      cout << x_incr << " " << " " << endl;

      if(x_incr*x_incr + y_incr*y_incr == 0 || x_incr*x_incr + y_incr*y_incr == 2)
        ++tot_steps;
      else {
        x += x_incr;
        y += y_incr;
      }
    }

    cout << sqrt(x*x + y*y) << endl;
    cout << tot_steps << endl;

 //   dist_sum += sqrt(x*x + y*y);
 }

//  cout << endl << dist_sum / trial << endl;

  return 0;
}
*/


/*/ Random walk, proper
int main() {
  const int trial = 10, tot_steps = 100;
  double dist_sum = 0;

  for(int i = 0; i < trial; ++i) {
    int x = 0, y = 0;

    int steps_taken = 0;

    while (steps_taken < tot_steps) {
   //   srand(time(NULL));
      int x_incr = rand() % 3 - 1;
      int y_incr = rand() % 3 - 1;

     // cout << x_incr << " " << y_incr << endl;


      if(x_incr*x_incr + y_incr*y_incr == 1) {
        x += x_incr;
        y += y_incr;
        steps_taken++;
      }
    }

    cout << endl << sqrt(x*x + y*y) << endl;

    dist_sum += sqrt(x*x + y*y);
 }

  cout << endl << dist_sum / trial << endl;

  return 0;
}
*/


/*/ Rocket Fuel
#include <iostream>

using namespace std;

// struct for the log data for each racer
struct LogItem {
  int racer_id;
  long long start_t, end_t;
};

// struct for the score for each racer
struct ScoreItem {
  int racer_id, score;
};


// heap-sort the input log data
void max_heapify_in(LogItem* p_data_array, int heap_size, int root_index) {
  int largest = 0, l_index = 2 * root_index, r_index = 2 * root_index + 1;
  LogItem temp;

  if(l_index <= heap_size && (*(p_data_array + (root_index - 1))).start_t < (*(p_data_array + (l_index - 1))).start_t)
    largest = l_index;
  else largest = root_index;
  if(r_index <= heap_size && (*(p_data_array + (largest - 1))).start_t < (*(p_data_array + (r_index - 1))).start_t)
    largest = r_index;

  if(largest != root_index) {
    temp = *(p_data_array + (root_index - 1));
    *(p_data_array + (root_index - 1)) = *(p_data_array + (largest - 1));
    *(p_data_array + (largest - 1)) = temp;
    max_heapify_in(p_data_array, heap_size, largest);
  }
}

void build_max_heap_in(LogItem* p_data_array, int array_size) {
  for(int i = array_size / 2; i > 0; --i)
    max_heapify_in(p_data_array, array_size, i);
}

void heap_sort_in(LogItem* p_data_array, int array_size) {
  build_max_heap_in(p_data_array, array_size);
  LogItem temp;

  for(int i = array_size - 1; i > 0; --i) {
    temp = *p_data_array;
    *p_data_array = *(p_data_array + i);
    *(p_data_array + i) = temp;
    max_heapify_in(p_data_array, i, 1);
  }
}


// heap-sort the output scores
void max_heapify_out(ScoreItem* p_data_array, int heap_size, int root_index) {
  int largest = 0, l_index = 2 * root_index, r_index = 2 * root_index + 1;
  ScoreItem temp;

  if ( l_index <= heap_size &&
       ( (*(p_data_array + (root_index - 1))).score < (*(p_data_array + (l_index - 1))).score
         || ( (*(p_data_array + (root_index - 1))).score == (*(p_data_array + (l_index - 1))).score &&
              (*(p_data_array + (root_index - 1))).racer_id < (*(p_data_array + (l_index - 1))).racer_id ) ) )
    largest = l_index;
  else largest = root_index;
  if ( r_index <= heap_size &&
       ( (*(p_data_array + (largest - 1))).score < (*(p_data_array + (r_index - 1))).score
         || ( (*(p_data_array + (largest - 1))).score == (*(p_data_array + (r_index - 1))).score &&
              (*(p_data_array + (largest - 1))).racer_id < (*(p_data_array + (r_index - 1))).racer_id ) ) )
    largest = r_index;

  if(largest != root_index) {
    temp = *(p_data_array + (root_index - 1));
    *(p_data_array + (root_index - 1)) = *(p_data_array + (largest - 1));
    *(p_data_array + (largest - 1)) = temp;
    max_heapify_out(p_data_array, heap_size, largest);
  }
}

void build_max_heap_out(ScoreItem* p_data_array, int array_size) {
  for(int i = array_size / 2; i > 0; --i)
    max_heapify_out(p_data_array, array_size, i);
}

void heap_sort_out(ScoreItem* p_data_array, int array_size) {
  build_max_heap_out(p_data_array, array_size);
  ScoreItem temp;

  for(int i = array_size - 1; i > 0; --i) {
    temp = *p_data_array;
    *p_data_array = *(p_data_array + i);
    *(p_data_array + i) = temp;
    max_heapify_out(p_data_array, i, 1);
  }
}



int main() {
  int no_of_racers;
  cin >> no_of_racers;

  // array of the log data
  LogItem input_data[no_of_racers];

  // Fill up the log data array.
  for (int i = 0; i < no_of_racers; ++i) {
    cin >> input_data[i].racer_id;
    cin >> input_data[i].start_t;
    cin >> input_data[i].end_t;
  }

  // Sort the log data in the ascending order of start time.
  heap_sort_in(input_data, no_of_racers);

  // array for the scores
  ScoreItem output_data[no_of_racers];

  for (int i = 0; i < no_of_racers; ++i) {
    // Initialize the score struct for the racer under consideration (RUC).
    output_data[i].racer_id = input_data[i].racer_id;
    output_data[i].score = 0;

    // Since start time < end time for each racer,
    // no need to look beyond a racer whose start time is later than the end time of RUC.
    // This approach is expected to give the same performance
    // as the suggested "bucket" method,
    // because it's the same with the bucket method
    // that we'll have to look at all the buckets
    // whose time interval falls in (start time, end time) of RUC.
    // (Possibly slightly better, because in a bucket
    // whose time interval falls in (start time, end time) of RUC,
    // there could be racers whose start time is before that of RUC.)
    for (int j = i + 1; j < no_of_racers; ++j) {
      if (input_data[i].end_t < input_data[j].start_t)
        break;
      if (input_data[i].end_t > input_data[j].end_t)
        ++output_data[i].score;
    }
  }

  // Sort the result in the ascending order of the score (and racer id).
  heap_sort_out(output_data, no_of_racers);

  for (int i = 0; i < no_of_racers; ++i)
    cout << output_data[i].racer_id << ' ' << output_data[i].score << endl;

  return 0;
}
*/


/*
void CharCompNN(string a, string b) {
  for(char x : a)
    for(char y : b)
      if(x == y) {
        cout << x;
        break;
      }
}

void CharCompN(string a, string b) {
  int b_array[95] = {0};

  for(int y : b)
    ++b_array[y - 32];

  for(int x : a) {
    if(b_array[x - 32] != 0)
      cout << (char)x;
    else
      cout << " ";
  }
}



int ExcelColtoInt(string a) {
  int x = 0;

  for(int y : a)
    x = 26 * x + y - 64;

  return x;
}


int Area(int* input, int start, int finish) {
  if(start == finish)
    return input[start];

  int min = 10000;
  for(int i = start; i <= finish; ++i)
    if(input[i] < min)
      min = input[i];

  return (finish - start + 1) * min;
}
*/


/*/ 3 Stacks
Class ThreeStacks {
 public:
  ThreeStacks();
  ~ThreeStacks();
  void Push(int which_stack, int val);
  int Pop(int which_stack);


 private:
  int input_array[100];
  int multi_stack_end[3];
  int empty_lot;

};

ThreeStacks::ThreeStacks() {
  multi_stack_end[0] = -1, multi_stack_end[1] = -1, multi_stack_end[2] = -1;
  empty_lot = 0;
}

ThreeStacks::~ThreeStacks() {

}

void ThreeStacks::Push(int which_stack, int val) {
  input_array[empty_lot] = val;
  multi_stack_end[which_stack] = empty_lot;
  ++empty_lot;
}

int ThreeStacks::Pop(int which_stack) {
  int pop_val = input_array[multi_stack[which_stack]];
  if(multi_stack[which_stack] + 1 == empty_lot)



  return pop_val;
}
*/


/*/ ThumbTack, http://www.thumbtack.com/challenges/software-engineer

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <math.h>

using namespace std;

class DB {
 public:
  DB();
  void Set(string name, double value);
  void Get(string name);
  void UnSet(string name);
  void NumEqualTo(double value);

  void Begin();
  void RollBack();
  void Commit();

 private:
  // Use map for the database storage. Because it takes O(log N) for map to perform search operation with find/count,
  // SET and GET can be done in O(log N).
  map<string, double> db;
  // Another map to store the # of entries with a given value. NUMEQUALTO can be done in O(log N).
  map<double, int> count_db;
  // How many transactions are on.
  int transaction_count;
  // While a transaction is on, store the current ("name", "value") to a stack before making a change to it,
  // so that the program knows to what to restore "name" when ROLLBACK is called.
  vector< pair<string, double> > limbo_stack;
  // When a new transaction block starts by calling BEGIN, store the current size of limbo_stack to a stack,
  // to remember the start of a new block.
  vector<int> rollback_marks;

  // BareSet and BareUnSet don't care about whether a transaction is on or not.
  void BareSet(string name, double value);
  void BareUnSet(string name);
};

int main() {
  DB test;

  string command, name;
  int value;

  while (cin >> command) {
    if (command == "SET") {
      cin >> name;
      cin >> value;
      test.Set(name, value);
    }
    else if (command == "GET") {
      cin >> name;
      test.Get(name);
    }
    else if (command == "UNSET") {
      cin >> name;
      test.UnSet(name);
    }
    else if (command == "NUMEQUALTO") {
      cin >> value;
      test.NumEqualTo(value);
    }
    else if (command == "BEGIN")
      test.Begin();
    else if (command == "ROLLBACK")
      test.RollBack();
    else if (command == "COMMIT")
      test.Commit();
    else if (command == "END")
      break;
    else
      cout << "Wrong command\n";
  }

  return 0;
}


DB::DB() {
  transaction_count = 0;
}

void DB::Set(string name, double value) {
  if (db.count(name)) {
    if (transaction_count != 0)
      // If a transaction is on and "name" is already in the database, store the current ("name", "value") to limbo_stack,
      // so that when ROLLBACK is called, the program will restore it.
      limbo_stack.push_back(make_pair(name, db[name]));

    --count_db[db[name]];
    db[name] = value;
  }
  else {
    if (transaction_count != 0)
      // If a transaction is on but "name" is not in the database, SET inserts a new entry to the database,
      // which, when ROLLBACK is called, should be removed.
      // In order to signify that this "name" didn't exist before BEGIN is called, its entry to limbo_stack is paired with NAN.
      limbo_stack.push_back(make_pair(name, sqrt(-1.0)));

    db.insert(make_pair(name, value));
  }

  if (count_db.count(value))
    ++count_db[value];
  else
    count_db.insert(make_pair(value, 1));
}

void DB::BareSet(string name, double value) {
  if (db.count(name)) {
    --count_db[db[name]];
    db[name] = value;
  }
  else
    db.insert(make_pair(name, value));

  if (count_db.count(value))
    ++count_db[value];
  else
    count_db.insert(make_pair(value, 1));
}

void DB::Get(string name) {
  if (db.count(name))
    cout << db[name] << endl;
  else
    cout << "NULL\n";
}

void DB::UnSet(string name) {
  if (db.count(name)) {
    if (transaction_count != 0)
      // If a transaction is on and "name" is already in the database, UnSetted entry should be restored when ROLLBACK is called.
      // limbo_stack remembers it.
      limbo_stack.push_back(make_pair(name, db[name]));

    --count_db[db[name]];
    db.erase(name);
  }
}

void DB::BareUnSet(string name) {
  if (db.count(name)) {
    --count_db[db[name]];
    db.erase(name);
  }
}

void DB::NumEqualTo(double value) {
  if (count_db.count(value))
    cout << count_db[value] << endl;
  else
    cout << 0 << endl;
}

void DB::Begin() {
  ++transaction_count;

  // Sore the location in limbo_stack where a new transaction block starts to a stack (rollback_marks).
  rollback_marks.push_back(limbo_stack.size());
}

void DB::RollBack() {
  if (transaction_count != 0) {
    --transaction_count;

    // The scope of the most recent transaction is entries in limbo_stack from the last BEGIN call to the end.
    // The last BEGIN call can be read from the last entry of rollback_marks.
    for (int i = limbo_stack.size(); i != rollback_marks.back(); --i) {
      // If the value of an entry in limbo_stack is a proper number, ROLLBACK will restore the value of "name" to it.
      if (!isnan(limbo_stack.back().second))
        BareSet(limbo_stack.back().first, limbo_stack.back().second);
      // That the value of an entry in limbo_stack is NAN implies this "name" didn't exist prior to BEGIN call.
      // ROLLBACK will remove the entry for this "name" from the database.
      else
        BareUnSet(limbo_stack.back().first);

      limbo_stack.pop_back();
    }

    rollback_marks.pop_back();
  }
  else
    cout << "NO TRANSACTION\n";
}

void DB::Commit() {
  transaction_count = 0;
  limbo_stack.clear();
  rollback_marks.clear();
}
*/


/*/ Facebook: The Tower of Hannoi
#include <iostream>

#define NDisks 4
#define KPegs 5

using namespace std;

void MoveDisks(int* p_disks_in_peg, int* p_in_array, int* p_out_array, int pos, int* p_count) {
  if(*(p_in_array + pos) == *(p_out_array + pos))
    return;

  for(int i = 0; i < pos; ++i) {
    if(*(p_in_array + i) == *(p_out_array + pos) || *(p_in_array + i) == *(p_in_array + pos)) {
      int j = -1;
      for(int k = 0; k < KPegs; ++k)
        if(*(p_disks_in_peg + k) == 0 && k + 1 != *(p_out_array + pos)) {
          j = k;
          ++(*(p_disks_in_peg + k));
          break;
        }
      if(j == -1)
        for(int k = i + 1; k < NDisks; ++i)
          if(*(p_in_array + k) != *(p_in_array + pos) && *(p_in_array + k) != *(p_out_array + pos)) {
            j = *(p_in_array + k) - 1;
            ++(*(p_disks_in_peg + (*(p_in_array + k) - 1)));
            break;
          }
      --(*(p_disks_in_peg + (*(p_in_array + i) - 1)));
      cout << *(p_in_array + i) << '\t' << j + 1 << endl;
      *(p_in_array + i) = j + 1;
      ++(*p_count);
    }
  }

  --(*(p_disks_in_peg + (*(p_in_array + pos) - 1)));
  ++(*(p_disks_in_peg + (*(p_out_array + pos) - 1)));
  cout << *(p_in_array + pos) << '\t' << *(p_out_array + pos) << endl;
  *(p_in_array + pos) = *(p_out_array + pos);
  ++(*p_count);
}


int main() {
  int in_config[NDisks] = {5,1,2,1};
  int out_config[NDisks] = {5,5,5,5};
  int count_moves = 0;
  int disks_in_peg[KPegs] = {0,0,0,0,0};
  for(int i = 0; i < NDisks; ++i)
    ++disks_in_peg[in_config[i] - 1];

  for(int i = NDisks - 1; i >= 0; --i)
    MoveDisks(disks_in_peg, in_config, out_config, i, &count_moves);

  cout << count_moves << endl;

  return 0;
}
*/


/*/ Day Trading
#include <iostream>
#include <fstream>
#include <string>
#include <queue>

using namespace std;

class History {
  public:
    History() {
      past.push_back(0);
      past.push_back(0);
      past.push_back(0);
    }
    void Push(int change) {
      past.pop_front();
      past.push_back(change);
    }
    const int& operator[] (int index) const {
      return past[index];
    }
    const int& back() {
      return past.back();
    }

  private:
    deque<int> past;
};


int main() {
  fstream my_data("stock1000.txt", ios_base::in);
  fstream my_book("book.txt", ios_base::out);
  double cash = 10000., shares = 0.;
  double balance = cash;
  double price, buffer;
  int time_count = 1;
  History trend;
  int UP = 1, DOWN = -1, STAY = 0;

  my_book << "period \t" << "price \t" << "cash \t" << "shares \t" << "balance" << endl;

  my_data >> price;
  buffer = price;
  my_book << time_count << '\t' << price << '\t' << cash << '\t' << shares << '\t' << balance << endl;

  while(my_data >> price) {
    ++time_count;

    if(price == buffer)
      trend.Push(STAY);

    else if(price > buffer) {
      if(trend.back() == DOWN)
        if(trend[0] == DOWN && trend[1] == DOWN) {
          shares += cash / price;
          cash -= price * shares;
          balance = cash + shares * price;
        }
      trend.Push(UP);
    }

    else {
      if(trend.back() == UP)
        if(trend[0] == UP && trend[1] == UP) {
          cash += price * shares;
          shares = 0;
          balance = cash;
        }
      trend.Push(DOWN);
    }

    buffer = price;
    if(cash < .00001)
      cash = 0;
    if(balance < .00001)
      balance = 0;
    my_book << time_count << '\t' << price << '\t' << cash << '\t' << shares << '\t' << balance << endl;
  }

  my_data.close();
  my_book.close();

  return 0;
}
*/
