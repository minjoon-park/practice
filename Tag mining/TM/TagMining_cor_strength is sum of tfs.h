#ifndef TAGMINING_H
#define TAGMINING_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>

#define DFCUT 5
#define SizeOfTrainingSet 6034195
#define SizeOfValidationSet 100
// no. of tags: 42048, df_map size: 8611064,
// word_map size: DFCUT = 2 -> 2103574, DFCUT = 5 -> 835930
// no. of test set: 2013337

using namespace std;

class Item{
public:
	Item();

	int item_no;
	string title;
	string body;
	set<string> tags;
};


class TagMining {
 public:
	TagMining(string working_dir_name);

  void BuildDfMapAndTagMaps();
  void BuildInvertedIndex();
  void LoadTagMaps();
  void BuildCorrelationMap();

  void LoadCorMap();
  void DetermineThreshold();

 private:
  string working_dir;
  map< int, set<string> > tag_map;
  map< string, set<int> > inverted_tag_map;
  map<string, int> df_map;
  map< string, map<int, double> > word_map;
  map< string, map<string, double> > correlation_map;
  double threshold;

  void PrepStopwords(set<string>* stopwords);
  void BlockRemoval(string* contents, string modifier1, string modifier2);
  void ModifierRemoval(string* contents, string modifier);
  string SiftOff(string* contents, set<string>* stopwords);
  void InsertToDfMap(Item* item);
  double PosWeightFtn(int pos, int length);
  void AddEntry(map<int, double>* loc_modified_tf, const int& item_no, double val);
  void AddTitleWordsToInvertedIndex(int item_no, string* title);
  void AddBodyWordsToInvertedIndex(int item_no, string* body);

};



#endif // TAGMINING_H
