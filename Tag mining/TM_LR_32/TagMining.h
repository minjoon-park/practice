#ifndef TAGMINING_H
#define TAGMINING_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>

#define DFCUT 3
#define SizeOfTrainingSet 6034195
#define SizeOfTotalSet 8047532
#define no_of_tags 42048
#define no_of_words 300157
#define size_of_learning_set 5000000

using namespace std;


class TagMining {
 public:
	TagMining(string working_dir_name);

  void LoadMaps();
  void TrainForEachTag(int tag_id, double lambda);
  void ValidateForEachTag(int tag_id);
  void Validate();
  void Test(int start, int fin);
  void WriteAnswerMap(int start);

  void BuildAnswerMap(int start);
  void WriteSubmitFile();

 private:
  string working_dir;

  map<int, string> tag_set;
  map<string, int> df_map;
  map< int, set<int> > inverted_tag_map;

  double m_log_likelihood;

  map<int, double> thresholds_map;
  map< int, vector< pair<string, double> > > answer_map;



};



#endif // TAGMINING_H
