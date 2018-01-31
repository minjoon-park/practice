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


using namespace std;

class TagMining {
 public:
	TagMining(string working_dir_name);

  void ParseTrainingDataAndBuildDfMap();
  void ConvertParsedDataFileToDocVectorFile();
  void BuildCorrelationMap();
  void ExamineCorMap();

  void LoadMaps();
  void LoadCorMap();
  void BuildTrainingSet(int size_of_training_set);
  void Train(double threshold_val, int min_no_of_common_words_val);

 private:
  string working_dir;

  map<string, int> df_map;
  map< string, set<int> > inverted_tag_map;
  map< string, map<string, double> > correlation_map;
  map< int, pair< set<string>, map<string, double> > > training_set;

  int no_of_tags_to_predict;
  double threshold;
  int min_no_of_common_words;

  void PrepStopwords(set<string>* stopwords, string dic_filename);
  void BlockRemoval(string* contents, string modifier1, string modifier2);
  void ModifierRemoval(string* contents, string modifier);
  string SiftOff(string* contents, set<string>* stopwords);

  void InsertToDfMap(string* contents);

  double PosWeightFtn(int pos, int length);
  void BuildLocalTagSet(const string& tags, set<string>* local_tags);
  void AddTitleWordsToDocVec(const string& title, map<string, double>* doc_vec);
  void AddBodyWordsToDocVec(const string& body, map<string, double>* doc_vec);
  void AddEntry(map<string, double>* tag_cor_strength_map, string tag, double val);
  void AddToCorrelationMap(set<string>* local_tags, map<string, double>* doc_vec);
};



#endif // TAGMINING_H
