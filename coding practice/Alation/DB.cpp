// A solution for "Indexing" problem by Minjoon Park

#include "DB.h"

DBI::DBI(string file_name) {
  db_file_name = file_name;
  FillIdx();
}

bool NodeComp(v_Node n1, v_Node n2) {
  return (n1.name.compare(n2.name) < 0 ? true : false);
}

void DBI::FillIdx() {
  ifstream in(db_file_name, ios_base::in);
  string name;
  int score, id = 0;

  map< string, vector<int> > idx_temp;  // map does the sorting automatically.

  while (in >> name >> score) {
    if (idx_temp.count(name))
      idx_temp[name].push_back(id);
    else
      idx_temp.insert(make_pair(name, vector<int>(1, id)));

    int us_p = name.find('_');    // If "name" has '_', the substring after '_' is treated
    if (us_p != string::npos) {   // as a new "name" with the same "id".
      string after_us = name.substr(us_p + 1, name.size() - us_p - 1);

      if (idx_temp.count(after_us))
        idx_temp[after_us].push_back(id);
      else
        idx_temp.insert(make_pair(after_us, vector<int>(1, id)));
    }

    ++id;
  }

  in.close();

  for (map< string, vector<int> >::iterator it = idx_temp.begin(); it != idx_temp.end(); ++it) {
    v_Node node(it->first, it->second);
    idx.push_back(node);    // Copy map Index into vector, because we need a constant
  }                         // time access to Index for binary-search.

  ofstream out("idx.txt", ios_base::out);

  for (int i = 0; i < idx.size(); ++i) {    // Write Index to a file.
    out << idx[i].name << ' ';
    for (int j = 0; j < idx[i].ids.size(); ++j)
      out << (idx[i].ids)[j] << ' ';
    out << endl;
  }

  out.close();
}



DBS::DBS(string db_file, string idx_file) {
  db_file_name = db_file;
  idx_file_name = idx_file;
  LoadIdx();
  v_len = idx.size() - 1;
}

void DBS::LoadIdx() {
  ifstream db_in(db_file_name, ios_base::in);

  string name;
  int val;

  while (db_in >> name >> val)
    db.push_back(make_pair(name, val));

  db_in.close();

  ifstream idx_in(idx_file_name, ios_base::in);
  string ln_buffer;

  while (getline(idx_in, ln_buffer)) {
    stringstream ss(ln_buffer);
    ss >> name;

    vector<int> vec_temp;
    while (ss >> val)
      vec_temp.push_back(val);

    v_Node node_temp(name, vec_temp);

    idx.push_back(node_temp);
  }

  idx_in.close();
}

class ScoreComp {   // Comparator for top_10
 public:
  bool operator() (const pair<int, int>& p_1, const pair<int, int>& p_2) {
    return p_1.second < p_2.second;
  }
};

void DBS::Search(string query) {
  int lower = BiSearch(query, 0, v_len, 'l');   // Location of the name closest to and >= query.

  string upper_query = query.replace(query.size() - 1, 1, 1, query.back() + 1);
      // A new query that ends the candidate interval.
  int upper = BiSearch(upper_query, lower, v_len, 'u');
      // Location of the name closest to and < upper_query.

  set<pair<int, int>, ScoreComp> top_10;

  for (int i = lower; i <= upper; ++i) {
    for (int j = 0; j < idx[i].ids.size(); ++j) {
      if (top_10.size() == 10) {
        if (top_10.begin()->second < db[(idx[i].ids)[j]].second)
          top_10.erase(top_10.begin());
        else
          continue;
      }

      top_10.insert(make_pair((idx[i].ids)[j], db[(idx[i].ids)[j]].second));
    }
  }

  if (top_10.empty())
    cout << "No match\n";
  else {
    for (auto rit = top_10.rbegin(); rit != top_10.rend(); ++rit)
      cout << db[rit->first].first << ' ';
    cout << endl;
  }
}


int DBS::BiSearch(string query, int s, int e, char l_or_u) {
  if (s > e)    // When no match is found, return the bigger between s and e, which is always s,
    return (l_or_u == 'l' ? s : e);   // for the case of query. For upper_query, return e.
        // (If you think about it, it's almost trivial.)
  int m = (s + e) / 2;

  if (query.compare(idx[m].name) > 0)
    return BiSearch(query, m + 1, e, l_or_u);
  else if (query.compare(idx[m].name) < 0)
    return BiSearch(query, s, m - 1, l_or_u);
  else
    return (l_or_u == 'l' ? m : m - 1);   // For the case of upper_query, if there's a match,
        // return the one right below it, because we don't want to include upper_query itself.
}






