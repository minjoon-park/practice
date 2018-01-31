// A solution for "Indexing" problem by Minjoon Park, ver.2.1

/*
The main problem with the other version that employs a binary search method is that
when there are huge number (~ K) of words matching the query, choosing the top 10
with highest scores takes O(K) time.

To address this issue, now I use the radix tree as data structure for Index. The basic
implementation of radix tree is copied from

https://code.google.com/p/generic-library/source/browse/#svn%2Ftrunk%2Ftrunk%2FdataStructures%2FradixTree

My contribution is that

1. At each node, add a size-10 container, "tops", to store highest score words included
in the subtree rooted at the given node. That is, returning highest ranking words matching
the query takes only constant time.

2. Modify Insert/Remove methods to allow queries for names with '_'. That is, substring
after '_' is inserted/removed with the same vector DB index as the original name.

3. Modify Insert and Remove methods, to allow handling "tops". Especially, any
insertion/removal of an entry to the original database (DB) updates the radix tree Index,
such that "tops" at each node keeps containing at most 10 highest score entries.

4. Add method for serialization (WriteTree) and deserialization (ReadTree) of Index.

Complexity is

1. Building Index: worst case ~ N (DB size) * k (length of the longest word). But since a radix tree
stores many characters within a single node, the depth of a tree is generally shorter than k.

2. Searching for query and returning top 10: worst case ~ k, but again, faster in general.

3. Inserting: same as 2.

4. Removing: same as 2, but with possibly larger coefficient, because if the removal
decreases the size of a "tops" below 10, the tops of the immediate child nodes should be
looked up for a new 10-th element.

In practice, I assume a random access to a DB entry is possible, i.e., I don't have to
go through the entire DB to reach a certain element. I mimic this property by implementing
DB as a vector of (string name, int score)-pair, so that I can access any pair by its vector
index. The struct dbEntry reflects this implementation.
In order to keep the vector indices intact by insertion, a new pair is always added at the
end of the DB vector. As for removal, the slot for a removed entry is kept in order not to
change vector indices. Only the values are replaced by non-names.
*/

#ifndef RADIXTREE_H
#define RADIXTREE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <unordered_set>
#include <sstream>

#define topN 10

using namespace std;

class ScoreComp {   // Comparator for tops
 public:
  bool operator() (const pair<int, int>& p_1, const pair<int, int>& p_2) {
    return p_1.second < p_2.second;
  }
};

class rEdge; /*forward declaration*/

typedef map<int, rEdge*> _transMap;


struct dbEntry {
  dbEntry(string v_name, int v_id, int v_score) {
    name = v_name, id = v_id, score = v_score;
  }

  string name;
  int id, score;
};


class rNode {
 public:
	rNode() : isRoot(false), isLeaf(0) {
    _id = ++rNode::nodeCnt;
	}
	rNode(bool root = false, int leaf = 0) {
    _id = ++rNode::nodeCnt;
    isRoot = root;
    isLeaf = leaf;
    _parentEdge = NULL;
	}
	~rNode() {
    _parentEdge = NULL;
	}

	rEdge& inComingEdge() {
    return *_parentEdge;
	}
	void parentEdge(rEdge* e) {
    _parentEdge = e;
	}
	void leaf(int x) {
	  isLeaf += x;
  }
  bool root() {
	  return isRoot;
  }
	int leaf() {
	  return isLeaf;
  }
	int id() {
	  return _id;
  }
  void id(int v_id) {
    _id = v_id;
  }
	void Insert(dbEntry&, _transMap&);
	vector<int> wordsStartingWith(string& query, _transMap&);
	void edgesFrom(vector<rEdge*>&, _transMap&);
	void RemoveId(int id, _transMap&);
	int Remove(dbEntry&, _transMap&);
	rNode* lookup(string& qury, _transMap&);

	set<pair<int, int>, ScoreComp> tops;
	int par_edge_id;

 private:
	rNode(const rNode&);
	const rNode& operator=(const rNode&);

	void updateTops(dbEntry&);
	vector<int> PrintTops();

	static int nodeCnt;
	int _id;
	bool isRoot;
	int isLeaf;
	rEdge* _parentEdge;
};


class rEdge {
 public:
	rEdge(rNode *fNode, rNode *tNode, string &str) {
    fromNode = fNode;
	  toNode = tNode;
	  _value = new string(str);
	}
	rEdge() : fromNode(NULL), toNode(NULL), _value(NULL) {
	}
	~rEdge() {
    delete toNode;
	  toNode = NULL;
	  delete _value;
	  _value = NULL;
	}

	string& value() {
	  return *_value;
  }
	rNode& from() {
	  return *fromNode;
  }
	rNode& to() {
	  return *toNode;
  }
	void from(rNode &frm) {
	  fromNode = &frm;
  }
	void from(rNode *frm) {
	  fromNode = frm;
	  return ;
  }
	void to(rNode &t) {
	  toNode = &t;
  }
	void to(rNode *t) {
	  toNode = t;
  }

 private:
	rEdge(const rEdge&);
	rEdge& operator=(const rEdge&);

	rNode* fromNode;
	rNode* toNode;
	string* _value;
};


class rTree {
 public:
	rTree() {
    root = new rNode(true, 0);
    transitionMap = new _transMap();
	}
	~rTree();

	void ImportDB(string db_file);
	void BuildTree();
	void Insert(dbEntry&);
	void Insert(string name, int score);
	void Remove(dbEntry&);
	void wordsStartingWith(string& query);

	void WriteTree(string file_name);
	void ReadTree(string file_name);

 private:
	rTree(const rTree&);
	rTree& operator=(const rTree&);

	bool lookup(string& qury) {
    return (root->lookup(qury, *transitionMap)) ? true : false;
	}

  vector< pair<string, int> > db;
	rNode* root;
	_transMap* transitionMap;
};

#endif // RADIXTREE_H
