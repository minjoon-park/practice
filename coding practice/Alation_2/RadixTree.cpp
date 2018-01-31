// A solution for "Indexing" problem by Minjoon Park, ver.2.1

#include "RadixTree.h"


int rNode::nodeCnt = 0;

// Gives a unique id to each edge
inline int hashCode(int num, char ch) {
  return num * 200 + ch;
}

// Pattern matching between inserted word/query and the value of edge
int findPatMatch(const string &str1, const string &str2) {
  int i = 0;

  while (i < str1.size()) {
    if (str1[i] == str2[i])
	    ++i;
    else
	    break;
  }

  return i;
}

// A standard java name can have $, _, numbers, alphabets
int char_range[10] = {'$', 1, '0', 10, 'A', 26, '_', 1, 'a', 26};

// Keeps tops with at most 10 highest score words
void rNode::updateTops(dbEntry& entry) {
  if (tops.size() == topN) {
    if (tops.begin()->second < entry.score)
      tops.erase(tops.begin());
    else
      return ;
  }

  tops.insert(make_pair(entry.id, entry.score));
}

// updateTops() does the job of filling up tops at each node
void rNode::Insert(dbEntry& entry, _transMap &transMap) {
  updateTops(entry);

  // find if current node has a transition on the 1st char of string to be inserted
  string word = entry.name;
  int idx = hashCode(_id, word[0]);

  if (!transMap.count(idx)) {
    rNode *newNode = new rNode(false, 1);
    rEdge *edge = new rEdge(this, newNode, word);
    newNode->parentEdge(edge);
    newNode->updateTops(entry);
    transMap[idx] = edge;
    return ;
  }

  // find pattern match b/w e and word
  rEdge& e = *(transMap[idx]);
  int match = findPatMatch(e.value(), word);

  if (match == word.size() && match == e.value().size()) {
    e.to().leaf(1);
    e.to().updateTops(entry);
    return ;
  }
  if (match == e.value().size()) {
    dbEntry _tmp(word.substr(match, word.size()), entry.id, entry.score);
    e.to().Insert(_tmp, transMap);
  }
  else {  // Split existing edge
    // Create a new internal node
    rNode *intNode = new rNode(false, 0);
    // Create a new edge
    string _tmp = e.value().substr(0, match);
    rEdge *splitEdge = new rEdge(this, intNode, _tmp);
    intNode->parentEdge(splitEdge);
    intNode->tops = e.to().tops;
    intNode->updateTops(entry);
    // Erase e from parent's map, and insert the splitEdge into it
    transMap[hashCode(_id, splitEdge->value()[0])] = splitEdge;

    // Modify current edge
    e.value().erase(0, match);
    e.from(intNode);
    transMap[hashCode(intNode->id(), e.value()[0])] = &e;

    if (match == word.size())
	    splitEdge->to().leaf(1);
    else {
	    // Create node and edge for the new leaf
	    rNode *leaf = new rNode(false, 1);
	    string _tmp = word.substr(match, word.size());
	    rEdge *edge2Leaf = new rEdge(intNode, leaf, _tmp);
	    leaf->parentEdge(edge2Leaf);
	    leaf->updateTops(entry);
	    transMap[hashCode(intNode->id(), edge2Leaf->value()[0])] = edge2Leaf;
    }
  }
}

// Tells if a prefix is present in corpus
rNode* rNode::lookup(string &qury, _transMap &transMap) {
  int idx = hashCode(_id, qury[0]);

  if (!transMap.count(idx))
    return NULL;

  rEdge& leavEdge = *(transMap[idx]);
  int match = findPatMatch(leavEdge.value(), qury);

  if (match == qury.size())
    return &(leavEdge.to());

  string _tmp = qury.substr(match, string::npos);

  return leavEdge.to().lookup(_tmp, transMap);
}

// Find all the edges leaving from the given node
void rNode::edgesFrom(vector<rEdge*> &edges, _transMap &transMap) {
  for (int i = 0; i < 10; i += 2)
    for (int j = char_range[i]; j < char_range[i] + char_range[i + 1]; ++j) {
      int idx = _id * 200 + j;
      if (!transMap.count(idx))
        continue;
      edges.push_back(transMap[idx]);
    }
}

// Remove the vector DB id from tops of the given node
// If, after removal, the size of tops decreases below 10,
// search the immediate child nodes for a new 10-th element
void rNode::RemoveId(int id, _transMap& transMap) {
  int bef_size = tops.size();

  for (auto it = tops.begin(); it != tops.end(); ++it)
    if (it->first == id) {
      tops.erase(it);
      break;
    }

  if (bef_size == 10 && bef_size - tops.size() == 1) {
    vector<rEdge*> leavingEdges;
    this->edgesFrom(leavingEdges, transMap);

    set<pair<int, int>, ScoreComp> tmp_tops;

    for (int i = 0; i < leavingEdges.size(); ++i) {
      set<pair<int, int>, ScoreComp>& child_tops = leavingEdges[i]->to().tops;

      for (auto it = child_tops.begin(); it != child_tops.end(); ++it) {
        if (tmp_tops.size() == topN) {
          if (tmp_tops.begin()->second < it->second)
            tmp_tops.erase(tmp_tops.begin());
          else
            continue;
        }
        tmp_tops.insert(make_pair(it->first, it->second));
      }
    }

    tops = tmp_tops;
  }
}

// Remove an element from the given node by updating tops and
// also parent and child nodes, if necessary
int rNode::Remove(dbEntry& entry, _transMap& transMap) {
  rNode *pres = NULL;
  string del_name = entry.name;
  int del_id = entry.id;

  // Name not present in tree, or name found is not a leaf
  if (!(pres = lookup(del_name, transMap)) || !pres->leaf())
    return -1;

  pres->leaf(-1);

  rNode* move_up = pres;   // Remove from the entire tree the id of the name you want to delete.
  while (!move_up->root()) {
    move_up->RemoveId(del_id, transMap);
    move_up = &(move_up->inComingEdge().from());
  }
  move_up->RemoveId(del_id, transMap);

  if (pres->tops.empty()) {
    rEdge& inEdge = pres->inComingEdge();
    rNode& parent = inEdge.from();

    // Erase the edge from parent's map
    transMap.erase(hashCode(parent.id(), (inEdge.value())[0]));

    vector<rEdge*> leavingEdges;
    parent.edgesFrom(leavingEdges, transMap);

    // If parent has only one child, then merge parent with the child
    if (leavingEdges.size() == 1) {
      rEdge& onlyEdge = *(leavingEdges[0]);
      if (!parent.leaf()) {
        rEdge& parInEdge = parent.inComingEdge();
        rNode& granPar = parInEdge.from();
        onlyEdge.value() = parInEdge.value() + leavingEdges[0]->value();
        onlyEdge.from(granPar);
        // Nnow erase parInEdge from granPar's map
        transMap.erase(hashCode(granPar.id(), (parInEdge.value())[0]));
        // Insert onlyEdge into granPar's map
        transMap[hashCode(granPar.id(), (onlyEdge.value())[0])] = &onlyEdge;
      }
    }
  }

  return 0;
}

// Return the contents of tops
vector<int> rNode::PrintTops() {
  vector<int> ret;

  for (auto rit = tops.rbegin(); rit != tops.rend(); ++rit)
    ret.push_back(rit->first);

  return ret;
}

// Recursively finds names starting with "qury"
vector<int> rNode::wordsStartingWith(string &qury, _transMap &transMap) {
  int idx = hashCode(_id, qury[0]);

  if (!transMap.count(idx))
    return vector<int>();

  rEdge& leavEdge = *(transMap[idx]);
  int match = findPatMatch(leavEdge.value(), qury);

  if (match == qury.size() && match <= leavEdge.value().size())
    return leavEdge.to().PrintTops();
  else {
    string _tmp = qury.substr(match, string::npos);
    return leavEdge.to().wordsStartingWith(_tmp, transMap);
  }
}


rTree::~rTree() {
  for (_transMap::iterator itr = transitionMap->begin(); itr != transitionMap->end(); ++itr) {
    rEdge* edge = itr->second;
    delete edge;
  }
  transitionMap = NULL;
  delete root;
  root = NULL;
}

void rTree::ImportDB(string db_file) {
  ifstream in(db_file, ios_base::in);
  string name;
  int score;

  while (in >> name >> score)
    db.push_back(make_pair(name, score));

  in.close();

  cout << "DB of " << db.size() << " entries imported\n";
}

void rTree::BuildTree() {
  for (int i = 0; i < db.size(); ++i) {
    dbEntry entry(db[i].first, i, db[i].second);
    Insert(entry);
  }

  cout << "RadixTree built\n";
}

// Insertion when input is given in dbEntry form
void rTree::Insert(dbEntry& entry) {
  root->Insert(entry, *transitionMap);

  int us_p = entry.name.find('_');
  if (us_p != string::npos) {
    dbEntry us_entry(entry.name.substr(us_p + 1, string::npos), entry.id, entry.score);
    root->Insert(us_entry, *transitionMap);
  }
}

// Insertion when input is given in (name, score) form
void rTree::Insert(string name, int score) {
  db.push_back(make_pair(name, score));

  dbEntry entry(name, db.size() - 1, score);
  Insert(entry);

  cout << "Insertion of (" << name << ", " << score << ")-pair into the DB is now reflected in Index\n";
}

void rTree::Remove(dbEntry& entry) {
  int ret = root->Remove(entry, *transitionMap);

  if (ret == 0) {
    int us_p = entry.name.find('_');
    if (us_p != string::npos) {
      dbEntry us_entry(entry.name.substr(us_p + 1, string::npos), entry.id, entry.score);
      root->Remove(us_entry, *transitionMap);
    }

    db[entry.id].first = "###";
    cout << "Removal of \"" << entry.name << "\" from the DB is now reflected in Index\n";
  }
  else
    cout << "No such name found in the DB\n";
}

// Finds names starting with "query" or including "_query"
void rTree::wordsStartingWith(string &query) {
  vector<int> answer = root->wordsStartingWith(query, *transitionMap);

  if (answer.empty())
    cout << "No matches\n";
  else {
    for (int i = 0; i < answer.size(); ++i)
      cout << db[answer[i]].first << ' ';
    cout << endl;
  }
}

// Serialization of Index
void rTree::WriteTree(string file_name) {
  ofstream out(file_name, ios_base::out);

  unordered_set<rNode*> nodes;
  unordered_set<string> edges;

  for (auto it = transitionMap->begin(); it != transitionMap->end(); ++it) {
    string tmp;
    tmp += (to_string(it->first) + ' ');

    rEdge* edge = it->second;
    tmp += (to_string(edge->from().id()) + ' ' + to_string(edge->to().id()) + ' ' + edge->value());

    edges.insert(tmp);

    nodes.insert(&edge->from());
    nodes.insert(&edge->to());
  }

  out << nodes.size() << endl;

  for (auto it = nodes.begin(); it != nodes.end(); ++it) {
    out << (*it)->id() << ' ' << (*it)->root() << ' ' << (*it)->leaf() << ' ';

    if ((*it) == root)
      out << -1 << ' ';
    else {
      rEdge& edge = (*it)->inComingEdge();
      out << edge.from().id() * 200 + edge.value()[0] << ' ';
    }

    for (auto tops_it = (*it)->tops.begin(); tops_it != (*it)->tops.end(); ++tops_it)
      out << tops_it->first << ' ' << tops_it->second << ' ';

    out << endl;
  }

  out << "######\n" << transitionMap->size() << endl;

  for (auto it = edges.begin(); it != edges.end(); ++it)
    out << *it << endl;

  out.close();

  cout << "Index is written to the file \"" << file_name << "\".\n";
}

// Deserialization of Index
void rTree::ReadTree(string file_name) {
  ifstream in(file_name, ios_base::in);

  map<int, rNode*> nodes;

  string ln_buffer;
  getline(in, ln_buffer);

  while (getline(in, ln_buffer)) {
    if (ln_buffer == "######")
      break;

    stringstream ss(ln_buffer);

    int n_id, is_root, is_leaf, v_par_edge_id, db_id, score;

    ss >> n_id >> is_root >> is_leaf >> v_par_edge_id;

    rNode* node = new rNode(is_root, is_leaf);
    node->id(n_id);
    node->par_edge_id = v_par_edge_id;

    set<pair<int, int>, ScoreComp> cur_tops;
    while (ss >> db_id >> score)
      cur_tops.insert(make_pair(db_id, score));
    node->tops = cur_tops;

    nodes.insert(make_pair(n_id, node));
  }

  getline(in, ln_buffer);

  while (getline(in, ln_buffer)) {
    stringstream ss(ln_buffer);

    int edge_id, from_node_id, to_node_id;
    string edge_val;

    ss >> edge_id >> from_node_id >> to_node_id >> edge_val;

    rEdge* edge = new rEdge(nodes[from_node_id], nodes[to_node_id], edge_val);

    transitionMap->insert(make_pair(edge_id, edge));
  }

  for (auto it = nodes.begin(); it != nodes.end(); ++it) {
    if (it->first != 1)
      it->second->parentEdge(transitionMap->at(it->second->par_edge_id));
    else
      root = it->second;
  }

  in.close();

  cout << "Index is read in from the file \"" << file_name << "\".\n";
}

