#ifndef TAGMINING_H
#define TAGMINING_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <math.h>

#define DFCUT 100
#define SizeOfTotalSet 6034195
#define SizeOfTrainingSet 6034195


using namespace std;

class TagMining {
 public:
	TagMining(string working_dir_name);

  void ParseLearningDataAndBuildMaps();
  void CheckTagsDist();
  void BuildTrainItemVectors();

  void LoadTrainItemVectors();

  void PrepSVMLearningData();
  void PrepSVMTrainingData();
  void ParseTestData();
  void PrepSVMTestData();

  void LoadMaps();

  void BuildThresholdsMap();
  void BuildThresholdsMap_2();
  void DetermineThreshold(int tag_id);

  void LoadThresholdsMap();
  void BuildAnswerMap(int start, int how_many_per_run, int how_many_runs);
  void WriteSubmitFile();

 private:
  string working_dir;

  map<string, int> tag_set;
  map< int, set<int> > inverted_tag_map;
  map< string, pair<int, int> > df_map;
  map<int, string> train_item_vector_map;

  map<int, double> thresholds_map;

  map< int, map<double, string> > answer_map;

  void PrepStopwords(set<string>* stopwords, string dic_filename);
  void BlockRemoval(string* contents, string modifier1, string modifier2);
  void ModifierRemoval(string* contents, string modifier);
  string SiftOff(string* contents, set<string>* stopwords);

  void InsertToDfMap(string* contents);

  double PosWeightFtn(int pos, int length);

  int MAX(int x, int y) {
    return x < y ? y : x;
  }

  void AddTitleWordsToDocVec(const string& title, map<string, double>* doc_vec);
  void AddBodyWordsToDocVec(const string& body, map<string, double>* doc_vec);

  void FillNegExs(ofstream& out_file, int tag_id, char t_or_v);



};



#endif // TAGMINING_H
